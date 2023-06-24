#ifndef GORILLAGAME_SH_H
#define GORILLAGAME_SH_H
#include "SoundManager.h"

class SMS {
private:
    SoundManager* soundManager = nullptr;
    static SMS* instance;

    SMS() {}

public:
    static SMS& getInstance();

    void init(SoundManager* sm);

    [[nodiscard]] SoundManager *get() const;

    // Prevent copy and aSMSignment
    SMS(SMS const&) = delete;
    void operator=(SMS const&)  = delete;
};



#endif //GORILLAGAME_SH_H
