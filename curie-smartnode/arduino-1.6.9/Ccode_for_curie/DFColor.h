#ifndef DFCOLOR_H
#define DFCOLOR_H
#define RED 3
#define GREEN 4
#define BLUE 5
#define YELLOW 6


#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include "Timer.h"

byte counter = 0;
void Count(){
    counter++;
}

class DFColor:public Node{
    private:
        String name;
        uint8_t s0;
        uint8_t s1;
        uint8_t s2;
        uint8_t s3;
        uint8_t s_out; 
        QueueList<Node*> node;
        //white balance params
        uint8_t maxR = 0, maxG = 0, maxB = 0; 
        double WB_RED = 1;
        double WB_GREEN = 1;
        double WB_BLUE = 1;
        uint8_t R = 0, G = 0, B = 0;
        uint8_t type;
        byte cR = 0, cG = 0, cB = 0; 
        uint8_t intervalID;
        bool debug;
    public:
        DFColor();
        ~DFColor(); 
       
        void setUp(String name,uint8_t s0,uint8_t s1,uint8_t s2,uint8_t s3,uint8_t s_out,QueueList<Node*> &node,bool debug);
        void setWhiteBalance();
        void onMsg();
        void readAverageColor();
        void colorDetection();
        void callBack();

};

DFColor::DFColor(){}
DFColor::~DFColor(){
    timer.clearInterval(intervalID);
}

void DFColor::setUp(String name,uint8_t s0,uint8_t s1,uint8_t s2,uint8_t s3,uint8_t s_out,QueueList<Node *> &node,bool debug){
    this->name=name;
    this->s0=s0;
    this->s1=s1;
    this->s2=s2;
    this->s3=s3;
    this->s_out=s_out;
    this->node=node;
    this->debug=debug;
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    pinMode(s_out, INPUT);


    digitalWrite(s0, HIGH);
    digitalWrite(s1, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    
    attachInterrupt(s_out, Count, CHANGE);
    timer.setInterval(10,this);
    
}

void DFColor::onMsg(){
    int16_t msg[3]={0};
    if (debug){
        Serial.print(name);
        Serial.println(" start");
    }
    if (eventStore.getEvent().msg[0]==0){ //设置白平衡
        setWhiteBalance();
    }
    if (eventStore.getEvent().msg[0]==1){ //采集
        readAverageColor();
        //Neurons输入信息
        msg[0]=R;
        msg[1]=G;
        msg[2]=B;
        eventStore.addEvent(node,input,"payload",msg,3);
        if(debug){
            Serial.println(msg[0]);
            Serial.println(msg[1]);
            Serial.println(msg[2]);
        }
    }
}
void DFColor::callBack(){
    switch (type){
        case RED:
            cR = counter;
            counter = 0;
            break;
        case GREEN:
            cG = counter;
            counter = 0;
            break;
        case BLUE:
            cB = counter;
            counter = 0;
            break;
    }
}
void DFColor::colorDetection(){
    //set color filter
    switch (type){
        case RED:
            digitalWrite(s2, LOW);
            digitalWrite(s3, LOW);
            break;
        case GREEN:
            digitalWrite(s2, HIGH);
            digitalWrite(s3, HIGH);
            break;
        case BLUE:
            digitalWrite(s2, LOW);
            digitalWrite(s3, HIGH);
            break;
    }
    counter = 0;
}

void DFColor::setWhiteBalance(){ 
    for(int i=0;i<3;i++){
        readAverageColor();
        if (R > WB_RED)
            maxR = R;
        if (B > WB_BLUE)
            maxB = B;
        if (G > WB_GREEN)
            maxG = G;
    }    
    WB_RED = 255.0 / (double)maxR;
    WB_BLUE = 255.0 / (double)maxB;
    WB_GREEN = 255.0 / (double)maxG;
    if(debug)
        Serial.println("White Balance set.");
    //delay(1000);
}
void DFColor::readAverageColor()
{
    int sumR = 0, sumG = 0, sumB = 0;
    for (int i = 0; i < 5; i++)
    {
        type = RED;
        colorDetection();
        delay(20);
        type = GREEN;
        colorDetection();
        delay(20);
        type = BLUE;
        colorDetection();
        delay(20);
        sumR += cR;
        sumG += cG;
        sumB += cB;
    }
    R = (sumR / 5) * WB_RED;
    G = (sumG / 5) * WB_GREEN;
    B = (sumB / 5) * WB_BLUE;

    if (debug){
        Serial.print("Color: ");
        Serial.print("R = ");
        Serial.print(cR, DEC);
        Serial.print(", G = ");
        Serial.print(cG, DEC);
        Serial.print(", B = ");
        Serial.println(cB, DEC);
    }
}

#endif
