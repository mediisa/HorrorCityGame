#ifndef MONSTERS2_HPP
#define MONSTERS2_HPP

#include "Monster.h"

#include <string>

class Dracula : public Monster {
private:
    bool coffinsDestroyed[4]; 

public:
    Dracula();

    void usePower(Hero* hero, Board* board) override;

    bool isDefeated() const override;

    void destroyCoffin(int index);
    int getCoffinsDestroyedCount() const;
    int getCoffinIndexByLocation(const std::string& location) const;
};

class InvisibleMan : public Monster {
private:
    bool evidenceCollected[5]; 

public:
    InvisibleMan();

    void usePower(Hero* hero, Board* board) override;

    bool isDefeated() const override;

    void collectEvidence(int index);
    int getEvidenceCollectedCount() const;
    int getEvidenceIndexByLocation(const std::string& location) const;
};

#endif