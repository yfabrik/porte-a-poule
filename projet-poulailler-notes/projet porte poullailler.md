automatisation d'une porte de poullailler à l'aide d'un moteur pas à pas controllé par un microcontrolleur pour qu'elle s'ouvre et se ferme au lever et coucher du soleil

# composants
  - arduino nano
  - module temps RTC DS3231
  - moteur pas à pas 5V 28byj-48 
  - carte controle du moteur ULN2003
  - capteur de fin de course
  - batterie 12V
  - convertisseur 12V 5V LM7805

### stepped motor
le moteur pas à pas fonctionne grace a 2 electro-aimant pour faire tourner l'axe il a besoin d'une pause entre chaque changement il a pas le temps de se déplacer le min interval *serai* de 2ms entre chaque pas

# montage

![[arduino_bb.png]]
les port D8,D9,D10,D11 controle le moteur
le port D4 est pour la butté
ces ports peuvent etre changés

pour la communication avec le module RTC
port A4 SDA  
port A5 SCL

# physique
testé  de faire bouger la porte avec un engrenage et une crémaillere :
pas centré sa fait dévier la porte avant de la lever

# calcul 
calcul du nombre de pas/nbtour nécéssaire pour ouvrir/fermer la porte
1. mesure de la distance voulu
2. mesure du rayon de l'engrenage
3. le moteur choisi fait  tour en 2038 étapes
4. longueur = radius x2 pi x 2038 x nbtour

# code projet 
## projet simple
ouvrir/fermer la porte toute les x milisecondes

```c++
#include <Stepper.h>
#define motorPin1 8   // IN1 on the ULN2003 driver
#define motorPin2 9   // IN2 on the ULN2003 driver
#define motorPin3 10  // IN3 on the ULN2003 driver
#define motorPin4 11  // IN4 on the ULN2003 driver

#define STEPS_TOUR 2038  //le nombre d'etape pour faire 1 tour  le moteur en full-step mode fait 2048 en half step mode 4096
Stepper myStepper(STEPS_TOUR, motorPin1, motorPin3, motorPin2, motorPin4);

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

## projet avec butté
rajout d'une butté "au cas où, on sait jamais"
soit au niveau fermé de la porte soit au niveau ouvert 

avec le projet simple	si l'arduino s'arrète à un moment au redémmarage il va faire monter la porte, donc si jamais la porte était déjà en position haute, elle va vouloir aller encore plus haut et donc des chose vont se casser

rajout d'un homing au démmarage de l'arduino
permet à l'arduino d'avoir une position initiale connue
```c++
//descend jusqu'a ce que la butté soit cliqué
#define inputPin 4
void setup() {
  //reset / homing
  pinMode(inputPin, INPUT_PULLUP);
  while (digitalRead(inputPin) == HIGH) {
    myStepper.step(-1);
    delay(30);
  }
}


//monte jusqu'a ce que le compteur arrive à 0 ou que la butté soit cliqué
float nb_tours = 2.5;
int limite = nb_tours * STEPS_TOUR;
#define inputPin 4
void setup() {
  //reset / homing
  pinMode(inputPin, INPUT_PULLUP);
  while (digitalRead(inputPin) == HIGH && limite > 0) {
    myStepper.step(-1);
    limite--;
    delay(15);
  }
}

```

`  pinMode(inputPin, INPUT_PULLUP);` 
permet d'activer la resistance interne du pin choisi, la valeur par défaut est haute
permet de stabiliser le signal
sinon faut rajouter une resistance dans le circuit 

## projet avec horaire
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

#### lib dusk2dawn 
faut renommer Math.h en math.h
dusk2dawn permet d'obtenir l'heure du lever et coucher du soleil

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


## à voir 
