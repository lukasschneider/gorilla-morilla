#ifndef PH_H
#define PH_H
#include "../GameObjects/Player.h"

class PS {
private:
    Player* player = nullptr;
    static PS* instance;

    PS() {}

public:
    static PS& getInstance();

    void init(Player* p);

    [[nodiscard]] Player* get() const;

    // Prevent copy and assignment
    PS(PS const&) = delete;
    void operator=(PS const&)  = delete;
};




#endif