#include "tunezHandler.h"
#include <Arduino.h>
#include "driver/ledc.h"
#include "Melodies/melodyClass.h"
#include <vector>
#include "ricardo_pins.h"


TunezHandler::TunezHandler():
_playing(true)
{ 
    tune_queue.reserve(10); //we shoudlnt really need more than 10
};

void TunezHandler::setup()
{
    /*
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT, //we are driving a buzzer at full volume so we dont really care about duty res here
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 5000,//inital frquency
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_channel_config_t ledc_channel = {
        .gpio_num = Buzzer,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 256 //set duty to max for max volume     
    };*/

    //ledc_timer_config(&ledc_timer);
    //ledc_channel_config(&ledc_channel);
    ledcSetup(0,0,8);
    ledcAttachPin(Buzzer,0);
    ledcWrite(0,255); //max volume
    ledcWriteTone(0,0); //write 0 hz so no noise

};

void TunezHandler::play(melody_base_t *melody,bool loop){

     tune_t new_tune;
     new_tune.melody = melody;
     new_tune.loop = loop;

     if(melody->getPriority()){
         //high priority tune
         //insert high priority tune at front of vector 
        tune_queue.insert(tune_queue.begin(),new_tune);

     }else{
         //low priority tune
        //check if current playing tune is low priority and is looping
        //if this is ture then remove that tune
        //this is to make sure we dont contiously loop a tune without being able to override it
        if(!(tune_queue.front().melody->getPriority()) && tune_queue.front().loop){
            tune_queue.erase(tune_queue.begin()); // remove the first element in the tune queue
        }

         tune_queue.push_back(new_tune);//add new tune to end of queue

     }

     note_duration = 0; //reset note_duration to force update function to play now


};



void TunezHandler::update(){

    if(tune_queue.size() > 0){ //check there are tunez to play
        
        if (((millis() - prev_time) > note_duration) && _playing){
            //time to update index to next on
            if(tune_queue.front().index < tune_queue.front().melody->getSize()){
                //get new freuqnecy and note duration
                uint16_t new_frequency = tune_queue.front().melody->getNote(tune_queue.front().index).pitch;
                note_duration = tune_queue.front().melody->getNote(tune_queue.front().index).duration;
                //update ledc driver with new frequnecy
                //ledc_set_freq(LEDC_HIGH_SPEED_MODE,LEDC_TIMER_0,new_frequency);
                ledcWriteTone(0,new_frequency);
                tune_queue.front().index += 1; //increment index by 1
            }else{
                //reached the end of the melody
                if (tune_queue.front().loop){//if we are looping
                    tune_queue.front().index = 0;
                    //uint16_t new_frequency = tune_queue.front().melody->getNote(tune_queue.front().index).pitch;
                    //ledc_set_freq(LEDC_HIGH_SPEED_MODE,LEDC_TIMER_0,new_frequency);
                    //ledcWriteTone(0,new_frequency);
                }else{
                    //delete the first element in the tune queue 
                    tune_queue.erase(tune_queue.begin());
                }
            }
            prev_time = millis(); //update previous time
        }
    }else{
        ledcWriteTone(0,0);
        //ledc_set_freq(LEDC_HIGH_SPEED_MODE,LEDC_TIMER_0,0); // write 0 frequency so no noise is produced
    }
};

void TunezHandler::pause(){
    _playing = false;
}
void TunezHandler::unpause(){
    _playing = true;
}

void TunezHandler::skip(){//note we need to ensure this is safe if there are no element in the queue
    if (tune_queue.size() > 0){
        tune_queue.erase(tune_queue.begin()); // remove first element in tune queue to skip current melody
    }
}

void TunezHandler::clear(){
    tune_queue.clear();
}