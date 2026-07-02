#include "Character.h"


Character::Character(const std::string& name, const std::string& location , Typechara type) 
    : name(name), location(location) , type(type) {}
Character::~Character() {}

std::string Character::getName() const {
    return name;
}
std::string Character::getLoc() const {
    return location;
}
void Character::setLoc(const std::string& loc) {
    location = loc; 
}



