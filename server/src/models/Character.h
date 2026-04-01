#pragma once
#include <string>
#include <vector>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

// 성장 단계
// 0: 원형 슬라임
// 1: 눈 생성
// 2: 입 생성
// 3: 팔 생성
// 4: 다리 생성
// 5: 종족 선택 완료

enum class Race {
    NONE      = 0,
    HUMAN     = 1,
    ORC       = 2,
    ELF       = 3,
    BEASTMAN  = 4
};

struct Character {
    std::string id;
    std::string userId;
    std::string name;
    int         growthStage = 0;    // 0~5
    Race        race = Race::NONE;
    int         exp = 0;
    int         level = 1;
    std::vector<std::string> equippedCosmetics;  // 장착 중인 치장 아이템 ID 목록
    std::vector<std::string> ownedCosmetics;     // 보유 치장 아이템 ID 목록

    static std::string raceToString(Race r) {
        switch (r) {
            case Race::HUMAN:    return "human";
            case Race::ORC:      return "orc";
            case Race::ELF:      return "elf";
            case Race::BEASTMAN: return "beastman";
            default:             return "none";
        }
    }

    static Race raceFromString(const std::string& s) {
        if (s == "human")    return Race::HUMAN;
        if (s == "orc")      return Race::ORC;
        if (s == "elf")      return Race::ELF;
        if (s == "beastman") return Race::BEASTMAN;
        return Race::NONE;
    }
};
