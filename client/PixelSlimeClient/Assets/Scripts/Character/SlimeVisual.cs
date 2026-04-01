using UnityEngine;

namespace PixelSlime.Character
{
    /// <summary>
    /// growthStage(0~5)에 따라 슬라임 스프라이트를 교체.
    /// Inspector에서 stage별 Sprite를 배열로 연결.
    /// </summary>
    public class SlimeVisual : MonoBehaviour
    {
        [SerializeField] private SpriteRenderer spriteRenderer;

        [Header("Growth Stage Sprites (index = stage)")]
        [SerializeField] private Sprite[] stageSprites;   // 0:원형, 1:눈, 2:입, 3:팔, 4:다리, 5:종족완료

        void OnEnable()
        {
            GameEvents.OnCharacterLoaded    += OnCharacterLoaded;
            GameEvents.OnGrowthStageChanged += OnStageChanged;
        }

        void OnDisable()
        {
            GameEvents.OnCharacterLoaded    -= OnCharacterLoaded;
            GameEvents.OnGrowthStageChanged -= OnStageChanged;
        }

        private void OnCharacterLoaded(CharacterData data) => ApplyStage(data.growthStage);
        private void OnStageChanged(int stage)             => ApplyStage(stage);

        private void ApplyStage(int stage)
        {
            if (stageSprites == null || stage >= stageSprites.Length) return;
            spriteRenderer.sprite = stageSprites[stage];
        }
    }
}
