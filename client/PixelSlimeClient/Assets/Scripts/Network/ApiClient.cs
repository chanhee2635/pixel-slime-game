using System;
using System.Text;
using System.Collections;
using UnityEngine;
using UnityEngine.Networking;

namespace PixelSlime.Network
{
    public class ApiClient : MonoBehaviour
    {
        public static ApiClient Instance { get; private set; }

        [SerializeField] private string baseUrl = "http://localhost:8080";

        private string _authToken;

        void Awake()
        {
            if (Instance != null) { Destroy(gameObject); return; }
            Instance = this;
            DontDestroyOnLoad(gameObject);
        }

        public void SetToken(string token) => _authToken = token;

        // ── GET ──────────────────────────────────────────────
        public void Get(string path, Action<string> onSuccess, Action<string> onError = null)
        {
            StartCoroutine(SendRequest(UnityWebRequest.Get(baseUrl + path), onSuccess, onError));
        }

        // ── PUT ──────────────────────────────────────────────
        public void Put(string path, object body, Action<string> onSuccess, Action<string> onError = null)
        {
            string json = JsonUtility.ToJson(body);
            var req = new UnityWebRequest(baseUrl + path, "PUT");
            req.uploadHandler   = new UploadHandlerRaw(Encoding.UTF8.GetBytes(json));
            req.downloadHandler = new DownloadHandlerBuffer();
            req.SetRequestHeader("Content-Type", "application/json");
            StartCoroutine(SendRequest(req, onSuccess, onError));
        }

        // ── POST ─────────────────────────────────────────────
        public void Post(string path, object body, Action<string> onSuccess, Action<string> onError = null)
        {
            string json = JsonUtility.ToJson(body);
            var req = new UnityWebRequest(baseUrl + path, "POST");
            req.uploadHandler   = new UploadHandlerRaw(Encoding.UTF8.GetBytes(json));
            req.downloadHandler = new DownloadHandlerBuffer();
            req.SetRequestHeader("Content-Type", "application/json");
            StartCoroutine(SendRequest(req, onSuccess, onError));
        }

        private IEnumerator SendRequest(UnityWebRequest req, Action<string> onSuccess, Action<string> onError)
        {
            if (!string.IsNullOrEmpty(_authToken))
                req.SetRequestHeader("Authorization", "Bearer " + _authToken);

            yield return req.SendWebRequest();

            if (req.result == UnityWebRequest.Result.Success)
                onSuccess?.Invoke(req.downloadHandler.text);
            else
                onError?.Invoke(req.error);
        }
    }
}
