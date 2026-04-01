#include "GrowthService.h"
#include <algorithm>

constexpr int GrowthService::EXP_TABLE[];

GrowthResult GrowthService::addExp(Character& character, int exp) {
    GrowthResult result;
    result.newStage = character.growthStage;

    if (character.growthStage >= 5)
        return result;  // 이미 최대 단계

    character.exp += exp;

    int required = requiredExpForStage(character.growthStage);
    if (character.exp >= required) {
        character.exp -= required;
        character.growthStage++;
        result.stageUp = true;
        result.newStage = character.growthStage;

        if (character.growthStage == 5)
            result.raceSelectable = true;
    }

    return result;
}

bool GrowthService::selectRace(Character& character, Race race) {
    if (character.growthStage < 5) return false;
    if (race == Race::NONE) return false;
    character.race = race;
    return true;
}

bool GrowthService::equipCosmetic(Character& character, const std::string& itemId) {
    auto it = std::find(character.ownedCosmetics.begin(),
                        character.ownedCosmetics.end(), itemId);
    if (it == character.ownedCosmetics.end()) return false;  // 미보유

    auto already = std::find(character.equippedCosmetics.begin(),
                              character.equippedCosmetics.end(), itemId);
    if (already != character.equippedCosmetics.end()) return false;  // 이미 장착

    character.equippedCosmetics.push_back(itemId);
    return true;
}

bool GrowthService::unequipCosmetic(Character& character, const std::string& itemId) {
    auto it = std::find(character.equippedCosmetics.begin(),
                        character.equippedCosmetics.end(), itemId);
    if (it == character.equippedCosmetics.end()) return false;
    character.equippedCosmetics.erase(it);
    return true;
}

int GrowthService::requiredExpForStage(int stage) {
    if (stage < 0 || stage >= 5) return 0;
    return EXP_TABLE[stage];
}

std::string GrowthService::stageDescription(int stage) {
    switch (stage) {
        case 0: return "원형 슬라임 - 낯선 오두막에 나타났다";
        case 1: return "눈이 생겨났다 - 세상을 볼 수 있게 됐다";
        case 2: return "입이 생겨났다 - 말을 할 수 있게 됐다";
        case 3: return "팔이 생겨났다 - 물건을 만질 수 있게 됐다";
        case 4: return "다리가 생겨났다 - 걸어다닐 수 있게 됐다";
        case 5: return "종족이 결정될 시간 - 나는 누구인가?";
        default: return "";
    }
}
