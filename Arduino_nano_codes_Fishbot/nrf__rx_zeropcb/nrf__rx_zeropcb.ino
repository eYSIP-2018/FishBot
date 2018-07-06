

// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led 5
#define CE_PIN   9
#define CSN_PIN  10

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[10]; // this must match dataToSend in the TX
bool newData = false;

//===========

void setup() {

    Serial.begin(9600);
    pinMode(led, OUTPUT);
    
    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(108); 
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
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
        else if(dataReceived[8] == '0')
        {
            digitalWrite(led, LOW);
            //delay(1000);         
        
        }
        newData = false;
    }
}
