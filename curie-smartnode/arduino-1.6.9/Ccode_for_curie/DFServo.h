#ifndef DFSERVO_H
#define DFSERVO_H

#include <arduino.h>
#include <QueueList.h>
#include <Servo.h> 
#include "Node.h"
#include "EventStore.h"
Servo servo;
class DFServo:public Node
{
    private:
        String name;
        uint8_t DPin;
        QueueList<Node*> node;
        bool debug;
    public:
        DFServo();
        ~DFServo();
    void
       setUp(String name,uint8_t DPin,QueueList<Node *> &node, bool debug),
       onMsg();
  

};

DFServo::DFServo(){}
DFServo::~DFServo(){
}

void DFServo::setUp(String name,uint8_t DPin,QueueList<Node *> &node, bool debug){
    this->name = name;
    this->DPin = DPin;
    this->node = node;
    this->debug = debug;
}

void DFServo::onMsg(){
    CurieTimerOne.stop();
  //CurieTimerOne.kill();
    servo.attach(DPin);
    int16_t pos = eventStore.getEvent().msg[0];
    servo.write(pos);

    Serial.print("Servo Analog Value:");
    Serial.println(pos);
    delay(1000);

   if (servo.attached()){
       //Serial.println(servo.attached());
       servo.detach();
       //Serial.println(servo.attached());
       CurieTimerOne.start(1000,&timerCallBack);
   }
}

#endif



