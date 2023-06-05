arret du moteur pour éco 
moteur allumé 300 mA
éteint 20mA
lib LowPower pour mettre en veille l'arduino 

accelerer le moteur? moins il reste allumé moins il consomme
y'aurai un delay mini de 2ms entre chaque step
donc 2x2038 = 4076 soit un peu + de 4s pour 1 tour
60 /4.076 =14 donc max 14 tours min

rajout de fonctions pour éteindre le moteur :
```c++
void motorOff() {
  statepin1 = digitalRead(motorPin1);
  statepin2 = digitalRead(motorPin2);
  statepin3 = digitalRead(motorPin3);
  statepin4 = digitalRead(motorPin4);

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
void motorOn() {
  digitalWrite(motorPin1, statepin1);
  digitalWrite(motorPin2, statepin2);
  digitalWrite(motorPin3, statepin3);
  digitalWrite(motorPin4, statepin4);
  delay(10);
}
```
peut etre suffisant ?
```c++
void motorOff() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
```

```c++
#include <RTClib.h>
#include <Dusk2Dawn.h>
#include <LowPower.h>

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

RTC_DS3231 rtc;
Dusk2Dawn geneve(46.1200, 6.0900, 1);
bool isOpen = false;

int statepin1;
int statepin2;
int statepin3;
int statepin4;

void setup() {
  //control time
  rtc.begin();
  //reset / homing
  pinMode(inputPin, INPUT_PULLUP);
  while (digitalRead(inputPin) == HIGH && limite > 0) {
    myStepper.step(1);
    limite--;
    delay(15);
  }

  myStepper.setSpeed(5);
  isOpen = true;
  motorOff();
}

void loop() {
  DateTime now = rtc.now();
  int matin = geneve.sunrise(now.year(), now.month(), now.day(), false);
  int soir = geneve.sunset(now.year(), now.month(), now.day(), false);
  int minToday = now.hour() * 60 + now.minute();

  if (minToday > matin && minToday < soir && isOpen == false) {
    motorOn();
    myStepper.step(nb_tours * STEPS_TOUR);
    isOpen = !isOpen;
    motorOff();
  } else if ((minToday > soir || minToday < matin) && isOpen) {
    motorOn();
    myStepper.step(nb_tours * STEPS_TOUR * -1);
    isOpen = !isOpen;
    motorOff();
  }

  if (minToday < matin)
    pause((matin - minToday));
  else if (minToday < soir)
    pause((soir - minToday));
  else
    pause((24 * 60 - minToday));
}

void pause(int minutes) {
  if (minutes <= 0)
    minutes = 1;

//minute pair divisé par 8 reste 0, minutes impair reste 4
  int surplus = minutes % 2;
  int cycles = minutes * 60 / 8;
  for (int i = 0; i < cycles; i++) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
  if (surplus != 0) {
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  }
}

//mettre en veille le moteur pour eco batterie 
//moteur allumé = 300mA
//moteur éteint = 20mA
void motorOff() {
  statepin1 = digitalRead(motorPin1);
  statepin2 = digitalRead(motorPin2);
  statepin3 = digitalRead(motorPin3);
  statepin4 = digitalRead(motorPin4);

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
void motorOn() {
  digitalWrite(motorPin1, statepin1);
  digitalWrite(motorPin2, statepin2);
  digitalWrite(motorPin3, statepin3);
  digitalWrite(motorPin4, statepin4);
  delay(10);
}

```