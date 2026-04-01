#pragma once
#include <string>
#include <vector>

// 집 등급
// 1: 오두막 (Cabin)
// 2: 목조 주택 (Wood House)
// 3: 석조 저택 (Stone Mansion)
// 4: 마법 궁전 (Magic Palace)

struct FurnitureItem {
    std::string itemId;
    int posX = 0;
    int posY = 0;
    int rotation = 0;   // 0, 90, 180, 270
};

struct House {
    std::string id;
    std::string userId;
    int         grade = 1;              // 1~4
    std::string wallpaperId;
    std::string floorId;
    std::string bgmId;
    std::vector<FurnitureItem> furniture;
    std::vector<std::string>   ownedItems;  // 보유 가구/장식 아이템 ID
};
