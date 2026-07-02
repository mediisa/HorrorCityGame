#ifndef MONSTERCARD_HPP
#define MONSTERCARD_HPP

#include "Card.h"
#include <vector>
#include <string>

enum MonsterCardType { 
    FormOfTheBat, Sunrise, Thief, TheDelivery, FortuneTeller, FormerEmployer, 
    HurriedAssistant, TheInnocent, EgyptianExpert, TheIchthyologist 
};

class MonsterCard : public Card {
private:
    MonsterCardType type;
    int itemsToAdd;
    std::vector<std::string> monsterStrikeOrder;
    int movementUnits;
    int diceCount;

public:
    MonsterCard(const std::string& name, MonsterCardType type, int itemsToAdd, 
                const std::vector<std::string>& monsterStrikeOrder, int movementUnits, int diceCount);

    MonsterCardType getType() const;
    int getItemsToAdd() const;
    const std::vector<std::string>& getMonsterStrikeOrder() const;
    int getMovementUnits() const;
    int getDiceCount() const;

    void play() override; 

    static std::vector<MonsterCard> createMonsterCards();
};

#endif

