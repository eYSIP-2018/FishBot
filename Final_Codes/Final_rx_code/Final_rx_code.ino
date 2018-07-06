/*
 * Authors: Hrishikesh Shedekar, Tejas Gurav, Abhishek Rai
 * Description: Our program created for the FishBot (Robotic Fish Project) for eYSIP 2018.
 * This program includes code for the FishBot which receives commands from another arduino.
 *
 * Function List: setup(), loop(), getData(), decide_n_actuate(), detach_pectoral()
 */

#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

Servo servo1; // Standard servo placed in the Main Body/ Head of the FishBot which moves the Middle Body
Servo servo2; // Micro Servo for Pectoral Fin 1
Servo servo3; // Micro Servo for Pectoral Fin 2
Servo servo4; // Standard servo placed in the Middle Body which moves the Tail


RF24 radio(9, 10); // CE, CSN pins of the nRF24L01 module are connected to these pins [CE: Chip Enable, CSN: Chip Select Not]
const byte address[6] = "00001"; //5 Byte address of nrf device

char dataReceived; // variable to store received character; this must match dataToSend in the TX
bool newData = false; // variable to indicate whether a new data has arrived or not, so that decide_n_actuate function can execute only when a new command is received from the transmitter 
char dataToSend;   // variable to store the character to be sent

//***********************************************************************

int select1=3; // variable to decide what motion the Middle body will undergo, initialized to 3 for stopping motion of middle body when FishBot is first switched ON .
int select2=3; // variable to decide what motion the Tail will undergo, initialized to 3 for stopping motion of Tail when FishBot is first switched ON  . 

char previous='p'; // variable to keep track of previously received command
char current='c'; // variable to store the currently received command
int detach_flag=0;  // variable to ensure the detach of micro_servo motors used for pectoral fins to overcome/avoid any twitching or jitter 
int i = 0; // variable to iterate over the angle_array. 


int angle_array[4][8]={                     //2D array where rows correspond to a particular motion direction & columns correspond to the angles through which 
  //the servo motor moves to execute that particular motion.
  { 60, 75, 90, 105, 120, 105, 90, 75},     //forward  
  { 60, 75, 60, 75, 60, 75, 60, 75},        //right turn
  { 105, 120, 105, 120, 105, 120, 105, 120},//left turn
  { 90, 90, 90, 90, 90, 90, 90, 90}         //stop
  };

void setup() {

  Serial.begin(9600); //Start serial communication port
  Serial.println("Rx Starting"); //Print a line on serial monitor to indicate start of code. 

 //***********  Initialize the nRF24L01 module ************************************************
 
  radio.begin(); // Begin the radio module
  radio.openReadingPipe(0, address); // set the address over which the transmitter and receiver will communicate
  radio.setPALevel(RF24_PA_MAX); // set the power amplification level of the module. We can choose from the following options:
  //                                RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.startListening();     // initializes the module to listen to active transmitters
  radio.setDataRate(RF24_250KBPS); //set data rate of transmission to 250kbps
  
 //********************************************************************************************

 //attach the servo motors to pins 3, 4, 5, 6 of Arduino Nano
 
  servo1.attach(3); // micro servo for 1st pectoral fin
  servo2.attach(4); // standard servo for moving Middle Body
  servo3.attach(5); // standard servo for moving Tail
  servo4.attach(6); // micro servo for 2nd pectoral fin
 
//initialize the pectoral fins to horizontal level
  servo1.write(90);
  servo4.write(90);
  delay(150);  //delay for ensuring correct servo movement
}


void loop() {
  // put your main code here, to run repeatedly:

    getData(); // call  the getData function
    decide_n_actuate(); //call the decide_n_actuate function
  
  servo2.write(angle_array[select1][i]); // write the required angle to the servo motor continuously, depending on the value of select1 and i as set by the decide_n_actuate function.
  servo3.write(angle_array[select2][i]); // write the required angle to the servo motor continuously, depending on the value of select2 and i as set by the decide_n_actuate function.

//if current received command is not same as previous one then the pectoral fins need to move to a new angle, hence detach_flag is made 0 and detach_pectoral() function is not called.
//else if current command is same as previous pectoral fins need not move. 
 if (current != previous)
{detach_flag=0;}

if (select1==0 && detach_flag==0) //Forward Motion
{
  servo1.write(90); // make both pectoral fins horizontal for forward motion
  servo4.write(90);
  detach_flag=1;   // set the detach_flag to avoid jitter in pectoral fins
  }

// make pectoral fins at opposite angle to create lift on one side and drag on the other to assist in turning motion  
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
  
i++; // Update index of variable for array 
if(i>7)i=0;

delay(200); // delay for providing enough time for the servo Motors to rotate through the required angles
//By changing the delay the speed of the fish can be varied, but make sure to choose an optimum value so that the servo can complete its motion.


previous = current;


if (detach_flag==1)
{detach_pectoral();} // call detach_pectoral function, to avoid any jitter or twitching of micro servos

}

/*getData() function: receives the commands sent as characters from the 
 * transmitter module and stors it in dataReceived variable
 */

void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
        current = dataReceived; // update variable current
        
    }
}

/* decide_n_actuate() function: checks the received command and updates the select1 and select2 variables which decide which row of angle_array is iterated over
 */
void decide_n_actuate() {
    if (newData == true) {
        if (dataReceived== 'F')
          {
             Serial.println("Forward");
             select1 = 0;   //row 0 of angle_array is chosen for Middle body
             select2 = 0;   //row 0 of angle_array is chosen for Tail
             servo1.attach(3); 
             servo4.attach(6);
             
          }
        else if(dataReceived == 'B')
        {
            Serial.println("Reverse");     // take no action    
        }
        else if(dataReceived == 'R')
        {
            Serial.println("Right"); 
            select1 = 1;  //row 1 of angle_array is chosen for Middle body
            select2 = 0;  //row 0 of angle_array is chosen for Tail
            servo1.attach(3); 
            servo4.attach(6);
                    
        }
        else if(dataReceived == 'L')
        {
            Serial.println("Left");
            select1 = 2;  //row 2 of angle_array is chosen for Middle body
            select2 = 0;  //row 0 of angle_array is chosen for Tail
            servo1.attach(3); 
            servo4.attach(6);
                    
        }
        else if(dataReceived == 'S')
        {
            Serial.println("Switch pressed: STOP");
            select1 = 3;   // Bring the Middle body to normal position(90 degree)
            select2 = 3;   // Bring the Tail to normal position(90 degree)        
            detach_pectoral();
      
        }
        newData = false;
    }
}

/* detach_pectoral() function: detaches the micro servo motors(pectoral fins) from the arduino pins once their required angle is reached, to avoid jitter/twitching
 */
void detach_pectoral()
{
  servo1.detach();
  servo4.detach();
  }
