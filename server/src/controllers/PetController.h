#pragma once
#include <drogon/HttpController.h>

// API 엔드포인트:
//   GET    /api/pet         → 내 펫 목록 조회
//   POST   /api/pet         → 펫 입양 (알 부화)
//   PUT    /api/pet/feed    → 먹이 주기
//   PUT    /api/pet/play    → 놀아주기
//   PUT    /api/pet/equip   → 펫 치장 아이템 장착
//   PUT    /api/pet/unequip → 펫 치장 아이템 해제

class PetController : public drogon::HttpController<PetController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(PetController::getPets,      "/api/pet",         drogon::Get);
        ADD_METHOD_TO(PetController::adoptPet,     "/api/pet",         drogon::Post);
        ADD_METHOD_TO(PetController::feedPet,      "/api/pet/feed",    drogon::Put);
        ADD_METHOD_TO(PetController::playWithPet,  "/api/pet/play",    drogon::Put);
        ADD_METHOD_TO(PetController::equipPet,     "/api/pet/equip",   drogon::Put);
        ADD_METHOD_TO(PetController::unequipPet,   "/api/pet/unequip", drogon::Put);
    METHOD_LIST_END

    void getPets(const drogon::HttpRequestPtr& req,
                 std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void adoptPet(const drogon::HttpRequestPtr& req,
                  std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void feedPet(const drogon::HttpRequestPtr& req,
                 std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void playWithPet(const drogon::HttpRequestPtr& req,
                     std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void equipPet(const drogon::HttpRequestPtr& req,
                  std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void unequipPet(const drogon::HttpRequestPtr& req,
                    std::function<void(const drogon::HttpResponsePtr&)>&& callback);

private:
    std::string extractUserId(const drogon::HttpRequestPtr& req);
};
