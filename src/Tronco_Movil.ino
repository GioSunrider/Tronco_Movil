/*
 * Designed by @GioSunrider
 * Tronco Movil V0.1
 * CC-BY-SA
 *
 */

// ------------------------------------------------------------------------------------------
//    Esquema Coche
// ------------------------------------------------------------------------------------------ 
//    ________  
//   |        | B
//   |        |
//   |        | 
// A |________| 
     
// ------------------------------------------------------------------------------------------
// Librerias
// ------------------------------------------------------------------------------------------

#include <FlySkyIBus.h>

// ------------------------------------------------------------------------------------------
// DEFINICION PINES Pololu Dual MC33926 Motor Driver Shield
//              ARDUINO MICRO PRO
// ------------------------------------------------------------------------------------------

#define ENABLE  15

#define MotorA1 19
#define MotorA2 18

#define MotorB1 14
#define MotorB2 16

#define PWMA    6
#define PWMB    10


// ------------------------------------------------------------------------------------------
// DEFINICION PINES Entrada iBus
//       Emisora Flysky FS-i6
//      Receptor Flysky FS-X6B
// ------------------------------------------------------------------------------------------

float Channel_1;  //Ahead and back pin
float Channel_2;  //Left and Right pin
float Channel_3;  //ENABLE

// ------------------------------------------------------------------------------------------
// Distancia entre es extremo de los ejes y el centro
// Declaracion de Variable Auxiliares
// ------------------------------------------------------------------------------------------

float speed_0 = 0;
float speed_1 = 0;
float speed_2 = 0;

// ------------------------------------------------------------------------------------------
// Setup
// -----------------------------------------------------------------------------------------

void setup(){

  IBus.begin(Serial1);

  pinMode(ENABLE,OUTPUT);

  pinMode(PWMA,OUTPUT);
  pinMode(PWMB,OUTPUT);

  pinMode(MotorA1,OUTPUT);
  pinMode(MotorA2,OUTPUT);

  pinMode(MotorB1,OUTPUT);
  pinMode(MotorB2,OUTPUT);

  Serial.begin(9600);
}

// ------------------------------------------------------------------------------------------
// Convert the movement in (X, Y, W) to wheels speed.
// ------------------------------------------------------------------------------------------

void set_speed(int motor, float spd) {
  if (spd < 0){
    spd = map(spd, 0, 5, 0, 255);
    switch (motor) {
      case 0:
        digitalWrite(MotorA1, HIGH);
        digitalWrite(MotorA2, LOW);
        analogWrite(PWMA, spd);
        break;
      case 1:
        digitalWrite(MotorB1, LOW);
        digitalWrite(MotorB2, HIGH);
        analogWrite(PWMB, spd);
        break;
     }
  }

  if (spd > 0){
  spd = map(spd, 0, -5, 0, 255);
    switch (motor) {
      case 0:
        digitalWrite(MotorA1, LOW);
        digitalWrite(MotorA2, HIGH);
        analogWrite(PWMA, spd);
        break;
      case 1:
        digitalWrite(MotorB1, HIGH);
        digitalWrite(MotorB2, LOW);
        analogWrite(PWMB, spd);
        break;
    }
  }
}

void vector_movement(float X, float Y)
{

  speed_0 = X + Y;  
  speed_1 = X - Y;      

  set_speed(0, speed_0);
  set_speed(1, speed_1);
}


// ------------------------------------------------------------------------------------------
// Comienzo de los ciclos del programa
// ------------------------------------------------------------------------------------------

void loop() {

// ------------------------------------------------------------------------------------------
// Se lee cada canal via Ibus, se establece valores entre 0 y 1000 y en funcion de la
// posicion del joystick se establece un valor de -1 a 1. Para ello se mapea el valor entre
// 0 y 100 y posteriormente se divice entre 100 y -100 para dar el valor en el rango deseado.
// ------------------------------------------------------------------------------------------

IBus.loop();

Channel_1 = IBus.readChannel(0) - 1000; //Valores entre 0 y 1000 //Y
Channel_2 = IBus.readChannel(1) - 1000; //Valores entre 0 y 1000 //X
Channel_3 = IBus.readChannel(2) - 1000; //Valores entre 0 y 1000 //ENABLE


    if (Channel_3 > 500) //"ARMADO" de los motores
    {
  
      digitalWrite(ENABLE, HIGH);
  
      if(Channel_1>500){
  
        Channel_1 = map(Channel_1, 500, 1000, 0, 100);
  
      } else {
  
        Channel_1 = map(Channel_1, 500, 0, 0, 100);
        Channel_1 = -Channel_1;
  
      }
  
      if(Channel_2>500){
  
        Channel_2 = map(Channel_2, 500, 1000, 0, 100);
  
      } else {
  
        Channel_2 = map(Channel_2, 500, 0, 0, 100);
        Channel_2 = -Channel_2;
  
      }
  
//Escritura
    vector_movement(Channel_2, Channel_1);
  
    }else {
      digitalWrite(ENABLE, LOW);
    }  
}
