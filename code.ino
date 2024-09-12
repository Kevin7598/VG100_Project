int val_forward;
int val_back;
int val_right;
int val_left;
int flag_forward;
int flag_back;
int flag_right;
int flag_left;
int state;
int buzzer = 11;
#define EchoPin  13
#define TrigPin  12
#define value  200

#include "MecanumCar_v2.h"
mecanumCar mecanumCar(3, 2);
#include "ir.h"

IR IRreceive(A3);

void setup()
{
  Serial.begin(9600);
  mecanumCar.Init();
  pinMode(EchoPin, INPUT);
  pinMode(TrigPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  val_forward = analogRead(0);
  val_right = analogRead(1);
  val_left = analogRead(4);
}

void testlight()
{
  val_forward = analogRead(A0);
  val_right = analogRead(A1);
  val_left = analogRead(A4); 
  if (val_forward > value - 40) flag_forward = 1;
  else {flag_forward = 0;}
  if (val_right > value) flag_right = 1;
  else {flag_right = 0;}
  if (val_left > value) flag_left = 1;
  else {flag_left = 0;}
}

void test_state(){
  if ((flag_forward == 1) && (flag_left == 0) && (flag_right == 0)) state = 1;
  else if (((flag_left == 0) && (flag_right == 1) && (flag_forward == 1)) || ((flag_left == 0) && (flag_right == 1) && (flag_forward == 0))) state = 2; 
  else if (((flag_left == 1) && (flag_right == 0) && (flag_forward == 0)) || ((flag_left == 1) && (flag_right == 0) && (flag_forward == 1))) state = 3;
  else state = 4;
}

void alarm(){
  mecanumCar.Stop();
  digitalWrite(buzzer, HIGH);
  mecanumCar.right_led(1);
  mecanumCar.left_led(1);
  delay(2000);
  digitalWrite(buzzer, LOW);
  mecanumCar.right_led(0);
  mecanumCar.left_led(0);
  delay(1000);
}

float Get_Distance(void) {
  float dis;
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  dis = pulseIn(EchoPin, HIGH) /58.2;
  delay(50);
  return dis;
}

void loop() {
  testlight();
  test_state();
  float distance = Get_Distance();
  if (state == 1) {
    mecanumCar.Advance();
    mecanumCar.right_led(1);
    mecanumCar.left_led(1);
    }
  if (state == 2) {
    mecanumCar.Turn_Right();
    mecanumCar.right_led(1);
    mecanumCar.left_led(0);
    }
  if (state == 3) {
    mecanumCar.Turn_Left();
    mecanumCar.right_led(0);
    mecanumCar.left_led(1);
    }
  if (state == 4) {
    mecanumCar.Stop();
    mecanumCar.right_led(0);
    mecanumCar.left_led(0);
  }
  if (distance < 4) {
      alarm();
  }
  int key = IRreceive.getKey();
  if (key != -1) {
    Serial.println(key);
    switch (key)
    {
      case 64: mecanumCar.Stop();       break;
      case 70: mecanumCar.Advance();    break;
      case 21: mecanumCar.Back();       break;
      case 68: mecanumCar.Turn_Left();  break;
      case 67: mecanumCar.Turn_Right(); break;
    }
  }
}