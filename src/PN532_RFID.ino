SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// I entered this code 

// // EPD includes
#include <SPI.h>
// #include "../lib/E_Paper/src/EPD_3in52.h"
// #include "../lib/E_Paper/src/imagedata.h"
// #include "../lib/E_Paper/src/epdpaint.h"

#include "../lib/EPD4_2/src/epd4in2b_V2.h"
#include "../lib/EPD4_2/src/imagedata.h"
#include "../lib/EPD4_2/src/epdpaint.h"

// RFID includes
#include "../lib/PN532/src/DFRobot_PN532.h"

// Fuel Guage includes
#include "../lib/MAX17201/src/max1720x.h"

////--  EPD --/////

Epd epd;

#define COLORED     0
#define UNCOLORED   1

//UBYTE image[11000];
//UBYTE image2[11000];

String VAR1;
String VAR2;
String VAR3;
String VAR4;
String VAR5;
String VAR6;
String SCAN;


////--  RFID Declarations --/////
#define  BLOCK_SIZE       10      // 16 
#define  PN532_IRQ        2
#define  POLLING          0
#define  INTERRUPT        1

#define  READ_BLOCK_NO    2       // The block to be read

DFRobot_PN532_IIC  nfc(PN532_IRQ, POLLING);
uint8_t dataRead[16] = {0}; 

/////--  MAX17201 Fuel Gauge declarations --/////
char msgFG[64];
max1720x gauge;

/////--  OTHER declarations --/////

//#define HALO D11
//#define BATON D22

void setup() {

Serial.begin(115200);

delay(5000);

// pinMode(D6, OUTPUT);
// digitalWrite (D6, HIGH);
// delay(2000);
// digitalWrite (D6, LOW);
// delay(2000);

// pinMode(BATON, OUTPUT);
// digitalWrite(BATON,HIGH);

// pinMode(D23, OUTPUT);
// digitalWrite(D23, HIGH);

// pinMode(D23, OUTPUT);
// digitalWrite(D23, LOW);

//gauge.reset();                                  // Resets MAX1720x
//delay(2000);                                      // Waits for the initial measurements to be made

// TEST PANEL 3.52" --- START

Serial.println();
Serial.print("Initializing e-Paper display");
if (epd.Init() != 0) { 
  Serial.print("Failed to initialize display");
  return;
  }

// TEST PANEL 3.52" --- END

Serial.println();
Serial.print("Initializing PN532");
while (!nfc.begin()) {
  Serial.print(".");
  delay (1000);
}

//EPD();

 Serial.println();
 Serial.println("Waiting for a card......");  

I2C_Scanner();

}

void I2C_Scanner() {

byte error, address;
int nDevices;
 
  delay(1000);
 
  nDevices = 0;
  
  for(address = 1; address < 127; address++ )
  { 
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      Serial.println(address,HEX);
      nDevices++;
    }
    
      else if (error==4)
        {
          if (address<16)
          Serial.print("0");
          Particle.publish(String(address,HEX), PRIVATE);
    }    
  }
  
  if (nDevices == 0)
    //Serial.println("No I2C devices found");
    Particle.publish("No I2C found");
      else
        Particle.publish("done");
        Serial.println("Done");
}


void RFID () {

if (nfc.scan()) {
    if (nfc.readData(dataRead, READ_BLOCK_NO) != 1) {
      // Serial.print("Block ");
      // Serial.print(READ_BLOCK_NO);
      Serial.println("FAILED - PLEASE SCAN AGAIN!");
    }
    else {
      // Serial.print("Block ");
      // Serial.print(READ_BLOCK_NO);
      Serial.println("ENJOY THE EVENT!");
      //Particle.publish("Ejoy");

      //Serial.print("Data read(string):");
      //Serial.println((char *)dataRead);
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
    
    //total_capacity = 1200.00;                                        // Stipulate mA rating of battery  (??? move to SETUP after testing ???)
    //nominal_cell_voltage = 3.00;                                     // Stipulate nominal Voltage rating of battery as primary cells with flat discharge curve is used
    //Capacity_Factor = 5.00;

    //uint16_t a = gauge.getCoulombsQH();
    //int16_t b = ((int16_t)a * (-0.5));                                // 0.5 when 0.01ohm sense resistor is used.

    //cell_temp = gauge.getTemperature();
    //cell_v = (gauge.getVoltage()/1000);                               // Read Cell voltage (in mV) and convert to V

    //remaining_capacity = total_capacity - (b);
    //New_SoC = (remaining_capacity / total_capacity) * 100;

    //SoC = New_SoC; 

    delay(50);

    // Construct data package for Paticle.publish // 
    Serial.println("Constructing message for Particle.publish...");                 //DEBUG

        
        snprintf(msgFG, sizeof(msgFG)
            , "%.2f C\n"                                // cell temp in [C]
            ", %.2f mV\n"                               // Voltage in [mV]
            ", %.2f mA\n"                               //  Current in [mA]
            ", %.2f %%\r\n"                             // SoC     in [%] - needs escaping hence %% / end with a CR / LF
            , gauge.getTemperature()
            , gauge.getVoltage()
            ,gauge.getCurrent()
            , gauge.getSOC()
            );

    //Particle.publish("Battery Statistics", msgFG);       // Send messages to Particle Cloud
    Serial.print(msgFG);
}

void EPD() {

  // Serial.println("Constructing message for display...");

  // VAR1 = "Office Room";
  // VAR2 = "Movie Night";
  // VAR3 = "20:00";
  // VAR4 = "22:00";
  // VAR5 = "Tom &";
  // VAR6 = "Cruise";
  // SCAN = "D-Day ";
  
  // epd.display_NUM(EPD_3IN52_WHITE);
  // epd.lut_GC();
  // epd.Clear();
  // epd.refresh();

  // delay(500);

  // epd.SendCommand(0x50);
  // epd.SendData(0x17);

  // Paint paint(image, 240, 360);     // width should be the multiple of 8   
  // paint.SetRotate(3);               // Top right (0,0)
  // paint.Clear(UNCOLORED);

  // paint.DrawStringAt(5, 5, "ROOM: ", &Font20, COLORED);
  // paint.DrawStringAt(100, 5, String(VAR1), &Font24, COLORED);

  // paint.DrawStringAt(5, 35, "TITLE: ", &Font20, COLORED);
  // paint.DrawStringAt(100, 35, String(VAR2), &Font24, COLORED);

  // paint.DrawStringAt(5, 65, "TIME: ", &Font20, COLORED);
  // paint.DrawStringAt(100, 65, String(VAR3) + " - " + String(VAR4), &Font24, COLORED);

  // paint.DrawHorizontalLine(0,136,360,COLORED);
  // paint.DrawVerticalLine(180,136,104,COLORED);
    
  // paint.DrawFilledRectangle(0,145,170,240, COLORED);
  // paint.DrawStringAt(5, 155, "SPEAKER: ", &Font20, UNCOLORED);
  // paint.DrawStringAt(5, 185, (VAR5), &Font24, UNCOLORED);
  // paint.DrawStringAt(5, 215, (VAR6), &Font24, UNCOLORED);

  // paint.DrawFilledRectangle(190,145,360,240, COLORED);   
  // paint.DrawStringAt(200, 180, String(SCAN), &Font24, UNCOLORED);                 //This should be printed from the scan function 

  // epd.display_part(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());  // (Y, X)
  // epd.refresh();
  // delay(25);
  // epd.sleep();

  
  /////////////// e-Paper 4.2" Raw Panel ------------->

  Serial.print("4.2inch e-paper test\r\n ");

  // //   /* This clears the SRAM of the e-paper display */
   epd.ClearFrame();

   unsigned char image[1500];
   Paint paint(image, 400, 28);    //width should be the multiple of 8 

   paint.Clear(UNCOLORED);
   paint.DrawStringAt(0, 0, "e-Paper Demo", &Font24, COLORED);
   epd.SetPartialWindowBlack(paint.GetImage(), 100, 40, paint.GetWidth(), paint.GetHeight());

   paint.Clear(COLORED);
   paint.DrawStringAt(100, 2, "Hello world", &Font24, UNCOLORED);
   epd.SetPartialWindowRed(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());
  
   paint.SetWidth(64);
   paint.SetHeight(64);

   paint.Clear(UNCOLORED);
   paint.DrawRectangle(0, 0, 40, 50, COLORED);
   paint.DrawLine(0, 0, 40, 50, COLORED);
   paint.DrawLine(40, 0, 0, 50, COLORED);
   epd.SetPartialWindowBlack(paint.GetImage(), 72, 120, paint.GetWidth(), paint.GetHeight());
  
   paint.Clear(UNCOLORED);
   paint.DrawCircle(32, 32, 30, COLORED);
   epd.SetPartialWindowBlack(paint.GetImage(), 200, 120, paint.GetWidth(), paint.GetHeight());

   paint.Clear(UNCOLORED);
   paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
   epd.SetPartialWindowRed(paint.GetImage(), 72, 200, paint.GetWidth(), paint.GetHeight());

   paint.Clear(UNCOLORED);
   paint.DrawFilledCircle(32, 32, 30, COLORED);
   epd.SetPartialWindowRed(paint.GetImage(), 200, 200, paint.GetWidth(), paint.GetHeight());

  // /* This displays the data from the SRAM in e-Paper module */
   epd.DisplayFrame();

  /* This displays an image */
  epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);

  /* Deep sleep */
  //epd.Sleep();

  /////////////// e-Paper 4.2" Raw Panel -------------<

}

void loop() {

  //I2C_Scanner();
  //Fuel_Gauge();
  EPD();
  //RFID();
  delay(5000);
  
}
