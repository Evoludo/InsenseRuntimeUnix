#ifndef GETSTRING_H
#define GETSTRING_H

#ifdef CONTIKI_SKY

#define getstring __getstring
extern char *__getstring(void);

#define LINE_LENGTH 80

#endif/* CONTIKI_SKY */

#endif /* GETSTRING_H */
