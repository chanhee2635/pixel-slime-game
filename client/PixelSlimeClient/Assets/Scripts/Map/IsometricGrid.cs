using UnityEngine;
using UnityEngine.Tilemaps;

namespace PixelSlime.Map
{
    /// <summary>
    /// 아이소메트릭 좌표 변환 유틸리티.
    /// Grid Cell Layout = "Isometric Z As Y", Cell Size = (1, 0.5, 1) 기준.
    /// </summary>
    public class IsometricGrid : MonoBehaviour
    {
        public static IsometricGrid Instance { get; private set; }

        [SerializeField] private Grid grid;
        [SerializeField] private Tilemap groundTilemap;

        void Awake()
        {
            if (Instance != null) { Destroy(gameObject); return; }
            Instance = this;
        }

        /// <summary>월드 좌표 → 타일 셀 좌표</summary>
        public Vector3Int WorldToCell(Vector3 worldPos)
            => grid.WorldToCell(worldPos);

        /// <summary>타일 셀 좌표 → 월드 좌표 (셀 중심)</summary>
        public Vector3 CellToWorld(Vector3Int cellPos)
            => grid.GetCellCenterWorld(cellPos);

        /// <summary>해당 셀에 타일이 존재하는지 확인</summary>
        public bool HasGround(Vector3Int cellPos)
            => groundTilemap.HasTile(cellPos);

        /// <summary>
        /// Y축 SortingOrder 계산.
        /// 아이소메트릭에서 Y가 낮을수록(화면 아래) 앞에 그려져야 함.
        /// </summary>
        public static int CalcSortingOrder(Vector3 worldPos)
            => Mathf.RoundToInt(-worldPos.y * 100);
    }
}
