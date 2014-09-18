/*
This code reads the analog values from the analog pin A0 and 
sends it serially to the LCD backpack to display sensor values.
*/

int sensorPin = A0;     // select the input pin for the potentiometer
int sensorValue = 0;    // variable to store the value coming from the sensor

void setup() 
{
  Serial.begin(9600);    // initialize the serial port with baudrate 9600.
}

void loop() 
{
  
  sensorValue = analogRead(sensorPin);  // read the value from the sensor:  
  Serial.print("pri ");                 // serial print
  Serial.print(sensorValue);
  Serial.print(";");
  delay(1000);
  Serial.print("clr");
  delay(1000);
}
