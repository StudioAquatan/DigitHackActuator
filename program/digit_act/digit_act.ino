#include "SimpleWebServer.h"
#include <WiFi.h>
#include "setting.h"

#define MOTOR0 25
#define MOTOR1 26
#define LED 27
#define BUTTON 16

IPAddress ip(192, 168, 42, 2);
IPAddress gateway(192, 168, 42, 1);
IPAddress subnet(255, 255, 255, 0);

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

void menu() {
  const char* cur = digitalRead(LED) ? "ON" : "OFF";
  String s =  "HTTP/1.1 200 OK\r\nContent-type:text/html\r\n\r\n"
              "<HTML><BODY style='font-size:48px;'>ESP32_HTTPD_LED1<br/>"
              "<br/><a href=/ledon>ON</a><br/><a href=/ledoff>OFF</a><br/><br/>"
              "Current Status = ";
  s = s + String(cur) + "</BODY></HTML>\r\n";
  server.send_response(s.c_str());
}

bool flgDoor = false;
void doorOn(){
  flgDoor = true;
  Serial.println("door on");
}
void doorOff(){
  flgDoor = false;
}

bool flgLab = false;
void labOn(){
  flgLab = true;
  Serial.println("lab on");
}
void labOff(){
  flgLab = false;
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
  delay(10);
}


