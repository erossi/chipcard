#include <avr/io.h>
#include "uart.h"

/*!
 * Initialize the UART to 9600 Bd, tx/rx, 8N1.
 */
void uart_init(void)
{
#if F_CPU < 2000000UL && defined(U2X)
  UCSRA = _BV(U2X);             /* improve baud rate error by using 2x clk */
  UBRRL = (F_CPU / (8UL * UART_BAUD)) - 1;
#else
  UBRRL = (F_CPU / (16UL * UART_BAUD)) - 1;
#endif

  /*! tx/rx enable */
  UCSRB = _BV(TXEN) | _BV(RXEN);
  UCSRC = _BV(URSEL) | _BV(USBS) | _BV(UCSZ0) | _BV(UCSZ1); /* 8n2 */
}

char uart_getchar(void)
{
	if (bit_is_set(UCSRA, RXC))
		return(UDR);
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

  loop_until_bit_is_set(UCSRA, UDRE);
  UDR = c;
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
