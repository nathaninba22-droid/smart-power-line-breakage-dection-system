#include <esp_now.h>
#include <WiFi.h>
int relaypin = 26;
char status2[];
char status3[];
typedef struct struct_message {
  int senderId;
  float voltage[6];
  float current[6];
  float power[6];
} struct_message;

struct_message incomingData;
struct_message Data;

int check()
{
  for (int i = 0; i < 6; i++)
  {
    if (Data.power[i] != 0)
    {
      if(Data.power[i+1]==0 )
      {
        return -i; 
      }
      else
      {
        return i;  
      } 
    }
  }

unsigned long lastSeen2 = 0;
unsigned long lastSeen3 = 0;

const unsigned long TIMEOUT_MS = 3000; // consider offline if no packet in 3s

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  if (incomingData.senderId == 2) lastSeen2 = millis();
  if (incomingData.senderId == 3) lastSeen3 = millis();

  Serial.print("From ESP32 #");
  Serial.println(incomingData.senderId);

  if (incomingData.senderId == 2){
    for(int i=0;i<3;i++){
      Data.power[i]=incomingData.power[i];
    }
    else{
      for(int i=3;i<6;i++){
        Data.power[i]=incomingData.power[i-3];
      }
    }

  }
  
  if(incomingData.senderId == 2){
    if(Data.power[0] != 0||Data.power[1] != 0||Data.power[2] != 0||Data.power[3] != 0||Data.power[4] != 0||Data.power[5] != 0){
        if (check()<0)
        {
          Serial.println("THERE WAS A PROBLEM IN NODE ",check());
        }
        else 
        {
          Serial.println("THERE WAS A WIRE BREAK IN NODE",check());
          digitalWrite(relaypin,LOW);


        }
    }
  }
  Serial.println("-----");
}

void setup() {
  pinMode(relaypin,OUTPUT);
  digitalWrite(relaypin,HIGH);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

  static unsigned long lastCheck = 0;

  // Print status every 2 seconds
  if (millis() - lastCheck > 2000) {
    lastCheck = millis();

    Serial.print("ESP32 #2: ");
    Serial.println((millis() - lastSeen2 < TIMEOUT_MS) ? "ONLINE" : "OFFLINE");
    status2=(millis() - lastSeen2 < TIMEOUT_MS) ? "ONLINE" : "OFFLINE";
    Serial.print("ESP32 #3: ");
    Serial.println((millis() - lastSeen3 < TIMEOUT_MS) ? "ONLINE" : "OFFLINE");
    status3=(millis() - lastSeen3 < TIMEOUT_MS) ? "ONLINE" : "OFFLINE";
    if(status2=="OFFLINE")
    {
      if (status3=="OFFLINE")
      {
        Serial.println("THERE IS A PROBLEM IN MAIN JUNCTION");
      }
      else
      {
        Serial.println("THERE IS A PROBLEM IN JUNCTION 1");
      }
    }
    Serial.println("=====");
  }
}