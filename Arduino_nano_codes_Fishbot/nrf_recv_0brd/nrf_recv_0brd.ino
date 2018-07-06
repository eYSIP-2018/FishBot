
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led 2

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
}

/*
void loop() 
{
 // bool t;
    //t = radio.testRPD();
    //Serial.println(t ? "Strong signal > 64dBm" : "Weak signal < 64dBm" );
    if (radio.available()) 
    {
      char text[32] = "";
      radio.read(&text, sizeof(text));
      Serial.println(text);
      Serial.print("Packet count:");
      Serial.println(cnt);
      cnt++;
      p=radio.getPALevel();
      Serial.print("Power Level:");
      Serial.println(p);
   
    
   }//if end 
}// loop end 
*/
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
