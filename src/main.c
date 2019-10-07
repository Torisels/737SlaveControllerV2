#include <twi.h>
#include <twi.c>
#include <util/delay.h>
#define BOARD_ADDRESS 0xFA
#define MESS_DATA_TRANSMIT 0xB2
#define MESS_SETUP_MODE 0xB3
#define MESS_OUTPUT 0xB4

void onReceiveService(uint8_t* inBytes, int numBytes)
{
    uint8_t index = 0;

    //First byte: check if data was transmitted to correct MCU
    if(inBytes[index] != BOARD_ADDRESS)
        return;

    //Second byte is flag
    switch (inBytes[++index])
    {
        //Register order: DDRA, PINA,DDRB,PINB,DDRC,PINC,DDRD,PIND. Timers will be implemented
        case MESS_SETUP_MODE:
        {
            DDRA = inBytes[++index];
            PORTA = inBytes[++index];
            DDRB = inBytes[++index];
            PORTB = inBytes[++index];
            DDRC = inBytes[++index];
            PORTC = inBytes[++index];
            DDRD = inBytes[++index];
            PORTD = inBytes[++index];
            break;
        }

        case MESS_OUTPUT:
        {
            PORTA = inBytes[++index];
            PORTB = inBytes[++index];
            PORTC = inBytes[++index];
            PORTD = inBytes[++index];
            break;
        }
        default:
            break;
    }
}

// behind the scenes function that is called when data is requested
void onRequestService()
{
     uint8_t data[] = {BOARD_ADDRESS,PINA,PINB,PINC,PIND};
     twi_transmit(data, sizeof(data));

     //debugging LED
     PORTA ^= 1 << PA0;
}

void setup()
{
    sei();
    DDRA |= 1 <<PA0;
    twi_setAddress(0xFA);
    twi_attachSlaveTxEvent(onRequestService); // default callback must exist
    twi_attachSlaveRxEvent(onReceiveService); // default callback must exist
    twi_init();
}


int  main()
{
     setup();
     for(;;)
     {
         _delay_ms(10);
     }
}


