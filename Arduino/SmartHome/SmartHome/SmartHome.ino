#include <SPI.h>
#include <Ethernet.h>

// Set the MAC address
byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x66, 0x2D};

// Set the IP address
IPAddress ip(192, 168, 0, 122);

// Start a server at port 80 (http)
EthernetServer server(80);

const int ldrPin = A0;

const int ldrLed = 8;

void setup() {
  // Open serial communications
  Serial.begin(9600);

  // start the Ethernet connection and the server
  Ethernet.begin(mac, ip);
  server.begin();

  // Pin 2 - 7 output (leds)
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  //LDR ports
  pinMode(ldrLed, OUTPUT);
  pinMode(ldrPin, INPUT);
}


void loop() {
  // Verify status of LDR and act accordingly
  ldr();
  
  // Check if client connected
  EthernetClient client = server.available();
  
  if (client) { // If there is a client...
    boolean currentLineIsBlank = true;
    String buffer = ""; // A buffer for the GET request
    
    while (client.connected()) {

      if (client.available()) {
        char c = client.read();// Read the data of the client
        buffer += c; // Store the data in a buffer
        
        if (c == '\n' && currentLineIsBlank) {// if 2x new line ==> Request ended
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println(); // Blank line ==> end response
          break;
        }
        if (c == '\n') { // if New line
          currentLineIsBlank = true;
          buffer = "";  // Clear buffer
        } else if (c == '\r') { // If cariage return...
          //Read in the buffer if there was send "GET /?..."
          if(buffer.indexOf("GET /?led1=1")>=0) { // If led1 = 1
            digitalWrite(2, HIGH); // led 1 > on
          }
          if(buffer.indexOf("GET /?led1=0")>=0) { // If led1 = 0
            digitalWrite(2, LOW); // led 1 > off
          }
          if(buffer.indexOf("GET /?led2=1")>=0) { // If led2 = 1
            digitalWrite(3, HIGH); // led 2 > on
          }
          if(buffer.indexOf("GET /?led2=0")>=0) { // If led2 = 0
            digitalWrite(3, LOW); // led 2 > off
          }
          if(buffer.indexOf("GET /?led3=1")>=0) { // If led3 = 1
            digitalWrite(5, HIGH); // led 3 > on
          }
          if(buffer.indexOf("GET /?led3=0")>=0) { // If led3 = 0
            digitalWrite(5, LOW); // led 3 > off
          }
          if(buffer.indexOf("GET /?led4=1")>=0) { // If led3 = 1
            digitalWrite(6, HIGH); // led 4 > on
          }
          if(buffer.indexOf("GET /?led4=0")>=0) { // If led3 = 0
            digitalWrite(6, LOW); // led 4 > off
          }
          if(buffer.indexOf("GET /?led5=1")>=0) { // If led3 = 1
            digitalWrite(7, HIGH); // led 5 > on
          }
          if(buffer.indexOf("GET /?led5=0")>=0) { // If led3 = 0
            digitalWrite(7, LOW); // led 5 > off
          }
        } else {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }
}

void ldr(){
  int ldrValue = analogRead(ldrPin);
  if(ldrValue <= 200){
    digitalWrite(ldrLed, HIGH);
  }
  else{
    digitalWrite(ldrLed, LOW);
  }
  delay(1);
}
