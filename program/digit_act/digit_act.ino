#define MOTOR0 25
#define MOTOR1 26
#define LED 27
#define BUTTON 16

/*
 * digitalWrite(motor0,HIGH/LOW);
 * digitalWrite(motor1,HIGH/LOW); 
 * これでmotorのon/offができます
 * 
 */

void runMotor0(int runTime){
  digitalWrite(MOTOR0, HIGH);
  delay(runTime);
  digitalWrite(MOTOR0, LOW);
}
void runMotor1(int runTime){
  digitalWrite(MOTOR1, HIGH);
  delay(runTime);
  digitalWrite(MOTOR1, LOW);
}

void setup() {

  // pin mode setup
  pinMode(MOTOR0, OUTPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // serial setup
  Serial.begin(115200);
}



void loop() {

  Serial.println("hey!");
  delay(1000);

}


