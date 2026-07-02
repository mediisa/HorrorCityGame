#include "Card.h"

Card::Card(const std::string& name) : name(name) {}
Card::~Card() {}

std::string Card::getName() const {
    return name;
}

