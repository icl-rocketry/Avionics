#ifndef MELODYCLASS_H
#define MELODYCLASS_H

#include "pitches.h"
#include <array>


struct melody_note_t{
    uint16_t pitch;
    uint16_t duration;
};

class melody_base_t{
    public:
        virtual melody_note_t getNote(int i) = 0;
        virtual uint8_t getPriority() = 0;
        virtual size_t getSize() = 0;
        
};    

template<size_t S>
class melody_t:public melody_base_t{
    public:
        melody_t(std::array<melody_note_t,S> array_in,uint8_t priority):
        note_sequence(array_in),
        _priority(priority)
        {};

         melody_note_t getNote(int i){return note_sequence.at(i);}; //bounds checking
         uint8_t getPriority(){return _priority;};
         size_t getSize(){return _size;};

    private:
        std::array<melody_note_t,S> note_sequence;
        uint8_t _priority;
        static constexpr size_t _size = S;

};



//example impl

melody_t<8> scale = { {{ 
                    {NOTE_C4,1},
                    {NOTE_D4,1},
                    {NOTE_E4,1},
                    {NOTE_F4,1},
                    {NOTE_G4,1},
                    {NOTE_A4,1},
                    {NOTE_B4,1},
                    {NOTE_C5,1}}}, //create array of melody note values
                    9 // assign priority to this melody
                    };


#endif