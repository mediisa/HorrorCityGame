#include "Player.h"

Player::Player(const std::string& name, int garlicTime) 
    : name(name), garlicTime(garlicTime), hero(nullptr) {}

Player::~Player() {
    delete hero; 
}

std::string Player::getName() const {
    return name; 
}
int Player::getGarlicTime() const {
    return garlicTime;
}
Hero* Player::getHero() const {
    return hero;
}

void Player::setHero(Hero* h) {
    hero = h;
}

void Player::setGarlicTime(int a){
    garlicTime=a;
}
