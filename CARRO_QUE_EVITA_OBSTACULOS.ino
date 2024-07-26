
#include <Servo.h> 
#include <NewPing.h>
#include <AFMotor.h>  

NewPing Dist(A0, A1, 200); //(trig,echo,dist max)
Servo myServo; 

int distancia = 0;
int distanciaD = 0;
int distanciaIz = 0;
boolean Booleano = false;

AF_DCMotor Motor1(1, MOTOR12_1KHZ); 
AF_DCMotor Motor2(2, MOTOR12_1KHZ);
AF_DCMotor Motor3(3, MOTOR34_1KHZ);
AF_DCMotor Motor4(4, MOTOR34_1KHZ);

void setup() {
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
if (distancia >= 25 && distancia <= 250)
{
  Adelante();
}
else if (distancia < 25)
{
  Frenar();
  delay(150);
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

  int medirDistancia() 
  { 
    delay(50);
    int ditanciaCM = Dist.ping_cm();
    if(ditanciaCM <= 0 || ditanciaCM >= 250)
    {
      ditanciaCM = 250;
    }
    return ditanciaCM;
  }

void Frenar() {
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
    controlVelocidad();
  }
}

void Reversa() {
    Booleano = false;
    Motor1.run(FORWARD);      
    Motor2.run(FORWARD);
    Motor3.run(FORWARD);
    Motor4.run(FORWARD);  
    controlVelocidad();
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

 void controlVelocidad(){
   for (int velocidad = 0; velocidad < 160; velocidad +=2) 
   {
      Motor1.setSpeed(velocidad);
      Motor2.setSpeed(velocidad);
      Motor3.setSpeed(velocidad);
      Motor4.setSpeed(velocidad);
      delay(3);
   }
  }
