#include "Item.h"

Item::Item(const std::string& name, ItemType type, int power, int quantity, const std::string& location)
    : name(name), type(type), power(power), quantity(quantity), location(location) {}

std::string Item::getName() const {
    return name;
}
ItemType Item::getType() const { 
    return type; 
}
int Item::getPower() const { 
    return power; 
}
int Item::getQuantity() const { 
    return quantity; 
}
std::string Item::getLocation() const { 
    return location; 
}

void Item::setLocation(const std::string& loc){
    location = loc;
}
void Item::decreaseQuantity(){
    if (quantity > 0) {
        quantity--; 
    }    
}
void Item::increaseQuantity(){ 
    quantity++; 
}

std::string Item::getTypeString() const {
    switch (type) {
        case Red: return "Red";
        case Blue: return "Blue";
        case Yellow: return "Yellow";
        default: return "Unknown";
    }
}

