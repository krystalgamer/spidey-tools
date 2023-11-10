#include <stdio.h>
#include <stdarg.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void DebugPrintf(const char* format, ...) {

	static char output_string[1024];
	memset(output_string, 0, sizeof(output_string));

	va_list args;
	va_start(args, format);

	vsnprintf(output_string, sizeof(output_string), format, args);

	OutputDebugStringA(output_string);
	printf(output_string);
}

void DebugPrintfWithNewLine(const char* format, ...) {

	static char output_string[1024];
	memset(output_string, 0, sizeof(output_string));

	va_list args;
	va_start(args, format);

	vsnprintf(output_string, sizeof(output_string), format, args);

	OutputDebugStringA(output_string);
	printf(output_string);
	puts("");
}

void DebugPuts(const char* str) {
	OutputDebugStringA(str);
	puts(str);
}