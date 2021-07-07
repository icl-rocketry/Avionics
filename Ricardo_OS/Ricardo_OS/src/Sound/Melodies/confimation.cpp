#include "melodyClass.h"
#include "melodyLibrary.h"
#include "pitches.h"

melody_t<3> MelodyLibrary::confirmation = { {{ 
                    {NOTE_C4,250},
                    {NOTE_G4,250},
                    {NOTE_C5,250}
                    }}, //create array of melody note values
                    true // assign high priority to this
                    };