#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

int putchar(char ic) {
#if defined(__is_libk)
	char c = (char) ic;
	if(c == '\n'){
		set_term_col(0);
		terminal_setCursorPos(0, ++(*get_curr_term_row()));
	}else{
		terminal_write(&c, sizeof(c));
	}
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}
