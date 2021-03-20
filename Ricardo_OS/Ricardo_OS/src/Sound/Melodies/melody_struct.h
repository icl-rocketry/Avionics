#include "pitches.h"
#include <array>


class melody_base_t{
    public:
        virtual uint16_t getPitch(int i);
        virtual uint16_t getDuration(int i);
        virtual uint8_t getPriority();
        //virtual size_t getSize();
};    

template<size_t S>
class melody_t:melody_base_t{
    public:
         uint16_t getPitch(int i){return pitches[i]};
         uint16_t getDuration(int i){return duration[i]};
         uint8_t getPriority(){return priority};



    private:
        uint16_t pitches[s];
        uint16_t  duration[s];
        static constexpr size_t size = S;
        uint8_t priority;


};




