//
// Created by Jonas Luven on 24.06.23.
//

#include "sh.h"
SMS &SMS::getInstance() {
    if (instance == nullptr) {
        instance = new SMS;
    }
    return *instance;
}

void SMS::init(SoundManager *sm) {
    soundManager = sm;
}

SoundManager *SMS::get() const {
    return soundManager;
}

SMS* SMS::instance = nullptr;