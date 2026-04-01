#pragma once
#include <string>
#include <vector>

// 펫 성장 단계
// 0: 알
// 1: 유아기
// 2: 성장기
// 3: 성체

struct Pet {
    std::string id;
    std::string userId;
    std::string name;
    std::string type;           // 슬라임펫, 불꽃토끼, 얼음여우, 풀고양이 등
    int         growthStage = 0;
    int         hunger = 100;   // 0~100 (0이면 아픔)
    int         happiness = 100;// 0~100
    int         exp = 0;
    std::vector<std::string> equippedCosmetics;
    std::vector<std::string> ownedCosmetics;
    long long   lastFedAt = 0;      // unix timestamp
    long long   lastPlayedAt = 0;
};
