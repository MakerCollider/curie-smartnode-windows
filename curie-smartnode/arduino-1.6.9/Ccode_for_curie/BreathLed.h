#ifndef BREATHLED_H
#define BREATHLED_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"
class BreathLed:public Node
{
    private:
    	  String name;
    	  uint8_t PWMPin;
      	QueueList<Node *> node;
        bool debug;
        uint16_t interval;
        uint8_t intervalID;
        bool is_off;
    public:
    	  BreathLed();
       	~BreathLed();
    	  void setUp(String name,uint8_t PWMPin,uint16_t interval,QueueList<Node *> &node,bool debug);
        void onMsg();
        void callBack();
	
};
BreathLed::BreathLed(){
     is_off = true;
}
BreathLed::~BreathLed(){}
void BreathLed::setUp(String name,uint8_t PWMPin,uint16_t interval,QueueList<Node *> &node,bool debug){
  	this->name=name;
  	this->PWMPin=PWMPin;
  	this->interval=interval;
    this->node=node;
    this->debug=debug;
  	pinMode(PWMPin,OUTPUT);
}
void BreathLed::onMsg(){
    if (debug){
        Serial.print(name);
        Serial.print(" start");
    }

    if (eventStore.getEvent().msg[0]==1){
        if (is_off){
            intervalID=timer.setInterval(1000,this);
            is_off=false;

            if (debug){
                Serial.print(" ON:");
                Serial.println(eventStore.getEvent().msg[0]);
            }
            
        }   
    }
    if (eventStore.getEvent().msg[0]==0){
        if (!is_off){
            analogWrite(PWMPin,0);
            timer.clearInterval(intervalID);
            is_off=true;

            if (debug){
                Serial.print(" OFF:");
                Serial.println(eventStore.getEvent().msg[0]);
            }
            
        }
    }
}
void BreathLed::callBack(){
      for (byte value = 0 ; value <255 ; value++){ 
          analogWrite(PWMPin, value);
          delay(interval);

      } 
      for (byte value = 255;  value > 0 ; value--){ 
          analogWrite(PWMPin, value); 
          delay(interval); 
      }
}

#endif
