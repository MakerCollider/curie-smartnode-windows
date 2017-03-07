#ifndef DFPIRMOTION_H
#define DFPIRMOTION_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class DFPIRMotion:public Node
{
    private:
        String name;
        uint8_t DPin;
        uint16_t interval;
        QueueList<Node*> node;
        uint8_t intervalID;
        bool debug;
    public:
        DFPIRMotion();
        ~DFPIRMotion();
        void setUp(String name,uint8_t DPin,uint16_t interval,QueueList<Node*> &node,bool debug);
        void callBack();

};

DFPIRMotion::DFPIRMotion(){}
DFPIRMotion::~DFPIRMotion(){
    timer.clearInterval(intervalID);
}

void DFPIRMotion::setUp(String name,uint8_t DPin,uint16_t interval,QueueList<Node *> &node,bool debug){
    this->name = name;
    this->DPin = DPin;
    this->interval = interval;
    this->node = node;
    this->debug=debug;
    pinMode(DPin, INPUT);
    
    intervalID = timer.setInterval(this->interval,this);

    if (debug){
        Serial.print(name);
        Serial.println(" start");
    }
}

void DFPIRMotion::callBack(){
    int16_t msg[1]={0};
    int sensorValue = digitalRead(DPin);
    msg[0]=sensorValue;
    eventStore.addEvent(node,input,"payload",msg,1);
    

    if (debug){
        if (sensorValue == 1){
            Serial.print(name);
            Serial.println(": Somebody is in this area!");
        }
        else if (sensorValue == 0){
            Serial.print(name);
            Serial.println(": No Body!");
        }
    }
}
#endif




