cmake_minimum_required(VERSION 3.16)
project(PokemonPurple LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(PokemonPurple
    src/main.cpp
    src/OverworldGui.cpp
    src/BattleInfoPanel.cpp
    src/BattlerPanel.cpp
    src/BattleState.cpp
    src/Main.cpp
    src/PokeMap.cpp
    src/Player.cpp
    src/Battler.cpp
    src/Monster.cpp
    src/Move.cpp
        src/SpriteManager.cpp
    src/Types.cpp
    src/Effect.cpp
    src/Encounter.cpp
    src/Evolution.cpp
    src/LevelUpMove.cpp
    src/Item.cpp
    src/MartItem.cpp
    src/Trainer.cpp
    src/Npc.cpp
    src/NpcStrings.cpp
    src/WorldObject.cpp
    src/Warp.cpp
    src/Giver.cpp
    src/Trader.cpp
    src/Blocker.cpp
    src/FlyLocation.cpp
    src/TmLearnsets.cpp
    src/BlackjackDialog.cpp
    src/TextRenderer.cpp
)

target_link_libraries(PokemonPurple
    ${SDL2_LIBRARIES}
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
)
