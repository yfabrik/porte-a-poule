## version simple
juste creer une instance du moteur, lui mettre une vitesse et le faire tourner dans un sens puis dans l'autre

```c++
#include <Stepper.h>
#define motorPin1 8 // IN1 on the ULN2003 driver
#define motorPin2 9 // IN2 on the ULN2003 driver
#define motorPin3 10 // IN3 on the ULN2003 driver
#define motorPin4 11 // IN4 on the ULN2003 driver
#define STEPS_TOUR 2038 
Stepper myStepper(STEPS_TOUR, motorPin1, motorPin2, motorPin3, motorPin4);
float nb_tours = 1.5;

void setup() {
myStepper.setSpeed(5);
}

void loop() {
myStepper.step(nb_tours * STEPS_TOUR);
delay(2000);
myStepper.step(nb_tours * STEPS_TOUR * -1);
delay(2000);
}
```

## version avec butté
au cas ou l'arduino ai une coupure de courrant
rajout d'un capteur de fin de course pour detecter si la porte est fermé
quand l'arduino démare il ramene la porte à l'état fermé
code à rajouter:
```c++
#define inputPin 4
void setup(){
	//reset / homing
	pinMode(inputPin, INPUT_PULLUP);//état de la pin par défaut HIGH
	while (digitalRead(inputPin) == HIGH) {
		myStepper.step(-1);
		delay(30); //besoin d'un delay sinon le moteur comprend pas
	}
}
```

## version avec horloge
utilisation d'un module RTC pour obtenir l'heure et la date 
il faudra l'initialiser à la bonne date dabord avec un autre programme
```c++

```

utilisation de la date couplé avec une library qui permet d'obtenir l'aube et le crépuscule
la library Dusk2Dawn doit etre modifié : 
```c++
//changer dans Dusk2Dawn.cpp et Dusk2Dawn.h
#include<Math.h>
//en
#include<math.h>
```

code à rajouter/modifer:
```c++
#include <RTClib.h>
#include <Dusk2Dawn.h>
DS3231 rtc;
Dusk2Dawn geneve(46.1200, 6.0900, 300);//coordonne GPS
bool isClosed = true;
  
void setup() {
///reste du code////

Wire.begin();
rtc.begin();
isClosed = true;
}

void loop() {
DateTime now = rtc.now();
//dusk2down donne les valeur en minutes écoulé depuis minuit
int matin = geneve.sunrise(now.year(), now.month(), now.day(), false);
int soir = geneve.sunset(now.year(), now.month(), now.day(), false);
int minToday = now.hour() * 60 + now.minute();

if (minToday > matin && minToday < soir && isClosed) {
myStepper.step(nb_tours * STEPS_TOUR);
isClosed = !isClosed;
}
if ((minToday > soir || minToday < matin) && !isClosed) {
myStepper.step(nb_tours * STEPS_TOUR * -1);
isClosed = !isClosed;
}
```


code complet
```c++
#include <RTClib.h>
#include <Dusk2Dawn.h>
#include <Stepper.h>
#define motorPin1 8 // IN1 on the ULN2003 driver
#define motorPin2 9 // IN2 on the ULN2003 driver
#define motorPin3 10 // IN3 on the ULN2003 driver
#define motorPin4 11 // IN4 on the ULN2003 driver
#define STEPS_TOUR 2038 //le nombre d'etape pour faire 1 tour le moteur en full-step mode fait 2048 en half step mode 4096

Stepper myStepper(STEPS_TOUR, motorPin1, motorPin3, motorPin2, motorPin4);

float nb_tours = 1.5;

#define inputPin 4

DS3231 rtc;
Dusk2Dawn geneve(46.1200, 6.0900, 300);
bool isClosed = true;

void setup() {
//control time
Wire.begin();
rtc.begin();
//reset / homing
pinMode(inputPin, INPUT_PULLUP);
while (digitalRead(inputPin) == HIGH) {
myStepper.step(-1);
delay(30);
}
myStepper.setSpeed(5);
isClosed = true;
}

void loop() {
DateTime now = rtc.now();
int matin = geneve.sunrise(now.year(), now.month(), now.day(), false);
int soir = geneve.sunset(now.year(), now.month(), now.day(), false);
int minToday = now.hour() * 60 + now.minute();

if (minToday > matin && minToday < soir && isClosed) {
myStepper.step(nb_tours * STEPS_TOUR);
isClosed = !isClosed;
}
if ((minToday > soir || minToday < matin) && !isClosed) {
myStepper.step(nb_tours * STEPS_TOUR * -1);
isClosed = !isClosed;
}
}
```