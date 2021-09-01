#pragma once

#include "melodyClass.h"

#include <tuple>

namespace MelodyLibrary {
    
    extern melody_t<8> c_scalez;
    extern melody_t<1186> Fireflies;
    extern melody_t<110> zeldatheme;
    extern melody_t<64> cantinaband;
    extern melody_t<130> gameofthrones;
    extern melody_t<286> miichannel;
    extern melody_t<340> nevergonnagiveyouup;
    extern melody_t<3> confirmation;

    // enum class SONGS:uint8_t{
    // FIREFLIES,
    // ZELDATHEME,
    // CANTINABAND,
    // GAMEOFTHRONES,
    // MIICHANNEL,
    // NEVERGONNAGIVEYOUUP,
    // COUNT
    // };

    static const std::array<melody_base_t*,6> songLibrary{
        &Fireflies,
        &zeldatheme,
        &cantinaband,
        &gameofthrones,
        &miichannel,
        &nevergonnagiveyouup};

};


