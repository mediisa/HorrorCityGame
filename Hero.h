#ifndef HERO_HPP
#define HERO_HPP

#include "Character.h"
#include "Monster.h"
#include "Monsters2.h"
#include "Villager.h"
#include "Item.h"
#include "PerkCard.h"
#include "Board.h" 

#include <vector>
#include <string>
#include <memory>
#include <iostream> 

enum class HeroType{Archaeologist , Mayor , Scientist , Courier };

class Hero : public Character {
private:
    int actionPoints;
    bool hasSpecialAction;
    std::string specialActionDescription;
    std::vector<Item> inventory;
    std::vector<PerkCard> perkCards;
    HeroType htype;

public:
    Hero(const std::string& name, const std::string& location, HeroType htype, int actionPoints, 
         bool hasSpecialAction, const std::string& specialActionDescription);

    int getActionPoints() const;
    bool getHasSpecialAction() const;
    std::string getSpecialActionDescription() const;
    HeroType getHerotyoe() const;

    const std::vector<Item>& getInventory() const;
    const std::vector<PerkCard>& getPerkCards() const;

    void addItem(const Item& item);  
    void removeItem(const std::string& itemName);

    void addPerkCard(const PerkCard& card);
    void removePerkCard(const std::string& cardName);

    void move(Board* board , std::vector<Villager>& villagers , std::vector<PerkCard>& perkCardDeck);
    void guide(Board* board, std::vector<Villager>& villagers, std::vector<PerkCard>& perkCardDeck);
    void pickUp(Board* board);
    void advance(Board* board, std::vector<std::unique_ptr<Monster>>& monsters);
    void advanceAgainstDracula(Board* board , Dracula* dracula);
    void advanceAgainstInvisibleMan(Board* board , InvisibleMan* invisibleMan);
    void defeat( Board* board , std::vector<std::unique_ptr<Monster>>& monsters);
    void defeatDracula(Board* board , Dracula* dracula, std::vector<std::unique_ptr<Monster>>& monsters);
    void defeatInvisibleMan(Board* board , InvisibleMan* InvisibleMan , std::vector<std::unique_ptr<Monster>>& monsters);
    
    bool canAdvenceDracula(std::string& heroloc);
    bool canAdvenceInvisibleMan(std::string& heroloc);


    virtual void specialAction(Board* board) = 0; 

};

#endif 

