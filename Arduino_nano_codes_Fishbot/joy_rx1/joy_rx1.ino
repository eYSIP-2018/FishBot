
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led 2

RF24 radio(9, 10); // CE, CSN
unsigned char cnt=0;
const byte address[6] = "00001"; //5 Byte address of nrf device

char dataReceived; // this must match dataToSend in the TX
bool newData = false;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
    
  Serial.println("Rx Starting");
  radio.begin();
  radio.openReadingPipe(0, address);
  //radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  radio.setDataRate(RF24_250KBPS);
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
        /*Serial.print("Data received ");
        Serial.println(dataReceived);*/
        if (dataReceived== 'F')
          {
             Serial.println("Forward");
          }
        else if(dataReceived == 'B')
        {
            Serial.println("Reverse");         
        }
        else if(dataReceived == 'R')
        {
            Serial.println("Right");         
        }
        else if(dataReceived == 'L')
        {
            Serial.println("Left");         
        }
        else if(dataReceived == 'S')
        {
            Serial.println("Switch pressed");         
        }
        else if(dataReceived == '9')
        {
            Serial.println("Diagonal Right");         
        }
        newData = false;
    }
}
