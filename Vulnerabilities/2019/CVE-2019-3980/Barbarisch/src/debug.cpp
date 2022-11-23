#include <stdio.h>
#include <stdarg.h> 

#include "debug.h"

FILE* outlogfile = stdout;
unsigned int verbosity = VERBOSITY;

//wrapper around printf to handles levels of verbosity
void DebugFprintf(FILE* f, unsigned int lvl, const char *format, ...)
{
#ifdef _DEBUG
	if (lvl <= verbosity) {
		if (f) {
			va_list args = NULL;
			va_start(args, format);

			vfprintf(f, format, args);

			va_end(args);
		}
	}
#endif
}

void DebugFwprintf(FILE* f, unsigned int lvl, const wchar_t *format, ...)
{
#ifdef _DEBUG
	if (lvl <= verbosity) {
		if (f) {
			va_list args = NULL;
			va_start(args, format);

			vfwprintf(f, format, args);

			va_end(args);
		}
	}
#endif
}

void displayRawData(unsigned char* buf, int len)
{
	for (int i = 0; i < len; i++) {
		if (i > 0 && i % 16 == 0) {
			DbgFprintf(outlogfile, PRINT_INFO3, "\n");
		}
		DbgFprintf(outlogfile, PRINT_INFO3, "%.2x ", buf[i] & 0xFF);
	}
	DbgFprintf(outlogfile, PRINT_INFO3, "\n");
}