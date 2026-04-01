#include <drogon/drogon.h>
#include "utils/MongoUtil.h"
#include "utils/JwtUtil.h"

int main() {
    // 설정 파일 로드
    drogon::app().loadConfigFile("config/config.json");

    // custom_config에서 MongoDB / JWT 설정 읽기
    auto& customCfg = drogon::app().getCustomConfig();

    std::string mongoUri = customCfg.get("mongodb_uri", "mongodb://localhost:27017").asString();
    std::string mongoDb  = customCfg.get("mongodb_db",  "pixel_slime").asString();
    std::string jwtSecret = customCfg.get("jwt_secret", "secret").asString();
    int jwtExpires = customCfg.get("jwt_expires_hours", 72).asInt();

    // MongoDB 초기화
    MongoUtil::getInstance().init(mongoUri, mongoDb);
    LOG_INFO << "MongoDB connected: " << mongoUri << " / " << mongoDb;

    // JWT 초기화
    JwtUtil::init(jwtSecret, jwtExpires);
    LOG_INFO << "JWT initialized (expires: " << jwtExpires << "h)";

    // CORS 설정 (Unity 모바일 클라이언트 허용)
    drogon::app().registerPreHandlingAdvice(
        [](const drogon::HttpRequestPtr& req,
           drogon::AdviceCallback&& acb,
           drogon::AdviceChainCallback&& accb) {
            accb();
        }
    );

    LOG_INFO << "PixelSlimeServer starting on port 8080...";
    drogon::app().run();
    return 0;
}
