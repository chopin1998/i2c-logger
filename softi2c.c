#include "common.h"
#include "softi2c.h"
#include "uart.h"
#include "io.h"


ISR (SCK_vect)
{
    LED_PORT.OUTTGL = LED2;
    if (SWIIC.in_frame)
    {
        UART.DATA = (SWIIC_PORT.IN & SDA_PIN) + 0x30;
    }
}

ISR (SDA_vect)
{
    if (SWIIC_PORT.IN & SCK_PIN)
    {                           /* sck hi */
        if (SWIIC_PORT.IN & SDA_PIN)
        {                       /* sda hi */
            SWIIC.in_frame = 0;
        }
        else
        {                       /* sda low */
            SWIIC.in_frame = 1;
            UART.DATA = '\n';
        }
    }
    else
    {                           /* sck low */
        return;
    }
}


void swiic_init(void)
{
    SWIIC.in_frame = 0;
    
    SWIIC_PORT.DIRCLR = SCK_PIN | SDA_PIN;

    SWIIC_PORT.INTCTRL = PORT_INT1LVL_HI_gc | PORT_INT0LVL_HI_gc;
    SWIIC_PORT.INT0MASK = SCK_PIN;
    SWIIC_PORT.INT1MASK = SDA_PIN;

    SWIIC_PORT.SCK_PINCTRL = PORT_ISC_RISING_gc;
    SWIIC_PORT.SDA_PINCTRL = PORT_ISC_BOTHEDGES_gc;
}
