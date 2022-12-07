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