#define NUM_MODULES 2
#define DATA_SIZE   4

byte modules[NUM_MODULES]={0,1};

int error();
int send(byte addr, byte data[], int n);
int init();
int bleed();

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

int bleed() {
    if()
    return 0;
}