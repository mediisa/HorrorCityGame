#include "Villager.h"

Villager::Villager(const std::string& name, const std::string& location, const std::string& safeLocation)
    : Character(name, location , Typechara::VILLAGER) , safeLocation(safeLocation) {}

std::string Villager::getSafeLocation() const {
    return safeLocation;
}
bool Villager::isAtSafeLocation() const {
    return location == safeLocation;
}