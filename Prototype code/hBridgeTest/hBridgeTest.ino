/*
  Adafruit Arduino - Lesson 15. Bi-directional Motor
*/

int enablePin = 11;
int in1Pin = 10;
int in2Pin = 9;
int switchPin = 7;
int potPin = 1;

// Input pin of the temperature sensor located on the Peltier component
int peltierTempPin = 2;

int i = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
}

// Returns the temperature at the given pin
int getTemperature(int pin) {
  float temp;

  temp = analogRead(pin);
  temp = temp * 0.48828125;

  return temp;
}

void loop()
{
  int curSpeed = analogRead(potPin) / 4;
  boolean reverse = digitalRead(switchPin);

  i++;
  if (i >= 1000) {
    Serial.print("Speed = ");
    Serial.print(curSpeed);
    Serial.println();

    Serial.print("Reverse = ");
    Serial.print(reverse);
    Serial.println();

    i = 0;

    float peltierTemp = getTemperature(peltierTempPin);

    Serial.print("Peltier Temperature = ");
    Serial.print(peltierTemp);
    Serial.print("*C");
    Serial.println();
  }

  setMotor(curSpeed, reverse);
}

void setMotor(int newSpeed, boolean reverse)
{
  analogWrite(enablePin, newSpeed);
  digitalWrite(in1Pin, ! reverse);
  digitalWrite(in2Pin, reverse);
}
