#include "common.h"
#include "uart.h"
#include "uart_process.h"
#include "util.h"


void uart_process_init_linebuf(LINE_BUF_t *lb)
{
    memset(lb->buf, 0x00, LINE_BUF_MAX);
    lb->index = 0;
}


signed char uart_process_tick(RAW_QUEUE_t *q, LINE_BUF_t *lb, void (*process)(void),
                              unsigned char _STX, unsigned char _ETX)
{
    signed char recv;
    
    cli();
    recv = uart_dequeue(q);
    sei();

    if (recv == -1)
    {
        return -1;
    }
    else
    {
        /* as following */
    }

    if (lb->packet_status == OUT_PACKET)
    {
        if (recv == _STX)
        {
            lb->packet_status = IN_PACKET;
            
            uart_process_init_linebuf(lb);
        }
        else
        {
            /* ignore */
        }
    }
    else
    {                           /* packet_status == IN_PACK */
        if (recv == _ETX)
        {                       /* end of a packet */
            lb->packet_status = OUT_PACKET;
            lb->buf[lb->index] = 0x00; /* padding a terminal char */

            if (process != NULL)
            {
                process();
            }

            uart_process_init_linebuf(lb);
        }
        else
        {                       /* normally */
            lb->buf[lb->index] = recv;            
            lb->index++;

            if (lb->index >= LINE_BUF_MAX)
            {                   /* a huge packet ? */
                lb->packet_status = OUT_PACKET;

                uart_process_init_linebuf(lb);
            }
        }
    }
}


void uart_process_lb(void)
{
    P_LIST_t *para_head, *curr;
    char *p = NULL;
    unsigned char p_count = 0;

    unsigned char reg, val;
    unsigned char rev;
    signed int offset;

    para_head = (P_LIST_t *)alloca(sizeof(P_LIST_t));

    p = strtok(&LB.buf[0], "|");
    curr = para_head;

    while (p)
    {
        p_count++;

        curr->para = p;
        curr->next = (P_LIST_t *)alloca(sizeof(P_LIST_t));
        curr = curr->next;

        p = strtok(NULL, "|");
    }
    curr->next = NULL;

    if ( !strcmp(para_head->para, "hi") )
    {
        printf("hello\n");
    }
    else
    {
        printf("unknow cmd: %s\n", para_head->para);
    }

    // p_list_clear(para_head);
}

