#ifndef DICE_HPP
#define DICE_HPP

#include <vector>
#include <algorithm> 
#include <cstdlib>   
#include <ctime>     
#include <iostream>

class Dice {
public:

    static int roll();

    static std::vector<int> rollMultiple(int count);

    static bool hasPower(const std::vector<int>& results);

    static bool hasAttack(const std::vector<int>& results);
};

#endif 

