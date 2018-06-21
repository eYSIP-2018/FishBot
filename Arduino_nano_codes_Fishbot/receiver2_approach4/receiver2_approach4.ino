#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <math.h>
#include<Servo.h>

#define servo1 5
#define servo2 6
#define servo3 9
#define servo4 10

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;
//The servo angle is set using the equation: servo_angle=90-amplitude*(sin(w*i+p)) as 90=center position of servo motor shaft
//where:
      //The set of amplitudes for various gaits:
        int a1[11]={ 30, 20, 10, 10, 20, 20,-30,-30, 30,  20,  0};
        int a2[11]={  0,  0,-10, 10,-10, 10, 30,-30,  0,   0,  0};
      
      int i=0; //Variable for time-steps
      int w=5; //Variable for angular speed (omega)
      
      //The set of phase differences between consecutive servo joints for various gaits:
      int  p[11]={ 60,120, 90, 90, 60, 60,180,180,-60,-120,  0};
      
      //The set of delays between consecutive servo positions for various gaits:
      int df[11]={150,100,150,150,200,200,150,150,150, 150,150};//For fast motion
      int  d[11]={300,300,300,300,200,200,300,300,300, 300,150};//For slow motion
      
      int s=0; //Variable to select the set of parameters from above according to the gait to be performed
      //for   s=0: Normal-Caterpillar Forward, 
      //      s=1: Steep-Caterpillar Forward, //for hurdles and/or ramps
      //      s=2: Turn Right,
      //      s=3: Turn Left, 
      //      s=4: Sidewind Right, 
      //      s=5: Sidewind Left, 
      //      s=6: Roll Right, 
      //      s=7: Roll Left, 
      //      s=8: Normal-Caterpillar Reverse, 
      //      s=9: Steep-Caterpillar Reverse
      
      int r=0; //Variable which decides if rolling gait is to be performed (r=1 when rolling is to be performed, 0 otherwise)
      int f=0; //Variable which decides delay between two servo positions (f=1 for fast motion, 0 for slow motion)


char in;       //Character Variable to store the command received from transmitter via nRF module by SPI protocol
char prev='j'; //Character Variable to store the previous value of 'in' (Initialized to garbage value 'j')
int serp=0;
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() 
{
  Serial.begin(9600);
  servo_4.attach(servo4);
  servo_3.attach(servo3);
  servo_2.attach(servo2);
  servo_1.attach(servo1);
          servo_4.write(90-(0));
          servo_3.write(90-(0));
          servo_2.write(0);
          servo_1.write(90-(40));
          delay(150); 
  
  while(true)
  {
    if (Serial.available())
    {in=Serial.read();}
    if(in=='x')
    {
      delay(4500);
          servo_4.write(90-(0));
          servo_3.write(90-(0));
          servo_2.write(90);
          servo_1.write(90-(40));
          delay(150); 
          servo_1.write(90);
      break;
    }
    
  }
}

void loop() 
{    
  if (Serial.available())
        {in=Serial.read();}
       switch(in)
      {
        case 'a' :{s=1;r=0;serp=1;f=0;} break;
        case 'b' :{s=9;r=0;serp=1;f=0;} break;
        case 'c' :{s=2;r=0;serp=0;f=0;} break;
        case 'd' :{s=3;r=0;serp=0;f=0;} break;
        case 'l' :{s=0;r=0;serp=0;f=0;} break;
        case 'm' :{s=8;r=0;serp=0;f=0;} break;
        
        case 'A' :{s=1;r=0;serp=1;f=1;} break;
        case 'B' :{s=9;r=0;serp=1;f=1;} break;
        case 'C' :{s=2;r=0;serp=0;f=1;} break;
        case 'D' :{s=3;r=0;serp=0;f=1;} break;
        case 'L' :{s=0;r=0;serp=0;f=1;} break;
        case 'M' :{s=8;r=0;serp=0;f=1;} break;
        
        case 'p' :{s=4;r=0;serp=0;f=0;} break;
        case 'q' :{s=5;r=0;serp=0;f=0;} break;
        case 'r' :{s=6;r=1;serp=0;f=0;} break;
        case 's' :{s=7;r=1;serp=0;f=0;} break;
        
        case 'o' :{s=10;r=0;serp=0;f=0;} break;
        
      }    
          if(prev!=in)i=0;
          servo_4.write(90-(      a2[s]*(  ((1-r)*sin(w*i+3*p[s]*PI/180))  +  (r*cos(w*i+3*p[s]*PI/180))  )     )      );
          servo_3.write(90-(a1[s]*(sin(w*i+3*p[s]*PI/180)))   );
          servo_2.write(90-(      a2[s]*(  ((1-r)*sin(w*i+4*p[s]*PI/180))  +  (r*cos(w*i+4*p[s]*PI/180))  )     )      );
          servo_1.write(90-(a1[s]*(sin(w*i+4*p[s]*PI/180)))   );
          delay(f*df[s]+(1-f)*d[s]);
          i++;
          prev=in;
}
/*        void normalCaterpillarForward()
        {
          s=0;
          if(prev!='l') i=0;
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        
        void normalSerpentineForward()
        {
          s=1;
          if(prev!='a'&&prev!='A') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(5));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(-5));
          i++;
          delay(d[s]);
        }
   
        void normalTurnRight()
        {
          s=2;
          if(prev!='c'&&prev!='C') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        
        void normalTurnLeft()
        {
          s=3;
          if(prev!='d'&&prev!='D') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        
        void normalSidewindRight()
        {
          s=4;
          if(prev!='p'&&prev!='P') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        
        void normalSidewindLeft()
        {
          s=5;
          if(prev!='q'&&prev!='Q') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        void normalRollRight()
        {
          s=6;
          if(prev!='r'&&prev!='R') i=0;
          servo_4.write(90-(a2[s]*cos(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*cos(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        
        void normalRollLeft()
        {
          s=7;
          if(prev!='s'&&prev!='S') i=0;
          servo_4.write(90-(a2[s]*cos(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*cos(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        void normalCaterpillarBackward()
        {
          s=8;
          if(prev!='m'&&prev!='M') i=0;
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        void normalSerpentineBackward()
        {
          s=9;
          if(prev!='b'&&prev!='B') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(5));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(-5));
          i++;
          delay(d[s]);
        }
        
        
        
/////////////////////////FAST//////////////////////////
        void fastCaterpillarForward()
        {
          s=0;
          if(prev!='L') i=0;
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        
        void fastSerpentineForward()
        {
          s=1;
          if(prev!='a'&&prev!='A') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(5));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(-5));
          i++;
          delay(df[s]);
        }
   
        void fastTurnRight()
        {
          s=2;
          if(prev!='c'&&prev!='C') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        
        void fastTurnLeft()
        {
          s=3;
          if(prev!='d'&&prev!='D') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        
        void fastSidewindRight()
        {
          s=4;
          if(prev!='p'&&prev!='P') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        
        void fastSidewindLeft()
        {
          s=5;
          if(prev!='q'&&prev!='Q') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        void fastRollRight()
        {
          s=6;
          if(prev!='r'&&prev!='R') i=0;
          servo_4.write(90-(a2[s]*cos(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*cos(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        
        void fastRollLeft()
        {
          s=7;
          if(prev!='s'&&prev!='S') i=0;
          servo_4.write(90-(a2[s]*cos(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_2.write(90-(a2[s]*cos(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        void fastCaterpillarBackward()
        {
          s=8;
          if(prev!='m'&&prev!='M') i=0;
          servo_3.write(90-(a1[s]*sin(w*i+3*p[s]*PI/180)));
          servo_1.write(90-(a1[s]*sin(w*i+4*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        void fastSerpentineBackward()
        {
          s=9;
          if(prev!='b'&&prev!='B') i=0;
          servo_4.write(90-(a2[s]*sin(w*i+3*p[s]*PI/180)));
          servo_3.write(90-(5));
          servo_2.write(90-(a2[s]*sin(w*i+4*p[s]*PI/180)));
          servo_1.write(90-(-5));
          i++;
          delay(df[s]);
        }
        
        void straightenUp()
        {
          servo_4.write(90-(0));
          servo_3.write(90-(0));
          servo_2.write(90-(0));
          servo_1.write(90-(0));
          delay(150); 
        }
*/
