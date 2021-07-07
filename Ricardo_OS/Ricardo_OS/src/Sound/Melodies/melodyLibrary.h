#ifndef MELODYLIBRARY_H
#define MELODYLIBRARY_H
#include "melodyClass.h"

#include <tuple>

// namespace SONG {
    
//     extern melody_t<8> c_scalez;
//     extern melody_t<1186> Fireflies;
//     extern melody_t<110> zeldatheme;
//     extern melody_t<64> cantinaband;
//     extern melody_t<130> gameofthrones;
//     extern melody_t<286> miichannel;
//     extern melody_t<340> nevergonnagiveyouup;
// };

enum class MELODY:uint8_t{
    C_SCALEZ,
    CANTINABAND,
    FIREFLIES,
    GAMEOFTHRONES,
    MIICHANNEL,
    NEVERGONNAGIVEYOUUP,
    ZELDATHEME,
    COUNT
};

class MelodyLibrary{
    public:
        melody_base_t* get(MELODY melody){return song_library.at((uint8_t)melody);};
    
    private:
        static melody_t<8> c_scalez;
        static melody_t<1186> Fireflies;
        static melody_t<110> zeldatheme;
        static melody_t<64> cantinaband;
        static melody_t<130> gameofthrones;
        static melody_t<286> miichannel;
        static melody_t<340> nevergonnagiveyouup;

        std::array<melody_base_t*,(int)MELODY::COUNT> song_library{
                                            &c_scalez,
                                            &Fireflies,
                                            &zeldatheme,
                                            &cantinaband,
                                            &gameofthrones,
                                            &miichannel,
                                            &nevergonnagiveyouup};
};

#endif