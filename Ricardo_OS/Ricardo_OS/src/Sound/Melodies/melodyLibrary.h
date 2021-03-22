#ifndef MELODYLIBRARY_H
#define MELODYLIBRARY_H
#include "melodyClass.h"

melody_t<8> c_scalez = { {{ 
                    {NOTE_C4,100},
                    {NOTE_D4,100},
                    {NOTE_E4,100},
                    {NOTE_F4,100},
                    {NOTE_G4,100},
                    {NOTE_A4,100},
                    {NOTE_B4,100},
                    {NOTE_C5,100} 
                    }}, //create array of melody note values
                    true // assign high priority to this
                    };

#endif