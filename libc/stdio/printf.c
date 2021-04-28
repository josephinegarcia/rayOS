#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

struct param{
	char *bf;
};

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

void ui2a(unsigned int num, struct param *p){
	int n = 0;
    unsigned int d = 1;
	char* buf = p->bf;
    while (num / d >= 10)
        d *= 10;
    while (d != 0) {
        int dgt = num / d;
        num %= d;
        d /= 10;
        if (n || dgt > 0 || d == 0) {
            *buf++ = dgt + (dgt < 10 ? '0' : (0 ? 'A' : 'a') - 10);
            ++n;
        }
    }
	*buf = 0;
}

#ifdef __GNUC__
# define _TFP_GCC_NO_INLINE_  __attribute__ ((noinline))
#else
# define _TFP_GCC_NO_INLINE_
#endif

static void __attribute__ ((noinline)) ulli2a(unsigned long long int num, struct param *p){
    int n = 0;
    unsigned long long int d = 1;
    char *bf = p->bf;
    while (num / d >= 16)
        d *= 16;
    while (d != 0) {
        int dgt = num / d;
        num %= d;
        d /= 16;
        if (n || dgt > 0 || d == 0) {
            *bf++ = dgt + (dgt < 10 ? '0' : (0 ? 'A' : 'a') - 10);
            ++n;
        }
    }
    *bf = 0;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
	char bf[23];
	struct param p;
	p.bf = bf;
	int written = 0;
	char lng = 0;  /* 1 for long, 2 for long long */

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'l') {
			char ch = *(format++);
			lng = 1;
			if (ch == 'l') {
				*(format++);
				lng = 2;
			}
		}

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if(*format == 'x'){
			format++;
			
			if (2 == lng)
                ulli2a(va_arg(parameters, unsigned long long int), &p);
			//else if (1 == lng)
				 //uli2a(va_arg(parameters, unsigned long int), &p);

			const char* str = p.bf;
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;

		} else if(*format == 'u'){
			format++;
			ui2a(va_arg(parameters, unsigned int), &p);
			const char* str = p.bf;
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
