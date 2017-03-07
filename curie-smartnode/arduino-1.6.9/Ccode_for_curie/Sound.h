#ifndef SOUND_H
#define SOUND_H
extern "C" {
	#include <stdint.h>
}
#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class Sound:public Node
{
    private:
      	String name;
      	uint16_t interval;
      	uint8_t APin;
      	QueueList<Node *> node;
        bool is_off;
        bool debug;
        uint8_t intervalID;
        int oldSensorValue;
    public:
        Sound();
        ~Sound();
        void 
        	setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node *> &node,bool debug),
        	onMsg();
        void callBack();
};
Sound::Sound(){  
    is_off=true;
}
Sound::~Sound(){
   timer.clearInterval(intervalID);
}
void Sound::setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node *> &node,bool debug){
  	this->name=name;
  	this->interval=interval;
  	this->APin=APin;
    this->node=node;
    this->debug = debug;
}
void Sound::onMsg(){
    if (debug){
        Serial.print(name);
        Serial.println(" start");
    }

    if (eventStore.getEvent().msg[0]==1){
        if (is_off){
           intervalID=timer.setInterval(this->interval,this);
           is_off=false;
        }   
    }
    else if(eventStore.getEvent().msg[0]==0){
        if (!is_off){
            timer.clearInterval(intervalID);
            is_off=true;
        }
    }
}
void Sound::callBack(){
    int16_t msg[1]={0};
	  int sensorValue = analogRead(APin);
    if (sensorValue != oldSensorValue){
  	    msg[0] = sensorValue;
  	    eventStore.addEvent(node,input,"payload",msg,1);
        oldSensorValue = sensorValue;
  	

        if (debug){
            Serial.print("Sound Analog Value:");
            Serial.println(sensorValue);
        }
   }
}
#endif
