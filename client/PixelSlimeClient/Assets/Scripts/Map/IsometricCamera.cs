using UnityEngine;

namespace PixelSlime.Map
{
    /// <summary>
    /// 쿼터뷰 카메라 설정 및 타겟 추적.
    /// Camera: Orthographic, Pixel Perfect Camera 컴포넌트와 함께 사용.
    /// </summary>
    public class IsometricCamera : MonoBehaviour
    {
        [SerializeField] private Transform target;
        [SerializeField] private Vector3   offset = new Vector3(0f, 2f, -10f);
        [SerializeField] private float     smoothSpeed = 5f;

        void LateUpdate()
        {
            if (target == null) return;

            Vector3 desired = target.position + offset;
            transform.position = Vector3.Lerp(transform.position, desired, smoothSpeed * Time.deltaTime);
        }

        public void SetTarget(Transform t) => target = t;
    }
}
