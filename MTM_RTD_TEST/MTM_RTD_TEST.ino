#include <SPI.h>
#include <MAX31865.h>


#define RTD_CS_PIN   10


MAX31865_RTD rtd( MAX31865_RTD::RTD_PT100, RTD_CS_PIN );
SPISettings settingsRTD(8000000, MSBFIRST, SPI_MODE3);

void setup()
{
  Serial.begin( 9600 );
  SPI.begin();
  /* Allow the MAX31865 to warm up. */
  delay( 100 );

  /* Configure:
       V_BIAS enabled
       Auto-conversion
       1-shot disabled
       3-wire enabled
       Fault detection:  automatic delay
       Fault status:  auto-clear
       50 Hz filter
       Low threshold:  0x0000
       High threshold:  0x7fff
  */
  SPI.beginTransaction(settingsRTD);
  rtd.configure( true, true, false, true, MAX31865_FAULT_DETECTION_NONE,
                 true, true, 0x0000, 0x7fff );
}



void loop() 
{
  //SPI.beginTransaction(settingsRTD);
  rtd.read_all( );

  if( rtd.status( ) == 0 )
  {
    double temperature = rtd.temperature( );
    Serial.print( " T = ");
    Serial.print( temperature, 1 );
    Serial.println(" deg C" );
  }
  else 
  {
    Serial.print( "RTD fault register: " );
    Serial.print( rtd.status( ) );
    Serial.print( ": " );
    if( rtd.status( ) & MAX31865_FAULT_HIGH_THRESHOLD )
    {
      Serial.println( "RTD high threshold exceeded" );
    }
    else if( rtd.status( ) & MAX31865_FAULT_LOW_THRESHOLD )
    {
      Serial.println( "RTD low threshold exceeded" );
    }
    else if( rtd.status( ) & MAX31865_FAULT_REFIN )
    {
      Serial.println( "REFIN- > 0.85 x V_BIAS" );
    }
    else if( rtd.status( ) & MAX31865_FAULT_REFIN_FORCE )
    {
      Serial.println( "REFIN- < 0.85 x V_BIAS, FORCE- open" );
    }
    else if( rtd.status( ) & MAX31865_FAULT_RTDIN_FORCE )
    {
      Serial.println( "RTDIN- < 0.85 x V_BIAS, FORCE- open" );
    }
    else if( rtd.status( ) & MAX31865_FAULT_VOLTAGE )
    {
      Serial.println( "Overvoltage/undervoltage fault");
    }
    else
    {
      Serial.println( "Unknown fault; check connection" );
    }
  }

  delay( 100 );
}