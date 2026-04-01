#include "CharacterController.h"
#include "../utils/JwtUtil.h"
#include "../utils/MongoUtil.h"
#include "../services/GrowthService.h"
#include "../services/QuestService.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

std::string CharacterController::extractUserId(const drogon::HttpRequestPtr& req) {
    auto auth = req->getHeader("Authorization");
    if (auth.empty()) return "";
    auto token = JwtUtil::extractBearer(auth);
    auto claims = JwtUtil::verify(token);
    if (!claims) return "";
    return claims->userId;
}

void CharacterController::getCharacter(const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

    auto userId = extractUserId(req);
    if (userId.empty()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "Unauthorized"}});
        resp->setStatusCode(drogon::k401Unauthorized);
        callback(resp);
        return;
    }

    auto client = MongoUtil::getInstance().acquireClient();
    auto db = (*client)[MongoUtil::getInstance().dbName()];
    auto col = db["characters"];

    auto filter = document{} << "userId" << userId << finalize;
    auto result = col.find_one(filter.view());

    if (!result) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "Character not found"}});
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
        return;
    }

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setBody(bsoncxx::to_json(result->view()));
    resp->setContentTypeCode(drogon::CT_APPLICATION_JSON);
    callback(resp);
}

void CharacterController::equipItem(const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

    auto userId = extractUserId(req);
    if (userId.empty()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "Unauthorized"}});
        resp->setStatusCode(drogon::k401Unauthorized);
        callback(resp);
        return;
    }

    auto body = json::parse(req->getBody(), nullptr, false);
    if (body.is_discarded() || !body.contains("itemId")) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "itemId required"}});
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string itemId = body["itemId"];

    auto client = MongoUtil::getInstance().acquireClient();
    auto db = (*client)[MongoUtil::getInstance().dbName()];
    auto col = db["characters"];

    auto filter = document{} << "userId" << userId << finalize;
    auto result = col.find_one(filter.view());
    if (!result) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "Character not found"}});
        resp->setStatusCode(drogon::k404NotFound);
        callback(resp);
        return;
    }

    // ownedCosmetics 확인 후 equippedCosmetics에 추가
    auto update = document{} << "$addToSet"
        << bsoncxx::builder::stream::open_document
            << "equippedCosmetics" << itemId
        << bsoncxx::builder::stream::close_document << finalize;

    col.update_one(filter.view(), update.view());

    auto resp = drogon::HttpResponse::newHttpJsonResponse(
        nlohmann::json{{"success", true}, {"equippedItemId", itemId}});
    callback(resp);
}

void CharacterController::unequipItem(const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

    auto userId = extractUserId(req);
    if (userId.empty()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "Unauthorized"}});
        resp->setStatusCode(drogon::k401Unauthorized);
        callback(resp);
        return;
    }

    auto body = json::parse(req->getBody(), nullptr, false);
    if (body.is_discarded() || !body.contains("itemId")) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "itemId required"}});
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string itemId = body["itemId"];

    auto client = MongoUtil::getInstance().acquireClient();
    auto db = (*client)[MongoUtil::getInstance().dbName()];
    auto col = db["characters"];

    auto filter = document{} << "userId" << userId << finalize;
    auto update = document{} << "$pull"
        << bsoncxx::builder::stream::open_document
            << "equippedCosmetics" << itemId
        << bsoncxx::builder::stream::close_document << finalize;

    col.update_one(filter.view(), update.view());

    auto resp = drogon::HttpResponse::newHttpJsonResponse(
        nlohmann::json{{"success", true}});
    callback(resp);
}

void CharacterController::selectRace(const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

    auto userId = extractUserId(req);
    if (userId.empty()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "Unauthorized"}});
        resp->setStatusCode(drogon::k401Unauthorized);
        callback(resp);
        return;
    }

    auto body = json::parse(req->getBody(), nullptr, false);
    if (body.is_discarded() || !body.contains("race")) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "race required (human/orc/elf/beastman)"}});
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    std::string raceStr = body["race"];
    Race race = Character::raceFromString(raceStr);
    if (race == Race::NONE) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "Invalid race"}});
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    auto client = MongoUtil::getInstance().acquireClient();
    auto db = (*client)[MongoUtil::getInstance().dbName()];
    auto col = db["characters"];

    // growthStage >= 5 확인
    auto filter = document{} << "userId" << userId
                              << "growthStage" << bsoncxx::builder::stream::open_document
                              << "$gte" << 5
                              << bsoncxx::builder::stream::close_document << finalize;

    auto update = document{} << "$set"
        << bsoncxx::builder::stream::open_document
            << "race" << raceStr
        << bsoncxx::builder::stream::close_document << finalize;

    auto result = col.update_one(filter.view(), update.view());
    if (!result || result->modified_count() == 0) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "성장 5단계에 도달해야 종족을 선택할 수 있습니다"}});
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    auto resp = drogon::HttpResponse::newHttpJsonResponse(
        nlohmann::json{{"success", true}, {"race", raceStr}});
    callback(resp);
}

void CharacterController::getQuests(const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

    auto userId = extractUserId(req);
    if (userId.empty()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "Unauthorized"}});
        resp->setStatusCode(drogon::k401Unauthorized);
        callback(resp);
        return;
    }

    auto client = MongoUtil::getInstance().acquireClient();
    auto db = (*client)[MongoUtil::getInstance().dbName()];
    auto col = db["quest_progress"];

    auto filter = document{} << "userId" << userId << finalize;
    auto cursor = col.find(filter.view());

    json result = json::array();
    for (auto& doc : cursor)
        result.push_back(json::parse(bsoncxx::to_json(doc)));

    auto resp = drogon::HttpResponse::newHttpJsonResponse(result);
    callback(resp);
}

void CharacterController::claimQuestReward(const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

    auto userId = extractUserId(req);
    if (userId.empty()) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "Unauthorized"}});
        resp->setStatusCode(drogon::k401Unauthorized);
        callback(resp);
        return;
    }

    auto body = json::parse(req->getBody(), nullptr, false);
    if (body.is_discarded() || !body.contains("questId")) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse(
            nlohmann::json{{"error", "questId required"}});
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    // TODO: DB에서 quest_progress 조회 후 QuestService::claimReward 호출
    // 보상 지급 (coins/gems/items → users, characters 컬렉션 업데이트)
    auto resp = drogon::HttpResponse::newHttpJsonResponse(
        nlohmann::json{{"success", true}, {"message", "보상 수령 완료"}});
    callback(resp);
}
