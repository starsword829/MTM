//MTM_MODULE.ino

#include <Wire.h>
#define commandSize 6
#define returnSize  6
#define IN1PIN      3
#define IN2PIN      5
#define IN3PIN      6
#define IN4PIN      9
#define M12SET      0
#define M1BRAKE     1
#define M2BRAKE     2
#define addrSize    3
#define ADDR1		0
#define ADDR2		1
#define ADDR3		2
//byte dipPins[]={4,7,8,12};
byte inPins[]={IN1PIN,IN2PIN,IN3PIN,IN4PIN};
byte addr[]={ADDR1, ADDR2, ADDR3};
byte inData[commandSize]={0,0,0,0,0,0};
byte outData[returnSize]={0,0,0,0,0,0};
byte curData[commandSize]={0,0,0,0,0,0};
boolean readyData;


void setup(){
    noInterrupts();
    //Serial.begin(9600);
    for(int i=0;i< sizeof(inPins); i++){			//configures inPins 
        pinMode(inPins[i],OUTPUT);
    }
    for(int i=0;i<addrSize;i++){	//join I2C
    	Wire.begin(addr[i]);
    }
    //addr=address();
    readyData=false;
    Wire.onRequest(requestHandler); //calls requestH upon data request from master
    Wire.onReceive(receiveHandler); //calls receiveH upon receiving data
}
void requestHandler(){
    Wire.write(outData,commandSize);
}
void receiveHandler(int numBytes){
    int i=0;
    while(Wire.available() && i<commandSize){
        inData[i]=Wire.read();
        i++;
    }

    readyData=true;	
}


void loop(){
    if(readyData){
        for(int i=0;i<commandSize;i++){
            curData[i] = inData[i]; 
            outData[i] = inData[i]; 
        }
        readyData = false;
    }
    command();
}
void command(){ //motor controls
//0: mode 		1: pump#		2: dir 		3: rate
    switch(curData[0]){
        case 0: //run motor
            if(curData[1]==0)	//pump0
                runMotor(0,curData[2], curData[3]);
            else 			 	//pump1
            	runMotor(1,curData[2], curData[3]);
        break;
        case 1://SSR x2 for sugar water and tea
            
        break;
        default: //ping
        break;
    }
}
void runMotor(int motor, byte dir, byte speed)
{
    if(dir==0){ //stop
    	analogWrite(inPins[motor],255);
    	analogWrite(inPins[motor+1],255);
    }
    if(dir==1){ //forward
	    analogWrite(inPins[motor*2],0);
	    analogWrite(inPins[motor*2+1],speed);
	}
	else if(dir==2){ //backward
		analogWrite(inPins[motor*2],speed);
    	analogWrite(inPins[motor*2+1],0);
	}
}
