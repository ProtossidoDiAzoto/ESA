#include "RC.h"

void setup(){
	pinMode(READ_PIN,INPUT);
  Serial.begin(BAUDRATE);
}

void loop(){
  /*Avvio ciclo*/
  short counter = 0;
  float values = 0;
  /*Campionamento per valori*/
  while(counter < WINDOW_MAX_SIZE){
    values = values + analogRead(READ_PIN);
    counter++;
  }
  /*Termino il ciclo*/
  long timer = millis();
  int timerSec = timer/1000;
  float vCapacitor = Vc(VS, timerSec, TAU, CAPACITOR_CAPACITY);
  Serial.print(filterValue(values, counter));
  Serial.print(",");
  Serial.print(timer);
  Serial.print(",");
  Serial.print(vCapacitor);
  Serial.print(",");
  Serial.println(currentIntensity(vCapacitor, VS, timerSec, TAU));
}

float filterValue(float values, int counter){
  return (values / counter) * (VS / ADC_MAX_VALUE);
}

float Vc(float vs, long t, float R, float C){
   float e = 2.72;
   return vs * pow(e,(-1)*(t/(R*C)));
}

float currentIntensity(float vc, float vs, long t, float r){
  return (1-((vc*t)/vs)) * (vs/r);
}
