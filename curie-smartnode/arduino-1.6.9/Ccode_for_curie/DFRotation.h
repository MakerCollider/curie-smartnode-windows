#ifndef DFROTATION_H
#define DFROTATION_H
extern "C" {
  #include <stdint.h>
}
#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"
class DFRotation:public Node
{
  private:
      String name;
      uint16_t interval;
      uint8_t APin;
      QueueList<Node *> node;
      bool is_off;
      bool debug;
      uint8_t intervalID;
      int oldOutputValue;
  public:
      DFRotation();
      ~DFRotation();
      void 
        setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node *> &node, bool debug),
        onMsg();
      void callBack();
};
DFRotation::DFRotation(){  
  is_off = true;
}
DFRotation::~DFRotation(){}
void DFRotation::setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node *> &node,bool debug){
    this->name=name;
    this->interval = interval;
    this->APin = APin;
    this->node = node;
    this->debug = debug;
}
void DFRotation::onMsg(){

    if(debug){
      Serial.print(name);
      Serial.println(" start");
    }

    if (eventStore.getEvent().msg[0]==1){
        if (is_off){
            intervalID=timer.setInterval(this-> interval,this);
            is_off=false;
        }   
    }
    if (eventStore.getEvent().msg[0]==0){
        if(!is_off){
            timer.clearInterval(intervalID);
            is_off=true;
        }
    }
}
void DFRotation::callBack(){
    int16_t msg[1]={0};
    int sensorValue = analogRead(APin);  // value read from the pot
    // map it to the range of the analog out:
    int outputValue = map(sensorValue, 0, 1023, 0, 255); // value output to the PWM (analog out)
    
    if (outputValue != oldOutputValue){
        msg[0] = outputValue;
        eventStore.addEvent(node,input,"payload",msg,1);
        oldOutputValue = outputValue;

        if (debug){
            Serial.print("Rotation Analog Value:");
            Serial.println(outputValue);
        }
    }
}
#endif
