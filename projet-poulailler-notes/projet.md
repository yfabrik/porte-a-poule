#arduino #porte-poule 

# definitions
projet pour automatiser l'ouverture et la fermeture d'une porte de poulailler

# montage éléctrique

![[arduino_bb.png]]
les port D8,D9,D10,D11 controle le moteur
le port D4 est pour la butté
ces ports peuvent etre changés

pour la communication avec le module RTC
port A4 SDA  
port A5 SCL

c'est possible d'alimenter le circuit directement sur le VCC de l'arduino entre 7 et 12 V, et d'utiliser le +5V pour alimenter le reste du circuit MAIS ça dépend des modeles et j'ai pas de certitude pour le nano j'ai vu du max 800mA et d'autre qui dissent 100mA,est ce que ça concerce bien le pin 5V aussi,
le moteur pompe dans les 300mA, et comme j'en ai déja grillé un, je prend pas de risque
# différents type de portes

[diff type de porte](obsidian://open?vault=projet-poulailler-notes&file=les%20diff%C3%A9rents%20types%20de%20portes)

# contraintes
la cremaillere ça va pas, le jeu entre la porte et le chambranle fait que ça fait dabord pencher la porte 
peut etre que si c'est centré ça le fera pas

library low power ne marche pas sur tous les arduino ni ne marche de la meme façon

les piles clasique on oublie: on v'a en consommer des tonne et ça va pas tenir
# composants
[composants](obsidian://open?vault=projet-poulailler-notes&file=composants)
# alimentation
circuit doit etre alimenté en 5V, le convertisseur  LM7805 permet d'alimenter jusqu'en 12V
j'ai test la lib low power : franchement c'est pas flagrant
quand lemoteur tourne = 300mA 
en coupant le moteur  20mA
avec low power 10mA

# codes
## library utilisé
 <RTClib.h> comunication avec le module RTC, inclu la library 2wire ? pour les comunication i2c
 <Dusk2Dawn.h> obtention de l'heure du lever et coucher du soleil
 <LowPower.h> éco d'energie
 <Stepper.h> controle du moteur
## porte de base
[code](obsidian://open?vault=projet-poulailler-notes&file=compte-rendu%2Fcode%20simple)
## homing
[code](obsidian://open?vault=projet-poulailler-notes&file=compte-rendu%2Fcode%20avec%20butt%C3%A9)
## module de temps
[code](obsidian://open?vault=projet-poulailler-notes&file=compte-rendu%2Fcode%20avec%20RTC)
## économie d'energie
[code](obsidian://open?vault=projet-poulailler-notes&file=compte-rendu%2Fcode%20%C3%A9co%20batterie)


