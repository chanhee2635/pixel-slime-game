#pragma once
#include <string>
#include <vector>

enum class QuestType {
    DAILY   = 0,    // 일일 퀘스트
    GROWTH  = 1,    // 성장 퀘스트 (특정 단계 해금)
    HOUSE   = 2,    // 집 개조 퀘스트
    PET     = 3,    // 펫 육성 퀘스트
    SOCIAL  = 4     // 소셜 퀘스트 (투표, 미니게임)
};

struct QuestReward {
    int         coins = 0;
    int         gems = 0;
    std::string itemId;         // 치장 아이템 ID (없으면 empty)
    int         growthExp = 0;  // 성장 경험치
};

struct Quest {
    std::string id;
    std::string title;
    std::string description;
    QuestType   type;
    int         requiredGrowthStage = 0;    // 이 단계 이상에서 해금
    std::string targetAction;               // "explore_cabin", "feed_pet", "play_minigame" 등
    int         targetCount = 1;
    QuestReward reward;
};

struct UserQuestProgress {
    std::string userId;
    std::string questId;
    int         currentCount = 0;
    bool        completed = false;
    bool        rewardClaimed = false;
    long long   resetAt = 0;    // 일일퀘스트 리셋 시각
};
