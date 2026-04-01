using UnityEngine;

namespace PixelSlime.Map
{
    /// <summary>
    /// 쿼터뷰(아이소메트릭) 4방향 이동.
    /// 깊이 정렬은 IsometricSorter가 담당.
    /// </summary>
    [RequireComponent(typeof(Rigidbody2D))]
    public class IsometricMover : MonoBehaviour
    {
        [SerializeField] private float moveSpeed = 3f;
        [SerializeField] private SpriteRenderer spriteRenderer;

        // 쿼터뷰 이동 벡터 보정값 (화면 X:Y = 2:1 비율)
        private static readonly Vector2 IsoRight = new Vector2(1f,  -0.5f).normalized;
        private static readonly Vector2 IsoUp    = new Vector2(1f,   0.5f).normalized;

        private Rigidbody2D _rb;
        private Vector2     _input;

        void Awake()
        {
            _rb = GetComponent<Rigidbody2D>();
            _rb.gravityScale = 0;
            _rb.constraints  = RigidbodyConstraints2D.FreezeRotation;
        }

        void Update()
        {
            float h = Input.GetAxisRaw("Horizontal");
            float v = Input.GetAxisRaw("Vertical");
            _input = (IsoRight * h + IsoUp * v).normalized;

            if (h != 0)
                spriteRenderer.flipX = h < 0;
        }

        void FixedUpdate()
        {
            _rb.linearVelocity = _input * moveSpeed;
        }
    }
}
