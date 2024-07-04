#include <Servo.h>
#include "LiquidCrystal.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // liaison 4 bits de données
Servo Portail_1; //  Créez un objet servo
Servo Portail_2; //  Créez un objet servo

int pos_1 = 0;
int pos_2 = 0;

// Variables diverses
int Maximum = 100;
int etatbouton = 0;
int bouton = 10;
int b1 = 6;
int M1 = 7;
int places = 3;
float c;

// Capteurs infrarouges et variables associées
int inputPin = 9;
int inputPin2 = 8;
int inputPin3 = 13;
int val = 0;
int val2 = 0;
int val3 = 0;

// Chiffres test
int a = 1;
int b = 1;
int ce = 1;
int d;
int de;

// Voyant 1
int sheesh1;
int sheesh2 = 0;

// Voyant 2
// distance (en cm) en dessous de laquelle la LED s'allume
int dmin = 63;

// Variable des places
int chiffre = 3;
int etatPrecedentInputPin = LOW;
bool voitureDetectee = false; // Indique si une voiture est détectée

// Int Long
long temps;
long distance;

#define capteur_force A0

void setup()
{
  // Portail servo
  Portail_1.attach(A1);
  Portail_1.write(0);

  Portail_2.attach(A2);
  Portail_1.write(90);

  lcd.begin(16, 2); // utilisation d'un écran 16 colonnes et 2 lignes
  pinMode(bouton, INPUT);
  pinMode(M1, OUTPUT);
  Serial.begin(9600);
  pinMode(inputPin, INPUT);
  pinMode(inputPin2, INPUT);
  pinMode(inputPin3, INPUT);
}

void loop()
{
  etatbouton = digitalRead(bouton);
  int Seriale = analogRead(A0);

  if (etatbouton == HIGH)
  {
    // Phase Ouverture
    lcd.clear();
    lcd.print("Portail ouvert");
    digitalWrite(b1, HIGH);
    for (pos_1 = 1; pos_1 <= 180; pos_1++)
      Portail_1.write(pos_1);

    for (pos_2 = 2; pos_2 <= 90; pos_2--)
      Portail_2.write(pos_2);
    delay(5000);

    // Phase temps d'attente fermeture 2s
    lcd.clear();
    lcd.print("Fermeture...");
    digitalWrite(b1, LOW);
    digitalWrite(M1, HIGH);
    delay(3000);

    // Phase Réouverture avec boucle
    while (Seriale >= 100)
    {
      Seriale = analogRead(A0); // Mise à jour de la valeur de Seriale
      digitalWrite(M1, LOW);
      digitalWrite(b1, HIGH);
      for (pos_1 = 1; pos_1 <= 150; pos_1++)
        Portail_1.write(pos_1);

      for (pos_2 = 2; pos_2 <= 30; pos_2++)
        Portail_2.write(pos_2);
      delay(2000);
      lcd.clear();
      lcd.print("Reouverture 5S");
      delay(5000);

      for (pos_1 = 1; pos_1 <= 180; pos_1++)
        Portail_1.write(pos_1);

      for (pos_2 = 2; pos_2 <= 30; pos_2--)
        Portail_2.write(pos_2);
      delay(500);
      delay(5000);
      lcd.clear();
      lcd.print("Fermeture...");
      digitalWrite(b1, LOW);
      digitalWrite(M1, HIGH);
      delay(3000);
      lcd.clear();
    }
    // Phase Fermeture
    for (pos_1 = 180; pos_1 >= 90; pos_1--)
      Portail_1.write(pos_1);

    for (pos_2 = 180; pos_2 >= 90; pos_2--)
      Portail_2.write(pos_2);
    delay(500);
    delay(5000);
    digitalWrite(M1, LOW);
    lcd.clear();
  }
  else
  {
    // Lecture de l'état du capteur infrarouge
    int etatActuelInputPin = digitalRead(inputPin);

    if (etatActuelInputPin == HIGH && etatPrecedentInputPin == LOW && !voitureDetectee)
    {
      // Détection du passage de LOW à HIGH, on enlève 1 de "chiffre"
      chiffre = chiffre - 1;
      voitureDetectee = true; // Marque qu'une voiture a été détectée
      delay(2000);
    }
    else if (etatActuelInputPin == LOW && etatPrecedentInputPin == HIGH && voitureDetectee)
    {
      // Détection du passage de HIGH à LOW, on ajoute 1 à "chiffre"
      chiffre = chiffre + 1;
      voitureDetectee = false; // Réinitialise la détection de voiture
      delay(2000);
    }

    // Mettre à jour l'état précédent
    etatPrecedentInputPin = etatActuelInputPin;

    // Affichage du message par défaut
    lcd.clear();
    lcd.print("Bienvenue");
    lcd.setCursor(0, 1);
    lcd.print("Places rest. ");
    lcd.setCursor(13, 1);
    lcd.print(chiffre);
    delay(500);
  }
}