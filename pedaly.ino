#include "HX711-multi.h"
#include "Joystick.h"
#include <EEPROM.h>
// Definice vstupů pro HX711 moduly
#define CLK 2      // Arduino D2 -> HX711 SCK (všechny tři)
#define DOUT1 4    // Arduino D4 -> HX711 DT (brzda) 
#define DOUT2 5    // Arduino D5 -> HX711 DT (plyn)
#define DOUT3 6   //  Arduino D6 -> HX711 DT (spojka)
#define JOYSTICK_DEFAULT_REPORT_ID 0x04
long pedal1min = 0;
long pedal1max = 0;
long pedal2min = 0;
long pedal2max = 0;
long pedal3min = 0;
long pedal3max = 0;
long kalibraceZacatek;
bool kalibrace = false;
bool kalibraceStart = false;
char recievedChar;
bool newData = false;

byte DOUTS[3] = {DOUT1,DOUT2,DOUT3};
#define CHANNEL_COUNT sizeof(DOUTS)/sizeof(byte)
long int results[CHANNEL_COUNT];

//Definice , zapnutí jednotlivých os / počet tlačítek 
HX711MULTI scales(CHANNEL_COUNT, DOUTS, CLK);
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
JOYSTICK_TYPE_MULTI_AXIS, 0, 0,
true, true, true, false, false, false,
false, false, false, true, true);

  
void setup() {
  Joystick.begin();
  Serial.begin(115200);
  Serial.println("Setup....");
  Serial.flush();
//  Serial.print("EEprom: ");
//  Serial.println(EEPROMReadlong(0));
  if (EEPROMReadlong(0) == -1)
  {
    
    kalibrace = true;
    kalibraceStart = true;
    kalibraceZacatek = 0;
  }
  else
  {
    pedal1min = EEPROMReadlong(0);
    pedal1max = EEPROMReadlong(4);
    pedal2min = EEPROMReadlong(8);
    pedal2max = EEPROMReadlong(12);
    pedal3min = EEPROMReadlong(16);
    pedal3max = EEPROMReadlong(20);
  }
  
  //Nastavení rozsahu jednotl. os
  Joystick.setXAxisRange(-32767, 32767);
  Joystick.setYAxisRange(-32767, 32767);
  Joystick.setZAxisRange(-32767, 32767);
  Serial.println("program:");
  
}

void loop() {

scales.read(results); // přečtení všech 3 HX711, uložení výsledků do pole results
if (Serial.available() > 0) {
    // read the incoming byte:
    recievedChar = Serial.read();

    // say what you got:
    if (recievedChar == 'k'){
      kalibrace = true;
      kalibraceStart = true;
      kalibraceZacatek = millis();
      Serial.println("Kalibrace:");
      recievedChar = " ";
    }
  }
if (kalibraceStart)
{
  pedal1min = results[0];
  pedal2min = results[1];
  pedal3min = results[2];
  pedal1max = 0;
  pedal2max = 0;
  pedal3max = 0;
  Serial.println("Start kalibrace");
  Serial.print("Minimální hodnoty: Pedál 1:");
  Serial.print(pedal1min);
  Serial.print(" Pedál 2:");
  Serial.print(pedal2min);
  Serial.print(" Pedál 3:");
  Serial.println(pedal3min);
  Serial.println("Sešlápni opakovaně všechny 3 pedály na doraz do 10 sekund (brzdu dle libosti)");
  kalibraceStart = false;
}
if(kalibrace){
  if(results[0] > pedal1max){
    pedal1max = results[0];
  }
  if(results[1] > pedal2max){
    pedal2max = results[1];
  }
  if(results[2] > pedal3max){
    pedal3max = results[2];
  }
}
if (millis() > (kalibraceZacatek + 10000) && kalibrace == true){
  kalibrace = false;
  EEPROMWritelong(0,pedal1min);
  EEPROMWritelong(4,pedal1max);
  EEPROMWritelong(8,pedal2min);
  EEPROMWritelong(12,pedal2max);
  EEPROMWritelong(16,pedal3min);
  EEPROMWritelong(20,pedal3max);
  Serial.println("Konec kalibrace!");
  Serial.print("Maximální hodnoty: Pedál 1:");
  Serial.print(pedal1max);
  Serial.print(" Pedál 2:");
  Serial.print(pedal2max);
  Serial.print(" Pedál 3:");
  Serial.println(pedal3max);
}
if (results[0] >= pedal1min && results[0] <= pedal1max){
Joystick.setXAxis(constrain(prepocet(pedal1min,pedal1max,results[0]),-32767,32767));  // Brzda - 20.7.2020 - přidána kontrola rozsahu - pokud bude hodnota mimo, osa se neaktualizuje
}
if (results[1] >= pedal2min && results[1] <= pedal2max){
Joystick.setYAxis(constrain(prepocet(pedal2min,pedal2max,results[1]),-32767,32767));  // Plyn
}
if (results[2] >= pedal3min && results[2] <= pedal3max){
Joystick.setZAxis(constrain(prepocet(pedal3min,pedal3max,results[2]),-32767,32767));  // Spojka
}

delay(10);  // v příkladu knihovny HX711-multi byl tenhle delay, nevím proč, tak jsem ho tu nechal
}

long EEPROMReadlong(long address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
 
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWritelong(int address, long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
 
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

long prepocet (long pedalMin, long pedalMax, long hodnota){
  if((pedalMax - pedalMin)>65535){
    long delitel = (pedalMax-pedalMin)/65535 + 1;
    long offset = pedalMin/delitel + 32767;
    return (hodnota/delitel - offset);
  }
  else {
    long offset = pedalMin + 32767;
    return (hodnota-offset);
  }
  
}
