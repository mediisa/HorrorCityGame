#include "PerkCard.h"

PerkCard::PerkCard(const std::string& name, int quantity, PerkCardType type)
    : Card(name), quantity(quantity), type(type) {}

PerkCardType PerkCard::getType() const {
    return type; 
}
int PerkCard::getQuantity() const {
    return quantity;
}

void PerkCard::play() {
    std::cout << "Playing Perk Card: " << name << std::endl;
}

std::string PerkCard::getDetails() const {
    std::string typeStr;
    switch (type) {
        case VisitFromTheDetective:{
            typeStr = "Visit from the Detective"; 
            break;
        }case BreakOfDawn:{
            typeStr = "Break of Dawn";
            break;
        }case Overstock:{
            typeStr = "Overstock";
            break;
        }case LateIntoTheNight:{
            typeStr = "Late into the Night";
            break;
        }case Repel:{
            typeStr = "Repel";
            break;
        }case Hurry:{
            typeStr = "Hurry";
            break;
        }
    }
    return typeStr;
}

std::vector<PerkCard> PerkCard::createPerkCards() {
    std::vector<PerkCard> cards;
    cards.push_back(PerkCard("VisitFromTheDetective", 3, VisitFromTheDetective));
    cards.push_back(PerkCard("BreakOfDawn", 3, BreakOfDawn));
    cards.push_back(PerkCard("Overstock", 4, Overstock));
    cards.push_back(PerkCard("LateIntoTheNight", 4, LateIntoTheNight));
    cards.push_back(PerkCard("Repel", 3, Repel));
    cards.push_back(PerkCard("Hurry", 3, Hurry));
    return cards;
}

