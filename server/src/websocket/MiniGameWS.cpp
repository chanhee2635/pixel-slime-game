#include "MiniGameWS.h"
#include <nlohmann/json.hpp>
#include <drogon/drogon.h>
#include <random>

using json = nlohmann::json;

void MiniGameWS::handleNewConnection(const drogon::HttpRequestPtr& req,
                                      const drogon::WebSocketConnectionPtr& conn) {
    // 연결 시 userId는 쿼리 파라미터로 수신
    auto userId = req->getParameter("userId");
    conn->setContext(std::make_shared<std::string>(userId));
    LOG_INFO << "MiniGame WS connected: " << userId;
}

void MiniGameWS::handleNewMessage(const drogon::WebSocketConnectionPtr& conn,
                                   std::string&& message,
                                   const drogon::WebSocketMessageType& type) {
    if (type != drogon::WebSocketMessageType::Text) return;

    auto body = json::parse(message, nullptr, false);
    if (body.is_discarded()) return;

    std::string action = body.value("action", "");

    if (action == "join_queue") {
        std::string gameType = body.value("gameType", "catch_star");
        auto userId = *conn->getContext<std::string>();
        joinMatchQueue(conn, gameType, userId);

    } else if (action == "game_input") {
        // 플레이어 입력 처리 → 방 내 전체 브로드캐스트
        std::lock_guard<std::mutex> lock(_mutex);
        auto it = _connRoom.find(conn);
        if (it == _connRoom.end()) return;
        broadcastToRoom(it->second, message);

    } else if (action == "leave") {
        leaveRoom(conn);
    }
}

void MiniGameWS::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn) {
    leaveRoom(conn);
    LOG_INFO << "MiniGame WS disconnected";
}

void MiniGameWS::joinMatchQueue(const drogon::WebSocketConnectionPtr& conn,
                                 const std::string& gameType,
                                 const std::string& userId) {
    std::lock_guard<std::mutex> lock(_mutex);

    // 같은 gameType의 WAITING 방 탐색
    GameRoom* target = nullptr;
    for (auto& [id, room] : _rooms) {
        if (room.gameType == gameType &&
            room.state == RoomState::WAITING &&
            (int)room.players.size() < room.maxPlayers) {
            target = &room;
            break;
        }
    }

    // 없으면 새 방 생성
    if (!target) {
        std::string roomId = "room_" + std::to_string(std::rand());
        _rooms[roomId] = {roomId, gameType, RoomState::WAITING, {}, 4};
        target = &_rooms[roomId];
    }

    target->players.push_back(conn);
    _connRoom[conn] = target->id;

    json notify = {
        {"event", "room_joined"},
        {"roomId", target->id},
        {"playerCount", target->players.size()},
        {"maxPlayers", target->maxPlayers}
    };
    conn->send(notify.dump());

    // 방이 꽉 찼으면 게임 시작
    if ((int)target->players.size() >= target->maxPlayers)
        startGame(*target);
}

void MiniGameWS::leaveRoom(const drogon::WebSocketConnectionPtr& conn) {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _connRoom.find(conn);
    if (it == _connRoom.end()) return;

    auto& room = _rooms[it->second];
    auto& players = room.players;
    players.erase(std::remove(players.begin(), players.end(), conn), players.end());

    if (players.empty())
        _rooms.erase(it->second);

    _connRoom.erase(it);
}

void MiniGameWS::broadcastToRoom(const std::string& roomId, const std::string& message) {
    auto it = _rooms.find(roomId);
    if (it == _rooms.end()) return;
    for (auto& player : it->second.players)
        player->send(message);
}

void MiniGameWS::startGame(GameRoom& room) {
    room.state = RoomState::PLAYING;
    json start = {
        {"event", "game_start"},
        {"gameType", room.gameType},
        {"roomId", room.id},
        {"playerCount", room.players.size()}
    };
    for (auto& player : room.players)
        player->send(start.dump());
}

void MiniGameWS::endGame(GameRoom& room, const std::string& winnerId) {
    room.state = RoomState::FINISHED;
    json result = {
        {"event", "game_end"},
        {"winnerId", winnerId},
        {"roomId", room.id}
    };
    for (auto& player : room.players)
        player->send(result.dump());

    // TODO: 승자에게 코인/경험치 지급 (DB 업데이트)
}
