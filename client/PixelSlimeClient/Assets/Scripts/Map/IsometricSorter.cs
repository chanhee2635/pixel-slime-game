using UnityEngine;

namespace PixelSlime.Map
{
    /// <summary>
    /// 아이소메트릭 씬의 오브젝트에 붙여 Y축 기반 깊이 정렬을 자동으로 처리.
    /// 캐릭터, NPC, 오브젝트 모두에 사용.
    /// </summary>
    [RequireComponent(typeof(SpriteRenderer))]
    public class IsometricSorter : MonoBehaviour
    {
        // 타일 1칸의 픽셀 높이 기준 (Cell Size Y = 0.5 → 픽셀 단위로 100)
        private const int SortMultiplier = 100;

        [Tooltip("같은 Y에서 레이어 우선순위 (캐릭터=0, 오브젝트=-1 등)")]
        [SerializeField] private int sortingOffset = 0;

        private SpriteRenderer _sr;

        void Awake() => _sr = GetComponent<SpriteRenderer>();

        void LateUpdate()
        {
            _sr.sortingOrder = Mathf.RoundToInt(-transform.position.y * SortMultiplier) + sortingOffset;
        }
    }
}
