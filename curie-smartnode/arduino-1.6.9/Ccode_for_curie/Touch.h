#ifndef TOUCH_H
#define TOUCH_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"


class Touch:public Node
{
    private:
        String name;
        uint8_t DPin;
        uint8_t funType;
        uint16_t interval;
        QueueList<Node*> node;
        uint8_t intervalID;
        uint8_t lastValue;
        uint8_t status=0;
        bool debug;
    public:

        Touch();
        ~Touch();
        void setUp(String name,uint8_t DPin,uint8_t funType,QueueList<Node*> &node,bool debug);
        void callBack();
};

Touch::Touch(){}
Touch::~Touch(){
    timer.clearInterval(intervalID);
}

void Touch::setUp(String name,uint8_t DPin,uint8_t funType,QueueList<Node *> &node,bool debug){
    this->name = name;
    this->DPin = DPin;
    this->node = node;
    this->interval = 200;
    this->funType = funType;
    this->debug = debug;

    intervalID = timer.setInterval(this->interval,this);

    if (debug){
        Serial.print(name);
        Serial.println(" start");
    }
}
void Touch::callBack(){
    int16_t msg[1]={0};
    int sensorValue = digitalRead(DPin);

    if (funType == 0){ //Reset Button
        if (lastValue != sensorValue){
            msg[0] = sensorValue;
            eventStore.addEvent(node,input,"payload",msg,1);
                

            if (debug){ //debug
                if (sensorValue == 1){
                    Serial.print("KeyDown:"); 
                    Serial.println(sensorValue);
                }
                else if (sensorValue == 0){
                    Serial.print("KeyUp:"); 
                    Serial.println(sensorValue);
                }  
            }
                    
        }
        lastValue = sensorValue;
    }

    if (funType == 1){ //Locking Press-Button
        if (sensorValue == 0 && lastValue == 1){
            status?status = 0:status = 1;
            msg[0] = status;
            eventStore.addEvent(node,input,"payload",msg,1);
            

            if (debug){ //debug
                if (status == 1){
                    Serial.print("ON:"); 
                    Serial.println(status);
                }
                else if (status == 0){
                    Serial.print("OFF:"); 
                    Serial.println(status);
                } 
            }
        }
        lastValue = sensorValue;
    }
}

#endif
