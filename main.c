#include "common.h"

#include "uart.h"
#include "clock.h"
#include "uart_process.h"
#include "io.h"
#include "softi2c.h"


void init_io(void)
{
    
    LED_PORT.DIRSET = LED1 | LED2;
    LED_PORT.OUTSET = LED1 | LED2;    
}


void _uart_sendc(unsigned char c)
{
    uart_sendc(&UART, c);
}

void init_uart(void)
{
    uart_init(&UART);
    uart_queue_init(&Q_UART);

    uart_process_init_linebuf(&LB);
    LB.packet_status = OUT_PACKET;

    fdevopen(_uart_sendc, NULL);
}


int main(void)
{
    clock_rtc_init();
    clock_pll_init();
    clock_dfll_init();
    
    init_io();
    init_uart();
    _delay_us(300);
    swiic_init();
    
    PMIC.CTRL |= PMIC_HILVLEX_bm | PMIC_MEDLVLEX_bm | PMIC_LOLVLEX_bm | PMIC_RREN_bm;
    sei();
    
    printf("hello!\n");
    for (;;)
    {
        if (1)
        {
            uart_process_tick(&Q_UART, &LB, uart_process_lb, STX, ETX);
        }

        if (1)
        {
            // SLEEP.CTRL = SLEEP_SEN_bm | SLEEP_SMODE_PSAVE_gc;
            SLEEP.CTRL = SLEEP_SEN_bm | SLEEP_SMODE_IDLE_gc;
            __asm__ __volatile__ ("sleep");
        }
    }
}
