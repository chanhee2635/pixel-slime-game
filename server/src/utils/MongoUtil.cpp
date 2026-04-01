#include "MongoUtil.h"
#include <stdexcept>

MongoUtil& MongoUtil::getInstance() {
    static MongoUtil instance;
    return instance;
}

void MongoUtil::init(const std::string& uri, const std::string& dbName) {
    _dbName = dbName;
    mongocxx::uri mongoUri(uri);
    _pool = std::make_unique<mongocxx::pool>(mongoUri);
}

mongocxx::pool::entry MongoUtil::acquireClient() {
    if (!_pool)
        throw std::runtime_error("MongoUtil not initialized");
    return _pool->acquire();
}

bsoncxx::document::value MongoUtil::fromJson(const std::string& json) {
    return bsoncxx::from_json(json);
}

std::string MongoUtil::toJson(const bsoncxx::document::view& view) {
    return bsoncxx::to_json(view);
}
