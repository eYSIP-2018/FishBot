#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

Servo servo1; 
Servo servo2;
Servo servo3;
Servo servo4;


RF24 radio(9, 10); // CE, CSN
unsigned char cnt=0;
const byte address[6] = "00001"; //5 Byte address of nrf device

char dataReceived; // this must match dataToSend in the TX
bool newData = false;
char dataToSend;
char txNum = '0';
int incomingByte =0;

//***********************************************************************

int i = 0,j=0;
int select1=3;//initialize stop motion of middle body .
int select2=3;//initialize stop motion of Tail .
int select_pectoral=0; 
char previous='p';
char current='c';
int detach_flag=0;


int angle_array[4][8]={
  { 60, 75, 90, 105, 120, 105, 90, 75},     //forward
  { 60, 75, 60,75, 60, 75, 60, 75},         //right turn
  { 105, 120, 105,120, 105, 120, 105,120 }, //left turn
  { 90, 90, 90, 90, 90, 90, 90, 90}         //stop
  };

int a[8]={ 30, 60, 90, 120, 150, 120, 90, 60};
int b[8]={ 30, 60, 90, 120, 150, 120, 90, 60};
int c[8]={ 30, 60, 30,60, 30, 60, 30, 60};//for right
int d[8]={ 120, 150, 120,150, 120, 150, 120,150 };//for left

void setup() {

  Serial.begin(9600);
  pinMode(led, OUTPUT);
  Serial.println("Rx Starting");
 
  radio.begin();
  radio.openReadingPipe(0, address);
  //radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  radio.setDataRate(RF24_250KBPS);

  //servo1.attach(4);
  servo2.attach(4); 
  servo3.attach(5);
  servo1.attach(3); 
  servo4.attach(6); 
//  servo4.attach(7); 

 servo1.write(90);
 servo4.write(90);
 delay(150);
}


void loop() {
  // put your main code here, to run repeatedly:

    getData();
    showData();
  
  servo2.write(angle_array[select1][i]);
  servo3.write(angle_array[select2][i]);

/*
if (select_pectoral==1 && right ==1 )
{
  servo1.write(45);
  servo4.write(135);
  }
if (select_pectoral==1 && left ==1 )
{
  servo1.write(135);
  servo4.write(45);

  }
 */
 if (current != previous)
{detach_flag=0;}

if (select1==0 && detach_flag==0)
{
  servo1.write(90);
  servo4.write(90);
  detach_flag=1;
  }
if (select1==1 && detach_flag==0)
{
  servo1.write(45);
  servo4.write(45);  
  detach_flag=1;
}
if (select1==2 && detach_flag==0)

{
  servo1.write(135);
  servo4.write(135);
  detach_flag=1; 
}

else if(select1==0 && detach_flag==0)
{
  servo1.write(90);
  servo4.write(90);
  detach_flag=1;
  }
  
i++;
if(i>7)i=0;
delay(200);


previous = current;


if (detach_flag==1)
{detach_pectoral();}

}



void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
        current = dataReceived;
        
    }
}


void showData() {
    if (newData == true) {
        /*Serial.print("Data received ");
        Serial.println(dataReceived);*/
        if (dataReceived== 'F')
          {
             Serial.println("Forward");
             select1 = 0;
             select2 = 0;
             select_pectoral=0; 
             servo1.attach(3); 
             servo4.attach(6);
             
          }
        else if(dataReceived == 'B')
        {
            Serial.println("Reverse");         
        }
        else if(dataReceived == 'R')
        {
            Serial.println("Right"); 
            select1 = 1;
            select2 = 0;
            select_pectoral=1;
            servo1.attach(3); 
            servo4.attach(6);
                    
        }
        else if(dataReceived == 'L')
        {
            Serial.println("Left");
            select1 = 2;
            select2 = 0;
            select_pectoral=1;
            servo1.attach(3); 
            servo4.attach(6);
                    
        }
        else if(dataReceived == 'S')
        {
            Serial.println("Switch pressed: STOP");
            select1 = 3;   
            select2 = 3;            
            select_pectoral=0;
            servo1.detach();
            servo4.detach();
      
        }
        else if(dataReceived == '9')
        {
            Serial.println("Diagonal Right");         
        }
        newData = false;
    }
}


void send() {

    bool rslt;
    radio.powerUp();
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
    //delay(2000);
    radio.powerDown();
    /*Serial.print("Data Sent ");
    Serial.println(dataToSend);
    if (rslt) {
        Serial.println("  Acknowledge received");
    }
    else {
        Serial.println("  Tx failed");
    }*/
}

void detach_pectoral()
{
  servo1.detach();
  servo4.detach();
  }
