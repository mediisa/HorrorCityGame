#ifndef GAME_HPP
#define GAME_HPP

#include "Board.h"
#include "Card.h"
#include "Player.h"
#include "Monster.h"
#include "Monsters2.h"
#include "Hero.h"
#include "Heros4.h"
#include "Item.h"
#include "PerkCard.h"
#include "MonsterCard.h"
#include "Character.h" 
#include "Dice.h" 
#include "Villager.h"    

#include <vector>
#include <string>
#include <memory>  
#include <algorithm> 
#include <map>      
#include <random>   
#include <chrono> 
#include <iostream> 
#include <climits>  
#include <fstream>
#include <sstream>

class Game {
private:
    Board board;
    std::vector<Player> players;
    std::vector<std::unique_ptr<Monster>> monsters;
    std::vector<Villager> villagers; 
    std::vector<PerkCard> perkCardDeck;
    std::vector<MonsterCard> monsterCardDeck;
    int currentPlayerIndex;
    bool skipNextMonsterPhase;
    bool gameOver = false;

    void heroPhase();
    void monsterPhase();

public:
    Game();
    ~Game(); 

    void shuffleDecks();
    void setupGame();
    void getPlayerInfo();
    void determineStartingPlayer();
    void chooseHeroes();
    void dealInitialPerkCards();
    void placeInitialItems(); 
    void playGame();


    void playPerkCards(Hero* hero);
    void playVisitFromTheDetective();
    void playBreakOfDawn(Hero* hero);
    void playOverstock();
    void playLateIntoTheNight(Hero* hero);
    void playRepel();
    void playHurry();



    void applyMonsterCardItems(const MonsterCard& card);
    void applyMonsterCardEvent(const MonsterCard& card);
    void applyMonsterCardStrike(const MonsterCard& card);


    bool checkGameOver();
    void loop1_Game();
    void show1_menu();
    void menu_forHero(Hero* hero);
    void savegame_state();
    void loadgame_state();
};

#endif 

