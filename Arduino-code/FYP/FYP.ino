#include <PZEM004Tv30.h>
#include <math.h>

#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17
#endif

#if !defined(PZEM_SERIAL)
#define PZEM_SERIAL Serial2
#endif

#if defined(ESP32)
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
#elif defined(ESP8266)
//PZEM004Tv30 pzem(Serial1);
#else
PZEM004Tv30 pzem(PZEM_SERIAL);
#endif
#include <FirebaseESP32.h>
#include <WiFi.h>

// define hardaware pins 
#define speedPin 5
#define fwd_relay_pin 18
#define rev_relay_pin 19

// declare network and database credentials
#define WIFI_SSID "Srishtik" // your wifi SSID
#define WIFI_PASSWORD "Sris2310" //your wifi PASSWORD
#define FIREBASE_HOST "https://toggle-single-22dec-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "y5q05hcU3EC3LH7WZD5lyTS5hLBU5uBFdpbchJB9"  // your private key


// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 12;

// function to set speed
void set_speed(){
  FirebaseData firebaseData;
  if(Firebase.getString(firebaseData, "/Motor/Speed"))
  {
    int speedData = firebaseData.intData();
    if (speedData == 0){
      ledcWrite(ledChannel, 4095);
    }
    else{
      float dutyCycle = (3950 - (speedData * 2.633));
      Serial.println("Speed: ");
      Serial.println(speedData);
      ledcWrite(ledChannel, dutyCycle);
    }                                                                                                                                                                                                                                                
  }
  else{
    Serial.print("Error in getInt, ");
    Serial.println(firebaseData.errorReason());
  } 
}

// function to set direction
void set_direction(){
  FirebaseData firebaseData;
  if(Firebase.getString(firebaseData, "/Motor/Direction")){
    int directionData = firebaseData.intData();
    if (directionData == 1){
      digitalWrite(rev_relay_pin, LOW);
      digitalWrite(fwd_relay_pin, HIGH);
    }    
    else if(directionData == 0){
      digitalWrite(fwd_relay_pin, LOW);
      digitalWrite(rev_relay_pin, HIGH);                                                                                                                                                                                                                              
    }
  }
  else{
    Serial.print("Error in getInt, ");
    Serial.println(firebaseData.errorReason());
  } 
}

// function to send sensor data values
void send_sensor_value(){
   FirebaseData firebasedata;
   FirebaseJson json;
   float Voltage = pzem.voltage();
   float Current = pzem.current();
   float frequency = pzem.frequency();
   float power_active = pzem.power();
   float pf = pzem.pf();
   float Energy = pzem.energy();
   float phi = acos(pf);
   float power_reactive = Voltage*Current*sin(phi);
   
   json.set("/Voltage", Voltage);
   json.set("/Current", Current);
   json.set("/Energy", Energy);
   json.set("/pf", pf);
   json.set("/frequency", frequency);
   json.set("/power_active", power_active);
   json.set("/power_reactive", power_reactive);
   Firebase.updateNode(firebasedata,"/PZEM004T",json);
}

void setup ()
{
  pinMode(speedPin, OUTPUT);
  pinMode(fwd_relay_pin, OUTPUT);
  pinMode(rev_relay_pin, OUTPUT);
  Serial.begin(115200);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ") ;
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);   

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(speedPin, ledChannel);
  pzem.resetEnergy();
  
}

void loop() {   
  set_direction();
  set_speed();
  send_sensor_value();
}
