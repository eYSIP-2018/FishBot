#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define joyX A0
#define joyY A1

RF24 radio(9, 10); // CE, CSN connected to pin of pro mini
const byte address[6] = "00001";// address of transmitter 
unsigned cnt=0;//variable for packet count
unsigned char p=0;// variabe to get power level value 
char dataToSend;
char txNum = '0';
int incomingByte =0;

int button=2;
int buttonState = 0;
int buttonState1 = 0;



void setup() {
  Serial.begin(9600);
  Serial.println("Tx Starting");
  radio.begin();// begi the radio with default conditions
  // data_rate = 1MBps, PA_Leve = MAX
  radio.openWritingPipe(address);//open  pipe for transmission 
  radio.setPALevel(RF24_PA_MIN);// SET PA level LOW (-18 dBm)
  radio.setDataRate(RF24_250KBPS);//set data rate 250 kbps options are (1MBps,2MBps)
  radio.stopListening();// make radio transmitter ready to tarnsmitt.
  radio.powerUp();
  pinMode(7,OUTPUT);
  pinMode(button,INPUT);
  digitalWrite(button, HIGH);
}
 
void loop()
{
 int xValue = analogRead(joyX);
 int yValue = analogRead(joyY);
 
  /*Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);
  */
  buttonState = digitalRead(button);
  //Serial.println(buttonState);
  if (xValue>=0 && yValue<=10)
  {
  Serial.println("left");
  dataToSend ='L';
  send();
  Serial.print("Data sent:");
  Serial.println(dataToSend); 
  }
  
  if (xValue<=10 && yValue>=500)
  {
  Serial.println("reverse");
    dataToSend ='B';
    send();
  Serial.print("Data sent:");
  Serial.println(dataToSend);
  }

  if (xValue>=1020 && yValue>=500)
  {
  Serial.println("Forward");
    dataToSend ='F';
    send();
  Serial.print("Data sent:");
  Serial.println(dataToSend);
  }

  if (xValue>=500 && yValue>=1020)
  {
  Serial.println("Right");
    dataToSend ='R';
    send();
  Serial.print("Data sent:");
  Serial.println(dataToSend);
  }

  if (xValue>=1020 && yValue>=1020)
  { 
  Serial.println("diagonal right");
    dataToSend ='9';
    send();
  Serial.print("Data sent:");
  Serial.println(dataToSend);
    
  }

  if (buttonState == LOW)
  {
    Serial.println("Switch = High");
      dataToSend ='S';
      send();
  Serial.print("Data sent:");
  Serial.println(dataToSend);
    
  }
 /* buttonState1 = digitalRead(7);
  Serial.println("#####");
  Serial.println(buttonState1);*/
  delay(200);
}


void send() {

    bool rslt;
    radio.powerUp();
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
    //delay(2000);
    radio.powerDown();
}
