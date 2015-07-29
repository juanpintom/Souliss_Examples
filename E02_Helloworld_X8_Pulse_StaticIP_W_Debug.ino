/**************************************************************************
    Souliss - Hello World with 8x Pulse Outputs Static IP and Debug Enabled.
    
    Output PINS from 2 to 9. 
    
    PINS 10-13 used by W5100
    
    This is the basic example, control one Pulse Output via Android
    using SoulissApp (get it from Play Store).  
    
    Run this code on one of the following boards:
      - Arduino Ethernet (W5100) 
      - Arduino with Ethernet Shield (W5100)
      
    As option you can run the same code on the following, just changing the
    relevant configuration file at begin of the sketch
      - Arduino with W5200 Ethernet Shield
      - Arduino with W5500 Ethernet Shield
        
***************************************************************************/

#define MaCaco_DEBUG_INSKETCH
#define MaCaco_DEBUG  		    1

#define VNET_DEBUG_INSKETCH
#define VNET_DEBUG  		      1

// Configure the framework
#include "bconf/StandardArduino.h"          // Use a standard Arduino
#include "conf/ethW5100.h"                  // Ethernet through Wiznet W5100
#include "conf/Gateway.h"                   // The main node is the Gateway, we have just one node
#include "conf/Webhook.h"                   // Enable DHCP and DNS

// Include framework code and libraries
#include <SPI.h>
#include "Souliss.h"

//SLOTS
#define MYLEDLOGIC0          0    
#define MYLEDLOGIC1          1    
#define MYLEDLOGIC2          2    
#define MYLEDLOGIC3          3    
#define MYLEDLOGIC4          4    
#define MYLEDLOGIC5          5    
#define MYLEDLOGIC6          6    
#define MYLEDLOGIC7          7 

//PINES
#define PINOUT0              2
#define PINOUT1              3
#define PINOUT2              4
#define PINOUT3              5
#define PINOUT4              6
#define PINOUT5              7
#define PINOUT6              8
#define PINOUT7              9


uint8_t ip_address[4]  = {192, 168, 1, 77};
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway[4]  = {192, 168, 1, 1};
#define myvNet_address	ip_address[3]		// The last byte of the IP address (77) is also the vNet address

void setup()
{   
    Serial.begin(115200);
    Initialize();

    Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);
	  SetAsGateway(myvNet_address);
    
    Set_PulseOutput(MYLEDLOGIC0);
    Set_PulseOutput(MYLEDLOGIC1);
    Set_PulseOutput(MYLEDLOGIC2);
    Set_PulseOutput(MYLEDLOGIC3);
    Set_PulseOutput(MYLEDLOGIC4);
    Set_PulseOutput(MYLEDLOGIC5);
    Set_PulseOutput(MYLEDLOGIC6);
    Set_PulseOutput(MYLEDLOGIC7);
    
    // We connect a pushbutton between 5V and pin2 with a pulldown resistor 
    // between pin2 and GND, the LED is connected to pin9 with a resistor to
    // limit the current amount
    pinMode(PINOUT0, OUTPUT);                 // OUTPUT 
    pinMode(PINOUT1, OUTPUT);                 // OUTPUT 
    pinMode(PINOUT2, OUTPUT);                 // OUTPUT 
    pinMode(PINOUT3, OUTPUT);                 // OUTPUT 
    pinMode(PINOUT4, OUTPUT);                 // OUTPUT 
    pinMode(PINOUT5, OUTPUT);                 // OUTPUT 
    pinMode(PINOUT6, OUTPUT);                 // OUTPUT 
    pinMode(PINOUT7, OUTPUT);                 // OUTPUT 
    
}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_x10ms(5){   // We process the logic and relevant input and output every 50 milliseconds
            
            Logic_PulseOutput(MYLEDLOGIC0);                          // Drive the LED as per command
            Logic_PulseOutput(MYLEDLOGIC1);                          // Drive the LED as per command
            Logic_PulseOutput(MYLEDLOGIC2);                          // Drive the LED as per command
            Logic_PulseOutput(MYLEDLOGIC3);                          // Drive the LED as per command
            Logic_PulseOutput(MYLEDLOGIC4);                          // Drive the LED as per command
            Logic_PulseOutput(MYLEDLOGIC5);                          // Drive the LED as per command
            Logic_PulseOutput(MYLEDLOGIC6);                          // Drive the LED as per command
            Logic_PulseOutput(MYLEDLOGIC7);                          // Drive the LED as per command
            
            DigOut(PINOUT0, Souliss_T1n_Coil, MYLEDLOGIC0);                // Use the pin to give power to the LED according to the logic
            DigOut(PINOUT1, Souliss_T1n_Coil, MYLEDLOGIC1);                // Use the pin to give power to the LED according to the logic
            DigOut(PINOUT2, Souliss_T1n_Coil, MYLEDLOGIC2);                // Use the pin to give power to the LED according to the logic
            DigOut(PINOUT3, Souliss_T1n_Coil, MYLEDLOGIC3);                // Use the pin to give power to the LED according to the logic
            DigOut(PINOUT4, Souliss_T1n_Coil, MYLEDLOGIC4);                // Use the pin to give power to the LED according to the logic
            DigOut(PINOUT5, Souliss_T1n_Coil, MYLEDLOGIC5);                // Use the pin to give power to the LED according to the logic
            DigOut(PINOUT6, Souliss_T1n_Coil, MYLEDLOGIC6);                // Use the pin to give power to the LED according to the logic
            DigOut(PINOUT7, Souliss_T1n_Coil, MYLEDLOGIC7);                // Use the pin to give power to the LED according to the logic
        } 
              
        // Here we handle here the communication with Android, commands and notification
        // are automatically assigned to MYLEDLOGIC
        FAST_GatewayComms();                                        
        
    }
} 

