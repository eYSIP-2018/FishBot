
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define led 2

Servo servo1; 
Servo servo2;
Servo servo3;
Servo servo4;


RF24 radio(9, 10); // CE, CSN
unsigned char cnt=0;
const byte address[6] = "00001"; //5 Byte address of nrf device

char dataReceived; // this must match dataToSend in the TX
bool newData = false;
//***************************************************************

int a1[11]={ 30, 20, 10, 10, 30, 20,-30,-30, 30,  20,  0};
int a2[11]={  0,  0,-10, 10,-10, 10, 30,-30,  0,   0,  0};
      
int i=0; //Variable for time-steps
int w=4; //Variable for angular speed (omega)

//The set of phase differences between consecutive servo joints for various gaits:
int  p[11]={ 60,120, 90, 90, 60, 60,180,180,-60,-120,  0};
int df[11]={150,100,150,150,200,200,150,150,150, 150,150};//For fast motion
int  d[11]={300,300,300,300,300,200,300,300,300, 300,150};//For slow motion

int s=4;
char in;       //Character Variable to store the command received from transmitter via nRF module by SPI protocol
char prev='j';
int f=0; 
float angle1=0;
float angle2=0;     
//***************************************************************


void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  Serial.println("Rx Starting");
 
  radio.begin();
  radio.openReadingPipe(0, address);
  //radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  radio.setDataRate(RF24_250KBPS);

  servo1.attach(4);
  servo2.attach(5); 
  servo3.attach(6); 
  servo4.attach(7); 
}

//=============

void loop() {
    /*getData();
    showData();*/
    angle1=90-(30*(sin(w*i+1*p[s]*PI/180)));
    angle2=90-(50*(sin(w*i+2*p[s]*PI/180)));
    if(prev!=in)i=0;
          Serial.print("S1:");
          Serial.print(angle1);
          Serial.print("   S2:");
          Serial.println(angle2);
          servo2.write(angle1);
          servo3.write(angle2);
          Serial.print("   delay:");
          Serial.println(f*df[s]+(1-f)*d[s]);
          delay(400);
          //delay(f*df[s]+(1-f)*d[s]);
          i++;
          prev=in;
}

//==============

void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
}

void showData() {
    if (newData == true) {
        /*Serial.print("Data received ");
        Serial.println(dataReceived);*/
        if (dataReceived== 'F')
          {
             Serial.println("Forward");
             Forward();
          }
        else if(dataReceived == 'B')
        {
            Serial.println("Reverse");         
        }
        else if(dataReceived == 'R')
        {
            Serial.println("Right"); 
            Right();        
        }
        else if(dataReceived == 'L')
        {
            Serial.println("Left");
            Left();         
        }
        else if(dataReceived == 'S')
        {
            Serial.println("Switch pressed: STOP");         
        }
        else if(dataReceived == '9')
        {
            Serial.println("Diagonal Right");         
        }
        newData = false;
    }
}

void Forward()
{

}

void Left()
{
  
}

void Right()
{

}


