#include "Game.h"

Game::Game() : currentPlayerIndex(0), skipNextMonsterPhase(false) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(static_cast<unsigned int>(seed)); 

    monsters.push_back(std::make_unique<Dracula>());
    monsters.push_back(std::make_unique<InvisibleMan>());

    perkCardDeck = PerkCard::createPerkCards();
    monsterCardDeck = MonsterCard::createMonsterCards();

    shuffleDecks();
}

Game::~Game() {}

void Game::shuffleDecks() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);

    std::shuffle(perkCardDeck.begin(), perkCardDeck.end(), rng);
    std::shuffle(monsterCardDeck.begin(), monsterCardDeck.end(), rng);
}

void Game::setupGame() {
    getPlayerInfo();

    determineStartingPlayer();

    chooseHeroes();

    dealInitialPerkCards();

    placeInitialItems();
}

void Game::show1_menu(){ 
    std::cout<<"\n=== Main Menu ===\n";
    std::cout<<"1. NEW Game \n";
    std::cout<<"2. Save & Exit Game \n";
    std::cout<<"3. Load Game & Play \n";
    std::cout<<"4. Exit Game \n";
    std::cout<<"Enter your choice...\n";
}

void Game::menu_forHero(Hero* hero){
    std::cout << "Choose action: \n";
    std::cout << "1. Move \n";
    std::cout << "2. Guide Villager \n";
    std::cout << "3. Pick Up Item \n";
    std::cout << "4. Advance \n";
    std::cout << "5. Defeat \n";
    if (hero->getHasSpecialAction()) {
        std::cout<<"6. Special Action: "<<hero->getSpecialActionDescription()<< std::endl;
    }
    std::cout << "7.Back to Menu ...\n";
}

void Game::loop1_Game(){
    show1_menu();
    int a;
    std::cin>>a;
    do{
        switch(a){
            case 1:{
                setupGame();
                gameOver=false;
                playGame();
                break;
            }
            case 2:{
                savegame_state();
                exit(0);      
            }
            case 3:{
                loadgame_state();
                gameOver=false;
                playGame();
                break;        
            }
            case 4:{
                exit(0);
            }
            default:
            std::cout<<"Enter the (1-4) number .... \n";
        }
    }while(a>4);
}

void Game::getPlayerInfo() {
    std::string name;
    int garlicTime;

    std::cout << "Enter Player 1's name: ";
    std::cin >> name;
    std::cout << "Enter time since Player 1 last ate garlic (in hours): ";
    std::cin >> garlicTime;
    players.push_back(Player(name, garlicTime));

    std::cout << "Enter Player 2's name: ";
    std::cin >> name;
    std::cout << "Enter time since Player 2 last ate garlic (in hours): ";
    std::cin >> garlicTime;
    players.push_back(Player(name, garlicTime));
}

void Game::determineStartingPlayer() {
    if (players[1].getGarlicTime() < players[0].getGarlicTime()) {
        std::swap(players, players);
    }

    std::cout << players[0].getName() << " ate garlic more recently and will start the game!" << std::endl;
    currentPlayerIndex = 0;
}

void Game::chooseHeroes() {
    std::cout << players[0].getName() << ", choose your hero: \n";
    std::cout << "1. Archaeologist (4 actions, Special: Pick up items from neighboring location) \n";
    std::cout << "2. Mayor (5 actions, No special ability) \n";
    std::cout << "3. Scientist (4 actions, Ability: Increase item power by 1) \n";
    std::cout << "4. Courier (4 actions, Special: Move to another hero's location) \n";

    int choice;
    std::cin >> choice;

    Hero* hero1 = nullptr;
    Hero* hero2 = nullptr;

    std::vector<std::string> availableHeroNames = {"Archaeologist", "Mayor", "Scientist", "Courier"};

    auto createHeroByName = [&](const std::string& name) -> Hero* {
        if (name == "Archaeologist") return new Archaeologist();
        if (name == "Mayor") return new Mayor();
        if (name == "Scientist") return new Scientist();
        if (name == "Courier") return new Courier();
        return nullptr;
    };

    std::string selectedHero1Name;

    switch (choice) {
        case 1:{
            selectedHero1Name = "Archaeologist";
            break;
        }case 2:{
            selectedHero1Name = "Mayor";
            break;
        }case 3:{
            selectedHero1Name = "Scientist";
            break;
        }case 4:{
            selectedHero1Name = "Courier";
            break;
        }default:{
            selectedHero1Name = "Archaeologist"; 
            std::cout << "Invalid choice, defaulting to Archaeologist for Player 1." << std::endl;
            break;
        }
    }

    hero1 = createHeroByName(selectedHero1Name);
    
    availableHeroNames.erase(std::remove(availableHeroNames.begin(), availableHeroNames.end(), selectedHero1Name), availableHeroNames.end());

    std::cout << "Available heroes for " << players[1].getName() << ":" << std::endl;
    for(size_t i = 0; i < availableHeroNames.size(); ++i) {
        std::cout << (i + 1) << ". " << availableHeroNames[i] << std::endl;
    }

    std::cin >> choice;
    std::string selectedHero2Name;
    if (choice > 0 && choice <= static_cast<int>(availableHeroNames.size())) {
        selectedHero2Name = availableHeroNames[choice - 1];
    } else {
        selectedHero2Name = availableHeroNames[0]; 
        std::cout << "Invalid choice, defaulting to " << availableHeroNames[0] << " for Player 2." << std::endl;
    }
    hero2 = createHeroByName(selectedHero2Name);

    players[0].setHero(hero1);
    players[1].setHero(hero2);

    std::cout << players[0].getName() << " is playing as " << hero1->getName() << std::endl;
    std::cout << players[1].getName() << " is playing as " << hero2->getName() << std::endl;
}

void Game::dealInitialPerkCards() {
    for (auto& player : players) {
        if (!perkCardDeck.empty()) {
            player.getHero()->addPerkCard(perkCardDeck.back());
            perkCardDeck.pop_back();
        } else {
            std::cout << "Perk card deck is empty, cannot deal initial cards." << std::endl;
        }
    }
}

void Game::placeInitialItems() {
    std::cout << "Initial items are placed on the board." << std::endl;
}

void Game::playGame() {
    gameOver = false;

    while (!gameOver) {
        heroPhase();

        if (checkGameOver()) {
            gameOver = true;
            continue;
        }

        if (!skipNextMonsterPhase) {
            monsterPhase();
        } else {
            std::cout << "Monster Phase skipped due to Break of Dawn card!" << std::endl;
            skipNextMonsterPhase = false;
        }

        if (checkGameOver()) {
            gameOver = true;
            continue;
        }

        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    }
}

void Game::heroPhase() {
    Player& currentPlayer = players[currentPlayerIndex];
    Hero* hero = currentPlayer.getHero();

    std::cout << "\n=== " << currentPlayer.getName() << "'s Turn (" << hero->getName() << ") ===" << std::endl;
    std::cout << "Location: " << hero->getLoc() << std::endl;

    std::cout << "Inventory:" << std::endl;
    const auto& inventory = hero->getInventory();
    if (inventory.empty()) {
        std::cout << "  No items" << std::endl;
    } else {
        for (const auto& item : inventory) {
            std::cout << "  " << item.getName() << " (" << item.getTypeString() 
                      << ", Power: " << item.getPower() << ")" << std::endl;
        }
    }

    std::cout << "Perk Cards:" << std::endl;
    const auto& perkCards = hero->getPerkCards();
    if (perkCards.empty()) {
        std::cout << "  No perk cards" << std::endl;
    } else {
        for (size_t i = 0; i < perkCards.size(); i++) {
            std::cout << "  " << (i+1) << ". " << perkCards[i].getDetails() << std::endl;
        }
    }

    playPerkCards(hero);

    int remainingActions = hero->getActionPoints();
    while (remainingActions > 0) {
        std::cout << "\nRemaining Actions: " << remainingActions << std::endl;
        menu_forHero(hero);
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:{
                hero->move(&board, villagers, perkCardDeck);
                break;
            }case 2:{
                hero->guide(&board, villagers, perkCardDeck);
                break;
            }case 3:{
                hero->pickUp(&board);
                break; 
            }case 4:{
                hero->advance(&board, monsters);
                break;
            }case 5:{
                hero->defeat(&board, monsters);
                break;
            }case 6:{
                if (hero->getHasSpecialAction()) {
                    hero->specialAction(&board);
                } else {
                    std::cout << "This hero has no special action!" << std::endl;
                    continue;
                }
                break;
            }case 7:{
                loop1_Game();
            }default:{
                std::cout << "Invalid choice. Try again." << std::endl;
                continue;
            }
        }

        remainingActions--;
    }
}

void Game::playPerkCards(Hero* hero) {
    std::vector<PerkCard> currentPerkCards = hero->getPerkCards();

    if (currentPerkCards.empty()) return;

    std::cout << "\nDo you want to play any Perk Cards? (y/n): ";
    char choice;
    std::cin >> choice;

    if (choice != 'y' && choice != 'Y') return;

    std::cout << "Choose a Perk Card to play (0 to cancel):" << std::endl;
    for (size_t i = 0; i < currentPerkCards.size(); i++) {
        std::cout << (i+1) << ". " << currentPerkCards[i].getDetails() << std::endl;
    }

    int cardIndex;
    std::cin >> cardIndex;

    if (cardIndex <= 0 || cardIndex > static_cast<int>(currentPerkCards.size())) return;

    PerkCard card = currentPerkCards[cardIndex - 1]; 
    hero->removePerkCard(card.getName()); 

    switch (card.getType()) {
        case VisitFromTheDetective:
            playVisitFromTheDetective();
            break;
        case BreakOfDawn:
            playBreakOfDawn(hero);
            break;
        case Overstock:
            playOverstock();
            break;
        case LateIntoTheNight:
            playLateIntoTheNight(hero);
            break;
        case Repel:
            playRepel();
            break;
        case Hurry:
            playHurry();
            break;
    }
}

void Game::playVisitFromTheDetective() {
    std::cout << "Playing 'Visit from the Detective' card" << std::endl;
    std::cout << "Choose a location to place the Invisible Man:" << std::endl;

    const auto& locations = board.getLocations();
    for (size_t i = 0; i < locations.size(); i++) {
        std::cout << (i+1) << ". " << locations[i] << std::endl;
    }

    int locationIndex;
    std::cin >> locationIndex;

    if (locationIndex <= 0 || locationIndex > static_cast<int>(locations.size())) {
        std::cout << "Invalid location. Placing at a random location." << std::endl;
        locationIndex = rand() % locations.size() + 1;
    }

    std::string location = locations[locationIndex - 1];

    for (auto& monster : monsters) {
        if (monster->getName() == "Invisible Man") {
            monster->setLoc(location);
            std::cout << "Invisible Man placed at " << location << std::endl;
            break;
        }
    }
}

void Game::playBreakOfDawn(Hero* hero) {
    std::cout << "Playing 'Break of Dawn' card" << std::endl;
    std::cout << "Skipping the next Monster Phase" << std::endl;
    skipNextMonsterPhase = true;

    for (int i = 0; i < 2; i++) {
        ItemType type = static_cast<ItemType>(rand() % 3);
        int power = rand() % 5 + 1; 
        std::string name = "Random_Item_" + std::to_string(rand() % 1000); 
        Item item(name, type, power, 1, hero->getLoc());

        board.addItem(hero->getLoc(), item);
        std::cout << "Added " << name << " (" << (type == Red ? "Red" : type == Blue ? "Blue" : "Yellow") 
                  << ", Power: " << power << ") to " << hero->getLoc() << std::endl;
    }
}

void Game::playOverstock() {
    std::cout << "Playing 'Overstock' card" << std::endl;

    for (auto& player : players) {
        Hero* hero = player.getHero();

        ItemType type = static_cast<ItemType>(rand() % 3);
        int power = rand() % 5 + 1;
        std::string name = "Random_Item_Overstock_" + std::to_string(rand() % 1000); // Unique name
        Item item(name, type, power, 1, hero->getLoc());

        board.addItem(hero->getLoc(), item);
        std::cout << "Added " << name << " (" << (type == Red ? "Red" : type == Blue ? "Blue" : "Yellow") 
                  << ", Power: " << power << ") to " << hero->getLoc() << " for " << player.getName() << std::endl;
    }
}

void Game::playLateIntoTheNight(Hero* hero) {
    std::cout << "Playing 'Late into the Night' card" << std::endl;
    std::cout << "You get two additional actions this turn!" << std::endl;

    for (int i = 0; i < 2; i++) {
        std::cout <<"\nAdditional Action "<<(i+1)<<": \n";
        menu_forHero(hero);

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:{
                hero->move(&board, villagers, perkCardDeck);
                break;
            }case 2:{
                hero->guide(&board, villagers, perkCardDeck);
                break;
            }case 3:{
                hero->pickUp(&board);
                break; 
            }case 4:{
                hero->advance(&board, monsters);
                break;
            }case 5:{
                hero->defeat(&board, monsters);
                break;
            }case 6:{
                if (hero->getHasSpecialAction()) {
                    hero->specialAction(&board);
                } else {
                    std::cout << "This hero has no special action!" << std::endl;
                    continue;
                }
                break;
            }case 7:{
                loop1_Game();
            }default:{
                std::cout << "Invalid choice. Try again." << std::endl;
                continue;
            }
        }
    }
}

void Game::playRepel() {
    std::cout << "Playing 'Repel' card" << std::endl;
    std::cout << "Moving each monster 2 spaces away" << std::endl;

    for (auto& monster : monsters) {
        std::string currentLoc = monster->getLoc();
        std::vector<std::string> possibleLocations;
        std::map<std::string, int> dist;
        std::vector<std::string> q;

        q.push_back(currentLoc);
        dist[currentLoc] = 0;
        size_t head = 0;

        while(head < q.size()) {
            std::string loc = q[head++];
            if(dist[loc] == 2) {
                possibleLocations.push_back(loc);
                continue; 
            }
            if(dist[loc] > 2) continue; 

            const auto& neighbors = board.getPaths().at(loc);
            for(const auto& neighbor : neighbors) {
                if(dist.find(neighbor) == dist.end()) {
                    dist[neighbor] = dist[loc] + 1;
                    q.push_back(neighbor);
                }
            }
        }

        if (!possibleLocations.empty()) {
            std::string newLoc = possibleLocations[rand() % possibleLocations.size()];
            monster->setLoc(newLoc);
            std::cout << monster->getName() << " moved to " << newLoc << std::endl;
        } else {
            std::cout << monster->getName() << " could not find a location 2 steps away. Stays at " << currentLoc << std::endl;
        }
    }
}

void Game::playHurry() {
    std::cout<<"Playing 'Hurry' card \n";
    std::cout<<"Moving each hero 2 spaces \n";
    std::vector<Player*> allPlayers;
    for(auto& p : players) allPlayers.push_back(&p);

    for (auto* playerPtr : allPlayers) { 
        Hero* hero = playerPtr->getHero();
        std::string currentLoc = hero->getLoc();

        std::cout<<"Moving "<<playerPtr->getName()<<"'s hero ("<<hero->getName()<<"): \n";

        std::cout<<"First move for "<<hero->getName()<<" : \n";
        hero->move(&board, villagers, perkCardDeck);
        std::cout<<"Second move for "<<hero->getName()<<" : \n";
        hero->move(&board, villagers, perkCardDeck);
    }
}


void Game::monsterPhase() {
    if (monsterCardDeck.empty()) {
        std::cout<<"Monster card deck is empty! The town is lost! \n";
        return;
    }

    std::cout<<"\n=== Monster Phase === \n";

    MonsterCard card = monsterCardDeck.back(); 
    monsterCardDeck.pop_back();

    std::cout << "Drew Monster Card: " << card.getName() << std::endl;

    applyMonsterCardItems(card);
    applyMonsterCardEvent(card);
    applyMonsterCardStrike(card);
}

void Game::applyMonsterCardItems(const MonsterCard& card) {
    int itemsToAdd = card.getItemsToAdd();
    std::cout << "Adding " << itemsToAdd << " new items to the board" << std::endl;

    const auto& allLocations = board.getLocations(); 
    for (int i = 0; i < itemsToAdd; i++) {
        ItemType type = static_cast<ItemType>(rand() % 3);
        int power = rand() % 5 + 1; 
        std::string name = "Random_Monster_Item_" + std::to_string(rand() % 1000); 
        std::string location = allLocations[rand() % allLocations.size()];

        Item item(name, type, power, 1, location);
        board.addItem(location, item);

        std::cout << "Added " << name << " (" << (type == Red ? "Red" : type == Blue ? "Blue" : "Yellow") 
                  << ", Power: " << power << ") to " << location << std::endl;
    }
}

void Game::applyMonsterCardEvent(const MonsterCard& card) {
    std::cout << "Applying event: ";

    switch (card.getType()) {
        case FormOfTheBat:
            std::cout << "Form of the Bat" << std::endl;
            for (auto& monster : monsters) {
                if (monster->getName() == "Dracula") {
                    monster->setLoc(players[currentPlayerIndex].getHero()->getLoc());
                    std::cout << "Dracula moved to " << monster->getLoc() << std::endl;
                    break;
                }
            }
            break;

        case Sunrise:
            std::cout << "Sunrise" << std::endl;
            for (auto& monster : monsters) {
                if (monster->getName() == "Dracula") {
                    monster->setLoc("Crypt");
                    std::cout << "Dracula placed at Crypt" << std::endl;
                    break;
                }
            }
            break;

        case Thief: {
            std::cout << "Thief" << std::endl;
            std::string maxItemLoc;
            int maxItems = -1;

            for (const auto& pair : board.getItems()) {
                if (static_cast<int>(pair.second.size()) > maxItems) {
                    maxItems = pair.second.size();
                    maxItemLoc = pair.first;
                }
            }

            if (!maxItemLoc.empty() && maxItems > 0) { 
                for (auto& monster : monsters) {
                    if (monster->getName() == "Invisible Man") {
                        monster->setLoc(maxItemLoc);
                        board.removeItems(maxItemLoc);
                        std::cout << "Invisible Man moved to " << maxItemLoc << " and removed all items" << std::endl;
                        break;
                    }
                }
            } else {
                std::cout << "No locations with items for Thief to steal from." << std::endl;
            }
            break;
        }

        case TheDelivery:
            std::cout << "The Delivery" << std::endl;
            villagers.push_back(Villager("Wilbur & Chick", "Docks", "Church"));
            std::cout << "Wilbur & Chick placed at Docks" << std::endl;
            break;

        case FortuneTeller:
            std::cout << "Fortune Teller" << std::endl;
            villagers.push_back(Villager("Maleva", "Camp", "Church"));
            std::cout << "Maleva placed at Camp" << std::endl;
            break;

        case FormerEmployer:
            std::cout << "Former Employer" << std::endl;
            villagers.push_back(Villager("Dr. Cranly", "Laboratory", "Hospital"));
            std::cout << "Dr. Cranly placed at Laboratory" << std::endl;
            break;

        case HurriedAssistant:
            std::cout << "Hurried Assistant" << std::endl;
            villagers.push_back(Villager("Fritz", "Tower", "Hospital"));
            std::cout << "Fritz placed at Tower" << std::endl;
            break;

        case TheInnocent:
            std::cout << "The Innocent" << std::endl;
            villagers.push_back(Villager("Maria", "Barn", "Church"));
            std::cout << "Maria placed at Barn" << std::endl;
            break;

        case EgyptianExpert:
            std::cout << "Egyptian Expert" << std::endl;
            villagers.push_back(Villager("Prof. Pearson", "Cave", "Museum"));
            std::cout << "Prof. Pearson placed at Cave" << std::endl;
            break;

        case TheIchthyologist:
            std::cout << "The Ichthyologist" << std::endl;
            villagers.push_back(Villager("Dr. Read", "Institute", "Museum"));
            std::cout << "Dr. Read placed at Institute" << std::endl;
            break;
    }
}

void Game::applyMonsterCardStrike(const MonsterCard& card) {
    std::cout << "Monster Strike:" << std::endl;

    const auto& strikeOrder = card.getMonsterStrikeOrder();
    int movementUnits = card.getMovementUnits();
    int diceCount = card.getDiceCount();

    for (const auto& monsterName : strikeOrder) {
        if (monsterName == "Dracula") {
            for (auto& monster : monsters) {
                if (monster->getName() == "Dracula") {
                    monster->strike(movementUnits , diceCount , players , villagers , &board );
                    break;
                }
            }
        } else if (monsterName == "Invisible Man") {
            for (auto& monster : monsters) {
                if (monster->getName() == "Invisible Man") {
                    monster->strike(movementUnits, diceCount, players , villagers , &board );
                    break;
                }
            }
        } else if (monsterName == "Frenzied Monster") {
            if (!monsters.empty()) {
                int index = rand() % monsters.size();
                monsters[index]->strike(movementUnits , diceCount , players , villagers , &board);
            } else {
                std::cout << "No monsters on board for Frenzied Monster strike." << std::endl;
            }
        } else {
            std::cout << "Unknown monster name in strike order: " << monsterName << std::endl;
        }
    }
}

bool Game::checkGameOver() {
    if (monsters.empty()) {
        std::cout << "\n=== VICTORY! ===" << std::endl;
        std::cout << "All monsters have been defeated! The town is saved!" << std::endl;
        return true;
    }

    if (board.getTerrorLevel() >= 5) {
        std::cout << "\n=== DEFEAT! ===" << std::endl;
        std::cout << "Terror Level has reached maximum! The town is lost!" << std::endl;
        return true;
    }

    if (monsterCardDeck.empty()) {
        std::cout << "\n=== DEFEAT! ===" << std::endl;
        std::cout << "Monster cards have run out! The town is lost!" << std::endl;
        return true;
    }

    return false;
}

void Game::savegame_state() {
    std::ofstream outFile("saved_game.txt");
    if (!outFile) {
        std::cerr << "Error: Unable to create save file!" << std::endl;
        return;
    }

    
    outFile << "[GameInfo]\n";
    outFile << "currentPlayer=" << currentPlayerIndex << "\n";
    outFile << "skipMonsterPhase=" << (skipNextMonsterPhase ? 1 : 0) << "\n";
    outFile << "terrorLevel=" << board.getTerrorLevel() << "\n\n";

    
    outFile << "[Players]\n";
    outFile << "count=" << players.size() << "\n";
    
    for (const auto& player : players) {
        outFile << "playerName=" << player.getName() << "\n";
        outFile << "garlicTime=" << player.getGarlicTime() << "\n";
        
        Hero* hero = player.getHero();
        outFile << "heroType=" << static_cast<int>(hero->getHerotyoe()) << "\n";
        outFile << "heroLocation=" << hero->getLoc() << "\n";
        outFile << "actionPoints=" << hero->getActionPoints() << "\n";
        
        
        outFile << "heroInventory=";
        for (const auto& item : hero->getInventory()) {
            outFile << item.getName() << "," 
                    << static_cast<int>(item.getType()) << ","
                    << item.getPower() << ";";
        }
        outFile << "\n\n";
    }

    
    outFile << "[Monsters]\n";
    outFile << "count=" << monsters.size() << "\n";
    
    for (const auto& monster : monsters) {
        outFile << "monsterName=" << monster->getName() << "\n";
        outFile << "monsterLocation=" << monster->getLoc() << "\n";
        
        
        if (monster->getName() == "Dracula") {
            Dracula* dracula = dynamic_cast<Dracula*>(monster.get());
            outFile << "coffins=";
            for (int i = 0; i < 4; i++) {
                outFile << (dracula->getCoffinsDestroyedCount() ? 1 : 0) << " ";
            }
        }
        else if (monster->getName() == "Invisible Man") {
            InvisibleMan* im = dynamic_cast<InvisibleMan*>(monster.get());
            outFile << "evidence=";
            for (int i = 0; i < 5; i++) {
                outFile << (im->getEvidenceCollectedCount() ? 1 : 0) << " ";
            }
        }
        outFile << "\n\n";
    }

    
    outFile << "[Villagers]\n";
    outFile << "count=" << villagers.size() << "\n";
    
    for (const auto& villager : villagers) {
        outFile << "villagerName=" << villager.getName() << "\n";
        outFile << "location=" << villager.getLoc() << "\n";
        outFile << "safeLocation=" << villager.getSafeLocation() << "\n";
        outFile << "isSaved=" << (villager.isAtSafeLocation() ? 1 : 0) << "\n\n";
    }

    
    outFile << "[BoardItems]\n";
    const auto& items = board.getItems();
    for (const auto& pair : items) {
        if (!pair.second.empty()) {
            outFile << "location=" << pair.first << "\n";
            for (const auto& item : pair.second) {
                outFile << item.getName() << "," 
                        << static_cast<int>(item.getType()) << ","
                        << item.getPower() << ","
                        << item.getQuantity() << "\n";
            }
            outFile << "\n";
        }
    }


    outFile << "[PerkCards]\n";
    outFile << "count=" << perkCardDeck.size() << "\n";
    for (const auto& card : perkCardDeck) {
        outFile << "cardName=" << card.getName() << "\n";
        outFile << "cardType=" << static_cast<int>(card.getType()) << "\n";
        outFile << "quantity=" << card.getQuantity() << "\n\n";
    }

    outFile.close();
    std::cout << "Game saved successfully to saved_game.txt\n";

}

void Game::loadgame_state() {
    std::ifstream inFile("saved_game.txt");
    if (!inFile) {
        std::cerr << "Error: Unable to open save file!" << std::endl;
        return;
    }

    
    players.clear();
    monsters.clear();
    villagers.clear();
    perkCardDeck.clear();
    board = Board(); 

    std::string line;
    std::string currentSection;
    int playerCount = 0;
    int monsterCount = 0;
    int villagerCount = 0;
    int perkCardCount = 0;
    
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;
        
        if (line[0] == '[') {
            currentSection = line.substr(1, line.size()-2);
            continue;
        }

        std::istringstream iss(line);
        std::string key;
        std::getline(iss, key, '=');
        
        if (currentSection == "GameInfo") {
            if (key == "currentPlayer") {
                iss >> currentPlayerIndex;
            }
            else if (key == "skipMonsterPhase") {
                int tmp;
                iss >> tmp;
                skipNextMonsterPhase = (tmp == 1);
            }
            else if (key == "terrorLevel") {
                int level;
                iss >> level;
                for (int i = 0; i < level; i++) {
                    board.increaseTerrorLevel();
                }
            }
        }else if (currentSection == "Players") {
            if (key == "count") {
                iss >> playerCount;
                players.reserve(playerCount);
            }
            else if (key == "playerName") {
                std::string name;
                std::getline(iss, name);
                players.emplace_back(name, 0);
            }
            else if (key == "garlicTime") {
                int time;
                iss >> time;
                players.back().setGarlicTime(time);
            }
            else if (key == "heroType") {
                int type;
                iss >> type;
                Hero* hero = nullptr;
                
                switch (static_cast<HeroType>(type)) {
                    case HeroType::Archaeologist: hero = new Archaeologist(); break;
                    case HeroType::Mayor: hero = new Mayor(); break;
                    case HeroType::Scientist: hero = new Scientist(); break;
                    case HeroType::Courier: hero = new Courier(); break;
                }
                
                players.back().setHero(hero);
            }
            else if (key == "heroLocation") {
                std::string loc;
                std::getline(iss, loc);
                players.back().getHero()->setLoc(loc);
            }
            else if (key == "actionPoints") {
                int ap;
                iss >> ap;
            }
            else if (key == "heroInventory") {
                std::string inventoryStr;
                std::getline(iss, inventoryStr);
                std::istringstream invStream(inventoryStr);
                std::string itemStr;
                
                while (std::getline(invStream, itemStr, ';')) {
                    std::istringstream itemStream(itemStr);
                    std::string name;
                    int type, power;
                    char comma;
                    
                    std::getline(itemStream, name, ',');
                    itemStream >> type >> comma >> power;
                    
                    Item item(name, static_cast<ItemType>(type), power, 1, "");
                    players.back().getHero()->addItem(item);
                }
            }
        }else if (currentSection == "Monsters"){
            if (key == "count") {
                iss >> monsterCount;
                monsters.reserve(monsterCount);
            }
            else if (key == "monsterName") {
                std::string name, loc;
                iss >> name >> loc;
                if (name == "Dracula") {
                    monsters.push_back(std::make_unique<Dracula>());
                }
                else if (name == "Invisible Man") {
                    monsters.push_back(std::make_unique<InvisibleMan>());
                }


                if (!monsters.empty()) {
                    monsters.back()->setLoc(loc);
                }
            }
            else if (key == "coffins") {
                if (!monsters.empty() && monsters.back()->getName() == "Dracula") {
                    Dracula* dracula = dynamic_cast<Dracula*>(monsters.back().get());
                    for (int i = 0; i < 4; i++) {
                        int destroyed;
                        iss >> destroyed;
                        if (destroyed) {
                            dracula->destroyCoffin(i);
                        }
                    }
                }
            }
            else if (key == "evidence") {
                if (!monsters.empty() && monsters.back()->getName() == "Invisible Man") {
                    InvisibleMan* im = dynamic_cast<InvisibleMan*>(monsters.back().get());
                    for (int i = 0; i < 5; i++) {
                        int collected;
                        iss >> collected;
                        if (collected) {
                            im->collectEvidence(i);
                        }
                    }
                }
            }
        }else if (currentSection == "Villagers"){
            if (key == "count") {
                iss >> villagerCount;
                villagers.reserve(villagerCount);
            }
            else if (key == "villagerName") {
                std::string name, loc, safeLoc;
                int isSaved, health;
                iss >> name >> loc >> safeLoc >> isSaved;
                
                Villager villager(name, loc, safeLoc);
            }
        }else if (currentSection == "BoardItems") {
            if (key == "location") {
                std::string loc;
                std::getline(iss, loc);
                
                std::string itemLine;
                while (std::getline(inFile, itemLine) && !itemLine.empty()) {
                    std::istringstream itemIss(itemLine);
                    std::string name;
                    int type, effect, quantity;
                    char comma;
                    
                    std::getline(itemIss, name, ',');
                    itemIss >> type >> comma >> effect >> comma >> quantity;
                    
                    Item item(name, static_cast<ItemType>(type), effect, quantity, loc);
                    board.addItem(loc, item);
                }
            }
        }else if (currentSection == "PerkCards") {
            if (key == "count") {
                iss >> perkCardCount;
                perkCardDeck.reserve(perkCardCount);
            }
            else if (key == "cardName") {
                std::string name;
                int type, quantity;
                iss >> name >> type >> quantity;
                
                PerkCard card(name, quantity, static_cast<PerkCardType>(type));
                perkCardDeck.push_back(card);
            }
        }
    }

    std::cout << "Game loaded successfully from saved_game.txt\n";
    inFile.close();
}
