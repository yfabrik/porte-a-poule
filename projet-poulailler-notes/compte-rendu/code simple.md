fait juste tourner le moteur dans un sens puis dans l'autre
pour faire une pause de 12h = 12x60x60x1000 = 43200000 millisecondes

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
delay(43200000);
myStepper.step(nb_tours * STEPS_TOUR * -1);
delay(43200000);
}

```
