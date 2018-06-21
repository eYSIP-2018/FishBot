
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#define led 2

Servo servo1; 
Servo servo2;
Servo servo3;
Servo servo4;

int i = 0;



RF24 radio(9, 10); // CE, CSN
unsigned char p=0,cnt=0;
const byte address[6] = "00001"; //5 Byte address of nrf device

char dataReceived[10]; // this must match dataToSend in the TX
bool newData = false;

void setup() {
  //unsigned char p=0;
  Serial.begin(9600);
    pinMode(led, OUTPUT);
    
    Serial.println("SimpleRx Starting");
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
    getData();
    showData();
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
        Serial.print("Data received ");
        Serial.println(dataReceived);
        if (dataReceived[8] == '1')
          {
            digitalWrite(led, HIGH);
            //delay(1000); 
          }
        else if (dataReceived[8] == '2')
        {

                /*servo1.write(60);
                servo2.write(60);
                servo3.write(60);*/
                servo4.write(60);
                delay(250);
                /*servo1.write(140);
                servo2.write(140);
                servo3.write(140);*/
                servo4.write(140);
                delay(250);
        }
        else if(dataReceived[8] == '0')
        {
            digitalWrite(led, LOW);
            //delay(1000);         
        
        }
        newData = false;
    }
}
