#include "SimpleWebServer.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "setting.h"

#define MOTOR0 25
#define MOTOR1 26
#define LED 27
#define BUTTON 16

IPAddress ip(192, 168, 42, 2);
IPAddress gateway(192, 168, 42, 1);
IPAddress subnet(255, 255, 255, 0);

#define DETECT_TIME 30000
#define MOTOR_RUN_TIME 140

/*
 * digitalWrite(motor0,HIGH/LOW);
 * digitalWrite(motor1,HIGH/LOW); 
 * これでmotorのon/offができます
 * 
 */

SimpleWebServer server(
  APSSID, APPASS, 
  IPAddress(192,168,42,2),  // ip
  IPAddress(192,168,42,1),  // gateway
  IPAddress(255,255,255,0), 80); // subnet, port

bool flgDoor = false;
unsigned long t=0;
bool counting=false;
bool flgLab = false;

void doorOn(){
  flgDoor = true;
  Serial.println("door on");
}
void doorOff(){
  flgDoor = false;
}

void labOn(){
  if(flgDoor){
    flgLab = true;
    Serial.println("lab on");
  }
}
void labOff(){
  flgLab = false;
}

void resetFlgs(){
  doorOff();
  labOff();
  counting = false;
}

void defaultHandler(String requestLine){
  Serial.println("default_handler : " + requestLine);  
  String s = requestLine + " is not found";
  server.send_status(404, s.c_str());
}


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

void ledOn(){
  digitalWrite(LED, 1);
}
void ledOff(){
  digitalWrite(LED, 0);
}
void ledToggle(){
  if(digitalRead(LED)){
    ledOff();
  }else{
    ledOn();
  }
}

void ringChaim(){
  Serial.println("ring chaim");
  HTTPClient http;
  http.begin("http://192.168.42.1/play?file=chaim.wav");
  http.GET();
  http.end();
}

void ringWarning(){
  Serial.println("ring warning");
  HTTPClient http;
  http.begin("http://192.168.42.1/play?file=warning.wav");
  http.GET();
  http.end();
}

void setup() {

  // pin mode setup
  pinMode(MOTOR0, OUTPUT);
  pinMode(MOTOR1, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // serial setup
  Serial.begin(115200);
  delay(100);
  Serial.println("this is actuator program");

  // WiFi setup
  WiFi.config(ip, gateway, subnet);
  delay(100);
  WiFi.begin(APSSID, APPASS);
  Serial.println("WiFi connecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    ledToggle();
    delay(500);
  }
  Serial.println("\nconnencted");
  ledOff();
  Serial.print("Module IP : ");
  Serial.println(WiFi.localIP());

  // server setup
  server.add_handler("/door", doorOn);
  server.add_handler("/lab", labOn);
  server.add_default_handler(defaultHandler);
  
  server.begin();
  Serial.println("server start");
}

void loop() {
  server.handle_request();

  // ボスのドビラが開いたらカウント開始
  if(!counting && flgDoor){
    counting = true;
    t = millis();
    ringWarning();
  }

  // 一定時間内にドアノブに手が掛かったら音を鳴らす
  if(counting){
    if((millis()-t)>DETECT_TIME){
      resetFlgs();
    }else{
      if(flgLab){
        ringChaim();
        resetFlgs();
        Serial.println("motor run!!");
        runMotor0(MOTOR_RUN_TIME);
        Serial.println("reset motor position and push user button.");
        while(digitalRead(BUTTON)){
          delay(100);
        }
        Serial.println("ok. program start.");
      }
    }
  }
  delay(10);
}


