#ifndef GYOR_H
#define GYOR_H

#include <arduino.h>
#include <QueueList.h>
#include "Node.h"
#include "EventStore.h"
#include <CurieIMU.h>
#include "Timer.h"

class Gyro:public Node{
    private:
      	String name; 
        uint16_t interval;          

        QueueList<Node*>node; 
        bool is_off;
        bool debug;
        uint8_t intervalID;    
    public:
      	Gyro();
      	~Gyro();
      	void 
      		setUp(String name, uint16_t interval, QueueList<Node*> &node,bool debug),
      		onMsg();
          void callBack();
          float convertRawGyro(int gRaw);
};

Gyro::Gyro(){
    is_off = true;
}
Gyro::~Gyro(){
    timer.clearInterval(intervalID);
}

void Gyro::setUp(String name, uint16_t interval, QueueList<Node*> &node,bool debug){
    //Serial.println("Initializing IMU device...");
    CurieIMU.begin();

    CurieIMU.setGyroRange(250);
  	this->name = name;
  	this->node = node;
    this->interval = interval;
    this->debug = debug;

    CurieIMU.autoCalibrateGyroOffset();
    CurieIMU.setGyroOffsetEnabled(true);
}

float Gyro::convertRawGyro(int gRaw) {   
    float g = (gRaw * 250.0) / 32768.0;
    return g;
}

void Gyro::onMsg(){
    if (debug){
        Serial.print(name);
        Serial.println(" start");
    }

    if(eventStore.getEvent().msg[0]>0){
        if (is_off){
            intervalID = timer.setInterval(this->interval,this);
            is_off = false;
        }   
    }

    if(eventStore.getEvent().msg[0]==0){
        if (!is_off){
            timer.clearInterval(intervalID);
            is_off = true;
        }
    }
}

void Gyro::callBack(){
    

    
  

    int gxRaw, gyRaw, gzRaw;
    float value1[3];
    float Data_Raw[10][3];
    float Data_Deal[3]={0};

    //10组数据
    for(int i = 0;i < 10;i++){
        CurieIMU.readGyro(gxRaw, gyRaw, gzRaw);
        value1[0] = convertRawGyro(gxRaw);
        value1[1] = convertRawGyro(gyRaw);
        value1[2] = convertRawGyro(gzRaw);
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
        Data_Deal[j]=Data_Deal[j]/10;
    }

    //Serial显示数据 
    if (debug){
        Serial.print("Gyro:");
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
        msg[i] = Data_Deal[i];
        //msg[i]=0;
    }
    eventStore.addEvent(node,input,"payload",msg,3);

}

#endif
