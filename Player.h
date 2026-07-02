#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Hero.h" 
#include <string>
#include <iostream>

class Player {
private:
    std::string name;
    int garlicTime;
    Hero* hero; 

public:
    Player(const std::string& name, int garlicTime);
    Player(const Player&) = default;
    Player& operator=(const Player&) = delete;
    ~Player();

    std::string getName() const;
    int getGarlicTime() const;
    Hero* getHero() const;

    void setHero(Hero* h);
    void setGarlicTime(int a);
};

#endif 

