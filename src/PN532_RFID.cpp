/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/friedlbasson/Desktop/VSC/BL_RFID3/src/PN532_RFID.ino"
void setup();
void End_Point();
void myHandler(const char *event, const char *data);
void EPD();
void I2C_Scanner();
void RealTime(void);
void ApiParser_event1(void);
void ApiParser_event2(void);
void RFID ();
void Fuel_Gauge();
void loop();
#line 1 "/Users/friedlbasson/Desktop/VSC/BL_RFID3/src/PN532_RFID.ino"
SYSTEM_MODE(SEMI_AUTOMATIC);
//SYSTEM_THREAD(ENABLED);

//SerialLogHandler logHandler(LOG_LEVEL_TRACE);

#include <SPI.h>
#include "../lib/EPD4_2/src/epd4in2b_V2.h"
#include "../lib/EPD4_2/src/imagedata.h"
#include "../lib/EPD4_2/src/epdpaint.h"

#include "../lib/PN532/src/DFRobot_PN532.h"
#include "../lib/MAX17201/src/max1720x.h"

////--  EPD Declarations --/////

#define COLORED     0
#define UNCOLORED   1

int new_end_point_STATE = 0;
int current_end_point_STATE = 0;

unsigned long previousMillis1 = 0;  
unsigned long currentMillis1 = 0;
unsigned epaper_update_interval = 180000; //3600000

unsigned long previous_Publish_Millis = 0;   
unsigned long current_Publish_Millis = 0;
unsigned battery_level_interval = 120000; 

////--  RFID Declarations --/////

char testval[1200];
char *array[25];
String EvendId;
String ReaderMode;
String TimeStamp;
String Identifier;
String rfid_uid = "";
String hhmmss;
String data_time;

struct myStructure{

String id;
String reader_id;
String readerMode;
String roomName;
String title;
String startTime;
String endTime;
String timezone;
String timeZoneOffset;
String instructorFirstName;
String instructorLastName;
String isHybrid;

}ApiData1, ApiData2;

#define  BLOCK_SIZE       10     
#define  PN532_IRQ        2
#define  POLLING          0
#define  INTERRUPT        1

#define  READ_BLOCK_NO    2       // The block to be read

DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);
uint8_t dataRead[16] = {0}; 

/////--  MAX17201 Fuel Gauge declarations --/////
char msgFG[64];
max1720x gauge;
int SOC;
int V;

/////--  OTHER declarations --/////

#define HALO D11          // AUX Pin - Master On/Off button
#define BATON D22         // Battery Connect pin
#define BUZZER D9  
#define LDO_EN D23        // EN Pin for GDM regulator

char data[10] = "data";   //default data

int Handler_STATE = 0;

void setup() {

  Serial.begin(115200);

  Particle.subscribe("hook-response/get_data", myHandler, MY_DEVICES);
  Particle.setDisconnectOptions(CloudDisconnectOptions().graceful(true).timeout(5s));

  pinMode(BATON, OUTPUT);
  digitalWrite(BATON,HIGH);
  pinMode (BUZZER, OUTPUT);
  digitalWrite (BUZZER, LOW);
  pinMode(LDO_EN, OUTPUT);
  digitalWrite(LDO_EN, HIGH);

  gauge.reset();                                  // Resets MAX1720x

// Initialise RFID --- START
  Serial.println();
  Serial.print("Initializing PN532");
  while (!nfc.begin()) {
    Serial.print(".");
    delay (100);
 }
  // Serial.println();                              // DEBUG -- remove later
  // Serial.println("Waiting for a card......");    // DEBUG -- remove later
  // Initialise RFID --- END

//I2C_Scanner();                                  // RUN I2C DIAGNOSTICS

}

void End_Point() {

  Serial.println("Calling End Point function..."); 
      uint32_t freemem = System.freeMemory();
      Serial.print("free memory: ");
      Serial.println(freemem);

   if (Particle.connected() == false) {
      Serial.println("IF Statement executing");
      digitalWrite(LDO_EN, HIGH);
      delay(1000);
      Cellular.on();
      waitUntil(Cellular.isOn);
      Particle.connect();
      waitUntil(Particle.connected);
      
      Particle.publish("get_data", data, PRIVATE);
      new_end_point_STATE = 1;  
      }                           
}

void myHandler(const char *event, const char *data) {

// Split the string from API
    strcpy(testval, (const char *)data);
    int i = 0;
    char *p = strtok (( char *)testval, "|");

    while (p != NULL)
        {
            array[i++] = p;
            p = strtok (NULL, "|");
        }
    
    ApiParser_event1();
    Serial.print("\n"); 
    Serial.print("\n"); 
    ApiParser_event2();
    RealTime();
    delay(50);

    Handler_STATE = 1;
    new_end_point_STATE = 2;
}

void EPD() {

  Epd epd;
  Serial.print("Updating E-Paper Display\r\n ");

 if (epd.Init() != 0) { 
   Serial.print("Failed to initialize display");
   return;
   }

/* This clears the SRAM of the e-paper display */
  epd.ClearFrame();

  unsigned char image[50000];
  Paint paint(image, 400, 296);    //width should be the multiple of 8
 
  // Print BLACK text
  paint.Clear(UNCOLORED);
  paint.SetRotate(2);
  
    paint.DrawStringAt(10, 5, "TITLE", &Font24, COLORED);
    paint.DrawStringAt(10, 145, "Start ", &Font20, COLORED);
    paint.DrawStringAt(10, 165, "Time", &Font20, COLORED);
    paint.DrawStringAt(115, 165, String(ApiData2.startTime), &Font20, COLORED);
    paint.DrawStringAt(205, 145, "End   ", &Font20, COLORED);
    paint.DrawStringAt(205, 165, "Time", &Font20, COLORED);
    paint.DrawStringAt(310, 165, String(ApiData2.endTime), &Font20, COLORED);
  
    paint.DrawFilledRectangle(10, 35, 100, 120, COLORED);
    paint.DrawFilledRectangle(10, 27, 110, 27, COLORED);
    paint.DrawFilledRectangle(110, 27, 110, 120, COLORED);
    paint.DrawFilledRectangle(110, 120, 395, 120, COLORED);

    paint.DrawFilledRectangle(10, 140, 190, 140, COLORED);
    paint.DrawFilledRectangle(10, 185, 190, 185, COLORED);
    paint.DrawFilledRectangle(190, 140, 190, 185, COLORED);
    paint.DrawFilledRectangle(200, 140, 200, 185, COLORED);
    paint.DrawFilledRectangle(200, 185, 390, 185, COLORED);
    paint.DrawFilledRectangle(200, 140, 390, 140, COLORED);

    paint.DrawFilledRectangle(205, 185, 390, 185, COLORED);
    paint.DrawRectangle(10, 230, 260, 290, COLORED);
    paint.DrawRectangle(280, 200, 390, 290, COLORED);
    
    paint.DrawFilledRectangle(280, 200, 390, 222, COLORED);                                   // battery background
    paint.DrawStringAt(285, 205, "BATTERY %", &Font16, UNCOLORED);                            // Battery Title
    paint.DrawStringAt(320, 227, String(SOC) + "%", &Font16, COLORED);                        // SOC

    paint.DrawFilledRectangle(280, 244, 390, 266, COLORED);                                   // Time Zone background
    paint.DrawStringAt(285, 249, "Time Zone", &Font16, UNCOLORED);                            // Time Zone background
    paint.DrawStringAt(305, 271, String(ApiData1.timezone), &Font16, COLORED);                // GMT
    
    paint.DrawFilledRectangle(10, 230, 260, 200, COLORED);                                    // instructor background
    paint.DrawStringAt(15, 210, "INSTRUCTOR:", &Font20, UNCOLORED);                           // instructor Title
    paint.DrawStringAt(15, 240, String(ApiData1.instructorFirstName), &Font20, COLORED);      // instructor Title
    paint.DrawStringAt(15, 260, String(ApiData1.instructorLastName), &Font20, COLORED);       // instructor Title
    
     epd.SetPartialWindowBlack(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  //Print RED text
  paint.Clear(UNCOLORED);
  paint.SetRotate(2);
  
    paint.DrawStringAt(130, 5, "Starship IFT3 - ", &Font24, COLORED);
    paint.DrawStringAt(130, 35, "What to expect ", &Font24, COLORED);
    paint.DrawStringAt(130, 65, "from the next ", &Font24, COLORED);
    paint.DrawStringAt(130, 95, "flight test", &Font24, COLORED);
    epd.SetPartialWindowRed(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());

  epd.DisplayFrame();         // /* This displays the data from the SRAM in e-Paper module */
  delay(50);
  //epd.ClearFrame(); 
  
  epd.Sleep();                /* Deep sleep */
  new_end_point_STATE = 0;

  /* This displays an image */
  //epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);
  //epd.DisplayFrame(QR_BLACK, QR_RED);
}

void I2C_Scanner() {

// byte error, address;
// int nDevices;
 
// delay(1000);
 
//   nDevices = 0;
  
//   for(address = 1; address < 127; address++ )
//   { 
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
 
//     if (error == 0)
//     {
//       Serial.print("I2C device found at address 0x");
//       Serial.println(address,HEX);
//       nDevices++;
//     }
    
//       else if (error==4)
//         {
//           if (address<16)
//           Serial.print("0");
//           Particle.publish(String(address,HEX), PRIVATE);
//     }    
//   }
  
//   if (nDevices == 0)
//     Serial.println("No I2C devices found");
//       else
//         Serial.println("Done");
// }

//void RealTime(void){

// Time.zone(atoi(ApiData2.timeZoneOffset));             /* ad*/

//     Serial.print(Time.hourFormat12(Time.now()));      /* print serially current hour*/
//     Serial.print(":");
//     Serial.print(Time.minute(Time.now()));            /* print serially current minute*/
//     Serial.print(":");
//     Serial.print(Time.second(Time.now()));            /* print serially second hour*/
//     if(!Time.isPM())
//         Serial.println(" AM");
//     else
//         Serial.println(" PM");
//         Serial.println("\n");
//         Serial.println("\n");
}

void RealTime(void){

  Time.zone(atoi(ApiData1.timeZoneOffset));
  Time.zone(atoi(ApiData2.timeZoneOffset));

  String hour = String(Time.hourFormat12(Time.now()));
  String minute = String::format("%02i",Time.minute(Time.now()));
  String second = String::format("%02i",Time.second(Time.now()));
   
  String day = String(Time.day());
  String month = String(Time.month());
  String year = String(Time.year());

    if(!Time.isPM()){
        hhmmss = hour + ":" + minute + ":" + second +"AM";
          } else {
            hhmmss = hour + ":" + minute + ":" + second +"PM";
          }
            data_time = year + "-" + month + "-" + day + "T" + hhmmss;
 }

void ApiParser_event1(void){                  //store all sperate data from API into the struct variable

//ApiData.id = array[0];
  ApiData1.readerMode = array[1];
  ApiData1.roomName = array[2];
  ApiData1.id = array[3];
  ApiData1.title = array[4];
  ApiData1.startTime = array[5];
  ApiData1.endTime = array[6];
  ApiData1.timezone = array[7];
  ApiData1.timeZoneOffset = array[8];
  ApiData1.instructorFirstName = array[9];
  ApiData1.instructorLastName = array[10];
  ApiData1.isHybrid = array[11];

}

void ApiParser_event2(void){                  //store all sperate data from API into the struct variable

    //ApiData.id = array[0];
    ApiData2.readerMode = array[10];
    ApiData2.roomName = array[2];
    ApiData2.id = array[12];
    ApiData2.title = array[13];
    ApiData2.startTime = array[14];
    ApiData2.endTime = array[15];
    ApiData2.timezone = array[16];
    ApiData2.timeZoneOffset = array[17];
    ApiData2.instructorFirstName = array[18];
    ApiData2.instructorLastName = array[19];
    ApiData2.isHybrid = array[20];
    
}

void RFID () {

if (nfc.scan()) {
    if (nfc.readData(dataRead, READ_BLOCK_NO) != 1) {
      // Serial.print("Block ");                            // DIAGNOSTICS
      // Serial.print(READ_BLOCK_NO);                       // DIAGNOSTICS
      Serial.println("FAILED - PLEASE SCAN AGAIN!");
    }
    else {
      // Serial.print("Block ");                            // DIAGNOSTICS
      // Serial.print(READ_BLOCK_NO);                       // DIAGNOSTICS
      Serial.println("ENJOY THE EVENT!");
      //Particle.publish("Ejoy");

      //Serial.print("Data read(string):");                 // DIAGNOSTICS
      //Serial.println((char *)dataRead);                   // DIAGNOSTICS
      Serial.print("UID: ");
      for (int i = 0; i < BLOCK_SIZE; i++) {
        Serial.print(dataRead[i], HEX);
        Serial.print(" ");
        //Particle.publish("UID: " + String(dataRead[i], HEX), PRIVATE);
        dataRead[i] = 0;

      }
      Serial.println();
    }
    delay(500);
  }
}

void Fuel_Gauge() {

    SOC = gauge.getSOC();
    V = gauge.getVoltage();

    // Construct data package for Paticle.publish // 
    Serial.println("Constructing message...");          //DEBUG

        
        snprintf(msgFG, sizeof(msgFG)
            , "%.2f C\n"                                // cell temp in [C]
            ", %.2f mV\n"                               // Voltage in [mV]
            ", %.2f mA\n"                               //  Current in [mA]
            ", %.2f mA\n"                             // SoC in [%] - needs escaping hence %% / end with a CR / LF
            ", %.2f %%\r\n"
            , gauge.getTemperature()
            , gauge.getVoltage()
            , gauge.getCurrent()
            , gauge.getCapacity()
            , gauge.getSOC()
            );

    //Particle.publish("Battery Statistics", msgFG);        // Send messages to Particle Cloud
    Serial.print(msgFG);                                    //DEBUG

}

void loop() {

currentMillis1 = millis();

currentMillis1 = millis();
current_Publish_Millis = millis();

  if ((currentMillis1 - previousMillis1 >= epaper_update_interval) && (new_end_point_STATE == 0)) {
      previousMillis1 = currentMillis1;
      End_Point();
      } 

  if ((current_Publish_Millis - previous_Publish_Millis >= battery_level_interval)) {
      previous_Publish_Millis = current_Publish_Millis;
      Fuel_Gauge();
      //Particle.publish("Battery Statistics", msgFG);
      } 

  if (Handler_STATE == 1) {    
      Particle.disconnect();                                                 // Use only in SEMI_AUTOMATIC mode
      waitUntil(Particle.disconnected);
      Cellular.off();
      waitUntil(Cellular.isOff);
      delay(10);
      digitalWrite(LDO_EN, LOW);
      Serial.println("LDO Disconnected");
      Handler_STATE = 0;
      }

  if (new_end_point_STATE == 2) {
      EPD();
      }    

  RFID();
}


///////////////////////////////

//  char string2[] = char(ApiData1.startTime)
//  void GetHourFromString (int position, int length, char *myString);    
    
// //int main() {
 
// void timeformat() {    
//   GetHourFromString (12, 5, string2);
//   //return 0;
// }

// void GetHourFromString (int position, int length, char *myString) {

//     char Hour_min_endpoint[6];
//     strncpy(Hour_min_endpoint,myString+(position-1),length);
//     printf(Hour_min_endpoint);
// }
