int peltier = 3; //The N-Channel MOSFET is on digital pin 3
int power = 0; //Power level fro 0 to 99%
int peltier_level = map(power, 0, 99, 0, 255); //This is a value from 0 to 255 that actually controls the MOSFET

int max_power = 99;

//initializes/defines the output pin of the LM34 temperature sensor
int tempPin = 0;

float targetTemp = 23.0;
// Read temp = 15 -> 5 * powerPerDiff
// Read temp = 10 -> 100%

float powerPerDiff = 5;

float temp;

void setup() {
  Serial.begin(9600);

  //pinMode(peltier, OUTPUT);
}

void loop() {
  temp = analogRead(tempPin);

  temp = temp * 0.48828125;

  Serial.print("TEMPRATURE = ");
  Serial.print(temp);
  Serial.print("*C");
  Serial.println();
  
  float difference = abs(targetTemp - temp);
  float newPower = powerPerDiff * difference;

  Serial.print (newPower);
  Serial.println(" new power");

  //Serial.print (millivolts);
  //Serial.println(" millivolt");
  
  /*if(Serial.available() > 0)
    {*/

  //power = (int) newPower;
  power = 0;

  Serial.print (power);
  Serial.println(" converted power");

  if (power > 99) power = max_power;
  if (power < 0) power = 0;

  peltier_level = map(power, 0, 99, 0, 255);
  //}

  Serial.print("Power=");
  Serial.print(power);
  Serial.print(" PLevel=");
  Serial.println(peltier_level);

  analogWrite(peltier, peltier_level); //Write this new value out to the port

  delay(1000);

}
