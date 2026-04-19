#include <LiquidCrystal.h>
#include <math.h>

// Initialize LCD pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// Define Pins (removed spaces in names)
#define NTC_PIN A3
#define MOTOR_PIN 9
#define ALARM_PIN 13

// Steinhart-Hart Coefficients and Variables
float RL = 10000.0; // 10k Resistor used in voltage divider
float ca = 0.001129148;
float cb = 0.000234125;
float cc = 0.0000000876741;

float lowt = 25.0;
float hight = 45.0;
float temp;
int speed; // Speed should be an integer for PWM (0-255)

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(ALARM_PIN, OUTPUT);

  // Corrected LCD initialization
  lcd.begin(16, 2); 
  lcd.clear();
  lcd.print("Exp 1");
  lcd.setCursor(0, 1);
  lcd.print("TempControl");
  delay(3000);
  lcd.clear();
}

void loop() {
  int Rin = analogRead(NTC_PIN);
  
  // Calculate Resistance of NTC
  // Formula assumes NTC is connected to Ground and 10k resistor to 5V
  float R = RL * (1023.0 / (float)Rin - 1.0); 

  // Steinhart-Hart Equation
  float logR = log(R);
  temp = (1.0 / (ca + (cb * logR) + (cc * logR * logR * logR)));
  temp = temp - 273.15; // Convert Kelvin to Celsius

  // Display Temp on LCD (Added this so you can see it!)
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print(" C   "); // Extra spaces to clear previous digits

  // Map temperature to speed (0 to 100%)
  // Note: Using commas, not semicolons
  speed = map(temp * 10, lowt * 10, hight * 10, 0, 100);
  speed = constrain(speed, 0, 100);

  // Motor Control Logic
  if (temp < lowt) {
    analogWrite(MOTOR_PIN, 0);
  } 
  else {
    // Map percentage (0-100) to PWM duty cycle (60-255)
    // 60 is used as a minimum to ensure the motor actually spins
    analogWrite(MOTOR_PIN, map(speed, 0, 100, 60, 255));
  }

  // Alarm Logic
  if (temp >= hight) {
    digitalWrite(ALARM_PIN, HIGH);
  } 
  else {
    digitalWrite(ALARM_PIN, LOW);
  }
  
  delay(500); // Small delay to stabilize reading
}
