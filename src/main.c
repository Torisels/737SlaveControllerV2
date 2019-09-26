#include <twi.h>
#include <twi.c>
//#include <Arduino.h>


#define BOARD_ADDRESS 0xFA
#define MESS_DATA_TRANSMIT 0xB2

uint8_t rxBuffer[32];

void onReceiveService(uint8_t* inBytes, int numBytes)
{
    // don't bother if user hasn't registered a callback
//  if(!user_onReceive){
//    return;
//  }
    // don't bother if rx buffer is in use by a master requestFrom() op
    // i know this drops data, but it allows for slight stupidity
    // meaning, they may not have read all the master requestFrom() data yet
//    if(rxBufferIndex < rxBufferLength){
//        return;
//    }
    // copy twi rx buffer into local read buffer
    // this enables new reads to happen in parallel
    for(uint8_t i = 0; i < numBytes; ++i){
        rxBuffer[i] = inBytes[i];
    }
    // set rx iterator vars
//    rxBufferIndex = 0;
//    rxBufferLength = numBytes;
    // alert user program
    //user_onReceive(numBytes);
}

// behind the scenes function that is called when data is requested
void onRequestService(void)
{
    uint8_t data[] = {MESS_DATA_TRANSMIT,BOARD_ADDRESS,PINA,PINB,PINC,PIND};
     twi_transmit(data, sizeof(data));
}


void setup()
{
    twi_init();
    twi_attachSlaveTxEvent(onRequestService); // default callback must exist
    twi_attachSlaveRxEvent(onReceiveService); // default callback must exist
    twi_setAddress(BOARD_ADDRESS);

}



int  main()
{

}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
