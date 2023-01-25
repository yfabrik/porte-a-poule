module RTC( real time clock) 

utilisation de la date couplé avec une library qui permet d'obtenir l'aube et le crépuscule

#### lib dusk2dawn 
dusk2dawn permet d'obtenir l'heure du lever et coucher du soleil
si jamais y'a une erreur lors de la compilation, la library Dusk2Dawn doit etre modifié (d'apres ce que j'ai lu ça dependrai de la version de l'ide utilisé ou sur quelle plateforme c'est utilisé) : 
```c++
//changer dans Dusk2Dawn.cpp et Dusk2Dawn.h
#include<Math.h>
//en
#include<math.h>
```
```c++
//latitude longitude timezone (GMT+1 ici)
Dusk2Dawn geneve(46.1200, 6.0900, 1);
```

### mise a l'heure du module RTC

```c++
#include <DS3231.h> //Import DS3231 Code Library
#include <Wire.h> //Import Wire Code Library
DS3231 Clock;

void setup() // Start the I2C interface
{
Wire.begin();
Clock.setSecond(50);
Clock.setMinute(22);
Clock.setHour(10);
Clock.setDoW(1); 
Clock.setDate(29); 
Clock.setMonth(11);
Clock.setYear(22); 
}
```

le `Clock.setDoW(1);` set day of week est arbitraire
valeur comprise entre 1 et 7
si je dit  `Clock.setDoW(1);`  un samedi le jour 1 de la semaine est samedi

### utilisation
rajout des library RTClib et Dusk2Dawn


```c++
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
Dusk2Dawn geneve(46.1200, 6.0900, 1);
bool isOpen = false;

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
}

void loop() {
  DateTime now = rtc.now();
  int matin = geneve.sunrise(now.year(), now.month(), now.day(), false);
  int soir = geneve.sunset(now.year(), now.month(), now.day(), false);
  int minToday = now.hour() * 60 + now.minute();
  if (minToday > matin && minToday < soir && isOpen == false) {
    myStepper.step(nb_tours * STEPS_TOUR);
    isOpen = !isOpen;
    delay(60000);
  }

  if ((minToday > soir || minToday < matin) && isOpen) {
    myStepper.step(nb_tours * STEPS_TOUR * -1);
    isOpen = !isOpen;
    delay(60000);
  }
}
```