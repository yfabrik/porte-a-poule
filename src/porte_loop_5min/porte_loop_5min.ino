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
Dusk2Dawn geneve(46.1200, 6.0900, 300);
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
    myStepper.step(-1);
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

  //  if (now.minute() % 5 == 1 && isOpen) {
  delay(15000);
  motorOn();
  myStepper.step(nb_tours * STEPS_TOUR);
  isOpen = !isOpen;
  motorOff();
  // }
  delay(10000);
  // else if (now.minute() % 5 == 4 && isOpen == false) {
  motorOn();
  myStepper.step(nb_tours * STEPS_TOUR * -1);
  isOpen = !isOpen;
  motorOff();
  // }


  pause();
}

void pause() {
  //pause 1 min
  for (int i = 0; i < 8; i++) {
    // LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                  SPI_OFF, USART0_OFF, TWI_OFF);
  }
  for (int i = 0; i < 2; i++) {

    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

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