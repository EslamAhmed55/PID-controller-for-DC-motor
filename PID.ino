#include <util/atomic.h>


// Pins
#define ENCA 2
#define ENCB 3
#define PWM 5
#define IN1 9
#define IN2 8

float lastError = 0;
// globals
long prevT = 0;
int posPrev = 0;
// Use the "volatile" directive for variables
// used in an interrupt
volatile int pos_i = 0;
volatile float velocity_i = 0;
volatile long prevT_i = 0;

float v1Filt = 0;
float v1Prev = 0;
float v2Filt = 0;
float v2Prev = 0;

float eintegral = 0;
void setup() {
  // put your setup code here, to run once:

   Serial.begin(38400);

  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
    attachInterrupt(digitalPinToInterrupt(ENCA),
                  readEncoder,RISING);

}

void loop() {
  // put your main code here, to run repeatedly:

  

  

  int pos =0;
  float velocity2=0;

 ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    pos = pos_i;
    velocity2 = velocity_i;
  }

  // Compute velocity with method 1
  long currT = micros();
  float deltaT = ((float) (currT-prevT))/1.0e6;
  float velocity1 = (pos - posPrev)/deltaT;
  posPrev = pos;
  prevT = currT;
  

    // Convert count/s to RPM
  float v1 = velocity1/374.0*60.0;
  float v2 = velocity2/374.0*60.0;

 

   // Low-pass filter (25 Hz cutoff)
  v1Filt = 0.854*v1Filt + 0.0728*v1 + 0.0728*v1Prev;
  v1Prev = v1;
  v2Filt = 0.854*v2Filt + 0.0728*v2 + 0.0728*v2Prev;
  v2Prev = v2;


// Set a target
  float vt = 100*(sin(currT/1e6)>0);



  // Compute the control signal u
 
 


float kp = 7;
float ki = 20;
float kd = 0.2;




  float e = vt-v2Filt;
  eintegral = eintegral + e*deltaT;
  float derivative = (e - lastError) / deltaT;
  float u = kp*e + ki*eintegral+ kd * derivative;;


// Set the motor speed and direction
  int dir = 1;
  if (u<0){
    dir = -1;
  }
  int pwr = (int) fabs(u);
  if(pwr > 255){
    pwr = 255;
  }



  if (v2Filt<10)
  {
    v2Filt=0;
  }

setMotor(dir, pwr, PWM, IN1, IN2);
  Serial.print(vt);
  Serial.print(" ");
 Serial.print(v2Prev);
  Serial.println();
  
  delay(1);

  lastError = e;

}


void readEncoder(){
  // Read encoder B when ENCA rises
  int b = digitalRead(ENCB);
  int increment = 0;
  if(b>0){
    // If B is high, increment forward
    increment = 1;
  }
  else{
    // Otherwise, increment backward
    increment = -1;
  }
  pos_i = pos_i + increment;
 // Compute velocity with method 2
  long currT = micros();
  float deltaT = ((float) (currT - prevT_i))/1.0e6;
  velocity_i = increment/deltaT;
  prevT_i = currT;

}



void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal); // Motor speed
  if(dir == 1){ 
    // Turn one way
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    // Turn the other way
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else{
    // Or dont turn
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);    
  }
}