#ifndef DFLIGHT_H
#define DFLIGHT_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class DFLight:public Node
{
  private:
    String name;
    uint8_t APin;
    uint16_t interval;
    QueueList<Node*> node;
    bool is_off;
    uint8_t intervalID;
    bool debug;
    int oldSensorValue;
  public:
    DFLight();
    ~DFLight();
    void 
      setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node*> &node,bool debug),
      onMsg();
    void callBack();
};
DFLight::DFLight(){
    is_off=true;
}
DFLight::~DFLight(){
    timer.clearInterval(intervalID);
}
void DFLight::setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node*> &node,bool debug){
    this->name=name;
    this->APin=APin;
    this->interval=interval;
    this->node=node;
    this->debug=debug;
}
void DFLight::onMsg(){
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
    if (eventStore.getEvent().msg[0]==0){
        if (!is_off){
            timer.clearInterval(intervalID);
            is_off=true;
        }
    }
}
void DFLight::callBack(){
    int16_t msg[1]={0};
    int sensorValue = analogRead(APin);
    if (sensorValue != oldSensorValue){
        msg[0]=sensorValue;
        eventStore.addEvent(node,input,"payload",msg,1);
        oldSensorValue = sensorValue;
    
        if (debug){
            Serial.print("Light Analog Value:");
            Serial.println(sensorValue);
        }
    }
}
#endif

