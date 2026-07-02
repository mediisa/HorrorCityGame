#include "Hero.h"

Hero::Hero(const std::string& name, const std::string& location , HeroType htype , int actionPoints, 
         bool hasSpecialAction, const std::string& specialActionDescription)
    : Character(name, location , Typechara::HERO), actionPoints(actionPoints), 
      hasSpecialAction(hasSpecialAction), specialActionDescription(specialActionDescription) , htype(htype) {}

int Hero::getActionPoints() const {
    return actionPoints; 
}
bool Hero::getHasSpecialAction() const { 
    return hasSpecialAction; 
}
std::string Hero::getSpecialActionDescription() const {
    return specialActionDescription; 
}
HeroType Hero::getHerotyoe() const {
    return htype;
}

const std::vector<Item>& Hero::getInventory() const { 
    return inventory;
}
const std::vector<PerkCard>& Hero::getPerkCards() const {
    return perkCards; 
}

void Hero::addItem(const Item& item) {
    inventory.push_back(item);
}

void Hero::removeItem(const std::string& itemName) {
    auto it = inventory.begin();
    while (it != inventory.end()){
        if(it->getName() == itemName){
            it = inventory.erase(it);
        }else{
            it++;
        }
    }
}

void Hero::addPerkCard(const PerkCard& card) {
    perkCards.push_back(card);
}

void Hero::removePerkCard(const std::string& cardName) {
    auto it = perkCards.begin();
    while(it != perkCards.end()){
        if(it->getName() == cardName){
            it = perkCards.erase(it);
        }else{
            it++;
        }
    }
}

void Hero::move(Board* board , std::vector<Villager>& villagers , std::vector<PerkCard>& perkCardDeck){
    std::string currentloc = getLoc();
    if (board->getPaths().find(currentloc) == board->getPaths().end()) {
        std::cout<<"Error: Current location '"<<currentloc<<"' has no defined paths. \n";
        return;
    }

    const auto& neighbors = board->getPaths().at(currentloc);
    if (neighbors.empty()) {
        std::cout << "No neighboring locations to move to from " << currentloc << std::endl;
        return;
    }

    std::cout << "Choose a location to move to:" << std::endl;
    for (size_t i = 0; i < neighbors.size(); i++) {
        std::cout << (i+1) << ". " << neighbors[i] << std::endl;
    }

    int a;
    std::cin>>a;

    if (a <= 0 || a > static_cast<int>(neighbors.size())) {
        std::cout << "Invalid choice. Movement failed." << std::endl;
        return;
    }

    std::string newLoc = neighbors[a-1];
    setLoc(newLoc);
    std::cout << "Moved to " << newLoc << std::endl;

    std::vector<Villager*> villagersAtCurrentLoc;
    for (auto& villager : villagers) {
        if (villager.getLoc() == currentloc) {
            villagersAtCurrentLoc.push_back(&villager);
        }
    }

    for (auto* villagerPtr : villagersAtCurrentLoc) {
        std::cout << "Do you want to move " << villagerPtr->getName() << " with you? (y/n): ";
        char moveChoice;
        std::cin >> moveChoice;

        if (moveChoice == 'y' || moveChoice == 'Y') {
            villagerPtr->setLoc(newLoc);
            std::cout << villagerPtr->getName() << " moved to " << newLoc << std::endl;
            if (villagerPtr->isAtSafeLocation()) {
                std::cout << villagerPtr->getName() << " has reached their safe location!" << std::endl;
                if (!perkCardDeck.empty()) {
                    addPerkCard(perkCardDeck.back());
                    std::cout << "Received Perk Card: " << perkCardDeck.back().getDetails() << std::endl;
                    perkCardDeck.pop_back();
                } else {
                    std::cout << "Perk card deck is empty, no card received." << std::endl;
                }
                auto it = std::find_if(villagers.begin(), villagers.end(),
                                      [&villagerPtr](const Villager& v) { return v.getName() == villagerPtr->getName(); });
                if (it != villagers.end()) {
                    villagers.erase(it);
                    std::cout << villagerPtr->getName() << " safely removed from the game." << std::endl;
                }
            }
        }
    }
}

void Hero::guide(Board* board, std::vector<Villager>& villagers, std::vector<PerkCard>& perkCardDeck){
    std::string heroloc = getLoc();
    std::vector<std::pair<Villager* , std::string>> availableVill;

    for (auto& villager : villagers) {
        if (villager.getLoc() == heroloc) {
            if (board->getPaths().count(heroloc)) { 
                const auto& neighbors = board->getPaths().at(heroloc);
                for (const auto& neighbor : neighbors) {
                    availableVill.push_back({&villager, neighbor});
                }
            }
        } else if (board->areNeighbors(heroloc, villager.getLoc())) {
            availableVill.push_back({&villager, heroloc});
        }
    }

    if(availableVill.empty()){
        std::cout << "No villagers to guide! \n";
        return;
    }

    std::cout << "Choose a villager to guide: \n";
    for (size_t i = 0; i < availableVill.size(); i++) {
        std::cout << (i+1) << ". " << availableVill[i].first->getName() << " from " << availableVill[i].first->getLoc()<< " to " << availableVill[i].second <<std::endl;
    }

    int a;
    std::cin>>a;
    if (a <= 0 || a > static_cast<int>(availableVill.size())) {
        std::cout << "Invalid choice. Guide action failed. \n";
    }

    Villager* villager = availableVill[a-1].first;
    std::string newLoc = availableVill[a-1].second;

    villager->setLoc(newLoc);
    std::cout<<villager->getName()<<"guided to "<<newLoc<<std::endl;

    if(villager->isAtSafeLocation()){
        std::cout << villager->getName() << " has reached their safe location! \n";
        if (!perkCardDeck.empty()) {
            addPerkCard(perkCardDeck.back());
            std::cout << "Received Perk Card: " << perkCardDeck.back().getDetails() << std::endl;
            perkCardDeck.pop_back();
        } else {
            std::cout << "Perk card deck is empty, no card received." << std::endl;
        }

        auto it = std::find_if(villagers.begin(), villagers.end(),
                   [villager](const Villager& v) { return v.getName() == villager->getName(); });
        if (it != villagers.end()) {
            villagers.erase(it);
            std::cout << villager->getName() << " safely removed from the game." << std::endl;
        }
    }
}

void Hero::pickUp(Board* board){
    std::string heroloc = getLoc();
    const auto& locItems = board->getItems().at(heroloc);

    if (board->getItems().find(heroloc) == board->getItems().end()) {
        std::cout << "No items to pick up at "<<heroloc<<std::endl;
        return;
    }

    if(locItems.empty()){
        std::cout << "No items to pick up at " <<heroloc<<std::endl; 
        return;  
    }

    std::cout<<"ITEMS at "<<heroloc<<" : \n";
    for (size_t i = 0; i < locItems.size(); i++) {
        std::cout <<(i+1)<<". "<<locItems[i].getName()<<" ("<<locItems[i].getTypeString()<<", Power: "<< locItems[i].getPower() << ") \n";
    }

    std::cout << "Choose an item to pick up (0 to cancel): ";
    int a;
    std::cin>>a;

    if (a <= 0 || a > static_cast<int>(locItems.size())) {
        std::cout << "No item picked up. \n";
        return;
    }

    Item item = locItems[a-1]; 
    addItem(item);
    board->removeItem(heroloc, item.getName());

    std::cout<<"Picked up "<<item.getName()<<std::endl;
} 

void Hero::advance(Board* board, std::vector<std::unique_ptr<Monster>>& monsters){
    std::string heroloc = getLoc();
    bool canDracula = canAdvenceDracula(heroloc);
    bool canInvisibleMan = canAdvenceInvisibleMan(heroloc);

    if(!canDracula && !canInvisibleMan){
        std::cout<<"Cannot advance against any monster at "<<heroloc<<std::endl;
        return;
    }

    std::cout << "Choose a monster to advance against: \n";
    int index = 1;
    if(canAdvenceDracula(heroloc)){
        std::cout<<index<<". Dracula (destroy coffin at "<<heroloc<< ") \n";
        index++;
    }
    if(canAdvenceInvisibleMan(heroloc)){
        std::cout<<index<<". Invisible Man (place evidence) \n";
    }
    int a;
    std::cin>>a;
 
    if (a == 1 && canDracula) {
        for(auto& monster : monsters){
            if(monster->getName() == "Dracula"){
                advanceAgainstDracula(board , dynamic_cast<Dracula*>(monster.get()));
                break;
            }
        }
    } else if (a == index - (canAdvenceDracula(heroloc) ? 1 : 0) && canAdvenceInvisibleMan(heroloc)) {
        for(auto& monster : monsters){
            if(monster->getName() == "InvisibleMan"){
                advanceAgainstInvisibleMan(board , dynamic_cast<InvisibleMan*>(monster.get()));
                break;
            }
        }
    } else {
        std::cout << "Invalid choice. Advance action failed." << std::endl;
    }
}

bool Hero::canAdvenceDracula(std::string& heroloc){
    if(heroloc == "Cave" || heroloc == "Dungeon" || heroloc == "Crypt" || heroloc == "Graveyard"){
        return true;
    }else{
        return false;
    }
}

bool Hero::canAdvenceInvisibleMan(std::string& heroloc){
    if(heroloc == "Precinct"){
        return true;
    }else{
        return false;
    }
}

void Hero::advanceAgainstDracula(Board* board , Dracula* dracula){
    std::string heroloc = getLoc();

    if (!dracula) {
        std::cout << "Error: Dracula not found!" << std::endl;
        return;
    }

    int coffinIndex = dracula->getCoffinIndexByLocation(heroloc);

    if(coffinIndex == -1){
        std::cout<<"No coffin related to Dracula at " <<heroloc<<std::endl;
        return;
    }

    std::vector<Item> reditems;
    for(const auto& item : getInventory()){
        if(item.getType() == Red){
            reditems.push_back(item);
        }
    }

    if(reditems.empty()){
    std::cout<<"No Red items to destroy coffin! \n";   
    }

    std::cout<<"Choose Red items to destroy coffin (need total power of 6+): \n";
    for(size_t i=0 ; i<reditems.size() ; i++){
        std::cout<<(i+1)<<". "<<reditems[i].getName()<<"(power: "<<reditems[i].getPower()<<") \n";
    }

    std::vector<Item> selectItems;
    int sumPower = 0;
    std::vector<bool> itemUse(reditems.size() , false);

    while(sumPower < 6){
        std::cout << "Current total power: "<<sumPower<<"/6 \n";
        std::cout << "Available Red Items: \n";  
        bool anyAvailable = false;

        for (size_t i = 0; i < reditems.size(); ++i) {
            if (!itemUse[i]) {
                std::cout << (i+1) << ". " <<reditems[i].getName()<<" (Power: "<<reditems[i].getPower()<<") \n";
                anyAvailable = true;
            }
        }
        if (!anyAvailable) {
            std::cout << "No more red items available to select. \n";
            break;
        }
        
        std::cout << "Choose an item (0 to finish): \n";
        int a;
        std::cin>>a;
        if(a==0){
            break;
        }
        if(a<0 || a> static_cast<int>(reditems.size()) || itemUse[a-1]){
            std::cout << "Invalid choice or item already selected. \n";
            continue;
        }

        Item item =reditems[a-1];
        selectItems.push_back(item);
        sumPower += item.getPower();
        itemUse[a-1] = true;
    }

    if(sumPower >= 6){
        std::cout<<"Coffin destroyed with total power of " <<sumPower<< "! \n";

        for(const auto& item : selectItems){
            removeItem(item.getName());
        }

        dracula->destroyCoffin(coffinIndex);
    }else{
        std::cout<<"Not enough power to destroy coffin. Need " << (6 - sumPower) << " more. \n";
    }
}

void Hero::advanceAgainstInvisibleMan(Board* board , InvisibleMan* InvisibleMan){

    if (!InvisibleMan) {
        std::cout << "Error: Invisible Man not found!" << std::endl;
        return;
    }

    std::vector<std::pair<Item , int>> evidenceItems;
    for(const auto& item : getInventory()){
        std::string itemLoc = item.getLocation();
        int evidenceIndex = InvisibleMan->getEvidenceIndexByLocation(itemLoc);

        if(evidenceIndex == -1 && !(InvisibleMan->getEvidenceCollectedCount() > evidenceIndex) &&  (InvisibleMan->getEvidenceCollectedCount() == evidenceIndex || InvisibleMan->getEvidenceCollectedCount() < evidenceIndex)){
            evidenceItems.push_back({item , evidenceIndex});
        }
    }

    if(evidenceItems.empty()){
        std::cout << "No new evidence items to place from the required locations! \n"  ;
        return;
    }

    std::cout <<"Choose an evidence item to place: \n";
    for (size_t i = 0; i < evidenceItems.size(); i++) {
        std::cout << (i+1) << ". " <<evidenceItems[i].first.getName()<< " (from " << evidenceItems[i].first.getLocation() << ") \n";
    }

    int a;
    std::cin>>a;
    if ( a <= 0 || a > static_cast<int>(evidenceItems.size())) {
        std::cout << "Invalid choice. Advance action failed. \n";
        return;
    }

    Item itemToPlace = evidenceItems[a-1].first;
    int evidenceIndexCollect = evidenceItems[a-1].second;

    InvisibleMan->collectEvidence(evidenceIndexCollect);
    removeItem(itemToPlace.getName());
    std::cout<<"Evidence "<<itemToPlace.getName()<<" placed. \n";
}

void Hero::defeat( Board* board , std::vector<std::unique_ptr<Monster>>& monsters){
    std::string heroloc = getLoc();
    Monster* monstertoDefeat = nullptr;
    for(auto& monster : monsters){
        if(monster->getLoc() == heroloc){
            monstertoDefeat = monster.get();
            break;
        }
    }
    if(!monstertoDefeat){
        std::cout<<"NO monster at "<<heroloc<<"to defeat! \n";
    }

    if(monstertoDefeat->getName() == "Dracula"){
        defeatDracula(board , dynamic_cast<Dracula*>(monstertoDefeat) , monsters);
    }else if(monstertoDefeat->getName() == "InvisibleMan"){
        defeatInvisibleMan(board , dynamic_cast<InvisibleMan*>(monstertoDefeat) , monsters);
    }else{
        std::cout<<"Unknown monster type to defeat at this location.  \n";
    }
}

void Hero::defeatDracula(Board* board , Dracula* dracula , std::vector<std::unique_ptr<Monster>>& monsters){
    if(! dracula->isDefeated()){
        std::cout<<"Cannot defeat Dracula yet! "<< (4 - dracula->getCoffinsDestroyedCount())<< " coffins still need to be destroyed. \n";
        return;
    }

    std::vector<Item> yellowItems;
    for(const auto& item : getInventory()){
        if(item.getType() == Yellow){
            yellowItems.push_back(item);
        }
    }

    if(yellowItems.empty()){
        std::cout<<"NO Yellow items to defeat Dracula! \n";
        return;
    }
    
    std::cout<<"Choose Yellow items to defeat Dracula (need total power of 6+): \n";
    std::vector<bool> itemUse(yellowItems.size() , false);

    std::vector<Item> selecteditems;
    int sumPower = 0 ;

    while ( sumPower < 6) {
        std::cout << "Current total power: " <<sumPower<< "/6 \n";
        std::cout << "Available Yellow Items: \n";
        bool anyAvailable = false;
        for (size_t i = 0; i < yellowItems.size(); ++i) {
            if (!itemUse[i]) {
                std::cout << (i+1) << ". " << yellowItems[i].getName() << " (Power: " << yellowItems[i].getPower() << ") \n";
                anyAvailable = true;
            }
        }
        if (!anyAvailable) {
            std::cout << "No more yellow items available to select." << std::endl;
            break;
        }

        std::cout<<"Choose an item (0 to finish): \n";
        int a;
        std::cin>>a;

        if(a == 0){
            break;
        }
        if( a<0 || a > static_cast<int>(yellowItems.size()) || itemUse[a-1]){
            std::cout << "Invalid choice or item already selected. \n";
            continue;
        }

        Item item = yellowItems[a-1];
        selecteditems.push_back(item);
        sumPower += item.getPower();
        itemUse[a-1] = true;
    }

    if(sumPower >= 6){
        std::cout<<"Dracula defeated with total power of "<<sumPower<<"! \n";   

        for(const auto& item : selecteditems){
            removeItem(item.getName());
        }

        auto it = std::find_if(monsters.begin(), monsters.end(),
                              [](const std::unique_ptr<Monster>& m) { return m->getName() == "Invisible Man"; });
        if (it != monsters.end()) {
            monsters.erase(it);
        }
    }else{
        std::cout << "Not enough power to defeat Dracula. Need " << (6 - sumPower) << " more. \n"; 
    }
}

void Hero::defeatInvisibleMan(Board* board , InvisibleMan* invisibleMan , std::vector<std::unique_ptr<Monster>>& monsters){
    if(!invisibleMan->isDefeated()){
        std::cout <<"Cannot defeat Invisible Man yet! "<<(5 - invisibleMan->getEvidenceCollectedCount())<< " pieces of evidence still need to be collected. \n"; 
        return;
    }

    std::vector<Item> redItems;
    for (const auto& item : getInventory()) {
        if (item.getType() == Red) {
            redItems.push_back(item);
        }
    }

    if(redItems.empty()){
        std::cout<<"No Red items to defeat Invisible Man! \n";
        return;
    }

    std::cout<<"Choose Red items to defeat Invisible Man (need total power of 9+): \n";
    std::vector<bool> itemUse(redItems.size() , false);
    
    std::vector<Item> selecteditems;
    int sumPower = 0;

    while( sumPower < 9){
        std::cout << "Current total power: " << sumPower << "/9 \n";
        std::cout << "Available Red Items: \n";
        bool anyAvailable = false;
        for (size_t i = 0; i < redItems.size(); ++i) {
            if (!itemUse[i]) {
                std::cout << (i+1) << ". " <<redItems[i].getName()<< " (Power: " << redItems[i].getPower() << ") \n";
                anyAvailable = true;
            }
        }
        if (!anyAvailable) {
            std::cout << "No more red items available to select. \n";
            break;
        }

        std::cout<<"Choose an item (0 to finish): \n";
        int a;
        std::cin>>a;

        
        if(a == 0){
            break;
        }
        if( a<0 || a > static_cast<int>(redItems.size()) || itemUse[a-1]){
            std::cout << "Invalid choice or item already selected. \n";
            continue;
        }

        Item item = redItems[a-1];
        selecteditems.push_back(item);
        sumPower += item.getPower();
        itemUse[a-1] = true;
    }

    if(sumPower >= 9){
        std::cout<<"Invisible Man defeated with total power of "<<sumPower<<"! \n"; 

        for(const auto& item : selecteditems){
            removeItem(item.getName());
        }

        auto it = std::find_if(monsters.begin(), monsters.end(),
                              [](const std::unique_ptr<Monster>& m) { return m->getName() == "Invisible Man"; });
        if (it != monsters.end()) {
            monsters.erase(it);
        }
    }else{
        std::cout << "Not enough power to defeat Invisible Man. Need " << (9 - sumPower) << " more. \n";   
    }
}