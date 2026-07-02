cmake_minimum_required(VERSION 3.10)
project(HorrorCityGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# لیست فایل‌ها
set(SOURCES
    Board.cpp
    Card.cpp
    character.cpp
    Dice.cpp
    Game.cpp
    Hero.cpp
    Heros4.cpp
    Item.cpp
    main.cpp
    Monster.cpp
    MonsterCard.cpp
    Monsters2.cpp
    PerkCard.cpp
    Player.cpp
    Villager.cpp
)

# لیست فایل‌های هدر
set(HEADERS
    Board.h
    Card.h
    character.h
    Dice.h
    Game.h
    Hero.h
    Heros4.h
    Item.h
    Monster.h
    MonsterCard.h
    Monsters2.h
    PerkCard.h
    Player.h
    Villager.h
)

# ساخت اجرایی
add_executable(HorrorCityGame ${SOURCES} ${HEADERS})