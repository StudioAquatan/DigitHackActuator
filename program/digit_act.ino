int const motor0=25;
int const motor1=26;
int const led=27;
int const button=16;

/*
 * digitalWrite(motor0,HIGH/LOW);
 * digitalWrite(motor1,HIGH/LOW); 
 * これでmotorのon/offができます
 * 
 */

void setup() {
  // put your setup code here, to run once:
  pinMode(motor0,OUTPUT);
  pinMode(motor1,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  //Serial.begin(115200);
}



void loop() {
  // put your main code here, to run repeatedly:

}


