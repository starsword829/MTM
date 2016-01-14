#include <PID_v1.h>
#include <SPI.h>
#include <MAX31865.h>

#define RTD_CS_PIN   	10
#define HEATER_PIN		9
#define SERIAL_INC		500

void serialSend();

MAX31865_RTD rtd( MAX31865_RTD::RTD_PT100, RTD_CS_PIN );
SPISettings settingsRTD(8000000, MSBFIRST, SPI_MODE3);

double kp=15.95, ki=0.08, kd=0.00;
double input=0, output=0, setpoint=0;
unsigned long serialTime;

PID myPID(&input, &output, &setpoint,kp,ki,kd, DIRECT);

void setup() {
	SPI.begin();
	delay( 100 );
	SPI.beginTransaction(settingsRTD);
	rtd.configure( true, true, false, true, MAX31865_FAULT_DETECTION_NONE, true, true, 0x0000, 0x7fff );
	myPID.SetMode(AUTOMATIC);
	pinMode(HEATER_PIN, OUTPUT);
	analogWrite(HEATER_PIN, 0);
	serialTime=0;
	Serial.begin(9600);
}

void loop() {
	unsigned long now=millis();
	rtd.read_all( );
    input=rtd.temperature( );
	myPID.Compute();
	analogWrite(9,output); 
	if(millis()>serialTime)
	{
		serialSend();
		serialTime+=SERIAL_INC;
	}
	while(Serial.available()>1) {
		setpoint=10*(double)(Serial.read()-'0');
		setpoint+=(double)(Serial.read()-'0');
	}

}

void serialSend()
{
	Serial.print("setpoint: ");Serial.print(setpoint); Serial.print(" ");
	Serial.print("input: ");Serial.print(input); Serial.print(" ");
	Serial.print("output: ");Serial.print(output); Serial.println(" ");
}