# PixelSlimeServer - 빌드 가이드

## 1. vcpkg 설치 (처음 한 번만)

```powershell
cd C:/
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg integrate install
```

## 2. 의존성 설치

```powershell
cd C:/vcpkg
./vcpkg install drogon:x64-windows
./vcpkg install mongo-cxx-driver:x64-windows
./vcpkg install jwt-cpp:x64-windows
./vcpkg install nlohmann-json:x64-windows
```

> 주의: Drogon 설치에 10~20분 소요될 수 있음

## 3. MongoDB 설치 및 실행

- https://www.mongodb.com/try/download/community 에서 설치
- 서비스로 자동 시작 또는:
```powershell
mongod --dbpath C:/data/db
```

## 4. CMake 빌드

```powershell
cd C:/ClaudeJobs/pixel-slime-game/server
mkdir build
cd build

cmake .. -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

cmake --build . --config Release
```

## 5. 서버 실행

```powershell
cd C:/ClaudeJobs/pixel-slime-game/server/build/Release
./PixelSlimeServer.exe
```

## API 엔드포인트 요약

| 메서드 | 경로 | 설명 |
|---|---|---|
| GET | /api/character | 캐릭터 조회 |
| PUT | /api/character/equip | 치장 장착 |
| PUT | /api/character/unequip | 치장 해제 |
| PUT | /api/character/race | 종족 선택 |
| GET | /api/character/quests | 퀘스트 목록 |
| POST | /api/character/quest/claim | 퀘스트 보상 수령 |
| GET | /api/house | 집 조회 |
| PUT | /api/house/furniture | 가구 배치 |
| PUT | /api/house/upgrade | 집 업그레이드 |
| GET | /api/pet | 펫 조회 |
| PUT | /api/pet/feed | 먹이 주기 |
| PUT | /api/pet/play | 놀아주기 |
| WS | /ws/minigame | 미니게임 실시간 |
| WS | /ws/vote | 인기투표 실시간 |
