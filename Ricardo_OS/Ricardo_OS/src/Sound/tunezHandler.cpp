#include "tunezHandler.h"
#include <Arduino.h>
#include "driver/ledc.h"
#include "Melodies/melodyClass.h"
#include <vector>
#include "ricardo_pins.h"


TunezHandler::TunezHandler()
{ 
    tune_queue.reserve(10); //we shoudlnt really need more than 10
};

void TunezHandler::setup()
{
    ledcSetup(0,0,8);
    ledcAttachPin(Buzzer,0);
    ledcWrite(0,_volume); //max volume
    ledcWriteTone(0,0); //write 0 hz so no noise

};

void TunezHandler::play_by_idx(int song_idx,bool loop){
    if (song_idx > MelodyLibrary::songLibrary.size()){
        return;
    }
    play(*MelodyLibrary::songLibrary.at(song_idx),loop);

}


void TunezHandler::play(melody_base_t &melody,bool loop){

     tune_t new_tune{&melody,0,loop};


     if(melody.getPriority()){
         //high priority tune
         //insert high priority tune at front of vector 
        tune_queue.insert(tune_queue.begin(),new_tune);
        note_duration = 0; //reset note_duration to force update function to play now

     }else{
         //low priority tune
        //check if current playing tune is low priority and is looping
        //if this is ture then remove that tune
        if(tune_queue.size() > 0){
            if(!(tune_queue.front().melody->getPriority()) && tune_queue.front().loop){
                tune_queue.erase(tune_queue.begin()); // remove the first element in the tune queue
            }
        }

         tune_queue.push_back(new_tune);//add new tune to end of queue

     }

     


};



void TunezHandler::update(){

    if(tune_queue.size() > 0){ //check there are tunez to play
        
        if (((millis() - prev_time) > note_duration)){
            prev_time = millis(); //update previous time

            //time to update index to next on
            if(tune_queue.front().index < tune_queue.front().melody->getSize()){
                //get new freuqnecy and note duration
                uint16_t new_frequency = tune_queue.front().melody->getNote(tune_queue.front().index).pitch;
                note_duration = tune_queue.front().melody->getNote(tune_queue.front().index).duration;
                //update ledc driver with new frequnecy
                ledcWriteTone(0,new_frequency);
                tune_queue.front().index++; //increment index by 1
            }else{
                //reached the end of the melody
                if (tune_queue.front().loop){//if we are looping
                    tune_queue.front().index = 0;
                }else{
                    //delete the first element in the tune queue 
                    tune_queue.erase(tune_queue.begin());
                }
            }
            
        }
    }else{
        ledcWriteTone(0,0);
       
    }
};


void TunezHandler::skip(){//note we need to ensure this is safe if there are no element in the queue
    if (tune_queue.size() > 0){
        tune_queue.erase(tune_queue.begin()); // remove first element in tune queue to skip current melody
    }
}

void TunezHandler::clear(){
    tune_queue.clear();
}