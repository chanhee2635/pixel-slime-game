#pragma once
#include <string>

struct User {
    std::string id;
    std::string username;
    std::string passwordHash;
    std::string characterId;
    std::string houseId;
    int         gems = 0;       // 프리미엄 재화
    int         coins = 0;      // 일반 재화
    int         voteScore = 0;  // 인기투표 누적 점수
    long long   createdAt = 0;
    long long   lastLoginAt = 0;
};
