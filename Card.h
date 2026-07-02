#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include <iostream> 


class Card {
protected:
    std::string name;

public:
    Card(const std::string& name);
    virtual ~Card();

    std::string getName() const;
    virtual void play() = 0; 
};

#endif 

