using System;
using System.Collections.Generic;

namespace PixelSlime.Character
{
    public enum Race { None, Human, Orc, Elf, Beastman }

    [Serializable]
    public class CharacterData
    {
        public string id;
        public string userId;
        public string name;
        public int    growthStage;   // 0~5
        public string race;          // "none" | "human" | "orc" | "elf" | "beastman"
        public int    exp;
        public int    level;
        public List<string> equippedCosmetics;
        public List<string> ownedCosmetics;

        public Race RaceEnum => race switch
        {
            "human"    => Race.Human,
            "orc"      => Race.Orc,
            "elf"      => Race.Elf,
            "beastman" => Race.Beastman,
            _          => Race.None
        };
    }
}
