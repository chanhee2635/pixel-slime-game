#pragma once
#include <drogon/WebSocketController.h>
#include <unordered_map>
#include <mutex>
#include <string>

// 인기투표 WebSocket
// - 주간 캐릭터 투표 / 집 꾸미기 투표 실시간 갱신
// WebSocket 엔드포인트: /ws/vote

struct VoteSession {
    std::string type;       // "character" or "house"
    std::unordered_map<std::string, int> votes;  // targetId → 투표 수
};

class VoteWS : public drogon::WebSocketController<VoteWS> {
public:
    WS_PATH_LIST_BEGIN
        WS_PATH_ADD("/ws/vote");
    WS_PATH_LIST_END

    void handleNewMessage(const drogon::WebSocketConnectionPtr& conn,
                          std::string&& message,
                          const drogon::WebSocketMessageType& type) override;

    void handleNewConnection(const drogon::HttpRequestPtr& req,
                             const drogon::WebSocketConnectionPtr& conn) override;

    void handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn) override;

private:
    void broadcastRanking();
    std::string buildRankingJson();

    std::vector<drogon::WebSocketConnectionPtr> _clients;
    VoteSession _currentSession;
    std::mutex _mutex;
};
