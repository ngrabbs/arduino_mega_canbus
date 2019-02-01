// Listing 7-1
#include <LiquidCrystal.h>
#include <SPI.h>
#include "mcp_can.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN); // Set CS pin
LiquidCrystal lcd(4, 5, 6, 7, 8, 9); // pins for RS, E, DB4, DB5, DB6, DB7
float analogValue = 0;
float voltage = 0;
float map_sensor = 0;
void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
      Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS, MCP_8MHz))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}

void parse_hex(float analogValue) {

  Serial.println(analogValue);
}

unsigned char stmp[8] = {0x03, 0x0C, 9, 6, 5, 4, 3, 2};
void loop()
{

  analogValue = analogRead(0);
  voltage = 0.0048*analogValue;
  parse_hex(analogValue);
  lcd.setCursor(0, 0);
  lcd.print("map: ");
  lcd.print(52*voltage);
  lcd.setCursor(0, 1);
  lcd.print("volt: ");
  lcd.print(0.0048*analogValue);
  
  // send data:  id = 0x00, standard frame, data len = 8, stmp: data buf
    stmp[7] = stmp[7]+1;
    if(stmp[7] == 100)
    {
        stmp[7] = 0;
        stmp[6] = stmp[6] + 1;
        
        if(stmp[6] == 100)
        {
            stmp[6] = 0;
            stmp[5] = stmp[6] + 1;
        }
    }
    
    CAN.sendMsgBuf(0x01, 0, 8, stmp);
    

  delay(100);
  
  
}
