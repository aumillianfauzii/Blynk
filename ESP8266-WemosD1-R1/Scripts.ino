#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char auth[] = "O59idXaVBoGm4ZJEL2zsve21_MrcezMq"; //auth to connected from Blynk App
char ssid[] = "asd";   // your ssid 
char pass[] = "123456789"; // your pass

//For PIR Sensor
#define ledPin D9 // PinOut
#define pirPin D5 // PinOut
int pirState;
int val;
int x;

//For HCSR04 / Ultrasonik Sensor
#define  trig  D11 //PinOut
#define  echo  D12 //PinOut
long duration;
int distance;

//For LM35 Sensor
float vref = 3.3;
float resolution = vref / 1023.0;
float temperature, suhu, kelvin;


BlynkTimer timer;

BLYNK_CONNECTED() {
      Blynk.syncVirtual(V0);
  }

BLYNK_WRITE(V0){
 x = param.asInt();
 }

//For LM35 Sensor
void sendUptime()
{
 float temperature = analogRead(A0);//lm35 is connected to pin A0 on NodeMcu
  temperature = (temperature * resolution);
  temperature = temperature * 100.0; //Convert to Celcius
  suhu = (temperature * 1.80) + 32; //Convert to Farenheit
  kelvin = temperature + 273.15;    //Convert to Kelvin
  Serial.print("LM35 temperature: ");//serial print the value 
  Serial.println(temperature);
  //send the value to blynk application
  Blynk.virtualWrite(V11, temperature); // Blynk VirtualWrite for Celcius
  Blynk.virtualWrite(V12, suhu);        //Blynk VirtualWrite for Farenheit
  Blynk.virtualWrite(V13, kelvin);      //Blynk VirtualWrite for Kelvin
}

//For PIR Sensor
void PIRval(){
val = digitalRead(pirPin);
    if (val == HIGH) {
      digitalWrite(ledPin, HIGH);  
      }
      else {
        digitalWrite(ledPin, LOW); 
      }
   }
void pir(){
  x = digitalRead(ledPin);
  if (x == 1){
    if (digitalRead(pirPin) == HIGH)
    {
  //Blynk.notify("ALARM!!!");
    }
    }
  }

//For HCSR04 / Ultrasonic Sensor
void sendSensor()
{
  digitalWrite(trig, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay

  digitalWrite(trig, HIGH);  // tigPin high
  delayMicroseconds(10);      //trigPin high for 10 micro seconds
  digitalWrite(trig, LOW);   // trigPin low

  duration = pulseIn(echo, HIGH);   //Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;   //Calculating actual/real distance

  Serial.print("Distance = ");        //Output distance on arduino serial monitor
  Serial.println(distance);
  
  Blynk.virtualWrite(V40, distance);
  delay(1000);
}

void setup()
{
  Blynk.begin (auth, ssid, pass);

  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);

  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT); 
  //Serial.begin(9600);
  
 timer.setInterval(1000L, PIRval);  //voidPIRval
 timer.setInterval(1000L, pir);     //voidpir
 timer.setInterval(1000L, sendUptime); //void sendUptime
 timer.setInterval(1000L, sendSensor); //void sendSensor
}

void loop()
{
   Blynk.run();
   timer.run();
}
