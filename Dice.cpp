#include "Dice.h" 

int Dice::roll() {
    int result = rand() % 6;
    if (result == 0) {
        return 1;
    }else if (result == 1) {
        return 2;
    }else {
        return 0; 
    }                 
}

std::vector<int> Dice::rollMultiple(int count) {
    std::vector<int> results;
    for (int i = 0; i < count; i++) {
        results.push_back(roll());
    }
    return results;
}

bool Dice::hasPower(const std::vector<int>& results) {
    if(std::find(results.begin(), results.end(), 1) != results.end()){
        return true;
    }else{
        return false;
    }
}

bool Dice::hasAttack(const std::vector<int>& results) {
    if(std::find(results.begin(), results.end(), 2) != results.end()){
        return true;
    }else{
        return false;
    }

}

