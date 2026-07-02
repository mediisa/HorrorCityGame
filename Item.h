#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

enum ItemType { Red, Blue, Yellow };

class Item {
private:
    std::string name;
    ItemType type;
    int power;
    int quantity;
    std::string location;

public:
    Item(const std::string& name, ItemType type, int power, int quantity, const std::string& location);

    std::string getName() const;
    ItemType getType() const;
    int getPower() const;
    int getQuantity() const;
    std::string getLocation() const;

    void setLocation(const std::string& loc);
    void decreaseQuantity();
    void increaseQuantity();

    std::string getTypeString() const;
};

#endif 

