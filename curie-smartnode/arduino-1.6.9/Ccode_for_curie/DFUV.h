#ifndef DFUV_H
#define DFUV_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class DFUV:public Node
{
    private:
        String name;
        uint8_t APin;
        QueueList<Node*> node;
        uint8_t interval;
        uint8_t intervalID;
        bool debug;
    public:
        DFUV();
        ~DFUV(); 
        void setUp(String name,uint8_t DFDPin,uint16_t impulse,QueueList<Node*> &node,bool debug);
        void callBack();

};

DFUV::DFUV(){}
DFUV::~DFUV(){
    timer.clearInterval(intervalID);
}

void DFUV::setUp(String name,uint8_t APin,uint16_t interval,QueueList<Node *> &node,bool debug){
    this->name = name;
    this->APin = APin;
    this->interval = interval;
    this->node = node;
    this->debug = debug;
    pinMode(APin, INPUT);

    intervalID = timer.setInterval(this->interval,this);
    if (debug){
        Serial.print(name);
        Serial.println(" start");
    }
}

void DFUV::callBack(){
    int16_t msg[1]={0};
    uint8_t uvLevel = analogRead(APin);
    float outputVoltage = 5.0 * uvLevel/1024;
    float uvIntensity = (outputVoltage - 0.99) * (2.9 -  0.0) / (2.9 - 0.0) + 15.0;

    msg[0]=uvIntensity;
    eventStore.addEvent(node,input,"payload",msg,1);
    

    if (debug){   
        Serial.print("UVAnalogOutput: ");
        Serial.print(uvLevel);
     
        Serial.print(" OutputVoltage: ");
        Serial.print(outputVoltage);
     
        Serial.print(" UV Intensity: ");
        Serial.print(uvIntensity);
        Serial.println(" mW/cm^2");
    }
}
#endif



