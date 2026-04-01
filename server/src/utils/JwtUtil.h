#pragma once
#include <string>
#include <optional>

struct JwtClaims {
    std::string userId;
    std::string username;
};

class JwtUtil {
public:
    static void init(const std::string& secret, int expiresHours);

    static std::string generate(const std::string& userId, const std::string& username);
    static std::optional<JwtClaims> verify(const std::string& token);

    // Authorization 헤더에서 Bearer 토큰 추출
    static std::string extractBearer(const std::string& header);

private:
    static std::string _secret;
    static int _expiresHours;
};
