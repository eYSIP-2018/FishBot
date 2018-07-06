/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN connected to pin of pro mini
const byte address[6] = "00001";// address of transmitter 
unsigned cnt=0;//variable for packet count
unsigned char p=0;// variabe to get power level value 
char dataToSend[10] = "Message 2";
char txNum = '0';
int incomingByte =0;

/*  NRF24L01: -18dBm, -12dBm,-6dBM, and 0dBm 
 *            MIN   , LOW , HIGH  , MAX
 */
void setup()
{
  Serial.begin(9600);
  Serial.println("SimpleTx Starting");
  radio.begin();// begi the radio with default conditions
  // data_rate = 1MBps, PA_Leve = MAX
  radio.openWritingPipe(address);//open  pipe for transmission 
  radio.setPALevel(RF24_PA_MIN);// SET PA level LOW (-18 dBm)
  radio.setDataRate(RF24_250KBPS);//set data rate 250 kbps options are (1MBps,2MBps)
  radio.stopListening();// make radio transmitter ready to tarnsmitt.
  radio.powerUp();
}

void loop()
{
  if (Serial.available() > 0)
       {
               // read the incoming byte:
               incomingByte = Serial.read();
               Serial.print("I received: ");
               Serial.println(incomingByte, DEC);
               if (incomingByte == 97 )
               {Serial.print("A received ");
                dataToSend[8] = '0';
                send();
                Serial.print("Done sending a ");
                
                }
               else if (incomingByte == 98 )
               {
                Serial.print("B received ");
                dataToSend[8] = '1';
                send();
                Serial.print("Done sending b ");
                }
               else{}
       }
  
  /*const char text[] = "This is from transmitter";//data string to be sent
  radio.write(&text, sizeof(text)); 
  delay(1000);// wait for transmission 
  cnt++;// increase count of packet 
  Serial.print("Packet Count:");
  Serial.println(cnt);*/ 
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
