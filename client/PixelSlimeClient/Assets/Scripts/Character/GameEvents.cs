using System;
using PixelSlime.Character;

namespace PixelSlime.Character
{
    /// <summary>
    /// 전역 게임 이벤트 버스.
    /// </summary>
    public static class GameEvents
    {
        public static Action<CharacterData> OnCharacterLoaded;
        public static Action<int>           OnGrowthStageChanged;  // 새 growthStage 값
        public static Action<Race>          OnRaceSelected;
    }
}
