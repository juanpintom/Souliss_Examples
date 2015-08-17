/**************************************************************************
    Souliss - Energy Monitor with Static IP and Debug Enabled.
    
    This example uses EmonLib to read Current. 
    Library can be found here: https://github.com/openenergymonitor/EmonLib
    
    Run this code on one of the following boards:
      - Arduino Ethernet (W5100) 
      - Arduino with Ethernet Shield (W5100)
      
    As option you can run the same code on the following, just changing the
    relevant configuration file at begin of the sketch
      - Arduino with W5200 Ethernet Shield
      - Arduino with W5500 Ethernet Shield
        
***************************************************************************/

#include "EmonLib.h"             // Include Emon Library 
EnergyMonitor emon1;             // Create an instance 

#define CT_PIN  A1               // Pin to CT Sensor 


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
#define CURRENT             0
#define WATTS               2
 

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
    
    Set_Current(CURRENT);
    Set_Power(WATTS);

    emon1.current(CT_PIN, 10);       // Current: input pin, calibration. (use 10 with a 220ohm burden and SCT-013-000)

}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_210ms() {   // We process the logic and relevant input and output every 50 milliseconds
          Logic_Current(CURRENT);
          Logic_Power(WATTS);
        }
        FAST_710ms()    {
            float current = emon1.calcIrms(1480);
            Souliss_ImportAnalog(memory_map, CURRENT, &current); 
            float watt = current*230; 
            Souliss_ImportAnalog(memory_map, WATTS, &watt);
        }
              
        // Here we handle here the communication with Android, commands and notification
        // are automatically assigned to MYLEDLOGIC
        FAST_GatewayComms();                                        
        
    }
} 

