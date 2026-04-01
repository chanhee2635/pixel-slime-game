#pragma once
#include "../models/Quest.h"
#include <vector>
#include <string>

struct QuestCompleteResult {
    bool        success = false;
    QuestReward reward;
    std::string message;
};

class QuestService {
public:
    // 퀘스트 진행 업데이트 (action 수행 시 호출)
    static bool updateProgress(UserQuestProgress& progress, const std::string& action, int count = 1);

    // 보상 수령
    static QuestCompleteResult claimReward(UserQuestProgress& progress, const Quest& quest);

    // 일일 퀘스트 리셋 필요 여부
    static bool needsReset(const UserQuestProgress& progress);

    // 기본 퀘스트 목록 (DB 시딩용)
    static std::vector<Quest> getDefaultQuests();
};
