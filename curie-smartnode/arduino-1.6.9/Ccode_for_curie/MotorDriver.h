#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

class MotorDriver:public Node
{
    private:
        String name;
        uint8_t PWMA;
        uint8_t AIN1;
        uint8_t AIN2;
        uint8_t PWMB;
        uint8_t BIN1;
        uint8_t BIN2;
        uint16_t speed;
        QueueList<Node*> node;
        bool debug;

    public:
        MotorDriver();
        ~MotorDriver();
        void setUp(String name,uint8_t PWMA,uint8_t AIN1,uint8_t INA2,uint8_t PWMB,uint8_t INB1,uint8_t INB2,uint16_t speed,QueueList<Node *> &node,bool debug);
        void Forward(int speed);
        void Backward(int speed);
        void Leftward(int speed);
        void Rightward(int speed);
        void Stop();
        void onMsg();

};

MotorDriver::MotorDriver(){}
MotorDriver::~MotorDriver(){}

void MotorDriver::setUp(String name,uint8_t PWMA,uint8_t AIN1,uint8_t AIN2,uint8_t PWMB,uint8_t BIN1,uint8_t BIN2,uint16_t speed,QueueList<Node *> &node,bool debug){
    this->name = name;
    this->node = node;
    this->PWMA = PWMA;
    this->AIN1 = AIN1;
    this->AIN2 = AIN2;
    this->PWMB = PWMB;
    this->BIN1 = BIN1;
    this->BIN2 = BIN2;
    this->speed = speed;
    this->debug = debug;


    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);

}
void MotorDriver::Forward(int speedVal) {
    if (debug){
        Serial.println(" Forward");
    }
     digitalWrite(AIN1,HIGH);
     digitalWrite(AIN2,LOW);  
     analogWrite(PWMA,speedVal);

     digitalWrite(BIN1,LOW);
     digitalWrite(BIN2,HIGH);  
     analogWrite(PWMB,speedVal);
}
  
void MotorDriver::Backward(int speedVal){   
    if (debug){
        Serial.println(" Backward");
    } 
     digitalWrite(AIN1,LOW);
     digitalWrite(AIN2,HIGH);  
     analogWrite(PWMA,speedVal);

     digitalWrite(BIN1,HIGH);
     digitalWrite(BIN2,LOW);  
     analogWrite(PWMB,speedVal);  
} 

void MotorDriver::Leftward(int speedVal){  
    if (debug){
        Serial.println(" Leftward");
    }   
     digitalWrite(AIN1,LOW);
     digitalWrite(AIN2,HIGH);  
     analogWrite(PWMA,speedVal);

     digitalWrite(BIN1,LOW);
     digitalWrite(BIN2,HIGH);  
     analogWrite(PWMB,speedVal);
} 

void MotorDriver::Rightward(int speedVal){  
    if (debug){
        Serial.println(" Rightward");
    }    
     digitalWrite(AIN1,HIGH);
     digitalWrite(AIN2,LOW);  
     analogWrite(PWMA,speedVal);

     digitalWrite(BIN1,HIGH);
     digitalWrite(BIN2,LOW);  
     analogWrite(PWMB,speedVal); 
} 

void MotorDriver::Stop(){    

     digitalWrite(AIN1,LOW);
     digitalWrite(AIN2,LOW);  
     analogWrite(PWMA,0);

     digitalWrite(BIN1,LOW);
     digitalWrite(BIN2,LOW);  
     analogWrite(PWMB,0);  
} 


void MotorDriver::onMsg(){
    if (debug){
        Serial.print(name);
        Serial.print(" start");
    }


    int16_t cmdValue =  eventStore.getEvent().msg[0];
    switch(cmdValue){
        case 1:
           Forward(speed);
            break;
        case 2:
           Leftward(speed);
           break;
        case 3:
           Rightward(speed);
           break;
        case 4:
           Backward(speed);
           break;
        default:
           Stop();
        break;
    }
}   

#endif

