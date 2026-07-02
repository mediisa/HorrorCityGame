#include "Character.h"
#include "Monster.h"
#include "Player.h"
#include "Villager.h"
#include "Board.h"
#include "Hero.h"
#include "Dice.h"
#include <algorithm>
#include <climits>

Monster::Monster(const std::string& name, const std::string& location, 
        const std::string& power, const std::string& powerDescription , MonsterType mtype)
    : Character(name, location , Typechara::MONSTER), power(power), powerDescription(powerDescription) , mtype(mtype) {}

std::string Monster::getPower() const{
    return power;
}
std::string Monster::getPowerDescription() const {
    return powerDescription; 
}
MonsterType Monster::getmonstertype() const{
    return mtype;
}

void Monster::strike(int movementUnits , int diceCount , const std::vector<Player>& players , std::vector<Villager>& villagers , Board* board ){
    std::cout<<this->getName()<<"is striking! \n";

    bool monsterWithPerson = false;
    std::string monsterloc = this->getLoc();

    for(const auto& player : players ){
        if(player.getHero() && player.getHero()->getLoc() == monsterloc){
            monsterWithPerson = true;
            break;
        }
    }

    if(!monsterWithPerson){
        for ( auto& villager : villagers) {
            if (villager.getLoc() == monsterloc) {
                monsterWithPerson = true;
                break;
            }
        }  
    }
    if(!monsterWithPerson){
        moveTowardPerson(movementUnits , players , villagers , board);
        monsterloc = getLoc();
    }else{
        std::cout<<getName()<<" is already with a person at "<<monsterloc<<".  \n";
    }

    std::cout<<"Rolling "<<diceCount<<"dice for "<<getName()<<std::endl;

    std::vector<int> diceResults = Dice::rollMultiple(diceCount);
        std::cout<<"Dice results: ";
        for (int result : diceResults) {
            if (result == 0){
                std::cout<<"Blank \n";
            }else if (result == 1){
                std::cout << "Power(!) \n";
            }else if (result == 2){
                std::cout << "Attack(*)  \n";
            }
        }
    
    std::vector<Hero*> heroinLoc;
    std::vector<Villager*> villinLoc;

    for(const auto& player : players){
        if(player.getHero() && player.getHero()->getLoc() == monsterloc){
            heroinLoc.push_back(player.getHero());
        }
    }

    for(const auto& vill : villagers){
        if(vill.getLoc() == monsterloc){
            villinLoc.push_back(const_cast<Villager*>(&vill));
        }
    }

    if((!heroinLoc.empty() || !villinLoc.empty()) && Dice::hasAttack(diceResults)){
        if (!heroinLoc.empty()) {
            this->attackHero(heroinLoc[0],board);
        } else {
            this->attackVillager(villinLoc[0],board,villagers);
        }
    } else if (Dice::hasPower(diceResults)) {
        if (!heroinLoc.empty()) {
            this->usePower(heroinLoc[0], board);
        }   
    }
}

void Monster::moveTowardPerson(int units,const std::vector<Player>& Players , const std::vector<Villager>& villagers , Board* board){
    std::string monsterloc = this->getLoc();
    std::string targetloc ;
    int minDistance = INT_MAX;

    for(const auto& player : Players){
        if(player.getHero()){
            std::string heroloc = player.getHero()->getLoc();
            if(heroloc != monsterloc){
                int distance = board->calculateDistance(monsterloc, heroloc);
                if(distance != INT_MAX && distance < minDistance){
                    minDistance = distance;
                    targetloc = heroloc;
                }
            }
        }
    }
    
    for(const auto& vill : villagers){
        std::string villloc = vill.getLoc();
        if(villloc != monsterloc){
            int distance = board->calculateDistance(monsterloc, villloc);
            if(distance != INT_MAX && distance < minDistance){
                minDistance = distance;
                targetloc = villloc;
            }    
        }
    }

    if(targetloc.empty()){
        std::cout<<this->getName()<<" has no target to move toward. \n";
    }else{
        std::cout<<this->getName()<<" moving towards "<<targetloc<<" .  \n";

        for(int i=0 ; i < units ; i++ ){
            std::string nextloc = board->findShortestPath(monsterloc , targetloc);

            if(nextloc == monsterloc){
                std::cout<<this->getName()<<" cannot move further towards target. \n";
                break;
            }
            setLoc(nextloc);
            monsterloc=nextloc;
            std::cout<<this->getName()<<" moved to "<<monsterloc<<std::endl;

            bool reachedP = false;
            for (const auto& player : Players) {
                if (player.getHero() && player.getHero()->getLoc() == monsterloc) {
                    reachedP = true;
                    break;
                }
            }

            if (!reachedP) {
                for (const auto& villager : villagers) {
                    if (villager.getLoc() == monsterloc) {
                        reachedP = true;
                        break;
                    }
                }
            }

            if(reachedP || monsterloc == targetloc){
                std::cout<<this->getName()<<" reached "<<(reachedP ? "a person " : "target location ")<<"and stopped moving. \n ";
                break;
            }
        }
    }
}

void Monster::attackHero(Hero* hero , Board* board){
    std::cout<<getName()<<" attacks "<<hero->getName()<<"! \n";

    const auto& inventory = hero->getInventory();
    if(!inventory.empty()){
        std::cout<<hero->getName()<<" has items to defend. Use an item? (y/n) \n";
        char a;
        std::cin>>a;
        if(a == 'y' || a == 'Y'){
            std::cout<<"Choose an item to use for defense: \n";
            for(size_t i=0 ; i < inventory.size() ; i++ ){
                std::cout<<(i+1)<<". "<<inventory[i].getName()<<" ("<<inventory[i].getTypeString()<<", Power: "<<inventory[i].getPower()<<") \n";
            }
            int itemchoice;
            std::cin>>itemchoice;

            if(itemchoice>0 && itemchoice <= static_cast<int>(inventory.size())){
                std::string itemName = inventory[itemchoice-1].getName();
                hero->removeItem(itemName);
                std::cout<<hero->getName()<<" used "<<itemName<<" to defend against the attack! \n";
                return;
            }else{
                std::cout<<"Invalid item choice. No item use. \n";
            }
        } 
    }

    std::cout<<hero->getName()<<" takes damage! \n";
    board->increaseTerrorLevel();
    hero->setLoc("Hospital");
    std::cout<<hero->getName()<<" will start next turn at the hospital \n";

}

void Monster::attackVillager(Villager* villager , Board* board , std::vector<Villager>& villagers){
    std::cout<<getName()<<" attacks "<<villager->getName()<<"! \n";
    std::cout<<villager->getName()<<" is killed ! \n";

    board->increaseTerrorLevel();
    for (auto it = villagers.begin(); it != villagers.end(); ++it) {
        if (it->getName() == villager->getName()) {
            villagers.erase(it);
            break;
        }
    }
}

