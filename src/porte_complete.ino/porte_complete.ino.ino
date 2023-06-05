#include <RTClib.h>
#include <Dusk2Dawn.h>
#include <LowPower.h>
#include <Stepper.h>

////////editable//////////////
//gps + timezone
Dusk2Dawn geneve(46.1200, 6.0900, 1);

int offset_matin = 0;
int offset_soir = 0;

//pin butté
#define inputPin 4

//l'etat de la porte quand homing
bool isOpen = true;

//motor
#define motorPin1 8
#define motorPin2 9
#define motorPin3 10
#define motorPin4 11
float nb_tours = 2.5;

#define STEPS_TOUR 2038  //le nombre d'etape pour faire 1 tour  le moteur en full-step mode fait 2048 en half step mode 4096
///////////////////////////////

Stepper myStepper(STEPS_TOUR, motorPin1, motorPin3, motorPin2, motorPin4);
int limite = nb_tours * STEPS_TOUR;
int step_todo = nb_tours * STEPS_TOUR;
// module de temps
RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  //control time
  rtc.begin();
  //reset / homing
  pinMode(inputPin, INPUT_PULLUP);
  while (digitalRead(inputPin) == HIGH && limite > 0) {
    myStepper.step(1);
    limite--;
    delay(5);
  }
  myStepper.setSpeed(10);

  motorOff();
}

void loop() {
  DateTime now = rtc.now();
  int matin = geneve.sunrise(now.year(), now.month(), now.day(), false) + offset_matin;
  int soir = geneve.sunset(now.year(), now.month(), now.day(), false) + offset_soir;
  int minToday = now.hour() * 60 + now.minute();
  delay(10);

  if (minToday > matin && minToday < soir && isOpen == false) {
    myStepper.step(step_todo);
    isOpen = !isOpen;
    motorOff();
  } else if ((minToday > soir || minToday < matin) && isOpen) {
    myStepper.step(step_todo * -1);
    isOpen = !isOpen;
    motorOff();
  }
  delay(60000);
  

  // if (minToday < matin)
  //   pause((matin - minToday));
  // else if (minToday < soir)
  //   pause((soir - minToday));
  // else
  //   pause((24 * 60 - minToday));
}

void pause(int minutes) {
  if (minutes <= 0)
    minutes = 1;

  delay(minutes * 60000);
}

//mettre en veille le moteur pour eco batterie
//moteur allumé = 300mA
//moteur éteint = 20mA
void motorOff() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
