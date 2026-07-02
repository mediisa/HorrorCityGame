#include "MonsterCard.h"
#include <iostream> 

MonsterCard::MonsterCard(const std::string& name, MonsterCardType type, int itemsToAdd, 
            const std::vector<std::string>& monsterStrikeOrder, int movementUnits, int diceCount)
    : Card(name), type(type), itemsToAdd(itemsToAdd), 
      monsterStrikeOrder(monsterStrikeOrder), movementUnits(movementUnits), diceCount(diceCount) {}

MonsterCardType MonsterCard::getType() const {
    return type; 
}
int MonsterCard::getItemsToAdd() const {
    return itemsToAdd; 
}
const std::vector<std::string>& MonsterCard::getMonsterStrikeOrder() const {
    return monsterStrikeOrder; 
}
int MonsterCard::getMovementUnits() const {
    return movementUnits; 
}
int MonsterCard::getDiceCount() const {
    return diceCount; 
}

void MonsterCard::play() {
    std::cout << "Playing Monster Card: " << name << std::endl;
}

std::vector<MonsterCard> MonsterCard::createMonsterCards() {
    std::vector<MonsterCard> cards;

    for (int i = 0; i < 3; i++) {
        cards.push_back(MonsterCard("Form of the Bat", FormOfTheBat, 2, 
                                   {"Invisible Man"}, 1, 2));
    }

    for (int i = 0; i < 3; i++) {
        cards.push_back(MonsterCard("Sunrise", Sunrise, 0, 
                                   {"Invisible Man", "Frenzied Monster"}, 1, 2));
    }

    for (int i = 0; i < 5; i++) {
        cards.push_back(MonsterCard("Thief", Thief, 2, 
                                   {"Invisible Man", "Dracula"}, 1, 3));
    }

    cards.push_back(MonsterCard("The Delivery", TheDelivery, 3, 
                               {"Frenzied Monster"}, 1, 3));

    cards.push_back(MonsterCard("Fortune Teller", FortuneTeller, 3, 
                               {"Monster", "Frenzied Monster"}, 1, 2));

    cards.push_back(MonsterCard("Former Employer", FormerEmployer, 3, 
                               {"Invisible Man", "Frenzied Monster"}, 1, 2));

    cards.push_back(MonsterCard("Hurried Assistant", HurriedAssistant, 3, 
                               {"Dracula"}, 2, 3));

    cards.push_back(MonsterCard("The Innocent", TheInnocent, 3, 
                               {"Frenzied Monster", "Dracula", "Invisible Man"}, 1, 3));

    cards.push_back(MonsterCard("Egyptian Expert", EgyptianExpert, 3, 
                               {"Dracula", "Frenzied Monster"}, 2, 2));

    cards.push_back(MonsterCard("The Ichthyologist", TheIchthyologist, 3, 
                               {"Frenzied Monster"}, 1, 2));

    return cards;
}

