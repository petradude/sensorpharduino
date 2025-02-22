#include <SimpleTimer.h>
unsigned long prevMillis = 0;
#define phSamples 15
#define inputPH A0
int phArray1 [phSamples];  
int smoothPH;                     
float pHread;
float pHread1;
float voltage;
float pHvalue;
SimpleTimer timer;

void MeasurePHforSmoothing() {
  pHread1 = analogRead(inputPH);
  smoothPH = pHsmooth(pHread1, phArray1);
  voltage = smoothPH * 0.004887585533;
  pHvalue = 2.13121 * voltage + 2.66524;   // calibration 
}

int pHsmooth(int PHrawIn, int *phArray) {  
  int m, n, PHtemp, PHtop, PHbottom;
  long PHtotal;
  static int p;
  // static int raw[phSamples];
  static int PHsorted[phSamples];
  boolean PHdone;

  p = (p + 1) % phSamples;  
  phArray[p] = PHrawIn;                

  // Serial.print("raw = ");

  for (m = 0; m < phSamples; m++) { 
    PHsorted[m] = phArray[m];
  }

  PHdone = 0;               
  while (PHdone != 1) {      
    PHdone = 1;
    for (m = 0; m < (phSamples - 1); m++) {
      if (PHsorted[m] > PHsorted[m + 1]) {
        PHtemp = PHsorted[m + 1];
        PHsorted [m + 1] =  PHsorted[m] ;
        PHsorted [m] = PHtemp;
        PHdone = 0;
      }
    }
  }
  PHbottom = max(((phSamples * 20)  / 100), 1);
  PHtop = min((((phSamples * 80) / 100) + 1  ), (phSamples - 1)); 
  n = 0;
  PHtotal = 0;
  for ( m = PHbottom; m < PHtop; m++) {
    PHtotal += PHsorted[m];
    n++;
  }
  return PHtotal / n;
}

void setup() {
  Serial.begin(115200);
  timer.setInterval(1000L, MeasurePHforSmoothing);
}

void loop() {
  timer.run();
  if (millis() - prevMillis > 1000) {
    prevMillis = millis();
    Serial.print("ADC = ");
    Serial.println(pHread1);
    Serial.print("ADC_2 = ");
    Serial.println(smoothPH);
    Serial.print("V = ");
    Serial.println(voltage);
    Serial.print("pH = ");
    Serial.println(pHvalue);
  }

}
