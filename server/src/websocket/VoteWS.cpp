#include "VoteWS.h"
#include <nlohmann/json.hpp>
#include <algorithm>

using json = nlohmann::json;

void VoteWS::handleNewConnection(const drogon::HttpRequestPtr& req,
                                  const drogon::WebSocketConnectionPtr& conn) {
    std::lock_guard<std::mutex> lock(_mutex);
    _clients.push_back(conn);

    // 접속 즉시 현재 투표 현황 전송
    conn->send(buildRankingJson());
}

void VoteWS::handleNewMessage(const drogon::WebSocketConnectionPtr& conn,
                               std::string&& message,
                               const drogon::WebSocketMessageType& type) {
    if (type != drogon::WebSocketMessageType::Text) return;

    auto body = json::parse(message, nullptr, false);
    if (body.is_discarded()) return;

    std::string action = body.value("action", "");

    if (action == "vote") {
        std::string targetId = body.value("targetId", "");
        std::string voterId  = body.value("voterId", "");
        if (targetId.empty() || voterId.empty()) return;

        {
            std::lock_guard<std::mutex> lock(_mutex);
            _currentSession.votes[targetId]++;
        }

        // TODO: DB에 투표 기록 저장 (중복 투표 방지)
        broadcastRanking();
    }
}

void VoteWS::handleConnectionClosed(const drogon::WebSocketConnectionPtr& conn) {
    std::lock_guard<std::mutex> lock(_mutex);
    _clients.erase(std::remove(_clients.begin(), _clients.end(), conn), _clients.end());
}

void VoteWS::broadcastRanking() {
    auto msg = buildRankingJson();
    std::lock_guard<std::mutex> lock(_mutex);
    for (auto& client : _clients)
        client->send(msg);
}

std::string VoteWS::buildRankingJson() {
    // 투표 수 기준 내림차순 정렬
    std::vector<std::pair<std::string, int>> sorted(
        _currentSession.votes.begin(), _currentSession.votes.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    json ranking = json::array();
    int rank = 1;
    for (auto& [id, count] : sorted) {
        ranking.push_back({{"rank", rank++}, {"targetId", id}, {"votes", count}});
    }

    return json{{"event", "ranking_update"}, {"ranking", ranking}}.dump();
}
