// pin & chanel motor 1
#define RPWM_1 32
#define RPWM_CH1 0
#define LPWM_1 33
#define LPWM_CH1 1 
 
// pin & chanel motor 2
#define RPWM_2 13 
#define RPWM_CH2 2 
#define LPWM_2 12 
#define LPWM_CH2 3 

// pin & chanel motor 3
#define RPWM_3 25 
#define RPWM_CH3 4 
#define LPWM_3 26 
#define LPWM_CH3 5 

// pin & chanel motor 4
#define RPWM_4 4 
#define RPWM_CH4 6 
#define LPWM_4 27 
#define LPWM_CH4 7 

// pin encoder
#define ENCODER_1a 34
#define ENCODER_1b 14
#define ENCODER_2a 2
#define ENCODER_2b 15
#define ENCODER_3a 39
#define ENCODER_3b 22
#define ENCODER_4a 16
#define ENCODER_4b 17

//pin batre
#define BATTERY 35

const int freq = 5000;
const int resolution = 10; //Resolution 8, 10, 12, 15

const float L = 8.2; // panjang/2 (cm)
const float l = 10.5; // lebar/2 (cm)
const float circ = 18.85; // keliling lingkaran (cm)
const int pulse_per_rev = 330; // Pulse Per motor shaft Revolution

float vw_1 = 0.0;
float vw_2 = 0.0;
float vw_3 = 0.0;
float vw_4 = 0.0;

volatile int pulse_1 = 0;
volatile int pulse_2 = 0;
volatile int pulse_3 = 0;
volatile int pulse_4 = 0;

long currT = 0;
long prevT = 0;
float deltaT = 0.0;

// PID constants
float kp = 1.43;
float ki = 5;
float kd = 0.0;

float e_1 = 0.0;
float e_2 = 0.0;
float e_3 = 0.0;
float e_4 = 0.0;
float dedt_1 = 0.0;
float dedt_2 = 0.0;
float dedt_3 = 0.0;
float dedt_4 = 0.0;
float u_1 = 0.0;
float u_2 = 0.0;
float u_3 = 0.0;
float u_4 = 0.0;


float eprev_1 = 0.0;
float eprev_2 = 0.0;
float eprev_3 = 0.0;
float eprev_4 = 0.0;
float eintegral_1 = 0.0;
float eintegral_2 = 0.0;
float eintegral_3 = 0.0;
float eintegral_4 = 0.0;

void updateEncoder_1(){
  int b = digitalRead(ENCODER_1b);
  if(b > 0){
    pulse_1--;
  }
  else{
    pulse_1++;
  }
}
void updateEncoder_2(){
  int b = digitalRead(ENCODER_2b);
  if(b > 0){
    pulse_2++;
  }
  else{
    pulse_2--;
  }
}
void updateEncoder_3(){
  int b = digitalRead(ENCODER_3b);
  if(b > 0){
    pulse_3--;
  }
  else{
    pulse_3++;
  }
}
void updateEncoder_4(){
  int b = digitalRead(ENCODER_4b);
  if(b > 0){
    pulse_4--;
  }
  else{
    pulse_4++;
  }
}



// ESP NOW
#include <esp_now.h>
#include <WiFi.h>

uint8_t mac_Master[] = {"78:E3:6D:11:09:94"}; // 


typedef struct data {
  String sender = "Red";
  
  float t_vx_Red = 0.0;
  float t_vy_Red = 0.0;
  float vx_Red = 0.0;
  float vy_Red = 0.0;

  float t_vx_Green = 0.0;
  float t_vy_Green = 0.0;
  float vx_Green = 0.0;
  float vy_Green = 0.0;

  float t_vx_Blue = 0.0;
  float t_vy_Blue = 0.0;
  float vx_Blue = 0.0;
  float vy_Blue = 0.0;
} data;

data m_send;
data m_recieve;

void data_receive(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&m_recieve, incomingData, sizeof(m_recieve));
  m_send.vx_Red = m_recieve.t_vx_Red/1.3;
  m_send.vy_Red = m_recieve.t_vx_Red/1.3;
  esp_now_send(mac_Master, (uint8_t *) &m_send, sizeof(m_send));
}











void setup() {
  Serial.begin(115200);

  // buat PWM
  ledcSetup(RPWM_CH1, freq, resolution);
  ledcAttachPin(RPWM_1, RPWM_CH1);
  ledcSetup(LPWM_CH1, freq, resolution);
  ledcAttachPin(LPWM_1, LPWM_CH1);
  ledcSetup(RPWM_CH2, freq, resolution);
  ledcAttachPin(RPWM_2, RPWM_CH2);
  ledcSetup(LPWM_CH2, freq, resolution);
  ledcAttachPin(LPWM_2, LPWM_CH2);
  ledcSetup(RPWM_CH3, freq, resolution);
  ledcAttachPin(RPWM_3, RPWM_CH3);
  ledcSetup(LPWM_CH3, freq, resolution);
  ledcAttachPin(LPWM_3, LPWM_CH3);
  ledcSetup(RPWM_CH4, freq, resolution);
  ledcAttachPin(RPWM_4, RPWM_CH4);
  ledcSetup(LPWM_CH4, freq, resolution);
  ledcAttachPin(LPWM_4, LPWM_CH4);

  // Set encoder as input with internal pullup  
  pinMode(ENCODER_1a, INPUT_PULLUP); 
  pinMode(ENCODER_2a, INPUT_PULLUP); 
  pinMode(ENCODER_3a, INPUT_PULLUP); 
  pinMode(ENCODER_4a, INPUT_PULLUP); 
  
  // Attach interrupt 
  attachInterrupt(digitalPinToInterrupt(ENCODER_1a), updateEncoder_1, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_2a), updateEncoder_2, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_3a), updateEncoder_3, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_4a), updateEncoder_4, RISING);
  
  delay(1000);
  prevT = micros();
}

void loop() {
  // Mencari v_tw
    int t_vw_1 = 70*sin(prevT/1e6);
    int t_vw_2 = 70*sin(prevT/1e6);
    int t_vw_3 = 70*sin(prevT/1e6);
    int t_vw_4 = 70*sin(prevT/1e6);
  
  
  // Preamble
    currT = micros();
    deltaT = ((float) (currT - prevT))/( 1.0e6 );
    prevT = currT; // all in micro second
    
    Serial.print(t_vw_1);
    Serial.print('\t');
    Serial.print(vw_1);
    Serial.print('\t');
    Serial.print(vw_2);
    Serial.print('\t');
    Serial.print(vw_3);
    Serial.print('\t');
    Serial.println(vw_4);
  
  
  // mencari vw
    vw_1 = pulse_1*18.85/(330*deltaT);
    pulse_1 = 0;
    vw_2 = pulse_2*18.85/(330*deltaT);
    pulse_2 = 0;
    vw_3 = pulse_3*18.85/(330*deltaT);
    pulse_3 = 0;
    vw_4 = pulse_4*18.85/(330*deltaT);
    pulse_4 = 0;
  
  
  // PID Control
    // error
    e_1 = t_vw_1 - vw_1;
    e_2 = t_vw_2 - vw_2;
    e_3 = t_vw_3 - vw_3;
    e_4 = t_vw_4 - vw_4;
  
    // derivative
    dedt_1 = (e_1-eprev_1)/(deltaT);
    dedt_2 = (e_2-eprev_2)/(deltaT);
    dedt_3 = (e_3-eprev_3)/(deltaT);
    dedt_4 = (e_4-eprev_4)/(deltaT);
  
    // integral
    eintegral_1 = eintegral_1 + e_1*deltaT;
    eintegral_2 = eintegral_2 + e_2*deltaT;
    eintegral_3 = eintegral_3 + e_3*deltaT;
    eintegral_4 = eintegral_4 + e_4*deltaT;
  
    // control signal
    u_1 = kp*e_1 + kd*dedt_1 + ki*eintegral_1;
    u_2 = kp*e_2 + kd*dedt_2 + ki*eintegral_2;
    u_3 = kp*e_3 + kd*dedt_3 + ki*eintegral_3;
    u_4 = kp*e_4 + kd*dedt_4 + ki*eintegral_4;
    
    // store previous error
    eprev_1 = e_1;
    eprev_2 = e_2;
    eprev_3 = e_3;
    eprev_4 = e_4;
  

  // motor power
    if(u_1>0){
      ledcWrite(RPWM_CH1, u_1*7.54+109);
      ledcWrite(LPWM_CH1, 0);
    }
    else if(u_1<0){
      ledcWrite(RPWM_CH1, 0);
      ledcWrite(LPWM_CH1, -u_1*7.54+109);
    }
    else{
      ledcWrite(RPWM_CH1, 0);
      ledcWrite(LPWM_CH1, 0);
    }
    
    if(u_2>0){
      ledcWrite(RPWM_CH2, u_2*7.54+109);
      ledcWrite(LPWM_CH2, 0);
    }
    else if(u_2<0){
      ledcWrite(RPWM_CH2, 0);
      ledcWrite(LPWM_CH2, -u_2*7.54+109);
    }
    else{
      ledcWrite(RPWM_CH2, 0);
      ledcWrite(LPWM_CH2, 0);
    }
    
    if(u_3>0){
      ledcWrite(RPWM_CH3, u_3*7.54+109);
      ledcWrite(LPWM_CH3, 0);
    }
    else if(u_3<0){
      ledcWrite(RPWM_CH3, 0);
      ledcWrite(LPWM_CH3, -u_3*7.54+109);
    }
    else{
      ledcWrite(RPWM_CH3, 0);
      ledcWrite(LPWM_CH3, 0);
    }
    
    if(u_4>0){
      ledcWrite(RPWM_CH4, u_4*7.54+109);
      ledcWrite(LPWM_CH4, 0);
    }
    else if(u_4<0){
      ledcWrite(RPWM_CH4, 0);
      ledcWrite(LPWM_CH4, -u_4*7.54+109);
    }
    else{
      ledcWrite(RPWM_CH4, 0);
      ledcWrite(LPWM_CH4, 0);
    }
  
  
  delay(100);
}
