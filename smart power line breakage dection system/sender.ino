#include <esp_now.h>
#include <WiFi.h>

#define SENDER_ID 2   // change to 3 on the other board

// Replace with ESP32 #1's actual MAC address
uint8_t receiverMac[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};

typedef struct struct_message {
  int senderId;
  float voltage[3];
  float current[3];
  float power[3];
} struct_message;

struct_message myData;

// Adjust these pins to your actual sensor wiring
const int voltagePins[3] = {34, 35, 32};
const int currentPins[3] = {33, 25, 26};

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  myData.senderId = SENDER_ID;

  for (int i = 0; i < 3; i++) {
    float rawV = analogRead(voltagePins[i]);
    float rawI = analogRead(currentPins[i]);

    // Replace with your actual sensor calibration formulas
    myData.voltage[i] = rawV * (3.3 / 4095.0) * VOLTAGE_SCALE_FACTOR;
    myData.current[i] = rawI * (3.3 / 4095.0) * CURRENT_SCALE_FACTOR;
    myData.power[i]   = myData.voltage[i] * myData.current[i];
  }

  esp_now_send(receiverMac, (uint8_t *)&myData, sizeof(myData));
  delay(1000); // send every second
}