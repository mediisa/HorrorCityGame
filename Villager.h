#ifndef VILLAGER_HPP
#define VILLAGER_HPP

#include "Character.h"

#include <string>

class Villager : public Character {
private:
    std::string safeLocation;

public:
    Villager(const std::string& name, const std::string& location, const std::string& safeLocation);

    std::string getSafeLocation() const;
    bool isAtSafeLocation() const;
};

#endif