#include "QuestService.h"
#include <chrono>

bool QuestService::updateProgress(UserQuestProgress& progress,
                                   const std::string& action, int count) {
    if (progress.completed) return false;
    progress.currentCount += count;
    return true;
}

QuestCompleteResult QuestService::claimReward(UserQuestProgress& progress,
                                               const Quest& quest) {
    QuestCompleteResult result;

    if (progress.completed && progress.rewardClaimed) {
        result.message = "이미 보상을 수령했습니다.";
        return result;
    }
    if (progress.currentCount < quest.targetCount) {
        result.message = "퀘스트를 아직 완료하지 못했습니다.";
        return result;
    }

    progress.completed = true;
    progress.rewardClaimed = true;
    result.success = true;
    result.reward = quest.reward;
    result.message = "보상을 수령했습니다!";
    return result;
}

bool QuestService::needsReset(const UserQuestProgress& progress) {
    if (!progress.completed) return false;
    auto now = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return now >= progress.resetAt;
}

std::vector<Quest> QuestService::getDefaultQuests() {
    return {
        // 성장 퀘스트
        { "q_growth_01", "첫 탐험", "오두막 안을 탐험해보자", QuestType::GROWTH,
          0, "explore_cabin", 1, {50, 0, "", 100} },
        { "q_growth_02", "첫 식사", "오두막에서 음식을 먹어보자", QuestType::GROWTH,
          1, "eat_food", 1, {80, 0, "hat_basic", 150} },
        { "q_growth_03", "펫과 인사", "펫에게 인사를 건네보자", QuestType::GROWTH,
          2, "greet_pet", 1, {100, 0, "shirt_basic", 200} },
        { "q_growth_04", "집 청소", "오두막을 깨끗이 청소해보자", QuestType::GROWTH,
          3, "clean_house", 3, {150, 5, "furniture_table", 300} },
        { "q_growth_05", "종족의 각성", "자신의 정체성을 찾아라", QuestType::GROWTH,
          4, "select_race", 1, {500, 20, "", 700} },

        // 일일 퀘스트
        { "q_daily_01", "매일 돌보기", "펫에게 먹이를 3번 주기", QuestType::DAILY,
          1, "feed_pet", 3, {30, 0, "", 50} },
        { "q_daily_02", "집 꾸미기", "가구를 1개 배치하기", QuestType::DAILY,
          2, "place_furniture", 1, {20, 0, "", 30} },
        { "q_daily_03", "미니게임 참가", "미니게임 1회 참가", QuestType::SOCIAL,
          4, "play_minigame", 1, {100, 2, "", 80} },
    };
}
