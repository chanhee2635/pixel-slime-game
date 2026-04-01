using UnityEngine;
using PixelSlime.Network;

namespace PixelSlime.Character
{
    /// <summary>
    /// 서버에서 캐릭터 데이터를 받아 로컬 상태로 관리.
    /// </summary>
    public class CharacterManager : MonoBehaviour
    {
        public static CharacterManager Instance { get; private set; }

        public CharacterData Current { get; private set; }

        void Awake()
        {
            if (Instance != null) { Destroy(gameObject); return; }
            Instance = this;
            DontDestroyOnLoad(gameObject);
        }

        public void FetchCharacter()
        {
            ApiClient.Instance.Get("/api/character",
                json =>
                {
                    Current = JsonUtility.FromJson<CharacterData>(json);
                    Debug.Log($"[Character] stage={Current.growthStage} race={Current.race}");
                    GameEvents.OnCharacterLoaded?.Invoke(Current);
                },
                err => Debug.LogError("[Character] Fetch failed: " + err)
            );
        }
    }
}
