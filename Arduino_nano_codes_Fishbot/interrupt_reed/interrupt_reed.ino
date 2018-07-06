const byte ledPin = 13;
const byte interruptPin = 2;
volatile byte state = LOW;
unsigned char cnt=0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);
  Serial.println("REED Interrupt program Starting");

}

void loop() {
  digitalWrite(ledPin, state);
}

void blink() {
  state = !state;
  Serial.print("Interrupt occured");
  Serial.println(cnt);
  cnt++;
}
