#include <avr/io.h>
#include "uart.h"

/*!
 * Initialize the UART to 9600 Bd, tx/rx, 8N1.
 */
void uart_init(void)
{
/* improve baud rate error by using 2x clk */
#if F_CPU < 2000000UL && defined(U2X)
  UCSR0A = _BV(U2X0);
  UBRR0L = (F_CPU / (8UL * UART_BAUD)) - 1;
#else
  UBRR0L = (F_CPU / (16UL * UART_BAUD)) - 1;
#endif

  /*! tx/rx enable */
  UCSR0B = _BV(TXEN0) | _BV(RXEN0);
 /* 8n2 */
  UCSR0C = _BV(USBS0) | _BV(UCSZ00) | _BV(UCSZ01);
}

char uart_getchar(void)
{
	if (bit_is_set(UCSR0A, RXC0))
		return(UDR0);
	else
		return(0);
}

/*!
 * Send character c down the UART Tx, wait until tx holding register
 * is empty.
 */
void uart_putchar(const char c)
{
  if (c == '\n')
    uart_putchar('\r');

  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
}

/*!
 * Send a C (NUL-terminated) string down the UART Tx.
 */
void uart_printstr(const char *s)
{

  while (*s) {
      if (*s == '\n')
	uart_putchar('\r');

      uart_putchar(*s++);
    }
}
