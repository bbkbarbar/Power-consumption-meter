#include "EmonLib.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define I2C_ADDRESS_OF_DISPLAY  0x3C  // 0X3C+SA0 - 0x3C or 0x3D

#define AC_VOLTAGE             230.0
#define COST_PER_KWH            44.0

#define HOURS_IN_A_MONTH         730

#define EMON_INPUT_PIN            A0
// calibration_value = ( i(measured) / i(sensor) ) / R(burden)
// calibration_value = ( i(measured) / i(sensor) ) / 33R)
#define EMON_CALIBRATION_VALUE  13.3


#define AVG_SIZE                   3

EnergyMonitor emon1;
SSD1306AsciiAvrI2c oled;

double values[AVG_SIZE];

void setup() {
  oled.begin(&Adafruit128x64, I2C_ADDRESS_OF_DISPLAY, true);
  oled.setFont(Adafruit5x7);  
  oled.clear();  
  oled.set2X();

  emon1.current(EMON_INPUT_PIN, EMON_CALIBRATION_VALUE);
/*
  for(int i=0; i<AVG_SIZE; i++){
    values[i] = emon1.calcIrms(1480);;
  }/**/
}

int ptr = 0;

void loop() {

  /*
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  values[ptr] = Irms;
  ptr++;
  if(ptr>=AVG_SIZE){
    ptr = 0;
  }
  /**/

  double avgValue = 0;
  for(int i=0; i<AVG_SIZE; i++){
    avgValue += (emon1.calcIrms(2000));
  }
  avgValue /= AVG_SIZE;

  double power = (avgValue * AC_VOLTAGE);
  double cost = (power * COST_PER_KWH) / 1000;
  
  oled.clear();
  oled.println("I: " + String(avgValue) + " A\n"); 
  oled.println("P: " + (String( int(power+0.5))) + " W");         // Apparent power
  oled.println((String( int(cost+0.5))) + " Ft / h");         // Energy cost / hour
  oled.println((String( int((cost * HOURS_IN_A_MONTH)+0.5))) + " Ft");         // Energy cost / hour

}
