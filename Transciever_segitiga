#define sensorX 34
#define sensorY 35

int x = 0;
int y = 0;
int i = 0;

int prevT = micros();

// ESP NOW
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {"78:E3:6D:11:09:94"}; // 


typedef struct struct_message {
  String sender = "Transciever";
  
  float t_vx_red;
  float t_vy_red;
  float vx_red = 0.0;
  float vy_red = 0.0;

  float t_vx_green;
  float t_vy_green;
  float vx_green = 0.0;
  float vy_green = 0.0;

  float t_vx_blue;
  float t_vy_blue;
  float vx_blue = 0.0;
  float vy_blue = 0.0;
} struct_message;

struct_message ds; // data send
struct_message dr_red; // data recieved
struct_message dr_green;
struct_message dr_blue;

esp_now_peer_info_t peerInfo;








// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  // if (status ==0){
  //   success = "Delivery Success :)";
  // }
  // else{
  //   success = "Delivery Fail :(";
  // }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&dr_red, incomingData, sizeof(dr_red));
}

void setup() {
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  for (i=0; i<30; i++){
    ds.t_vx_red = 0;
    ds.t_vy_red = 70;
    
    esp_now_send(broadcastAddress, (uint8_t *) &ds, sizeof(ds));
    
    Serial.print(dr_red.vx_red);
    Serial.print('\t');
    Serial.print(dr_red.vy_red);
    Serial.print('\t');
    Serial.print(dr_green.vx_green);
    Serial.print('\t');
    Serial.print(dr_green.vy_green);
    Serial.print('\t');
    Serial.print(dr_blue.vx_blue);
    Serial.print('\t');
    Serial.println(dr_blue.vy_blue);
    delay(100);
  }
  for (i=0; i<30; i++){
    ds.t_vx_red = 70;
    ds.t_vy_red = 0;
    
    esp_now_send(broadcastAddress, (uint8_t *) &ds, sizeof(ds));
    
    Serial.print(dr_red.vx_red);
    Serial.print('\t');
    Serial.print(dr_red.vy_red);
    Serial.print('\t');
    Serial.print(dr_green.vx_green);
    Serial.print('\t');
    Serial.print(dr_green.vy_green);
    Serial.print('\t');
    Serial.print(dr_blue.vx_blue);
    Serial.print('\t');
    Serial.println(dr_blue.vy_blue);
    delay(100);
  }
  for (i=0; i<30; i++){
    ds.t_vx_red = 0;
    ds.t_vy_red = -70;
    
    esp_now_send(broadcastAddress, (uint8_t *) &ds, sizeof(ds));
    
    Serial.print(dr_red.vx_red);
    Serial.print('\t');
    Serial.print(dr_red.vy_red);
    Serial.print('\t');
    Serial.print(dr_green.vx_green);
    Serial.print('\t');
    Serial.print(dr_green.vy_green);
    Serial.print('\t');
    Serial.print(dr_blue.vx_blue);
    Serial.print('\t');
    Serial.println(dr_blue.vy_blue);
    delay(100);
  }
  for (i=0; i<30; i++){
    ds.t_vx_red = -70;
    ds.t_vy_red = 0;
    
    esp_now_send(broadcastAddress, (uint8_t *) &ds, sizeof(ds));
    
    Serial.print(dr_red.vx_red);
    Serial.print('\t');
    Serial.print(dr_red.vy_red);
    Serial.print('\t');
    Serial.print(dr_green.vx_green);
    Serial.print('\t');
    Serial.print(dr_green.vy_green);
    Serial.print('\t');
    Serial.print(dr_blue.vx_blue);
    Serial.print('\t');
    Serial.println(dr_blue.vy_blue);
    delay(100);
  }
}
