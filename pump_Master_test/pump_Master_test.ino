#include <Wire.h>
//#define FSR_PIN     0      
#define NUM_MODULES 3
#define DATA_SIZE   4
#define ADDR1       0xAA
#define ADDR2       0xBB
#define ADDR3       0xCC

#define P_TIMEOUT  1000
#define P_START     0
#define P_STOP      1
//default masses
// #define BLEED_MASS  5
// #define TOTAL_TIME  100
// #define MILK_TIME   30
// #define SUGAR_TIME  10
// #define TEA_TIME    50

byte modules[NUM_MODULES] = {ADDR1,ADDR2,ADDR3};
unsigned long previousMillis = 0;
//setup functions
int initialize();
int send(byte addr, byte data[], int n);
int sendTest(byte addr, int data);
int error();
// void startTimer();
// int maxTimer(unsigned long t);
//int ready = 0;
//pump functions
// int runPump(byte addr, byte pump, float mass);
// int bleed(byte addr);
// int dispenseMilk(byte addr);
// int dispenseSugar(byte addr);
// int dispenseTea1(byte addr);
// int dispenseTea2(byte addr);
//measurements
// int massFSR();
// int massChange(int initMass);
// void setTime(int milk, int sugar);
// int threshold = 0;                      //initial FSR value
//float dispenseTime[] = {BLEED_MASS, MILK_TIME, SUGAR_TIME, TEA_TIME};       //mass change setting for pump modes
byte connect[] = {0,0,0,0};
byte test[] = {0,1,1,255};
byte test2[] = {0,1,0,255};

void setup() {
    Serial.begin(9600);
    if(!initialize()) {
        Serial.println("Initialization Failed");
    }
    send(ADDR1, test, 4);
    delay(1000);
    send(ADDR1, test2, 4);
    delay(1000);
    send(ADDR2, test, 4);
    delay(1000);
    send(ADDR2, test2, 4);
    delay(1000);
    test[1] = 0;
    test2[1] = 0;
    send(ADDR1, test, 4);
    delay(1000);
    send(ADDR1, test2, 4);
    delay(1000);
    send(ADDR2, test, 4);
    delay(1000);
    send(ADDR2, test2, 4);
    delay(1000);
    //threshold = massFSR();
}

void loop() {
    // sendTest(0, massFSR());
    // Serial.println(massFSR());
    // delay(100);
}

int initialize() {
    //Check connected
    for(int i=0; i<NUM_MODULES; i++) {
        if(!send(modules[i], connect, 1)) {
            return error();
        }
        Serial.println("connected");
    }
    // for(inat i=0; i<NUM_MODULES; i++) {
    //     if(!bleed(modules[i])) {
    //         return error();
    //     }
    // }
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
    Serial.println("Sent: ");
    for(int i=0; i<4; i++){
        Serial.println(data[i]);
    }
    return 0; //returns 0 if sent successfully
}

// int sendTest(byte addr, int data){
//     Wire.begin();
//     Wire.beginTransmission(addr);
//     Wire.write(data);
//     if(Wire.endTransmission() != 0) {
//         return error();
//     }
//     Serial.println("Sent from sendTest");
//     return 0; //returns 0 if sent successfully
// }

int error() {
    Serial.println("ERROR");
    return -1;
}

// void startTimer() {
//     previousMillis=millis();
// }

// int maxTimer(unsigned long t) {
//     unsigned long currentMillis=millis();
//     if((unsigned long)(currentMillis-previousMillis)>=t) {
//         return -1;
//     }
//     return 0;
// }

// //pumps (mass) of liquid
// int runPump(byte addr, byte pump, float mass) {
// //0: mode       1: pump#        2: dir (0-2)      3: rate (0-255)
//     byte temp[4] = {P_START, pump, 1, 255};     //start pump command
//     if(!send(addr, temp, 2))
//         error();                        //error if unsuccessful transmission
//     temp[0] = P_STOP;                   //stop pump command
//     startTimer();
//     int initMass = massFSR();
//     while(massChange(initMass)< mass && !maxTimer(P_TIMEOUT));
//     if(!send(addr, temp, 2)) {
//         error();
//     }
//     return 0;
// }
// ////PUMP MODES////
// int bleed(byte addr) {
//     //clear air from both pumps
//     if(!runPump(addr, 0, dispenseTime[0]))
//         return error();
//     if(!runPump(addr, 1, dispenseTime[0]))
//         return error();
// } 
// int dispenseMilk(byte addr){
//     if(!runPump(addr, 0, dispenseTime[1]))          //pump0 is for milk
//         return error();
//     return ready = 1;
// }
// int dispenseSugar(byte addr){
//      if(!runPump(addr, 0, dispenseTime[2]))          //pump1 is for sugar
//         return error();
//      return ready = 2;
// }
// int dispenseTea1(byte addr){
//     if(!runPump(addr, 1, dispenseTime[3]))          //pump2 is for tea1
//         return error();
//     return ready = 3;
// }
// int dispenseTea2(byte addr){
//     if(!runPump(addr, 1, dispenseTime[3]))          //pump3 is for tea2
//         return error();
//     return ready = 3;
// }


// //get instantaneous FSR mass
// int massFSR(){
//     int sensorValue = 0;
//     int output = 2;
//     sensorValue = analogRead(FSR_PIN);
//     //scale from [0,1023] to [0,100]
//     if(sensorValue>=threshold)
//         output = (sensorValue-threshold)/10.23;
//     else
//         output = 0;
//     if(output<0 || output>1023)
//         return error();
//     return output;
// }
// int massChange(int initMass){
//     return massFSR()-initMass;
// }
// //allows UI to set milk, sugar amount
// void setTime(int milk, int sugar){
//     dispenseTime[1] = (milk/100)*MILK_TIME; //% of default milk
//     dispenseTime[2] = (sugar/100)*SUGAR_TIME; //% of default sugar
//     dispenseTime[3] = (TOTAL_TIME-dispenseTime[1]-dispenseTime[2]); //fills cup w tea
// }