#include "HX711-multi.h"
#include "Joystick.h"

// Pins to the load cell amp
#define CLK 2      // Arduino D2 -> HX711 SCK (všechny tři)
#define DOUT1 4    // Arduino D4 -> HX711 DT (brzda) 
#define DOUT2 5    // Arduino D5 -> HX711 DT (plyn)
#define DOUT3 6   //  Arduino D6 -> HX711 DT (spojka)
#define JOYSTICK_DEFAULT_REPORT_ID 0x04

#define BOOT_MESSAGE "MIT_ML_SCALE V0.8"

#define TARE_TIMEOUT_SECONDS 4

byte DOUTS[3] = {DOUT1,DOUT2,DOUT3};
//byte DOUTS[1] = {DOUT1};
#define CHANNEL_COUNT sizeof(DOUTS)/sizeof(byte)
long int results[CHANNEL_COUNT];

//Definice joysticku, zapnutí jednotlivých os / počet tlačítek (tady 0)
HX711MULTI scales(CHANNEL_COUNT, DOUTS, CLK);
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 0, 0,
  true, true, true, false, false, false,
  false, false, false, true, true);

  
void setup() {
  //Serial.begin(115200);
  //Serial.println(BOOT_MESSAGE);
  //Serial.flush();
  Joystick.begin();
  pinMode(11,OUTPUT);
  //Nastavení rozsahu jednotl. os
  Joystick.setXAxisRange(0, 32766);
  Joystick.setYAxisRange(0, 32766);
  Joystick.setZAxisRange(0, 32766);
}

void loop() {
scales.read(results); // přečtení všech 3 HX711, uložení výsledků do pole results
//Serial.println(results[2]/31);
results[1] = results[1] + 20000; // offset pro brzdu aby byla v kladných číslech
if(results[0]>0){         //u mýho loadcellu to občas vyhodí záporný vysoký číslo -65000 nebo tak nějak, proto tenhle if
Joystick.setXAxis(constrain(results[0]/31, 0, 32766));  // Brzda, funkce constrain ořízne číslo aby nebylo menší, než 0 a větší než 32766
//Joystick.setXAxis(16000);

}
Joystick.setYAxis(constrain(results[1]/7, 0, 32766));  // Plyn
//Serial.println(results[1]/7);

//Joystick.setZAxis(constrain(results[2]/7, 0, 32766));  // Spojka
//Serial.println(results[2]/6);

delay(10);  // v příkladu knihovny HX711-multi byl tenhle delay, nevím proč, tak jsem ho tu nechal
}
