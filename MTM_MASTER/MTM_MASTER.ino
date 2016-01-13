#define NUM_MODULES 2
#define DATA_SIZE   4
#define C_BLEED     0
#define BLEED_TIME  1000

byte modules[NUM_MODULES]={0,1};
unsigned long previousMillis=0;

int error();
int send(byte addr, byte data[], int n);
int init();
int bleed();
void startTimer();
int maxTimer(unsigned long t);

void setup() {
    delay(100);
    if(!init())
        Serial.println("Initialization Failed");
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
        if(!bleed(modules[i]))
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

int bleed(byte addr) {
    if(!send(addr, C_BLEED))
        error();
    startTimer();
    while(massChange()<10 && !maxTimer(BLEED_TIME));
    if(!send(addr, STOP))
        error();
    return 0;
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

int massFSR() {
    return -1;
}

int massChange() {
    return -1;
}