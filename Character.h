#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

class Board; 

enum class Typechara { HERO, VILLAGER, MONSTER };

class Character {
protected:
    std::string name;
    std::string location;
    Typechara type;

public:
    Character(const std::string& name, const std::string& location , Typechara type);
    virtual ~Character();

    std::string getName() const;
    std::string getLoc() const;
    Typechara getType() const;
    
    void setLoc(const std::string& loc);
};

#endif 

