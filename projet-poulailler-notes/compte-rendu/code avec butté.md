au cas ou l'arduino ai une coupure de courrant.
rajout d'un capteur de fin de course pour détecter si la porte est fermé.
quand l'arduino démare il ramène la porte à l'état fermé

rajout dans setup():
```c++
  //reset / homing
  pinMode(inputPin, INPUT_PULLUP);
  while (digitalRead(inputPin) == HIGH) {
    myStepper.step(-1);
    delay(10);
  }
```
le moteur a besoin d'avoir une pause entre chaque étape sinon il n'a pas le temps de se déplacer


le INPUT_PULLUP permet d'activer la résistance interne du pin pour éviter les parasite, sinon faudrait rajouter une resistance dans le montage
[détail](obsidian://open?vault=projet-poulailler-notes&file=pinOutput)

```c++
#include <Stepper.h>
#define motorPin1 8   // IN1 on the ULN2003 driver
#define motorPin2 9   // IN2 on the ULN2003 driver
#define motorPin3 10  // IN3 on the ULN2003 driver
#define motorPin4 11  // IN4 on the ULN2003 driver

#define STEPS_TOUR 2038  //le nombre d'etape pour faire 1 tour  le moteur en full-step mode fait 2048 en half step mode 4096
Stepper myStepper(STEPS_TOUR, motorPin1, motorPin3, motorPin2, motorPin4);

float nb_tours = 1.5;

#define inputPin 4

void setup() {
  //reset / homing
  pinMode(inputPin, INPUT_PULLUP);
  while (digitalRead(inputPin) == HIGH) {
    myStepper.step(-1);
    delay(30);
  }

  myStepper.setSpeed(5);
}

void loop() {
  myStepper.step(nb_tours * STEPS_TOUR);
  delay(2000);
  myStepper.step(nb_tours * STEPS_TOUR * -1);
  delay(2000);
}
```