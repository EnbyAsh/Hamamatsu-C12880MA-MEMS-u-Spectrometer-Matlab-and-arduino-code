/*
 * arduino code for spectroscope module 
 *  - check the right board is selected and check the com port for ploting in matlab
 *  - set the PWM_value to a brightness that dosnt over saturate the graph
 */

/*
   Macro Definitions
*/
#define SPEC_TRG         A0
#define SPEC_ST          A1
#define SPEC_CLK         A2
#define SPEC_VIDEO       A3
#define WHITE_LED        A4
#define LASER_404        A5
#define TRANS_LED        6
#define PWM_VALUE        50 //0 - 255 briteness from off to full 
#define SPEC_CHANNELS    288 // New Spec Channel

int x = 0;
uint16_t data[SPEC_CHANNELS];
int Iteration = 0;
String value;

void setup() {

  //Set desired pins to OUTPUT
  pinMode(SPEC_CLK, OUTPUT);
  pinMode(SPEC_ST, OUTPUT);
  pinMode(LASER_404, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  pinMode(TRANS_LED, OUTPUT);

  digitalWrite(SPEC_CLK, HIGH); // Set SPEC_CLK High
  digitalWrite(SPEC_ST, LOW); // Set SPEC_ST Low
  while ( x < 15){
    digitalWrite(WHITE_LED, HIGH);
    delay(1); 
    digitalWrite(WHITE_LED, LOW);
    x++;
  }
  digitalWrite(WHITE_LED, HIGH); // uncoment to set wight lite led on
 // digitalWrite(LASER_404, HIGH); // uncoment to tern on laser light
  analogWrite(TRANS_LED,  PWM_VALUE); // uncoment to tern of seperate wight light led
  Serial.begin(115200); // Baud Rate set to 115200



}

/*
   This functions reads spectrometer data from SPEC_VIDEO
   Look at the Timing Chart in the Datasheet for more info
*/
void readSpectrometer() {

  int delayTime = 1; // delay time

  // Start clock cycle and set start pulse to signal start
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, LOW);
  digitalWrite(SPEC_ST, HIGH);
  delayMicroseconds(delayTime);

  //Sample for a period of time
  for (int i = 0; i < 15; i++) {

    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delayTime);

  }

  //Set SPEC_ST to low
  digitalWrite(SPEC_ST, LOW);

  //Sample for a period of time
  for (int i = 0; i < 85; i++) {

    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delayTime);

  }

  //One more clock pulse before the actual read
  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delayTime);

  //Read from SPEC_VIDEO
  for (int i = 0; i < SPEC_CHANNELS; i++) {

    data[i] = analogRead(SPEC_VIDEO);

    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delayTime);

  }

  //Set SPEC_ST to high
  digitalWrite(SPEC_ST, HIGH);

  //Sample for a small amount of time
  for (int i = 0; i < 7; i++) {

    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delayTime);

  }

  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);

}

/*
   The function below prints out data to the terminal or
   processing plot
*/
void printData() {

  for (int i = 0; i < SPEC_CHANNELS; i++) {

    Serial.print(data[i]);
    Serial.print(',');

  }

 // Serial.print("\n");
}

void loop() {
  //value = Serial.readString();
  //if (value == 'sRead')
 // {
    while (Iteration < 4 )
    {
      delay(100);
      readSpectrometer();
      printData();
      Iteration++; 
    }
 // }
 // else
 // {
 //   Iteration = 0;
 // }
}
