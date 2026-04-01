#pragma once
#include <drogon/WebSocketController.h>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <string>

// 미니게임 방 상태
enum class RoomState {
    WAITING,    // 플레이어 대기 중
    PLAYING,    // 게임 진행 중
    FINISHED    // 결과 처리 중
};

struct GameRoom {
    std::string id;
    std::string gameType;   // "catch_star", "rhythm", "quiz"
    RoomState   state = RoomState::WAITING;
    std::vector<drogon::WebSocketConnectionPtr> players;
    int         maxPlayers = 4;
};

// WebSocket 엔드포인트: /ws/minigame
class MiniGameWS : public drogon::WebSocketController<MiniGameWS> {
public:
    WS_PATH_LIST_BEGIN
        WS_PATH_ADD("/ws/minigame");
    WS_PATH_LIST_END

    void handleNewMessage(const drogon::WebSocketConnectionPtr& conn,
                          std::string&& message,
                          const drogon::WebSocketMessageType& type) override;

    void handleNewConnection(const drogon::HttpRequestPtr& req,
                             const drogon::WebSocketConnectionPtr& conn) override;

    void handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn) override;

private:
    // 방 매칭 큐
    void joinMatchQueue(const drogon::WebSocketConnectionPtr& conn,
                        const std::string& gameType, const std::string& userId);
    void leaveRoom(const drogon::WebSocketConnectionPtr& conn);
    void broadcastToRoom(const std::string& roomId, const std::string& message);
    void startGame(GameRoom& room);
    void endGame(GameRoom& room, const std::string& winnerId);

    std::unordered_map<std::string, GameRoom> _rooms;
    // conn → roomId 매핑
    std::unordered_map<drogon::WebSocketConnectionPtr, std::string> _connRoom;
    std::mutex _mutex;
};
