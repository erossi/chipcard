#ifndef _UART_H_
#define _UART_H_

/* UART baud rate */
#define UART_BAUD 9600

void uart_init(void);
void uart_shutdown(void);
char uart_getchar(void);
void uart_putchar(const char c);
void uart_printstr(const char *s);

#endif
