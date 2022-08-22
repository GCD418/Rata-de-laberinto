#define pM(a, b) pinMode(a, b) 
#define O OUTPUT
#define I INPUT
#define dW(a, b) digitalWrite(a, b)
#define dR(a) digitalRead(a)
const int mda = 2, mdr = 3, mia = 4, mir = 5, sft = 6, sfe = 7, sdt = 8, sde = 9, sit = 12, sie = 13, actI = 10, actD = 11; //motor derecha adelante, motor derecha retro, motor izquierda adelante . . .
long tiempo1 = -1, tiempo2 = -1, distanciaIzquierda1, distanciaIzquierda2, distanciaDerecha1;
long distanciaDerecha2;
 
int inches = 0;
 
int cm = 0;
 
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}
 
long distancia(int x, int y)
{
	cm = 0.01723 * readUltrasonicDistance(x, y);
	return cm;
}
 
void stop()
{
	dW(mda, 0);
	dW(mdr, 0);
	dW(mia, 0);
	dW(mir, 0);
}

void derecha()
{
	digitalWrite(mia, 1);
 	analogWrite(actI, 130);
 	digitalWrite(mdr, 1);
 	analogWrite(actD, 130);
 	delay(1300);
 	stop();
}

void derecha(int time)
{
	digitalWrite(mia, 1);
 	analogWrite(actI, 130);
 	digitalWrite(mdr, 1);
 	analogWrite(actD, 130);
 	delay(time);
 	stop();	
}
 
void izquierda()
{
 	digitalWrite(mda, 1);
 	analogWrite(actD, 130);
 	digitalWrite(mir, 1);
 	analogWrite(actI, 130);
 	delay(1300);
 	stop();
}

void izquierda(int time)
{
	digitalWrite(mda, 1);
 	analogWrite(actD, 130);
 	digitalWrite(mir, 1);
 	analogWrite(actI, 130);
 	delay(time);
 	stop();	
}
 
void avanzar()
{
 	digitalWrite(mda, 1); //definir velocidad
 	analogWrite(actD, 130);
 	digitalWrite(mia, 1);
 	analogWrite(actI, 130);
}

void avanzar(int time)
{
 	digitalWrite(mda, 1); //definir velocidad
 	analogWrite(actD, 130);
 	digitalWrite(mia, 1);
 	analogWrite(actI, 130);
 	delay(time);
 	stop();
}

void retroceder()
{
 	digitalWrite(mdr, 1); //definir velocidad
 	analogWrite(actD, 130);
 	digitalWrite(mir, 1);
 	analogWrite(actI, 130);
}
 
bool detector_frontal()
{
 	if (distancia(sft, sfe) < 3)
 	{
 		return true;
 	}
 
}
bool detector_izquierda()
{
	if(distancia(sit, sie) < 3)
	{
		return true;
	}
}
bool detector_derecho()
{
 if (distancia(sdt, sde) < 3)
	{
		return true;
	}
}

void corregirCurso()
{
	if(tiempo1 == -1)
	{
		tiempo1 = millis();
		distanciaDerecha1 = readUltrasonicDistance(sdt, sde);
		distanciaIzquierda1 = readUltrasonicDistance(sit, sie);
	}
	tiempo2 = millis();
	distanciaDerecha2 = readUltrasonicDistance(sdt, sde);
	distanciaIzquierda2 = readUltrasonicDistance(sit, sie);
	if((tiempo2 - tiempo1) >= 1000)
	{
		if((distanciaDerecha2 - distanciaDerecha1) > 0)
		{
			izquierda(40);
		}
		else if((distanciaDerecha2 - distanciaDerecha1) < 0)
		{
			derecha(40);
		}
	}
}

void setup()
{
	pM(mda, O);
	pM(mdr, O);
	pM(mia, O);
	pM(mir, O);
}
 
void loop()
{
	while(!detector_frontal())
	{
		avanzar();
		delay(80);
		if(!detector_derecho())
		{
			stop();
			derecha();
			avanzar(60);
			break;
		}

		corregirCurso();
	}
	stop();
	if(!detector_izquierda())
	{
		stop();
		izquierda();
		avanzar(60);
	}
	else
	{
		izquierda();
		izquierda();
	}
}