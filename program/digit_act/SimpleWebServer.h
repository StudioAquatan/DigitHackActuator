#include <WiFi.h>
#include <WiFiServer.h>
#include <map>
using namespace std; 
 
typedef void (*Handler_t)(void) ;
typedef void (*DefaultHandler_t)(String) ;
typedef struct {
  int status;
  const char* cpDesc;
} response_t;
 
const response_t responses[] = {
  {200, "200 OK"},
  {404, "404 Not Found"},
  {500, "500 Server Error"},
  {0, 0}
};
 
class SimpleWebServer : public WiFiServer {
  IPAddress addr;
  IPAddress gateway;
  IPAddress subnet;
  String ssid;
  String password;
  std::map<String, Handler_t> request_handlers;
  DefaultHandler_t default_handler = 0;
  WiFiClient* client_connected;
 
public:
  SimpleWebServer(const char* ssid, const char* password, 
		  const IPAddress addr, const IPAddress gateway, const IPAddress subnet, int port = 80) : WiFiServer(port) {
    this->addr = addr;
	this->gateway = gateway;
    this->subnet = subnet;
    this->ssid = ssid;
    this->password = password;
    client_connected = NULL;
  }
 
  void begin() {
	// WiFi.config(addr, gateway, subnet);
    // Serial.print("Module IP : "); 
    // Serial.print(WiFi.localIP());
    // Serial.println(" " + ssid + String(" starts.")); 
    WiFiServer::begin();
    Serial.println("SimpleWebServer begin()");
  }
 
  void add_handler(String uri, Handler_t func) {
    request_handlers[uri] = func;
  }
 
  void add_default_handler(DefaultHandler_t func) {
    default_handler = func;
  }
 
  void send_response(const char* cp)
  {
    if (client_connected)
      client_connected->print(cp);
  }
 
  void send_status(int status, const char* cp = NULL)
  {
    if (!client_connected)
      return;
    const response_t* p = responses;
    while(p->status != 0) {
      if (p->status == status) {
        String s = "HTTP/1.1" + String(status) + String(" ") + String(p->cpDesc) + String("\r\n\r\n");
        const char* cpDesc = cp ? cp : p->cpDesc;
        s = s + String("<HTML><BODY style='font-size:48px;'>") + String(cpDesc) + String("</BODY></HTML>\r\n");
        client_connected->print(s);
        return;        
      }
      p++;
    }
    send_status(200);
  }
 
  void handle_request() {
    WiFiClient client = available();
    if (!client)
      return;
 
    String request_line = "";
    while (client) {
      client_connected = &client;
      while(client.available()) {
          String line = client.readStringUntil('\r');
          line.trim();
          if (line.startsWith("GET")) { // GETしかやってない
            int index = line.indexOf(" ", 5);
            if (index > 0)
              request_line = line.substring(4, index);
            else
              request_line = line.substring(4);
            request_line.trim();
            continue;
          }
          if (line.length() == 0) { // end of request headers.
            Serial.println("request_line = " + request_line);
            Handler_t f = request_handlers[request_line];
            if (!f) {
              if (default_handler)
                default_handler(request_line);
              else
                send_status(404);
            } else
              f();
            client.flush();
          }
      }
      client_connected = NULL;
      client.stop();
    }
  }
};
