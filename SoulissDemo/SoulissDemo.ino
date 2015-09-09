/**************************************************************************
    Souliss - Demo Sketch
    
    This is the demo example, show all typicals w/out Input - Output commands.
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
#define MaCaco_DEBUG  		    0

#define VNET_DEBUG_INSKETCH
#define VNET_DEBUG  		      0

// Configure the framework
#include "bconf/StandardArduino.h"          // Use a standard Arduino
#include "conf/ethW5100.h"                  // Ethernet through Wiznet W5100
#include "conf/Gateway.h"                   // The main node is the Gateway, we have just one node
#include "conf/Webhook.h"                   // Enable DHCP and DNS

// Include framework code and libraries
#include <SPI.h>
#include "Souliss.h"

// This identify the number of the LED logic
#define T11          0    // ON/OFF Digital Output with Timer Option
#define T12          1    // ON/OFF Digital Output with AUTO mode
#define T13          2    // Digital Input Value    **RANDOM IN
#define T14          3    // Pulse Digital Output
#define T15          4    // RGB IR Light
#define T16          6    // RGB LED Strip
#define T18          10   // ON/OFF Digital Output with pulse output	with Timer Option 
#define T19          11   // Dimmable Single Color LED Strip

#define T21          13   // Motorized devices with limit switches
#define T22          14   // Motorized devices with limit switches and middle position

#define T41          15   // Anti-theft integration 
                          //*************** ** RANDOM IN ** **********************
#define T51          16   // Generic
#define T52          18   // Souliss_Logic_T52 - Temperature measure (-20, +50) °C
#define T53          20	  // Souliss_Logic_T53 - Humidity measure (0, 100) %
#define T54          22   // Souliss_Logic_T54 - Light Sensor (0, 40) kLux
#define T55          24	  // Souliss_Logic_T55 - Voltage (0, 400) V
#define T56          26   // Souliss_Logic_T56 - Current (0, 25)  A
#define T57          28	  // Souliss_Logic_T57 - Power (0, 6500)  W
#define T58          30   // Souliss_Logic_T58 - Pressure measure (0, 1500) hPa 


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
    
    Set_T11(T11);
    Set_T12(T12);
    Set_T13(T13);
    Set_T14(T14);
    Set_T15(T15);
    Set_T16(T16);
    Set_T18(T18);
    Set_T19(T19);
    
    Set_T21(T21);
    Set_T22(T22);
    
    Set_T41(T41);
    
    Set_T51(T51);
    Set_T52(T52);
    Set_T53(T53);
    Set_T54(T54);
    Set_T55(T55);
    Set_T56(T56);
    Set_T57(T57);
    Set_T58(T58);
    
}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds
            Logic_T11(T11);  
            Logic_T12(T12);  
            Logic_T13(T13);  
            Logic_T14(T14);  
            Logic_T15(T15);      
            Logic_T16(T16);      
            Logic_T18(T18);      
            Logic_T19(T19);      
            
            Logic_T21(T21);      
            Logic_T22(T22);      
            
            Logic_T41(T41);  
            
        } 
              
        // Here we handle here the communication with Android, commands and notification
        // are automatically assigned to MYLEDLOGIC
        FAST_GatewayComms();                                        
        
    }
    
    EXECUTESLOW() { 
        UPDATESLOW();

        SLOW_10s() {  // Process the timer every 10 seconds  
            Timer_T11(T11);
            Timer_T12(T12);
            Timer_T16(T16);
            Timer_T18(T18);
            Timer_T19(T19);
            
            Timer_T21(T21);
            Timer_T22(T22);
            
            Timer_T41(T41);
        } 
        
        SLOW_x10s(2) {
            ImportAnalog(T51,random(0, 1024))
            Read_T51(T51);  
    	    
    	    ImportAnalog(T52,random(-20, 50))
    	    Logic_T52(T52); 
    	    
    	    ImportAnalog(T53,random(0, 100))
    	    Logic_T53(T53); 
        }
    	
    	SLOW_x10s(3) {
    	    ImportAnalog(T54,random(0, 40000))
    	    Logic_T54(T54); 
    	}
    	
    	SLOW_x10s(4) {
    	    ImportAnalog(T55,random(0, 400))
    	    Logic_T55(T55);  
    	    
    	    ImportAnalog(T56,random(0, 25))
    	    Logic_T56(T56);  
    	    
    	    //Change this with T55 * T56 calc
    	    ImportAnalog(T57,random(0, 6500))
    	    Logic_T57(T57);  
    	}
    	
    	SLOW_x10s(5) {
    	    ImportAnalog(T58,random(0, 1500))
    	    Logic_T58(T58);    
        }	
    }
} 

