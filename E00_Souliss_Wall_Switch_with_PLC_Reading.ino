/**************************************************************************
    Souliss - PLC Reading
    
    Using the scheme here:
    https://groups.google.com/forum/#!msg/souliss-it/PIYQUQf4W70/fjSdjvmN79UJ
    We can now the state of Lights (220v) and modify the state of typical based on the state of PLC.
    
    I added a button to switch the Output Relay, this acts like a wall switch.
    
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
#define MaCaco_DEBUG  		1

#define VNET_DEBUG_INSKETCH
#define VNET_DEBUG  		1

#define PLC_DEBUG               1

// Configure the framework
#include "bconf/StandardArduino.h"          // Use a standard Arduino
#include "conf/ethW5100.h"                  // Ethernet through Wiznet W5100
#include "conf/Gateway.h"                   // The main node is the Gateway, we have just one node
#include "conf/Webhook.h"                   // Enable DHCP and DNS

// Include framework code and libraries
#include <SPI.h>
#include "Souliss.h"

// This identify the number of the LED logic
#define MYLEDLOGIC          0 

#define PLC_READ_PIN        A0
#define BUTTON              A1
#define RELAY               A2

void setup()
{   
    Serial.begin(115200);
    Initialize();

    // Get the IP address from DHCP
    GetIPAddress();                          
    SetAsGateway(myvNet_dhcp);       // Set this node as gateway for SoulissApp  
    
    Set_SimpleLight(MYLEDLOGIC);        // Define a simple LED light logic
    
    // We connect a pushbutton between 5V and pin2 with a pulldown resistor 
    // between pin2 and GND, the LED is connected to pin9 with a resistor to
    // limit the current amount
    pinMode(PLC_READ_PIN, INPUT);
    pinMode(BUTTON, INPUT);                  // Hardware pulldown required
    pinMode(RELAY, OUTPUT);                 // Power the LED
}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_50ms() {   // We process the logic and relevant input and output every 50 milliseconds

            SoulissPLC_Read(MYLEDLOGIC, BUTTON, PLC_READ_PIN, RELAY);
            Logic_SimpleLight(MYLEDLOGIC);                          // Drive the LED as per command
           
        } 
              
        // Here we handle here the communication with Android, commands and notification
        // are automatically assigned to MYLEDLOGIC
        FAST_GatewayComms();                                        
        
    }
} 

uint8_t SoulissPLC_Read(uint8_t slot, uint8_t button_pin, uint8_t plc_pin, uint8_t relay_pin){
                           
            if(mInput(slot) == Souliss_T1n_OnCmd || mInput(slot) == Souliss_T1n_OffCmd){
                 if(PLC_DEBUG) Serial.println(mInput(slot));  
                 digitalWrite(relay_pin, !digitalRead(relay_pin)); 
                 memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;
                 return 99;  //Return 99 when cmd received from Souliss App Just for test.
            }
  
            if(digitalRead(button_pin) && InPin[button_pin] == PINRESET) { 
                 InPin[button_pin] = PINSET;
                 if(PLC_DEBUG) Serial.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            if(digitalRead(button_pin) && InPin[button_pin] == PINSET) { 
                 InPin[button_pin] = PINACTIVE;
                 if(PLC_DEBUG) Serial.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            if(!digitalRead(button_pin) && InPin[button_pin] == PINACTIVE) { 
                 InPin[button_pin] = PINRELEASED;
                 if(PLC_DEBUG) Serial.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            if(!digitalRead(button_pin) && InPin[button_pin] == PINRELEASED) { 
                 digitalWrite(relay_pin, !digitalRead(relay_pin));
                 InPin[button_pin] = PINRESET;
                 if(PLC_DEBUG) Serial.println(InPin[button_pin]);                 
                 return InPin[button_pin];
            }
            
            
            if(digitalRead(plc_pin)) {    
                memory_map[MaCaco_OUT_s + slot] = Souliss_T1n_OnCoil; 
                memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;                
                if(PLC_DEBUG) Serial.println("PLC ON"); 
                
            }
            else {
                memory_map[MaCaco_OUT_s + slot] = Souliss_T1n_OffCoil;
                memory_map[MaCaco_IN_s + slot] = Souliss_T1n_RstCmd;                
                if(PLC_DEBUG) Serial.println("PLC OFF");             
            }
}
