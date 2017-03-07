#ifndef BUZZER_H
#define BUZZER_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
class Buzzer:public Node
{
    private:
        String name;
        uint8_t DPin;
        QueueList<Node *> node;
        bool debug;
    public:
        Buzzer();
        ~Buzzer();
        void
          setUp(String name,uint8_t DPin,QueueList<Node *> &node,bool debug),
          onMsg();
  
};

Buzzer::Buzzer(){}
Buzzer::~ Buzzer(){}
void Buzzer::setUp(String name,uint8_t DPin,QueueList<Node *> &node,bool debug){
    this->name=name;
    this->DPin=DPin;
    this->node=node;
    this->debug=debug;
    pinMode(DPin,OUTPUT);
}
void Buzzer::onMsg(){
    if(debug){
        Serial.print(name);
        Serial.println(" start");
    }
    
    if (eventStore.getEvent().msg[0]==1){
        digitalWrite(DPin,HIGH);
    }
    else if(eventStore.getEvent().msg[0]==0){
        digitalWrite(DPin,LOW);
    }
}
#endif
