#define FSRpin      0      
#define NUM_MODULES 2
#define DATA_SIZE   4
#define BLEED_TIME  1000

#define C_BLEED     0
#define C_STOP      1

byte modules[NUM_MODULES]={0,1};
unsigned long previousMillis=0;
byte inPins[]={FSRpin};
//setup methods
int init();
int send(byte addr, byte data[], int n);
int error();
void startTimer();
int maxTimer(unsigned long t);
//
int bleedModule(byte addr);
int bleed(byte addr, byte pump);
int massFSR();

//sensor variables
int threshold = 0;

void setup() {
    delay(100);
    if(!init())
        Serial.println("Initialization Failed");
    threshold = massFSR();
}

void loop() {

}

int init() {
    //Check connected
    for(int i=0; i<NUM_MODULES; i++) {
        if(!send(modules[i], 0, 1))
            return error();
    }
    for(int i=0; i<NUM_MODULES; i++) {
        if(!bleedModule(modules[i]))
            return error();
    }
}

int send(byte addr, byte data[], int n) {
    if(!Wire.isEnabled())
        Wire.begin();
    Wire.beginTransmission(addr);
    Wire.write(data, n);
    if(Wire.endTransmission())
        return error();
    return 0;
}

int error() {
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


//bleed both pumps
int bleedModule(byte addr) {
    if(!bleed(addr, 0))
        return error();
    if(!bleed(addr, 1))
        return error();
} 

//bleed one pump
int bleed(byte addr, byte pump) {
    byte temp[2] = {pump, C_BLEED};
    if(!send(addr, temp, 2))
        error();
    temp[1] = C_STOP;
    startTimer();
    while(massChange()<10 && !maxTimer(BLEED_TIME));
    if(!send(addr, temp, 2))
        error();
    return 0;
}

//get instantaneous FSR mass
int massFSR() {
    int sensorValue = 0;
    float output = 0.0; //mapped to smaller range
    sensorValue = analogRead(FSRpin);
    sensorValue = (float)sensorValue;
    threshold = (float)threshold;
    //scale from [0,1023] to [0,10]
    if(sensorValue>threshold)
        output = (sensorValue-threshold)/102.3;
    else
        output = 0;
    if(output<0 || output>10)
        return error();
    return output;
}

int massChange() {
    return -1;
}