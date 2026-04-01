#pragma once
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <memory>
#include <string>

class MongoUtil {
public:
    static MongoUtil& getInstance();

    void init(const std::string& uri, const std::string& dbName);

    // 풀에서 클라이언트 획득 (RAII)
    mongocxx::pool::entry acquireClient();

    const std::string& dbName() const { return _dbName; }

    // 편의 함수
    static bsoncxx::document::value fromJson(const std::string& json);
    static std::string toJson(const bsoncxx::document::view& view);

private:
    MongoUtil() = default;
    ~MongoUtil() = default;
    MongoUtil(const MongoUtil&) = delete;
    MongoUtil& operator=(const MongoUtil&) = delete;

    mongocxx::instance _instance{};
    std::unique_ptr<mongocxx::pool> _pool;
    std::string _dbName;
};
