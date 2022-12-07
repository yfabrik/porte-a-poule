#include <RTClib.h>
#include <Dusk2Dawn.h>

#include <Stepper.h>
#define motorPin1 8   // IN1 on the ULN2003 driver
#define motorPin2 9   // IN2 on the ULN2003 driver
#define motorPin3 10  // IN3 on the ULN2003 driver
#define motorPin4 11  // IN4 on the ULN2003 driver

#define STEPS_TOUR 2038  //le nombre d'etape pour faire 1 tour  le moteur en full-step mode fait 2048 en half step mode 4096
Stepper myStepper(STEPS_TOUR, motorPin1, motorPin3, motorPin2, motorPin4);

float nb_tours = 2.5;
int limite = nb_tours * STEPS_TOUR;
#define inputPin 4

DS3231 rtc;
Dusk2Dawn geneve(46.1200, 6.0900, 300);
bool isOpen = false;

void setup() {
  //control time
  Wire.begin();
  rtc.begin();
  //reset / homing
  pinMode(inputPin, INPUT_PULLUP);
  while (digitalRead(inputPin) == HIGH && limite > 0) {
    myStepper.step(-1);
    limite--;
    delay(15);
  }

  myStepper.setSpeed(5);
  isOpen = true;
}

void loop() {
  DateTime now = rtc.now();
  int matin = geneve.sunrise(now.year(), now.month(), now.day(), false);
  int soir = geneve.sunset(now.year(), now.month(), now.day(), false);
  int minToday = now.hour() * 60 + now.minute();
  if (now.minute() % 5 == 1 && isOpen == false) {
    myStepper.step(nb_tours * STEPS_TOUR);
    isOpen = !isOpen;
    delay(60000);
  }

  if (now.minute() % 5 == 4 && isOpen) {
    myStepper.step(nb_tours * STEPS_TOUR * -1);
    isOpen = !isOpen;
    delay(60000);
  }
}