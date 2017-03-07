#ifndef TIMER_H
#define TIMER_H
#include <CurieTimerOne.h>
#define MAXTIMER 10
void timerCallBack();
typedef struct timerList
{
	bool is_using=false;
	uint16_t interval=0;
	uint16_t count=0;
	Node* node;
	
}timerList;
class Timer
{
private:
	timerList timers[MAXTIMER];
public:
	Timer();
	~Timer();
	void
		setUp(),
		clearInterval(uint8_t id),
		exec();
	int8_t setInterval(uint16_t interval,Node* node);
};
Timer::Timer(){}
Timer::~Timer(){}
void Timer::setUp(){
	CurieTimerOne.start(1000,&timerCallBack);
}
int8_t Timer::setInterval(uint16_t interval,Node* node){
	int8_t i=0;
	for(;i<MAXTIMER;i++){
		if(!timers[i].is_using){
			//if(node->callBack==NULL)	
				//Serial.println("there was a parameter has not been set at \"setInterval\" ");
			//else{
				timers[i].is_using=true;
				timers[i].interval=interval;
				timers[i].count=interval;
				timers[i].node=node;
				break;
			//}
		}
	}
  	if(i==MAXTIMER){
    	i=-1;
	  	Serial.println("Error:the number of Timers overflow");
	}
  	return i;
}
void Timer::clearInterval(uint8_t id){
	timers[id].is_using=false;
}
void Timer::exec(){
	for(uint8_t i=0;i<MAXTIMER;i++){
		if(timers[i].is_using){
			timers[i].count--;
			if(timers[i].count==0){
				timers[i].count=timers[i].interval;
				timers[i].node->callBack();
			}
		}
	}
}
Timer timer;
void timerCallBack(){
	timer.exec();
}

#endif
