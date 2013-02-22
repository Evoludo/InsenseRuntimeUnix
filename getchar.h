#ifndef GETCHAR_H
#define GETCHAR_H

#ifdef CONTIKI_SKY



#define UART_STRING "uart_isr: "
#define QSIZE 82              /* Change if you need */
#define In_EOL '\n'             // ASCII <CR>

extern int uart_isr_input_handler(unsigned char c);
extern char _low_level_get(void);

#define getchar __getchar
extern int __getchar(void);

#endif

#endif /* GETCHAR_H */
