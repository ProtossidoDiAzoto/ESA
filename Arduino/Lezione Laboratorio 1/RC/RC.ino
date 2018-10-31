#include "RC.h"

String command = "full\n";
bool displayedError = false;

void setup(){
	pinMode(READ_PIN,INPUT);
  Serial.begin(BAUDRATE);
}

void loop(){
   
  short counter = 0;
  float values = 0;
    
  while(counter < WINDOW_MAX_SIZE){
    values = values + analogRead(READ_PIN);
    counter++;
  }
    
  long timer = millis();
  int timerSec = timer/1000;
  float vCapacitor = Vc(VS, timerSec, TAU, CAPACITOR_CAPACITY);
   
  if(Serial.available()){
    command = Serial.readString();
    }

   //ALL MEASURES DETECTED
   if(command =="full\n"){
            Serial.print(filterValue(values, counter));
            Serial.print(",");
            Serial.print(timer);
            Serial.print(",");
            Serial.print(vCapacitor);
            Serial.print(",");
            Serial.println(currentIntensity(vCapacitor, VS, timerSec, TAU));
            displayedError = false;
      } else 
      // CAPACITOR VOLTAGE and TIME COSTANT
         if(command =="capv\n"){
            Serial.print(filterValue(values, counter));
            Serial.print(",");
            Serial.println(timer);
          displayedError = false;
      } else 
      // CAPACITOR CURRENT and TIME COSTANT
         if(command =="capc\n"){
            Serial.print("i capacitor");
            Serial.print(",");
            Serial.println(timer);
          displayedError = false;
      } else {
          if(!displayedError){
            Serial.println("WRONG COMMAND: the only correct commands are: \n full to obtain all measures detected \n capv to obtain capacitor's voltage \n capn to obtain capacitor's current");
            displayedError = true;
          }
          
      }
}

float filterValue(float values, int counter){
  return (values / counter) * (VS / ADC_MAX_VALUE);
}

float Vc(float vs, long t, float R, float C){
   float e = 2.72;
   float exponent = pow(e,(-1)*(t/(R*C)));
   return vs * (1- exponent);
}

float currentIntensity(float vc, float vs, long t, float r){
  return (1-((vc*t)/vs)) * (vs/r);
}
