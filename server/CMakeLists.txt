cmake_minimum_required(VERSION 3.15)
project(PixelSlimeServer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vcpkg 로 설치: drogon, mongo-cxx-driver, jwt-cpp, nlohmann-json
find_package(Drogon CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)
find_package(bsoncxx CONFIG REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES
    "src/controllers/*.cpp"
    "src/websocket/*.cpp"
    "src/models/*.cpp"
    "src/services/*.cpp"
    "src/utils/*.cpp"
)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Drogon::Drogon
    mongo::mongocxx_shared
    mongo::bsoncxx_shared
    jwt-cpp::jwt-cpp
    nlohmann_json::nlohmann_json
)

# 빌드 후 config 폴더 복사
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
)
