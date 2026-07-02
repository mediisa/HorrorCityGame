#ifndef PERKCARD_HPP
#define PERKCARD_HPP

#include "Card.h"
#include <vector>
#include <string>
#include <iostream> 

enum PerkCardType { VisitFromTheDetective, BreakOfDawn, Overstock, LateIntoTheNight, Repel, Hurry };

class PerkCard : public Card {
private:
    PerkCardType type;
    int quantity;

public:
    PerkCard(const std::string& name, int quantity, PerkCardType type);

    PerkCardType getType() const;
    int getQuantity() const;
    void play() override; 

    std::string getDetails() const;

    static std::vector<PerkCard> createPerkCards();
};

#endif 

