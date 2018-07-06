#include <Servo.h>
#define led 13
Servo servo1; 
Servo servo2;
Servo servo3;
Servo servo4;

int i = 0;

void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);   // turn the LED on to indicate reset(HIGH is the voltage level)
  delay(1000); 
  digitalWrite(led, LOW);   // turn the LED on (HIGH is the voltage level)
  //servo1.attach(4);
  servo2.attach(5); 
  servo3.attach(6); 
  //servo4.attach(7); 
}

void loop() {
  /*
  for (i = 0; i < 180; i++) { 
    servo1.write(i);              
    servo2.write(i);     
    servo3.write(i);
    servo4.write(i);         
    delay(10);                      
  }
  for (i = 180; i > 0; i--) { 
    servo1.write(i);                
    servo2.write(i);     
    servo3.write(i);
    servo4.write(i);          
    delay(10);                      
  }
  */
  /*
  servo1.write(60);
  servo2.write(60);
  servo3.write(60);
  servo4.write(60);
  delay(1000);
  servo1.write(140);
  servo2.write(140);
  servo3.write(140);
  servo4.write(140);
  delay(1000);*/
  digitalWrite(led, LOW);
  servo2.write(90);
  servo3.write(90);
  delay(1000);
  /*servo2.write(30);
  delay(300);
  servo2.write(60);
  delay(300);
  servo2.write(90);
  delay(300);
   servo2.write(120);
  delay(300);
  servo2.write(150);
  delay(300);
    servo2.write(180);
  digitalWrite(led, HIGH);
  delay(10000);
  */
  
}
