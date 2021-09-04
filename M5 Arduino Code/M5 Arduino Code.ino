#include <M5Core2.h>

#define RXD2 13
#define TXD2 14

bool flag = false;
bool batFlag = true;
String label[3] = { "Car", "Motorcycle", "Pedestrian" };

void disp(int battery);

void setup() {
  M5.begin();
  SD.begin();
  M5.Lcd.setTextSize(4);
  M5.Lcd.fillScreen(WHITE);
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8E2, RXD2, TXD2);
}

// the loop routine runs over and over again forever:
void loop() {
  float batVoltage = M5.Axp.GetBatVoltage();
  int batPercentage = int(( batVoltage < 3.2 ) ? 0 : ( batVoltage - 3.2 ) * 100);
  disp(batPercentage);
  if (Serial2.available()) {
    String serial = Serial2.readString();
    if (serial.indexOf("Start") != -1) {  // Filter Noise from ESP32 Cam Module Sleep
      flag = true;
    } else if (flag) {
      serial = serial.substring(serial.indexOf("Star") + 4, serial.indexOf("Stop"));
      float array[3] = { serial.substring(0, serial.indexOf(",")).toFloat(), serial.substring(serial.indexOf(",") + 1, serial.indexOf(",", serial.indexOf(",") + 1)).toFloat(), serial.substring(serial.indexOf(",", serial.indexOf(",") + 1) + 1).toFloat() };
      int place = 0;
      Serial.println(array[0]);
      Serial.println(array[1]);
      Serial.println(array[2]);
      if (array[1] > array[0]) {
        if (array[2] > array[1]) {
          place = 2;
        } else {
          place = 1;
        }
      } else {
        if (array[2] > array[0]) {
          place = 2;
          
        }
      }
      if(place ==0){
M5.Lcd.drawJpgFile(SD, "/car.jpg", 0, 0);
      }
            else if(place ==1){
        M5.Lcd.drawJpgFile(SD, "/mot.jpg", 0, 0);
      }
else{
      M5.Lcd.drawJpgFile(SD, "/ped.jpg", 0, 0);  
      }
      Serial.println(label[place]);
      batFlag = true;
      flag = false;
    } else {
      flag = false;
    }
  }
}

void disp(int battery) {
static int batMem;
if (batMem != battery || batFlag) {
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(20, 5);
    M5.Lcd.println("Battery:"+String(batMem) + "%");
    M5.Lcd.setTextColor(BLACK);
    batMem = battery;
    M5.Lcd.setCursor(20, 5);
    M5.Lcd.println("Battery:"+String(battery) + "%");
    batFlag = false;
  }
}
