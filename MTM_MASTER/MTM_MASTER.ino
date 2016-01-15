#include <Wire.h>
#define FSR_PIN     0      
#define NUM_MODULES 1
#define DATA_SIZE   4

#define P_TIMEOUT  1000
#define P_START     0
#define P_STOP      1
#define BLEED_MASS  0.1
#define MILK_MASS   3
#define TEA_MASS    2

byte modules[NUM_MODULES] = {0};
unsigned long previousMillis = 0;
//setup functions
int initialize();
int send(byte addr, byte data[], int n);
int sendTest(byte addr, int data);
int error();
void startTimer();
int maxTimer(unsigned long t);
//pump functions
int runPump(byte addr, byte pump, float mass);
int bleed(byte addr);
int dispenseMilk(byte addr);
int dispenseTea(byte addr);
int massFSR();
int massChange(int initMass);

//sensor variables
int threshold = 0;                         //initial FSR value
float mass[] = {BLEED_MASS, MILK_MASS, TEA_MASS};       //mass change setting for pump modes
byte test[] = {0,1,2,3,4,5};

void setup() {
    Serial.begin(9600);
    send(0, test, 6);
    delay(100);
    if(!initialize()) {
        Serial.println("Initialization Failed");
    }
    threshold = massFSR();

}

void loop() {
    sendTest(0, massFSR());
    Serial.println(massFSR());
    delay(100);
}

int initialize() {
    //Check connected
    for(int i=0; i<NUM_MODULES; i++) {
        if(!send(modules[i], 0, 1)) {
            return error();
        }
    }
    for(int i=0; i<NUM_MODULES; i++) {
        if(!bleed(modules[i])) {
            return error();
        }
    }
    return 0;
}

int send(byte addr, byte data[], int n) {
    //if(!Wire.isEnabled())
    Wire.begin();
    Wire.beginTransmission(addr);
    Wire.write(data, n);
    if(Wire.endTransmission() != 0) {
        return error();
    }
    Serial.print("Sent");
    return 0; //returns 0 if sent successfully
}

int sendTest(byte addr, int data){
    Wire.begin();
    Wire.beginTransmission(addr);
    Wire.write(data);
    if(Wire.endTransmission() != 0) {
        return error();
    }
    Serial.println("Sent from sendTest");
    return 0; //returns 0 if sent successfully
}

int error() {
    Serial.println("ERROR");
    return -1;
}

void startTimer() {
    previousMillis=millis();
}

int maxTimer(unsigned long t) {
    unsigned long currentMillis=millis();
    if((unsigned long)(currentMillis-previousMillis)>=t) {
        return -1;
    }
    return 0;
}

//pumps (mass) of liquid
int runPump(byte addr, byte pump, float mass) {
//0: mode       1: pump#        2: dir (0-2)      3: rate (0-255)
    byte temp[4] = {P_START, pump, 1, 255};     //start pump command
    if(!send(addr, temp, 2))
        error();                        //error if unsuccessful transmission
    temp[0] = P_STOP;                   //stop pump command
    startTimer();
    int initMass = massFSR();
    while(massChange(initMass)< mass && !maxTimer(P_TIMEOUT));
    if(!send(addr, temp, 2)) {
        error();
    }
    return 0;
}

//clear air from both pumps
int bleed(byte addr) {
    if(!runPump(addr, 0, mass[0]))
        return error();
    if(!runPump(addr, 1, mass[0]))
        return error();
} 
//
int dispenseMilk(byte addr){
    if(!runPump(addr, 0, mass[1]))          //pump0 is for milk
        return error();
}
int dispenseTea(byte addr){
    if(!runPump(addr, 1, mass[2]))          //pump1 is for tea
        return error();
}

//get instantaneous FSR mass
int massFSR() {
    int sensorValue = 1;
    int output = 2;
    sensorValue = analogRead(FSR_PIN);
    sensorValue = (float)sensorValue;
    threshold = (float)threshold;
    //scale from [0,1023] to [0,10]
    if(sensorValue>=threshold)
         output = (sensorValue-threshold);///102.3;
     else
         output = 0;
    if(output<0 || output>1023)
        return error();
    return output;
}

int massChange(int initMass) {
    return massFSR()-initMass;
}
