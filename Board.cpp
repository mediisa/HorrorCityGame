#include "Board.h"

Board::Board() : terrorLevel(0) {
    initializeBoard();
}

void Board::initializeBoard() {
    
    locations = {"Inn", "Barn", "Crypt", "Precinct", "Dungeon", "Cave", "Camp", "Abbey", "Tower",
                "Mansion", "Museum", "Shop", "Docks", "Theatre", "Church", "Laboratory",
                "Institute", "Hospital", "Graveyard"};


    addPath("Cave", "Camp");
    addPath("Camp", "Inn");
    addPath("Camp", "Precinct");
    addPath("Camp", "Mansion");
    addPath("Inn", "Mansion");
    addPath("Precinct", "Mansion");
    addPath("Theatre", "Mansion");
    addPath("Camp", "Barn");
    addPath("Camp", "Theatre");
    addPath("Crypt", "Abbey");
    addPath("Abbey", "Mansion");
    addPath("Hospital", "Church");
    addPath("Graveyard", "Church");
    addPath("Institute", "Laboratory");
    addPath("Docks", "Tower");
    addPath("Dungeon", "Tower");
    addPath("Shop", "Laboratory");
    addPath("Theatre", "Tower");
    addPath("Theatre", "Shop");
    addPath("Church", "Museum");
    addPath("Church", "Mansion");
    addPath("Museum", "Mansion");
    addPath("Theatre", "Barn");
    addPath("Theatre", "Inn");
    addPath("Theatre", "Precinct");
    addPath("Inn", "Precinct");
    addPath("Barn", "Precinct");
    addPath("Barn", "Inn");

    
    initializeItems();
}

void Board::addPath(const std::string& loc1, const std::string& loc2) {
    paths[loc1].push_back(loc2);
    paths[loc2].push_back(loc1);
}

void Board::initializeItems() {
    std::vector<std::pair<std::string, std::string>> itemLocations = {
        {"Flower_Docks", "Docks"}, {"TarotDeck_Camp", "Camp"}, {"Garlic_Inn", "Inn"},
        {"MirroredBox_Mansion", "Mansion"}, {"Stake_Abbey", "Abbey"}, {"ScrollOfThoth_Museum", "Museum"},
        {"Violin_Camp", "Camp"}, {"Tablet_Museum", "Museum"}, {"Wolfsbane_Camp", "Camp"},
        {"Charm_Camp", "Camp"}, {"Dart_Inn", "Inn"}, {"FirePoker_Mansion", "Mansion"},
        {"Rapier_Theatre", "Theatre"}, {"Shovel_Graveyard", "Graveyard"}, {"Torch_Barn", "Barn"},
        {"Pitchfork_Barn", "Barn"}, {"Rifle_Barn", "Rifle"}, {"SilverCane_Shop", "Shop"}, // Rifle was duplicated, fixing here
        {"Knife_Docks", "Docks"}, {"Pistol_Precinct", "Precinct"}, {"BearTrap_Shop", "Shop"},
        {"Speargun_Institute", "Institute"}, {"AnatomyText_Institute", "Institute"},
        {"Centrifuge_Laboratory", "Laboratory"}, {"Kite_Tower", "Tower"}, {"Research_Tower", "Tower"},
        {"Telescope_Mansion", "Mansion"}, {"Searchlight_Precinct", "Precinct"}, {"Experiment_Laboratory", "Laboratory"},
        {"Analysis_Institute", "Institute"}, {"Rotenone_Institute", "Institute"}, {"CosmicRayDiffuser_Tower", "Tower"},
        {"Nebularium_Tower", "Tower"}, {"MonocaneMixture_Inn", "Inn"}, {"Fossil_Camp", "Camp"}
    };

    for (const auto& pair : itemLocations) {
        const std::string& name = pair.first;
        const std::string& loc = pair.second;
        ItemType type;
        int effect;

        if (name.find("Flower") != std::string::npos || name.find("Garlic") != std::string::npos) {
            type = Yellow; effect = 2;
        } else if (name.find("MirroredBox") != std::string::npos || name.find("Stake") != std::string::npos ||
                  name.find("Violin") != std::string::npos || name.find("Tablet") != std::string::npos ||
                  name.find("HolyWater") != std::string::npos || name.find("Cross") != std::string::npos) { 
            type = Yellow; effect = 3;
        } else if (name.find("ScrollOfThoth") != std::string::npos || name.find("Wolfsbane") != std::string::npos ||
                  name.find("Charm") != std::string::npos) {
            type = Yellow; effect = 4;
        } else if (name.find("AnatomyText") != std::string::npos || name.find("Centrifuge") != std::string::npos ||
                  name.find("Kite") != std::string::npos) {
            type = Blue; effect = 1;
        } else if (name.find("Research") != std::string::npos || name.find("Telescope") != std::string::npos ||
                  name.find("Searchlight") != std::string::npos || name.find("Experiment") != std::string::npos ||
                  name.find("Analysis") != std::string::npos) {
            type = Blue; effect = 2;
        } else if (name.find("Rotenone") != std::string::npos || name.find("CosmicRayDiffuser") != std::string::npos ||
                  name.find("Nebularium") != std::string::npos || name.find("MonocaneMixture") != std::string::npos ||
                  name.find("Fossil") != std::string::npos) {
            type = Blue; effect = 3;
        } else if (name.find("Rifle") != std::string::npos || name.find("SilverCane") != std::string::npos ||
                  name.find("Pistol") != std::string::npos) {
            type = Red; effect = 6;
        } else if (name.find("Torch") != std::string::npos || name.find("Rapier") != std::string::npos) {
            type = Red; effect = 5;
        } else if (name.find("Pitchfork") != std::string::npos || name.find("BearTrap") != std::string::npos ||
                  name.find("Speargun") != std::string::npos || name.find("Lantern") != std::string::npos) { 
            type = Red; effect = 4;
        } else if (name.find("FirePoker") != std::string::npos || name.find("Knife") != std::string::npos) {
            type = Red; effect = 3;
        } else {
            type = Red; effect = 2; 
        }

        Item item(name, type, effect, 1, loc);
        items[loc].push_back(item);
    }
}

const std::vector<std::string>& Board::getLocations() const{ 
    return locations; 
}
const std::map<std::string, std::vector<std::string>>& Board::getPaths() const{
    return paths;
}
const std::map<std::string, std::vector<Item>>& Board::getItems() const{
    return items;
}

bool Board::areNeighbors(const std::string& loc1, const std::string& loc2) const{
    auto it = paths.find(loc1);
    if (it != paths.end()) {
        const auto& neighbors = it->second;
        return std::find(neighbors.begin(), neighbors.end(), loc2) != neighbors.end();
    }
    return false;
}

void Board::addItem(const std::string& location, const Item& item) {
    items[location].push_back(item);
}

void Board::removeItem(const std::string& location, const std::string& itemName) {
    auto& locationItems = items[location];
    auto it = std::find_if(locationItems.begin(), locationItems.end(), 
                          [&itemName](const Item& item) { return item.getName() == itemName; });
    if (it != locationItems.end()) {
        locationItems.erase(it);
    }
}

void Board::removeItems(const std::string& location) {
    items[location].clear();
}

int Board::getTerrorLevel() const{
    return terrorLevel; 
}

void Board::increaseTerrorLevel() {
    terrorLevel++;
    std::cout << "Terror Level increased to " << terrorLevel << "!" << std::endl;
    if (terrorLevel >= 5) {
        std::cout << "Terror Level has reached maximum! The town is lost!" << std::endl;
    }
}

std::string Board::findShortestPath(const std::string& start, const std::string& end) {
    
    std::map<std::string, std::string> parent;
    std::vector<std::string> queue;

    queue.push_back(start);
    parent[start] = "";

    size_t head = 0; 
    while (head < queue.size()) {
        std::string current = queue[head++];

        if (current == end) {
            break;
        }

        
        if (paths.count(current)) {
            for (const auto& neighbor : paths.at(current)) {
                if (parent.find(neighbor) == parent.end()) {
                    queue.push_back(neighbor);
                    parent[neighbor] = current;
                }
            }
        }
    }

    if (parent.find(end) == parent.end()) {
        return start; 
    }

    
    std::string current = end;
    std::string nextStep = end; 

    while (parent[current] != start) {
        nextStep = current;
        current = parent[current];
    }

    
    if (current == start) {
        return nextStep; 
    }

    return start;
}

int Board::calculateDistance(const std::string& loc1 , const std::string& loc2) const{
    
    std::map<std::string, int> distance;
    std::vector<std::string> queue;

    queue.push_back(loc1);
    distance[loc1] = 0;

    size_t head = 0; 
    while (head < queue.size()) {
        std::string current = queue[head++];

        if (current == loc2) {
            break;
        }

        if (getPaths().count(current)) {
            for (const auto& neighbor : getPaths().at(current)) {
                if (distance.find(neighbor) == distance.end()) {
                    queue.push_back(neighbor);
                    distance[neighbor] = distance[current] + 1;
                }
            }
        }
    }

    if (distance.find(loc2) == distance.end()) {
        return INT_MAX; 
    }

    return distance[loc2];

}
