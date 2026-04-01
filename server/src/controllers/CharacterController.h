#pragma once
#include <drogon/HttpController.h>

// API 엔드포인트:
//   GET    /api/character          → 내 캐릭터 조회
//   PUT    /api/character/equip    → 치장 아이템 장착
//   PUT    /api/character/unequip  → 치장 아이템 해제
//   PUT    /api/character/race     → 종족 선택 (5단계 전용)
//   GET    /api/character/quests   → 퀘스트 목록 + 진행 상황
//   POST   /api/character/quest/claim  → 퀘스트 보상 수령

class CharacterController : public drogon::HttpController<CharacterController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(CharacterController::getCharacter,    "/api/character",               drogon::Get);
        ADD_METHOD_TO(CharacterController::equipItem,       "/api/character/equip",         drogon::Put);
        ADD_METHOD_TO(CharacterController::unequipItem,     "/api/character/unequip",       drogon::Put);
        ADD_METHOD_TO(CharacterController::selectRace,      "/api/character/race",          drogon::Put);
        ADD_METHOD_TO(CharacterController::getQuests,       "/api/character/quests",        drogon::Get);
        ADD_METHOD_TO(CharacterController::claimQuestReward,"/api/character/quest/claim",   drogon::Post);
    METHOD_LIST_END

    void getCharacter(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    void equipItem(const drogon::HttpRequestPtr& req,
                   std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    void unequipItem(const drogon::HttpRequestPtr& req,
                     std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    void selectRace(const drogon::HttpRequestPtr& req,
                    std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    void getQuests(const drogon::HttpRequestPtr& req,
                   std::function<void(const drogon::HttpResponsePtr&)>&& callback);

    void claimQuestReward(const drogon::HttpRequestPtr& req,
                          std::function<void(const drogon::HttpResponsePtr&)>&& callback);

private:
    // Authorization 헤더에서 userId 추출, 실패 시 빈 문자열
    std::string extractUserId(const drogon::HttpRequestPtr& req);
};
