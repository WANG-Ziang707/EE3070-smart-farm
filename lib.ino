#include "WiFiEsp.h"
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include "EE3070.h"

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiEspClient  client;

//------------------------------------------
int humidity = 0;
int brightness = 0;
int PH = 0;
int temperature = 0;
int waterflow = 0;
int distance = 0;

int dark = 50;
int dry = 300;
int sDistance = 10;

PUMP water(dry);
LIGHT light(dark);
ALARM alarm(sDistance);
//-----------------------------------------

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#define ESP_BAUDRATE  115200
#else
#define ESP_BAUDRATE  115200
#endif

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int number1 = 0;
int number2 = 0;
int number3 = 0;
int number4 = 0;

String myStatus = "";

void setup() {
  //Initialize serial and wait for port to open
  Serial.begin(115200);  // Initialize serial
  while(!Serial){
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  // initialize serial for ESP module  
  setEspBaudRate(ESP_BAUDRATE);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  Serial.print("Searching for ESP8266..."); 
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  Serial.println("found it!");
   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
}

void loop() {
  //---------------
// put your main code here, to run repeatedly:
  distance = alarm.distance();
  humidity = water.humidity();
  brightness = light.brightness();
  PH = water.PH();
  temperature = water.temperature();
  waterflow = water.waterflow(); // store the data from the sensors

   // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(500);     
    } 
    Serial.println("\nConnected.");
  }

  
  if(water.dry()){
    water.on();
  }
  else{
    water.off();
  }
  if(light.dark()){
    light.on();
  }
  else{
    light.off();
  }
  
  if(alarm.warning(distance)){ 
    alarm.on();
    alarm.statusLED(distance);
  }
  else{
    alarm.off();
    alarm.statusLED(distance);
  }
  
  //---------------------

number1 = temperature;
number2 = 1024- humidity;
number3 = brightness;
number4 = distance;

Serial.println(temperature);
Serial.println(1024- humidity);
Serial.println(brightness);



  // set the fields with the values
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);
  ThingSpeak.setField(3, number3);
  ThingSpeak.setField(4, number4);

  // figure out the status message
  if(number2 > 300){
    myStatus = String("soild is dry"); 
     
  ThingSpeak.setStatus(myStatus);
  }
   if(number1 < 10){
    myStatus = String("too cold");
  ThingSpeak.setStatus(myStatus);
  }
  else if(number1 > 35){
    myStatus = String("too hot");
  ThingSpeak.setStatus(myStatus);
  }
  if(number3 < 50){
    myStatus = String("too dark");
  ThingSpeak.setStatus(myStatus);
    }
  
 
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

}

// This function attempts to set the ESP8266 baudrate. Boards with additional hardware serial ports
// can use 115200, otherwise software serial is limited to 19200.
void setEspBaudRate(unsigned long baudrate){
  long rates[6] = {115200,74880,57600,38400,19200,9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for(int i = 0; i < 6; i++){
    Serial1.begin(rates[i]);
    delay(1);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(1);  
  }
    
  Serial1.begin(baudrate);
}
