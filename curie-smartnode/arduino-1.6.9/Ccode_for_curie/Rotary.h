#ifndef ROTARY_H
#define ROTARY_H
extern "C" {
  #include <stdint.h>
}
#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"
class Rotary:public Node
{
  private:
      String name;
      uint16_t interval;
      uint8_t APin;
      QueueList<Node *> node;
      bool is_off;
      bool debug;
      uint8_t intervalID;
  public:
      Rotary();
      ~Rotary();
      void 
        setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node *> &node,bool debug),
        onMsg();
      void callBack();
};
Rotary::Rotary(){  
  is_off=true;
}
Rotary::~Rotary(){}
void Rotary::setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node *> &node,bool debug){
    this->name = name;
    this->interval = interval;
    this->APin = APin;
    this->node = node;
    this->debug = debug;
}
void Rotary::onMsg(){
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
    if(eventStore.getEvent().msg[0]==0){
        if (!is_off){
            timer.clearInterval(intervalID);
            is_off=true;
        }
    }
}
void Rotary::callBack(){
    int16_t msg[1]={0};
    int sensorValue=analogRead(APin); 
    float voltage = (float)sensorvalue*5/1023;
	float degrees = (voltage*300)/5;
	msg[0]=degrees;
    eventStore.addEvent(node,input,"payload",msg,1);
    
    
    if (debug){
        Serial.print("Rotary Analog Value:");
        Serial.println(sensorValue);
    }
}
#endif
