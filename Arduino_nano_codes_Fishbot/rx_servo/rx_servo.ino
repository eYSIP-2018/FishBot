
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
int counter = 0; // Loop counter variable
float lag = .5712; // Phase lag between segments
int frequency = 1; // Oscillation frequency of segments.
int amplitude = 40; // Amplitude of the serpentine motion of the snake
int rightOffset = 5; // Right turn offset
int leftOffset = -5; // Left turn offset
int offset = 6; // Variable to correct servos that are not exactly centered
int delayTime = 5; // Delay between limb movements
int startPause = 5000;  // Delay time to position robot
int test = -3; // Test varialble takes values from -6 to +5
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
    for(counter = 0; counter < 360; counter += 1) {
    delay(delayTime);
    Serial.print("S1:");
    Serial.print(90+offset+amplitude*cos(frequency*counter*3.14159/180+1*lag));
    Serial.print("   S2:");
    Serial.println(90+offset+amplitude*cos(frequency*counter*3.14159/180+2*lag));
    servo2.write(90+offset+amplitude*cos(frequency*counter*3.14159/180+1*lag));
    servo3.write(90+offset+amplitude*cos(frequency*counter*3.14159/180+2*lag));
    }
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


