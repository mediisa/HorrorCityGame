#include "Heros4.h"

Archaeologist::Archaeologist() : Hero("Archaeologist", "Docks", HeroType::Archaeologist , 4, true, 
                      "Pick up any number of items from a neighboring location") {}
              

void Archaeologist::specialAction(Board* board){
    std::cout << "Using Archaeologist's special ability to pick up items from a neighboring location." << std::endl;

    std::string heroLoc = this->getLoc();
    const auto& neighbors = board->getPaths().at(heroLoc);

    std::cout << "Choose a neighboring location:" << std::endl;
    for (size_t i = 0; i < neighbors.size(); i++) {
        std::cout << (i+1) << ". " << neighbors[i] << std::endl;
    }

    int locationChoice;
    std::cin >> locationChoice;

    if (locationChoice <= 0 || locationChoice > static_cast<int>(neighbors.size())) {
        std::cout << "Invalid location. Special action failed." << std::endl;
        return;
    }

    std::string neighborLoc = neighbors[locationChoice - 1];
    const auto& locationItems = board->getItems().at(neighborLoc);

    if (locationItems.empty()) {
        std::cout << "No items at " << neighborLoc << std::endl;
        return;
    }

    std::cout << "Items at " << neighborLoc << ":" << std::endl;
    for (size_t i = 0; i < locationItems.size(); i++) {
        std::cout << (i+1) << ". " << locationItems[i].getName() 
                  << " (" << locationItems[i].getTypeString() 
                  << ", Power: " << locationItems[i].getPower() << ")" << std::endl;
    }

    std::cout << "Choose an item to pick up (0 to cancel): ";
    int itemChoice;
    std::cin >> itemChoice;

    if (itemChoice <= 0 || itemChoice > static_cast<int>(locationItems.size())) {
        std::cout << "No item picked up." << std::endl;
        return;
    }

    Item item = locationItems[itemChoice - 1];
    this->addItem(item);
    board->removeItem(neighborLoc, item.getName());

    std::cout << "Picked up " << item.getName() << " from " << neighborLoc << std::endl;
}


Mayor::Mayor() : Hero("Mayor", "Theatre", HeroType::Mayor , 5, false, "No special action") {}

void Mayor::specialAction(Board* board) {
    std::cout << "The Mayor has no special action." << std::endl;
}

Scientist::Scientist() : Hero("Scientist", "Institute",HeroType::Scientist , 4, false, 
                  "When using any item, increase its power by one unit") {}

void Scientist::specialAction(Board* board) {
    std::cout << "The Scientist's ability is passive and applies automatically when using items." << std::endl;
}

Courier::Courier() : Hero("Courier", "Shop",HeroType::Courier, 4, true, 
                "Move to a location where another hero is present") {}


void Courier::specialAction(Board* board) {
    std::cout << "Using Courier's special ability to move to another hero's location." << std::endl;
    std::vector<std::string> potentialHeroLocations;
    
    for (const auto& loc : board->getLocations()) {
        if (loc != this->getLoc()) {
            potentialHeroLocations.push_back(loc);
        }
    }

    if (potentialHeroLocations.empty()) {
        std::cout << "No other locations to move to (or no other heroes to follow)!" << std::endl;
        return;
    }

    std::cout << "Choose a location to move to (simulating another hero's location):" << std::endl;
    for (size_t i = 0; i < potentialHeroLocations.size(); i++) {
        std::cout << (i+1) << ". " << potentialHeroLocations[i] << std::endl;
    }

    int choice;
    std::cin >> choice;

    if (choice <= 0 || choice > static_cast<int>(potentialHeroLocations.size())) {
        std::cout << "Invalid choice. Special action failed." << std::endl;
        return;
    }

    std::string newLoc = potentialHeroLocations[choice - 1];
    this->setLoc(newLoc);
    std::cout << "Moved to " << newLoc << std::endl;
}