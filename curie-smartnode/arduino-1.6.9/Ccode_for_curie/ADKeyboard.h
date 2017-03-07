#ifndef ADKEYBOARD_H
#define ADKEYBOARD_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class ADKeyboard:public Node
{
    private:
        String name;
        uint8_t APin;
        uint16_t S1to;
        uint16_t S1from;
        uint16_t S2to;
        uint16_t S2from;
        uint16_t S3to;
        uint16_t S3from;
        uint16_t S4to;
        uint16_t S4from;
        uint16_t S5to;
        uint16_t S5from;
        uint16_t interval;
        uint8_t lastKeyValue;
        QueueList<Node*> node;
        uint8_t intervalID;
        bool debug;
       
    public:
        ADKeyboard();
        ~ADKeyboard(); 
        void setUp(String name,uint8_t APin,uint16_t S1to,uint16_t S1from,uint16_t S2to,uint16_t S2from,uint16_t S3to,uint16_t S3from,uint16_t S4to,uint16_t S4from,uint16_t S5to,uint16_t S5from,uint16_t interval,QueueList<Node*> &node,bool debug);
        void callBack();
};

ADKeyboard::ADKeyboard(){}
ADKeyboard::~ADKeyboard(){
    timer.clearInterval(intervalID);
}

void ADKeyboard::setUp(String name,uint8_t APin,uint16_t S1from,uint16_t S1to,uint16_t S2from,uint16_t S2to,uint16_t S3from,uint16_t S3to,uint16_t S4from,uint16_t S4to,uint16_t S5from,uint16_t S5to,uint16_t interval,QueueList<Node *> &node,bool debug){
    this->name = name;
    this->APin = APin;
    this->interval = interval;
    this->node = node;

    pinMode(APin, INPUT);

    this->S1from = S1from;
    this->S1to = S1to;
    this->S2from = S2from;
    this->S2to = S2to;
    this->S3from = S3from;
    this->S3to = S3to;
    this->S4from = S4from;
    this->S4to = S4to;
    this->S5from = S5from;
    this->S5to = S5to;

    this->debug = debug;
    lastKeyValue = 0;

    intervalID = timer.setInterval(interval,this);

    if (debug){
    	Serial.print(name);
    	Serial.println(" start");
	}
}

void ADKeyboard::callBack(){
    int16_t msg[2]={0};
    int sensorValue = analogRead(APin);
    uint8_t keyValue = -1;
    if (sensorValue >= S1from && sensorValue <= S1to){
        keyValue = 1;
    }
    else if (sensorValue >= S2from && sensorValue <= S2to){
        keyValue = 2;
    }
    else if (sensorValue >= S3from && sensorValue <= S3to){
        keyValue = 3;
    }
    else if (sensorValue >= S4from && sensorValue <= S4to){
        keyValue =  4;
    }
    else if (sensorValue >= S5from && sensorValue < S5to){
        keyValue =  5;
    }
    else{
        keyValue = 0;
    }

    if (lastKeyValue != keyValue){
        msg[0] = keyValue;
        msg[1] = sensorValue;
        eventStore.addEvent(node,input,"payload",msg,2);
        lastKeyValue = keyValue;


        if (debug){
            Serial.print("Analog Value:");
            Serial.print(sensorValue);
            Serial.print(" Key Value:");
            Serial.println(keyValue);
        }
    }
}
#endif




