#ifndef BOARD_HPP
#define BOARD_HPP

#include "Item.h"

#include <vector>
#include <string>
#include <map>
#include <algorithm> 
#include <iostream>
#include <climits> 


class Hero;
class Villager;

class Board {
private:
    std::vector<std::string> locations;
    std::map<std::string, std::vector<std::string>> paths;
    std::map<std::string, std::vector<Item>> items;
    int terrorLevel;

public:
    Board();

    void initializeBoard();
    void addPath(const std::string& loc1, const std::string& loc2);
    void initializeItems();

    const std::vector<std::string>& getLocations() const;
    const std::map<std::string, std::vector<std::string>>& getPaths() const;
    const std::map<std::string, std::vector<Item>>& getItems() const;

    bool areNeighbors(const std::string& loc1, const std::string& loc2) const;

    void addItem(const std::string& location, const Item& item);
    void removeItem(const std::string& location, const std::string& itemName);
    void removeItems(const std::string& location);

    int getTerrorLevel() const;
    void increaseTerrorLevel();

    std::string findShortestPath(const std::string& start, const std::string& end);

    int calculateDistance(const std::string& loc1 , const std::string& loc2) const;
};

#endif 

