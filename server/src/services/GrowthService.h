#pragma once
#include "../models/Character.h"
#include <string>
#include <optional>

struct GrowthResult {
    bool    stageUp = false;        // 성장 단계 증가 여부
    int     newStage = 0;
    bool    raceSelectable = false; // 5단계 도달 시 true
};

class GrowthService {
public:
    // 경험치 추가 후 성장 결과 반환
    static GrowthResult addExp(Character& character, int exp);

    // 종족 선택 (5단계에서 호출)
    static bool selectRace(Character& character, Race race);

    // 치장 아이템 장착/해제
    static bool equipCosmetic(Character& character, const std::string& itemId);
    static bool unequipCosmetic(Character& character, const std::string& itemId);

    // 단계별 필요 경험치
    static int requiredExpForStage(int stage);

    // 단계별 해금 설명
    static std::string stageDescription(int stage);

private:
    // 단계별 경험치 테이블 (0→1, 1→2, 2→3, 3→4, 4→5)
    static constexpr int EXP_TABLE[] = { 100, 200, 350, 500, 700 };
};
