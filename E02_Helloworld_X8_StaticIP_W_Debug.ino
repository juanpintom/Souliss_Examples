/**************************************************************************
    Souliss - Hello World with Static IP and Debug Enabled.
    
    This is the basic example, control one LED via a push-button or Android
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

// This identify the number of the LED logic
#define MYLEDLOGIC0          0    
#define MYLEDLOGIC1          1    
#define MYLEDLOGIC2          2    
#define MYLEDLOGIC3          3    
#define MYLEDLOGIC4          4    
#define MYLEDLOGIC5          5    
#define MYLEDLOGIC6          6    
#define MYLEDLOGIC7          7 

#define PINOUT0              2
#define PINOUT0              2
#define PINOUT0              2
#define PINOUT0              2
#define PINOUT0              2


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
    pinMode(9, OUTPUT);                 // Power the LED
}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds
            DigIn(2, Souliss_T1n_ToggleCmd, MYLEDLOGIC);            // Use the pin2 as ON/OFF toggle command
            Logic_SimpleLight(MYLEDLOGIC);                          // Drive the LED as per command
            DigOut(9, Souliss_T1n_Coil, MYLEDLOGIC);                // Use the pin9 to give power to the LED according to the logic
        } 
              
        // Here we handle here the communication with Android, commands and notification
        // are automatically assigned to MYLEDLOGIC
        FAST_GatewayComms();                                        
        
    }
} 

