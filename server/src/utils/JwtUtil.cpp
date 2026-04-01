#include "JwtUtil.h"
#include <jwt-cpp/jwt.h>
#include <chrono>

std::string JwtUtil::_secret;
int JwtUtil::_expiresHours = 72;

void JwtUtil::init(const std::string& secret, int expiresHours) {
    _secret = secret;
    _expiresHours = expiresHours;
}

std::string JwtUtil::generate(const std::string& userId, const std::string& username) {
    auto now = std::chrono::system_clock::now();
    auto token = jwt::create()
        .set_issuer("pixel-slime")
        .set_type("JWS")
        .set_issued_at(now)
        .set_expires_at(now + std::chrono::hours(_expiresHours))
        .set_payload_claim("userId", jwt::claim(userId))
        .set_payload_claim("username", jwt::claim(username))
        .sign(jwt::algorithm::hs256{_secret});
    return token;
}

std::optional<JwtClaims> JwtUtil::verify(const std::string& token) {
    try {
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{_secret})
            .with_issuer("pixel-slime");

        auto decoded = jwt::decode(token);
        verifier.verify(decoded);

        JwtClaims claims;
        claims.userId   = decoded.get_payload_claim("userId").as_string();
        claims.username = decoded.get_payload_claim("username").as_string();
        return claims;
    } catch (...) {
        return std::nullopt;
    }
}

std::string JwtUtil::extractBearer(const std::string& header) {
    const std::string prefix = "Bearer ";
    if (header.substr(0, prefix.size()) == prefix)
        return header.substr(prefix.size());
    return "";
}
