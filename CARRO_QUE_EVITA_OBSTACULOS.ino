/*CODIGO PARA CARRO QUE EVITA OBSTACULOS */
// importaciones para el manejo de los motores y el sensor
#include <Servo.h>
#include <NewPing.h>
#include <AFMotor.h>

NewPing Dist(A0, A1, 300); //(trig,echo,dist max)
NewPing Dist2(A2 , A3 , 50); //(trig , echo , dist max)
Servo myServo;

// definicion de las variables de distancia
int distanciaVacio = 0;
int distancia = 0;
int distanciaD = 0;
int distanciaIz = 0;
boolean Booleano = false;

//Definicion de los motores
AF_DCMotor Motor1(1, MOTOR12_1KHZ);
AF_DCMotor Motor2(2, MOTOR12_1KHZ);
AF_DCMotor Motor3(3, MOTOR34_1KHZ);
AF_DCMotor Motor4(4, MOTOR34_1KHZ);

void setup() {
  Serial.begin(9600);
  myServo.attach(10);
  myServo.write(115);
  delay(1000);
  myServo.write(65);
  delay(500);
  myServo.write(160);
  delay(500);
  myServo.write(115);
}

void loop() {
distancia = medirDistancia();
distanciaVacio = medirDistanciaVacio();
Serial.println(distanciaVacio);

if (distancia >= 25 && distancia <= 300 && distanciaVacio <= 5)
  {
    Adelante();
  }

else if (distancia <= 25 || distanciaVacio >= 6)
  {
    Frenar();
    delay(200);
    Reversa();
    delay(250);
    Frenar();
    delay(250);
    distanciaD = mirarDerecha();
    delay(250);
    distanciaIz = mirarIzquierda();
    delay(250);

    if(distanciaD >= distanciaIz)
      {
        GirarDerecha();
        Frenar();
      }

    else if(distanciaIz >= distanciaD)
      {
        GirarIzquierda();
        Frenar();
      }
    }
  }


int mirarDerecha()
  {
      myServo.write(60);
      delay(350);
      int distancia = medirDistancia();
      delay(50);
      myServo.write(115);
      return distancia;
  }

int mirarIzquierda()
  {
      myServo.write(165);
      delay(350);
      int distancia = medirDistancia();
      delay(50);
      myServo.write(115);
      return distancia;
  }

int medirDistanciaVacio()
{
  delay(50);
  int distanciaVacio = Dist2.ping_cm();
  if (distanciaVacio <= 0 || distanciaVacio >= 50)
  {
    distanciaVacio = 50;
  }
  return distanciaVacio;
}

int medirDistancia()
  {
    delay(50);
    int ditanciaCM = Dist.ping_cm();
    if(ditanciaCM <= 0 || ditanciaCM >= 300)
    {
      ditanciaCM = 300;
    }
    return ditanciaCM;
  }

void Frenar() 
 {
    Motor1.run(RELEASE);
    Motor2.run(RELEASE);
    Motor3.run(RELEASE);
    Motor4.run(RELEASE);
 }

void Adelante() {
 if(Booleano == false)
  {
    Booleano = true;
    Motor1.run(BACKWARD);
    Motor2.run(BACKWARD);
    Motor3.run(BACKWARD);
    Motor4.run(BACKWARD);
    controlVelocidad(150);
  }
}

void Reversa() {
    Booleano = false;
    Motor1.run(FORWARD);
    Motor2.run(FORWARD);
    Motor3.run(FORWARD);
    Motor4.run(FORWARD);
    controlVelocidad(220);
    delay(200);
}

void GirarDerecha() {
  Motor1.run(BACKWARD);
  Motor2.run(BACKWARD);
  Motor3.run(RELEASE);
  Motor4.run(RELEASE);
  delay(500);
}

void GirarIzquierda() {
  Motor1.run(RELEASE);
  Motor2.run(RELEASE);
  Motor3.run(BACKWARD);
  Motor4.run(BACKWARD);
  delay(500);
}

void controlVelocidad(int velocidad_max){
  for (int velocidad = 0; velocidad < velocidad_max; velocidad +=2)
   {
      Motor1.setSpeed(velocidad);
      Motor2.setSpeed(velocidad);
      Motor3.setSpeed(velocidad);
      Motor4.setSpeed(velocidad);
      delay(3);
   }
  }
