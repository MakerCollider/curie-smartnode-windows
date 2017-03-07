##ifndef GLOBAL_H
#define GLOBAL_H 
#include <Arduino.h>
#include <QueueList.h>

class Global
{
	typedef struct global
	{
		String name;
		int16_t value;
	}global;
private:
	QueueList<global> globalList;
public:
	Global();
	~Global();
	void set(String &name,int16_t &value);
	int16_t get(String &name);
};
void Global::set(String &name,int16_t &value){
	global temp={name,value};
	bool isExist=false;
	for(int i=0;i<globalList.count();i++){
		if(globalList.at(i).name==name){
			globalList.replace(i,temp);
			isExist=true;
			break;
		}
	}
	if(!isExist)
		globalList<<temp;
	
}
int16_t Global::get(String &name){
	for(int i=0;i<globalList.count();i++){
		if(globalList.at(i).name==name){
			return globalList.at(i).value;
		}
	}
	Serial.println("this global variable is not exist!");
}
#endif