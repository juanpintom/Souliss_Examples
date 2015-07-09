/**************************************************************************
    Souliss - Hello World
    
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

#define PLC_READ_PIN        3
#define BUTTON              2
#define RELAY               9

void setup()
{   
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
            //DigIn(2, Souliss_T1n_ToggleCmd, MYLEDLOGIC);            // Use the pin2 as ON/OFF toggle command
            
           /* if(digitalRead(BUTTON) && InPin[BUTTON] == PINRESET) { 
                 InPin[BUTTON] = PINSET;
            }
            if(digitalRead(BUTTON) && InPin[BUTTON] == PINSET) { 
                 InPin[BUTTON] = PINSET;
            }
            if(!digitalRead(BUTTON) && InPin[BUTTON] == PINSET) { 
                 InPin[BUTTON] = PINRELEASED;
            }
            if(!digitalRead(BUTTON) && InPin[BUTTON] == PINRELEASED) { 
                 digitalWrite(RELAY, !digitalRead(RELAY));
                 InPin[BUTTON] = PINRELEASED;
            }
            
            if(digitalRead(PLC_READ_PIN)) mOutput(MYLEDLOGIC) = Souliss_T1n_OnCoil; 
            else mOutput(MYLEDLOGIC) = Souliss_T1n_OffCoil;*/
            SoulissPLC_Read(MYLEDLOGIC, BUTTON, PLC_READ_PIN, RELAY);
            Logic_SimpleLight(MYLEDLOGIC);                          // Drive the LED as per command
            
            
            //DigOut(9, Souliss_T1n_Coil, MYLEDLOGIC);                // Use the pin9 to give power to the LED according to the logic
        } 
              
        // Here we handle here the communication with Android, commands and notification
        // are automatically assigned to MYLEDLOGIC
        FAST_GatewayComms();                                        
        
    }
} 

uint8_t SoulissPLC_Read(uint8_t slot, uint8_t button_pin, uint8_t plc_pin, uint8_t relay_pin){
    
      if(digitalRead(button_pin) && InPin[button_pin] == PINRESET) { 
                 InPin[button_pin] = PINSET;
                 return InPin[button_pin];
            }
            if(digitalRead(button_pin) && InPin[button_pin] == PINSET) { 
                 InPin[button_pin] = PINACTIVE;
                 return InPin[button_pin];
            }
            if(!digitalRead(button_pin) && InPin[button_pin] == PINACTIVE) { 
                 InPin[button_pin] = PINRELEASED;
                 return InPin[button_pin];
            }
            if(!digitalRead(button_pin) && InPin[button_pin] == PINRELEASED) { 
                 digitalWrite(relay_pin, !digitalRead(relay_pin));
                 InPin[button_pin] = PINRESET;
                 return InPin[button_pin];
            }
            
            //if(digitalRead(PLC_READ_PIN)) mOutput(slot) = Souliss_T1n_OnCoil; 
            //else mOutput(slot) = Souliss_T1n_OffCoil;
            
            if(digitalRead(plc_pin)) mInput(slot) = Souliss_T1n_OnCmd; 
            else mOutput(slot) = Souliss_T1n_OffCmd;
            
    
}
