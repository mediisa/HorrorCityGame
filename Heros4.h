#ifndef HEROS4_HPP
#define HEROS4_HPP

#include "Hero.h"

#include <string>

class Archaeologist : public Hero {
public:
    Archaeologist();
    void specialAction(Board* board) override;
};

class Mayor : public Hero {
public:
    Mayor();
    void specialAction(Board* board) override; 
};

class Scientist : public Hero {
public:
    Scientist();
    void specialAction(Board* board) override; 
};

class Courier : public Hero {
public:
    Courier();
    void specialAction(Board* board) override;
};

#endif