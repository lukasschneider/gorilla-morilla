#include "ph.h"

PS &PS::getInstance() {
    if (instance == nullptr) {
        instance = new PS;
    }
    return *instance;
}

void PS::init(Player *p) {
    player = p;
}

Player *PS::get() const {
    return player;
}

PS* PS::instance = nullptr;

