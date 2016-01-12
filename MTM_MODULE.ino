//MTM_MODULE.ino

#include <Wire.h>
#define commandSize 6
#define returnSize  3
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
byte addr[]={ADDR1, ADDR2, ADDR3}
byte inData[commandSize]={};
byte outData[returnSize]={};
byte curData[commandSize]={};
boolean readyData;
byte motor1Speed=0;
byte motor2Speed=0;
char motor1Status;
char motor2Status;
void setup(){
    noInterrupts();
    //Serial.begin(9600);
    // for(int i=0;i<dip;i++){
    //     pinMode(dipPins[i],INPUT);
    //     digitalWrite(dipPins[i],HIGH);
    // }
    for(int i=0;i<4;i++){			//configures inPins 
        pinMode(inPins[i],OUTPUT);
    }
    for(int i=0;i<addrSize;i++){	//join I2C bus
    	Wire.begin(addr[i]);
    }
    //addr=address();
    readyData=false;
    Wire.onRequest(requestHandler);
    Wire.onReceive(receiveHandler);
}
byte address(){
    byte j=0;
    for(int i=0;i<dip;i++){
        j=(j<<1)|digitalRead(dipPins[i]);
    }
    return j;
}
void requestHandler()
{
    Wire.write(outData,commandSize);
}
void receiveHandler(int numBytes)
{
    int i=0;
    while(Wire.available())
    {
        inData[i]=Wire.read();
        i++;
    }
    readyData=true;
}
void loop()
{
    if(readyData)
    {
        for(int i=0;i<6;i++)
            curData[i]=inData[i];
        readyData=false;
    }
    command();
}
void command()
{
    switch(curData[0])
    {
        case 0:
            if(curData[1])
                motorForward(0,curData[2]);
            else
                motorBackward(0,curData[2]);
            if(curData[3])
                motorForward(1,curData[4]);
            else
                motorBackward(1,curData[4]);
        break;
        case 1:
            motorBrake(0);
        break;
        case 2:
            motorBrake(1);
        break;
    }
}
void motorForward(int motor,byte speed)
{
    analogWrite(inPins[motor*2],0);
    analogWrite(inPins[motor*2+1],speed);
}
void motorBackward(int motor,byte speed)
{
    analogWrite(inPins[motor*2],speed);
    analogWrite(inPins[motor*2+1],0);
}
void motorBrake(int motor)
{
    analogWrite(inPins[motor],255);
    analogWrite(inPins[motor+1],255);
}