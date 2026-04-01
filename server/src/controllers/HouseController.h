#pragma once
#include <drogon/HttpController.h>

// API 엔드포인트:
//   GET    /api/house              → 내 집 조회
//   PUT    /api/house/furniture    → 가구 배치/이동
//   DELETE /api/house/furniture    → 가구 제거
//   PUT    /api/house/wallpaper    → 벽지 변경
//   PUT    /api/house/floor        → 바닥 변경
//   PUT    /api/house/upgrade      → 집 등급 업그레이드

class HouseController : public drogon::HttpController<HouseController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(HouseController::getHouse,         "/api/house",              drogon::Get);
        ADD_METHOD_TO(HouseController::placeFurniture,   "/api/house/furniture",    drogon::Put);
        ADD_METHOD_TO(HouseController::removeFurniture,  "/api/house/furniture",    drogon::Delete);
        ADD_METHOD_TO(HouseController::changeWallpaper,  "/api/house/wallpaper",    drogon::Put);
        ADD_METHOD_TO(HouseController::changeFloor,      "/api/house/floor",        drogon::Put);
        ADD_METHOD_TO(HouseController::upgradeHouse,     "/api/house/upgrade",      drogon::Put);
    METHOD_LIST_END

    void getHouse(const drogon::HttpRequestPtr& req,
                  std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void placeFurniture(const drogon::HttpRequestPtr& req,
                        std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void removeFurniture(const drogon::HttpRequestPtr& req,
                         std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void changeWallpaper(const drogon::HttpRequestPtr& req,
                         std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void changeFloor(const drogon::HttpRequestPtr& req,
                     std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void upgradeHouse(const drogon::HttpRequestPtr& req,
                      std::function<void(const drogon::HttpResponsePtr&)>&& callback);

private:
    std::string extractUserId(const drogon::HttpRequestPtr& req);
};
