#include "Monsters2.h"
#include "Monster.h"
#include "Character.h"
#include "Hero.h"
#include "Board.h"

Dracula::Dracula() : Monster("Dracula", "Crypt", "Dark Charm", 
                   "Charm the current hero and pull them toward Dracula" , MonsterType::Dracula) {
    for (int i = 0; i < 4; i++) {
        coffinsDestroyed[i] = false;
    }
}

void Dracula::usePower(Hero* hero, Board* board) {
    std::cout << "Dracula uses Dark Charm on " << hero->getName() << std::endl;
    hero->setLoc(this->getLoc());
    std::cout << hero->getName() << " is moved to " <<hero->getLoc() << std::endl;
}

bool Dracula::isDefeated() const {
    for (int i = 0; i < 4; i++) {
        if (!coffinsDestroyed[i]) return false;
    }
    return true;
}

void Dracula::destroyCoffin(int index) {
    if (index >= 0 && index < 4) {
        coffinsDestroyed[index] = true;
        std::cout << "Coffin destroyed! " << (4 - getCoffinsDestroyedCount()) << " coffins remaining." << std::endl;
    }
}

int Dracula::getCoffinsDestroyedCount() const {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (coffinsDestroyed[i]) count++;
    }
    return count;
}

int Dracula::getCoffinIndexByLocation(const std::string& location) const {
    if (location == "Cave") return 0;
    if (location == "Dungeon") return 1;
    if (location == "Crypt") return 2;
    if (location == "Graveyard") return 3;
    return -1;
}

InvisibleMan::InvisibleMan() : Monster("Invisible Man", "Inn", "Unseen Stalk", 
                        "Stalk a villager and move two spaces toward them" , MonsterType::InvisibleMan) {
    for (int i = 0; i < 5; i++) {
        evidenceCollected[i] = false;
    }
}

void InvisibleMan::usePower(Hero* hero, Board* board) {
    std::cout << "Invisible Man uses Unseen Stalk!" << std::endl;
    std::string monsterLoc = this->getLoc();
    std::string targetLoc = "";
    int minDistance = INT_MAX;

    std::vector<std::string> possibleVillagerLocations;
    for (const auto& loc : board->getLocations()) {
        if (loc != monsterLoc) {
            possibleVillagerLocations.push_back(loc);
        }
    }

    if (possibleVillagerLocations.empty()) {
        std::cout << "No villagers found on the board to stalk!" << std::endl;
        return;
    }

    targetLoc = possibleVillagerLocations[rand() % possibleVillagerLocations.size()];

    std::cout << "Invisible Man targets a villager at " << targetLoc << std::endl;

    for (int i = 0; i < 2; i++) {
        std::string nextLoc = board->findShortestPath(monsterLoc, targetLoc);

        if (nextLoc == monsterLoc || nextLoc.empty()) { 
            break;
        }

        setLoc(nextLoc);
        monsterLoc = nextLoc;

        std::cout << "Invisible Man moved to " << monsterLoc << std::endl;

        if (monsterLoc == targetLoc) {
            break;
        }
    }
}

bool InvisibleMan::isDefeated() const {
    for (int i = 0; i < 5; i++) {
        if (!evidenceCollected[i]) return false;
    }
    return true;
}

void InvisibleMan::collectEvidence(int index) {
    if (index >= 0 && index < 5) {
        evidenceCollected[index] = true;
        std::cout << "Evidence collected! " << (5 - getEvidenceCollectedCount()) << " pieces of evidence remaining." << std::endl;
    }
}

int InvisibleMan::getEvidenceCollectedCount() const {
    int count = 0;
    for (int i = 0; i < 5; i++) {
        if (evidenceCollected[i]) count++;
    }
    return count;
}

int InvisibleMan::getEvidenceIndexByLocation(const std::string& location) const {
    if (location == "Institute") return 0;
    if (location == "Laboratory") return 1;
    if (location == "Barn") return 2;
    if (location == "Mansion") return 3;
    if (location == "Inn") return 4;
    return -1;
}

