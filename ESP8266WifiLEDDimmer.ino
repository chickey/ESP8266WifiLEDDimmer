/*
 * Code base origionally found at https://eryk.io/2016/02/12/esp8266-wifi-led-dimmer/
 * 
 * Colin Hickey
 * Code has been modified to include some serial output and supporting two LED's connected on pin 0 and pin 2
 * 
 * Format of the request is now http://x.x.x.x/led/0/100 or http://x.x.x.x/led/1/100
 * 
 */
 
 
 
#include <ESP8266WiFi.h>

void LED_CHANGE();

// LED FET
#define ledPIN0    0
#define ledPIN2    2

int led_delay = 0;
#define time_at_colour 1200 
unsigned long TIME_LED = 0;
int LED1;
int LED2;
 
int LED_A = 0;
int LED_B = 0;

// WiFi Settings
const char *ssid =  "SSID";
const char *pass =  "password";

// Start WiFi Server
WiFiServer server(80);

void setup()
{
  
  pinMode(ledPIN0, OUTPUT); 
  pinMode(ledPIN2, OUTPUT);   
  
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("");
  
  server.begin();
  Serial.println("Webserver started");
  
}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (client) {
    
      // Read the first line of the request
      String req = client.readStringUntil('\r');
      Serial.println(req);
      client.flush();
    
      // Prepare the response. Start with the common header:
      String s = "HTTP/1.1 200 OK\r\n";
      s += "Content-Type: text/html\r\n\r\n";
      s += "<!DOCTYPE HTML>\r\n<html>\r\n";
      
      if((req.indexOf("/led/0/") != -1)) { //test for servo control sring
         
        // get RGB value
        int pos = req.length();
        int ind1 = req.indexOf("/led/0/") + 7;

        String teststring = req.substring(ind1, pos);
        int ind2 = teststring.indexOf('/');
        String ledValue1 = req.substring(ind1, ind2+ind1);
        
        int ledVal = map(ledValue1.toInt(),0,100,0,1023);
        LED1 = constrain(ledVal,0,1023);
        
        int diff_led = abs(LED1-LED_A);
        if(diff_led > 0){
          led_delay = time_at_colour / abs(LED1-LED_A); 
        }else{
          led_delay = time_at_colour / 100; 
        }
        
        // confirm to client
        s += "LED 1 value changed to ";
        s += ledValue1;
        s += "<br>";
        Serial.println("LED 1 value changed to " + String(ledValue1));
      }
      if ((req.indexOf("/led/1/") != -1)) { //test for servo control sring
         
        // get RGB value
        int pos = req.length();
        int ind1 = req.indexOf("/led/1/") + 7;
        String teststring = req.substring(ind1, pos);
        int ind2 = teststring.indexOf('/');
        String ledValue2 = req.substring(ind1, ind2+ind1);
        
        int ledVal = map(ledValue2.toInt(),0,100,0,1023);
        LED2 = constrain(ledVal,0,1023);
        
        int diff_led = abs(LED2-LED_B);
        if(diff_led > 0){
          led_delay = time_at_colour / abs(LED2-LED_B); 
        }else{
          led_delay = time_at_colour / 100; 
        }
        
        // confirm to client
        s += "LED 2 value changed to ";
        s += ledValue2;
        s += "<br>";
        Serial.println("LED 2 value changed to " + String(ledValue2));
      
      }
      
      s += "<br>";
      s += "Request OK!";
      
      s += "</html>\n";
     
      client.flush();
    
      // Send the response to the client
      client.print(s);
      //delay(1);
      Serial.println("Client disonnected");
    
      // The client will actually be disconnected 
      // when the function returns and 'client' object is detroyed
      
  }
  
  if(millis() - TIME_LED >= led_delay){
    TIME_LED = millis();
    LED_CHANGE();
  }
}

void LED_CHANGE()
{
  if(LED1 != LED_A){
    if(LED_A > LED1) LED_A = LED_A - 1;
    if(LED_A < LED1) LED_A++;
    analogWrite(ledPIN0, LED_A);
  }  
  if (LED2 != LED_B){
    if(LED_B > LED2) LED_B = LED_B - 1;
    if(LED_B < LED2) LED_B++;
    analogWrite(ledPIN2, LED_B);
  }
}
