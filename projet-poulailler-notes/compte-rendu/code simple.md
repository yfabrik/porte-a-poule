fait juste tourner le moteur dans un sens puis dans l'autre
pour faire une pause de 12h = 12x60x60x1000 = 43200000 millisecondes

les pins motor de mystepper doivent etre      pin1    PIN3    PIN2    pin4
sinon y'a des problemes (ça fait que un sens)
(je sais plus si les 2 choix vont dans le meme sens ou si y'en a 1 qui marche plus)
```c++
#include <Stepper.h>
#define motorPin1 8 // IN1 on the ULN2003 driver
#define motorPin2 9 // IN2 on the ULN2003 driver
#define motorPin3 10 // IN3 on the ULN2003 driver
#define motorPin4 11 // IN4 on the ULN2003 driver
#define STEPS_TOUR 2038 
Stepper myStepper(STEPS_TOUR, motorPin1, motorPin3, motorPin2, motorPin4);
float nb_tours = 1.5;

void setup() {
myStepper.setSpeed(5);
}

void loop() {
myStepper.step(nb_tours * STEPS_TOUR);
delay(43200000);
myStepper.step(nb_tours * STEPS_TOUR * -1);
delay(43200000);
}

```
