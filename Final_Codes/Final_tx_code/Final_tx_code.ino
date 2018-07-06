/*
 * Authors: Hrishikesh Shedekar, Tejas Gurav, Abhishek Rai
 * Description: Our program created for FishBot (Robotic Fish Project) for eYSIP 2018.
 * This program includes code for the remote controller which sends commands to the FishBot.
 *
 * Function List: setup(), loop(), send()
 */


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led 5   // Led to indicate if Transmission is successful or not

#define joyX A0  // Horizontal axis Potentiometer of Joystick module
#define joyY A1  // Vertical axis Potentiometer of Joystick module

RF24 radio(9, 10); // CE, CSN pins of the nRF24L01 module are connected to these pins [CE: Chip Enable, CSN: Chip Select Not]
const byte address[6] = "00001";// 5 byte address of transmitter 
char dataToSend;  // variable to store the character to be sent
int button=2;     // Button is connected to pin 2 of arduino 
int buttonState = 0;  // variable to store value from the in-built push button in the joystick module



void setup() {
  Serial.begin(9600);
  Serial.println("Tx Starting");
  
  pinMode(led, OUTPUT);
  radio.begin();// begin the radio with default conditions
  // data_rate = 1MBps, PA_Level = MAX
  radio.openWritingPipe(address);//open  pipe for transmission 
  radio.setPALevel(RF24_PA_MAX);// SET PA level High(0 dBm)
  radio.setDataRate(RF24_250KBPS);//set data rate 250 kbps options are (1MBps,2MBps)
  radio.stopListening();// make radio transmitter ready to tarnsmitt.
  radio.powerUp();
  pinMode(7,OUTPUT);
  pinMode(button,INPUT);
  digitalWrite(button, HIGH);
}
 
void loop()
{
 int xValue = analogRead(joyX); // read the analog value of Horizontal axis Potentiometer
 int yValue = analogRead(joyY); // read the analog value of Vertical axis Potentiometer
 
  /*Serial.print(xValue);
  Serial.print("\t");
  Serial.println(yValue);
  */
  buttonState = digitalRead(button); //read state of the Button


//Depending on the position of the joystick, and the corresponding values from the two potentiometers the command is encoded into characters F, B, L, R 
// which represent Forward, Back/Reverse, Left, Right respectively.
  
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

  if (buttonState == LOW) // if switch is pressed
  {
    Serial.println("Switch = High");
      dataToSend ='S';
      send();
  Serial.print("Data sent:");
  Serial.println(dataToSend);
    
  }
  delay(200);
}


void send() {
    bool rslt;
    radio.powerUp();
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );  // sends data to receiver 
    radio.powerDown(); //put nRF24L01 in Power Down mode to save energy.
    if (rslt) {
        Serial.println("  Acknowledge received");
        digitalWrite(led, LOW);  
    }
    else {
        Serial.println("  Tx failed");
        digitalWrite(led, HIGH); //turn ON led to indicate transmission failed
    }
}
