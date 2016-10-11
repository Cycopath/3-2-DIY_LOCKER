#include <Keypad.h>
#include <Servo.h>
#include "ESP8266.h"

#define SSID "iptime-dblab"
#define APPWD "0987654321"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(12,13);
ESP wifi(mySerial);

Servo ServoMotor;
char* password = "159";
int position = 0;
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = 
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {2,3,4,5};
byte colPins[COLS] = {6,7,8};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup(void) {
  ServoMotor.attach(11);
  LockedPosition(true);
  Serial.begin(9600);  
  Serial.print("setup begin\r\n");  
      
  Serial.print("FW Version:");  
  Serial.println(wifi.getVersion().c_str());  
        
  if (wifi.setOprToStationSoftAP()) {  
      Serial.print("to station + softap ok\r\n");  
   }
   else {  
       Serial.print("to station + softap err\r\n");  
   }  
   
   if (wifi.joinAP(SSID, APPWD)) {  
       Serial.print("Join AP success\r\n");  
       Serial.print("IP: ");  
       Serial.println(wifi.getLocalIP().c_str());      
    }
    else {  
       Serial.print("Join AP failure\r\n");  
    }  
      
    if (wifi.enableMUX()) {  
        Serial.print("multiple ok\r\n");  
    } else {  
        Serial.print("multiple err\r\n");  
    }  
      
    if (wifi.startTCPServer(8090)) {  
        Serial.print("start tcp server ok\r\n");  
    } else {  
        Serial.print("start tcp server err\r\n");  
    }  
      
    if (wifi.setTCPServerTimeout(10)) {   
        Serial.print("set tcp server timout 10 seconds\r\n");  
    } else {  
        Serial.print("set tcp server timout err\r\n");  
    }  
      
    Serial.print("setup end\r\n");  
  
    pinMode( 11, OUTPUT );    
  
}

void loop()
{
    uint8_t buffer[128] = {0};  
    uint8_t mux_id;  
    uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);  
    if (len > 0) {  
        Serial.print("Status:[");  
        Serial.print(wifi.getIPStatus().c_str());  
        Serial.println("]");  
          
        Serial.print("Received from :");  
        Serial.print(mux_id);  
        Serial.print("[");  
        for(uint32_t i = 0; i < len; i++) {  
            Serial.print((char)buffer[i]);  
        }  
        Serial.print("]\r\n");  
          
        if ( buffer[0] == '1' ) LockedPosition(false);    
        else if ( buffer[0] == '2' ) LockedPosition(true);   
          
   }  
  char key = keypad.getKey();
  if(key == '*' || key == '#') {
    position = 0;
    LockedPosition(true);
  }
  if(key == password[position]){
    position++;
  }
  if(position == 3){
    LockedPosition(false);
  }
  delay(100);
}
void LockedPosition(int locked)
{
  if (locked)
  {
    ServoMotor.write(11);
  }
  else
  {
    ServoMotor.write(90);
  }
}  
