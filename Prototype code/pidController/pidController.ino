//int peltier = 3; //The N-Channel MOSFET is on digital pin 3
//int peltier_level = map(power, 0, 99, 0, 255); //This is a value from 0 to 255 that actually controls the MOSFET

// Power values
//int power = 0; //Power level from 0 to 99%
//int max_power = 99; // Upper limit for power we want to send

// Input pin of the temperature sensor located on the Peltier component
int peltierTempPin = 0;

// Pins related to H-brige
int enablePin = 11;
int in1Pin = 10;
int in2Pin = 9;

// Controller values
int meanSamples = 1000; // number of temperature samples
int currentSamples = 0; // Current number of samples
float sumTemperature = 0; // Global value containing the sum of temperatures
float peltierTargetTemp = 20.0; // Target output temperature of the Peltier element

// PID values
float p = 3;
float pHot = 0.1;
float pCool = 1.0;

// Values for the current speed and reverse
boolean curReverse;
int curSpeed;

void setup() {
  Serial.begin(9600);

  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Set initial values
  curReverse = 0;
  curSpeed = 0;

  //pinMode(peltier, OUTPUT);
}

// Returns the temperature at the given pin
int getTemperature(int pin) {
  float temp;

  temp = analogRead(pin);
  temp = temp * 0.48828125;

  return temp;
}

// Controls the speed (aka. voltage output) and polarity of the H-bridge
void setMotor(int speed, boolean reverse)
{
  // TODO: Make sure that the power level/speed stays between 0% and 100%

  analogWrite(enablePin, speed);
  digitalWrite(in1Pin, ! reverse);
  digitalWrite(in2Pin, reverse);
}

void loop() {
  // Get current temperature
  float peltierTemp = getTemperature(peltierTempPin);
  sumTemperature += peltierTemp;
  currentSamples++;

  // Only do something every meanSamples loop
  if (currentSamples >= meanSamples) {
    // Compute mean temperature across the latest samples
    float meanPeltierTemp = sumTemperature / meanSamples;

    Serial.print("Peltier Mean Temperature = ");
    Serial.print(meanPeltierTemp);
    Serial.print("*C");
    Serial.println();

    // Compute temperature difference
    float tempDifference = abs(peltierTargetTemp - meanPeltierTemp);

    // If the difference is higher than 0, it means that we need to COOL
    if (peltierTargetTemp - meanPeltierTemp >= 0) {
      curReverse = true; // NEEDS TO BE CHECKED DEPENDING ON PELTIER FACING
      curSpeed = tempDifference * pCool;
    }
    // Else If the difference is below than 0, it means that we need to HEAT
    else {
      curReverse = false; // NEEDS TO BE CHECKED DEPENDING ON PELTIER FACING
      curSpeed = tempDifference * pHot;
    }

    Serial.print("Reverse = ");
    Serial.print(curReverse);
    Serial.println();

    Serial.print("Speed = ");
    Serial.print(curSpeed);
    Serial.println();


    // Reset values
    currentSamples = 0;
    sumTemperature = 0;
  }

  // TODO: CLAMP curSpeed HERE

  // Now write current values to the H-bridge
  setMotor(curSpeed, curReverse);

}


