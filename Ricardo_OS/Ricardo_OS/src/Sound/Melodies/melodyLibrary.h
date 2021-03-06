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
    CONFIRMATION,
    FIREFLIES,
    ZELDATHEME,
    CANTINABAND,
    GAMEOFTHRONES,
    MIICHANNEL,
    NEVERGONNAGIVEYOUUP,
    COUNT
};

class MelodyLibrary{
    public:
        inline melody_base_t* get(MELODY melody){
            if ((uint8_t)melody < (uint8_t)MELODY::COUNT){
                return song_library.at((uint8_t)melody);
            }else{
                return NULL;
            }
            
        };
    
    private:
        static melody_t<8> c_scalez;
        static melody_t<3> confirmation;
        static melody_t<1186> Fireflies;
        static melody_t<110> zeldatheme;
        static melody_t<64> cantinaband;
        static melody_t<130> gameofthrones;
        static melody_t<286> miichannel;
        static melody_t<340> nevergonnagiveyouup;

        std::array<melody_base_t*,(int)MELODY::COUNT> song_library{
                                            &c_scalez,
                                            &confirmation,
                                            &Fireflies,
                                            &zeldatheme,
                                            &cantinaband,
                                            &gameofthrones,
                                            &miichannel,
                                            &nevergonnagiveyouup};
};

#endif