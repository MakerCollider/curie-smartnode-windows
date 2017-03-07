#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include <CurieIMU.h>
#include "Timer.h"


class Accelerometer:public Node{
    private:
        String name;
        QueueList<Node*>node; //存储节点
        bool is_off;
        uint16_t interval;
        uint8_t intervalID;
        bool debug;
    public:
        Accelerometer();
        ~Accelerometer();
        void 
            setUp(String name,uint16_t interval,QueueList<Node*> &node,bool debug),
            onMsg();
        float convertRawAcceleration(int aRaw);//限制加速度范围（0-2g）
        void callBack();
};
Accelerometer::Accelerometer(){ 
    is_off=true;
}
Accelerometer::~Accelerometer(){
    timer.clearInterval(intervalID);
}

void Accelerometer::setUp(String name,uint16_t interval,QueueList<Node*> &node,bool debug){
    this->name = name;
    this->node = node;
    this->interval = interval;
    this->debug = debug;


    //Serial.print("CurieIMU initializing...");
    CurieIMU.begin();
    //Serial.println("Done");
    //Serial.println("About to calibrate. Make sure your board is stable and upright");//关于校准。确保你的板子是稳定和正直的

    //Serial.print("Starting Acceleration calibration...");//起动加速度校准…
    CurieIMU.autoCalibrateXAccelOffset(0);
    CurieIMU.autoCalibrateYAccelOffset(0);
    CurieIMU.autoCalibrateZAccelOffset(1);

    //使加速度偏移补偿
    //Serial.println("Enabling Acceleration offset compensation");
    CurieIMU.setAccelOffsetEnabled(true);
    CurieIMU.setAccelerometerRange(2);

}

float Accelerometer::convertRawAcceleration(int aRaw){
    float a = (aRaw * 2.0) / 32768.0;
    return a;
}
void Accelerometer::onMsg(){
    
    if (debug){
        Serial.print(name);
    	Serial.println(" Start!");
    }

    if (eventStore.getEvent().msg[0]>0){
        if (is_off){ 
            intervalID = timer.setInterval(this->interval,this);   
            is_off = false; 
        }   
    }
    if (eventStore.getEvent().msg[0]==0){
        if (!is_off){
            timer.clearInterval(intervalID);
            is_off = true;
        }
    } 
}

void Accelerometer::callBack(){ 

    int axRaw, ayRaw, azRaw;
    float value1[3];
    float Data_Raw[10][3];
    float Data_Deal[3]={0};
    //10组数据
    for(int i = 0;i < 10;i++){
        CurieIMU.readAccelerometer(axRaw, ayRaw, azRaw);
        value1[0] = convertRawAcceleration(axRaw);
        value1[1] = convertRawAcceleration(ayRaw);
        value1[2] = convertRawAcceleration(azRaw);
        for(uint8_t j=0;j<3;j++){
            Data_Raw[i][j] = value1[j];
        }
    }

    //均值滤波处理   
    for(int n=0;n<10;n++){           
        Data_Deal[0] += Data_Raw[n][0];  
        Data_Deal[1] += Data_Raw[n][1];  
        Data_Deal[2] += Data_Raw[n][2];     
    }

    for(uint8_t j=0;j<3;j++){
        Data_Deal[j]=(Data_Deal[j]/10)*127;
    }

    //Serial显示数据 
    if (debug){
        Serial.print("Accelerometer:");
        Serial.print("X=");
        Serial.print(Data_Deal[0]);
        Serial.print(" Y=");
        Serial.print(Data_Deal[1]);
        Serial.print(" Z=");
        Serial.println(Data_Deal[2]);
    }
    
    //添加节点事件
    int16_t msg[3]={0};
    for(int i=0;i<3;i++){
        msg[i]=Data_Deal[i];
        //msg[i]= 1;
    }
    eventStore.addEvent(node,input,"payload",msg,3);


}
#endif
