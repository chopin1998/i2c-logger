#ifndef _SOFTIIC_H
#define _SOFTIIC_H

#define SWIIC_PORT   PORTC
    #define SCK_PIN  PIN0_bm
    #define SDA_PIN  PIN2_bm
    #define SCK_vect PORTC_INT0_vect
    #define SDA_vect PORTC_INT1_vect
    #define SCK_PINCTRL PIN0CTRL
    #define SDA_PINCTRL PIN2CTRL


typedef struct {
    unsigned char in_frame;
} SWIIC_t;

volatile SWIIC_t SWIIC;
    


void swiic_init(void);


#endif
