## clic de fin de course
soit :
- on le place apres avoir fait le 'calcul' nombre de pas du moteur, et on le place peut etre un poil décalé 
- on s'en sert pour calculer le nombre de pas :
```c++
#include <Stepper.h>
#define inputPin 4
#define motorPin1 8
#define motorPin2 9
#define motorPin3 10
#define motorPin4 11
#define STEPS_TOUR 2038 //le nombre d'etape pour faire 1 tour le moteur en full-step mode fait 2048 en half step mode 4096

Stepper myStepper(STEPS_TOUR, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
int steps = 0;
Serial.begin(9600);
pinMode(inputPin, INPUT_PULLUP);
while (digitalRead(inputPin) == HIGH) {
myStepper.step(1);
steps++;
delay(5);
}
Serial.print(steps);
}
```

## soudure et étanchéité
pcb => https://easyeda.com/ ? pour réduire la taille et les fils qui pendouille ?

mettre le tout  dans un truc genre tupperware