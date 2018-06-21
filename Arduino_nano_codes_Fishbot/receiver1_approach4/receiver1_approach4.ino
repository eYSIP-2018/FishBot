//Include the various libraries required:
  #include <math.h>  //For special arithmetic operations
  #include<Servo.h>  //For controlling servos
  //For the nRF module which uses SPI communication protocol
  #include <SPI.h>
  #include <nRF24L01.h>
  #include <RF24.h>

//Define the sensor pins (Pins S0 and S1 are hardwired for 20% frequency scaling)
#define S2  A3  
#define S3  A4
#define sensorOut A5

//Define the RGB LED pins
#define red   2   
#define green 3
#define blue  4

//Define the buzzer pin
#define buzz A0

//Define the pins for 4 servo motors
#define servo5 5
#define servo6 6
#define servo7 9
#define servo8 10

//Create the servo objects for controlling the servo motors
Servo servo_5;
Servo servo_6;
Servo servo_7;
Servo servo_8;

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

RF24 radio(7, 8); // CE, CSN pins of the nRF receiver module

const byte address[6] = "00001"; //A unique address to ensure communication with proper transmitter only

void setup() 
{
  Serial.begin(9600);//Begin Serial communication with Board 2 at a baud rate of 9600 bps
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  
  servo_8.attach(servo8);
  servo_7.attach(servo7);
  servo_6.attach(servo6);
  servo_5.attach(servo5);
  
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(buzz, OUTPUT);

  digitalWrite(red,HIGH);
  digitalWrite(green,HIGH);
  digitalWrite(blue,HIGH);
  
  while(true)
  {
    if (radio.available())
     {
        radio.read(&in,sizeof(in));
        Serial.write(in); 
     }
     if(in=='x')
     {
      digitalWrite(buzz,HIGH);delay(2000);
      digitalWrite(buzz,LOW);delay(1000);
      digitalWrite(buzz,HIGH);delay(2000);
      digitalWrite(buzz,LOW);
      break;}
  }
  
}

void loop() 
{    
  if (radio.available())
  {
    radio.read(&in,sizeof(in));
    //Serial.println(in);
    Serial.write(in);
  }
       switch(in)
      {
        case 'a' :{s=1;r=0;f=0;} break;
        case 'b' :{s=9;r=0;f=0;} break;
        case 'c' :{s=2;r=0;f=0;} break;
        case 'd' :{s=3;r=0;f=0;} break;
        case 'l' :{s=0;r=0;f=0;} break;
        case 'm' :{s=8;r=0;f=0;} break;
        
        case 'A' :{s=1;r=0;f=1;} break;
        case 'B' :{s=9;r=0;f=1;} break;
        case 'C' :{s=2;r=0;f=1;} break;
        case 'D' :{s=3;r=0;f=1;} break;
        case 'L' :{s=0;r=0;f=1;} break;
        case 'M' :{s=8;r=0;f=1;} break;
        
        case 'p' :{s=4;r=0;f=0;} break;
        case 'q' :{s=5;r=0;f=0;} break;
        case 'r' :{s=6;r=1;f=0;} break;
        case 's' :{s=7;r=1;f=0;} break;
        
        case 'o' :{s=10;r=0;f=0;} break;
        case 't' :{colordetect();} break;
        case 'x': {
                      for(int k=0;k<5;k++)
                      {
                        digitalWrite(buzz,HIGH);
                        delay(1000);
                        digitalWrite(buzz,LOW);
                        delay(1000);
                      }
                  }break;
      }    
          if(prev!=in)i=0;
          servo_8.write(90-(      a2[s]*(  ((1-r)*sin(w*i+1*p[s]*PI/180))  +  (r*cos(w*i+1*p[s]*PI/180))  )     )      );
          servo_7.write(90-(a1[s]*(sin(w*i+1*p[s]*PI/180)))   );
          servo_6.write(90-(      a2[s]*(  ((1-r)*sin(w*i+2*p[s]*PI/180))  +  (r*cos(w*i+2*p[s]*PI/180))  )     )      );
          servo_5.write(90-(a1[s]*(sin(w*i+2*p[s]*PI/180)))   );
          delay(f*df[s]+(1-f)*d[s]);
          i++;
          prev=in;
}

void colordetect()
{
     int frequency = 0,rfrequency=0,gfrequency=0,bfrequency=0, r=0, g=0,b=0,current=0;
                            // Setting red filtered photodiodes to be read
                            digitalWrite(S2,LOW);
                            digitalWrite(S3,LOW);
                            frequency = pulseIn(sensorOut, LOW); // Reading the OUTPUT frequency
                            rfrequency = map(frequency, 46,280,255,0);  //RemaPIng the value of the frequency to the RGB Model of 0 to 255
                            delay(150);
                          
                            // Setting Green filtered photodiodes to be read
                            digitalWrite(S2,HIGH);
                            digitalWrite(S3,HIGH);
                            frequency = pulseIn(sensorOut, LOW);   // Reading the OUTPUT frequency
                            gfrequency = map(frequency, 62,336,255,0); //RemaPIng the value of the frequency to the RGB Model of 0 to 255
                             delay(150);
                          
                            // Setting Blue filtered photodiodes to be read
                            digitalWrite(S2,LOW);
                            digitalWrite(S3,HIGH);
                            frequency = pulseIn(sensorOut, LOW);  // Reading the OUTPUT frequency
                            bfrequency = map(frequency, 62,338,255,0); //RemaPIng the value of the frequency to the RGB Model of 0 to 255
                            delay(150);
                            
                          
                           if(rfrequency>gfrequency && rfrequency>bfrequency ) {//Serial.print("\n Red dectected");
                           digitalWrite(red,LOW);digitalWrite(buzz,HIGH); delay(500);digitalWrite(buzz,LOW); delay(1000);
                           digitalWrite(buzz,HIGH);delay(500);digitalWrite(buzz,LOW);digitalWrite(red,HIGH);}
                           /*if(gfrequency>rfrequency && gfrequency>bfrequency ) {//Serial.print("\n Green dectected");
                           digitalWrite(green,LOW);digitalWrite(buzz,HIGH);delay(500);digitalWrite(buzz,LOW);delay(1000);
                           digitalWrite(buzz,HIGH);delay(500);digitalWrite(buzz,LOW);digitalWrite(green,HIGH);}
                           if(bfrequency>rfrequency && bfrequency>gfrequency ) {//Serial.print("\n Blue dectected");
                           digitalWrite(blue,LOW);digitalWrite(buzz,HIGH);delay(500);digitalWrite(buzz,LOW);delay(1000);
                           digitalWrite(buzz,HIGH);delay(500);digitalWrite(buzz,LOW);digitalWrite(blue,HIGH);}*/
                            else{ 
                            digitalWrite(blue,LOW);digitalWrite(buzz,HIGH); delay(500);digitalWrite(buzz,LOW); delay(1000);
                           digitalWrite(buzz,HIGH);delay(500);digitalWrite(buzz,LOW);digitalWrite(blue,HIGH);
                            }
        }
        
/*        void normalCaterpillarForward()
        {
          s=0;
          if(prev!='l') i=0;
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        
        void normalSerpentineForward()
        {
          s=1;
          if(prev!='a'&&prev!='A') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(5));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(5));
          i++;
          delay(d[s]);
        }
   
        void normalTurnRight()
        {
          s=2;
          if(prev!='c'&&prev!='C') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        
        void normalTurnLeft()
        {
          s=3;
          if(prev!='d'&&prev!='D') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        
        void normalSidewindRight()
        {
          s=4;
          if(prev!='p'&&prev!='P') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        
        void normalSidewindLeft()
        {
          s=5;
          if(prev!='q'&&prev!='Q') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        void normalRollRight()
        {
          s=6;
          if(prev!='r'&&prev!='R') i=0;
          servo_8.write(90-(a2[s]*cos(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*cos(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        
        void normalRollLeft()
        {
          s=7;
          if(prev!='s'&&prev!='S') i=0;
          servo_8.write(90-(a2[s]*cos(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*cos(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        void normalCaterpillarBackward()
        {
          s=8;
          if(prev!='m'&&prev!='M') i=0;
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(d[s]);
        }
        void normalSerpentineBackward()
        {
          s=9;
          if(prev!='b'&&prev!='B') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(5));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(5));
          i++;
          delay(d[s]);
        }
        
        
        
/////////////////////////FAST//////////////////////////
        void fastCaterpillarForward()
        {
          s=0;
          if(prev!='L') i=0;
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        
        void fastSerpentineForward()
        {
          s=1;
          if(prev!='a'&&prev!='A') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(5));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(5));
          i++;
          delay(df[s]);
        }
   
        void fastTurnRight()
        {
          s=2;
          if(prev!='c'&&prev!='C') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        
        void fastTurnLeft()
        {
          s=3;
          if(prev!='d'&&prev!='D') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        
        void fastSidewindRight()
        {
          s=4;
          if(prev!='p'&&prev!='P') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        
        void fastSidewindLeft()
        {
          s=5;
          if(prev!='q'&&prev!='Q') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        void fastRollRight()
        {
          s=6;
          if(prev!='r'&&prev!='R') i=0;
          servo_8.write(90-(a2[s]*cos(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*cos(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        
        void fastRollLeft()
        {
          s=7;
          if(prev!='s'&&prev!='S') i=0;
          servo_8.write(90-(a2[s]*cos(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_6.write(90-(a2[s]*cos(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        void fastCaterpillarBackward()
        {
          s=8;
          if(prev!='m'&&prev!='M') i=0;
          servo_7.write(90-(a1[s]*sin(w*i+1*p[s]*PI/180)));
          servo_5.write(90-(a1[s]*sin(w*i+2*p[s]*PI/180)));
          i++;
          delay(df[s]);
        }
        void fastSerpentineBackward()
        {
          s=9;
          if(prev!='b'&&prev!='B') i=0;
          servo_8.write(90-(a2[s]*sin(w*i+1*p[s]*PI/180)));
          servo_7.write(90-(5));
          servo_6.write(90-(a2[s]*sin(w*i+2*p[s]*PI/180)));
          servo_5.write(90-(5));
          i++;
          delay(df[s]);
        }
   */     
        
/*       
        void straightenUp()
        {
          servo_8.write(90-(0));
          servo_7.write(90-(0));
          servo_6.write(90-(0));
          servo_5.write(90-(0));
          delay(150); 
        }
*/
