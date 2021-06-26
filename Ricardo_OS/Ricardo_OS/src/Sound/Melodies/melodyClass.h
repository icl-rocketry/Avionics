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
        virtual bool getPriority() = 0;
        virtual size_t getSize() = 0;

        virtual ~melody_base_t(){};
        
};    

template<size_t S>
class melody_t:public melody_base_t{
    public:
        melody_t(std::array<melody_note_t,S> array_in,bool priority):
        note_sequence(array_in),
        _priority(priority)
        {};

         melody_note_t getNote(int i){return note_sequence.at(i);}; //bounds checking
         bool getPriority(){return _priority;};
        size_t getSize(){return _size;};
         melody_base_t* get(){return this;}; // implict cast operator
         

    private:
        std::array<melody_note_t,S> note_sequence;
        bool _priority;
        static constexpr size_t _size = S;

};





#endif