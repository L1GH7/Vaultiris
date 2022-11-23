#include <stdlib.h>
#include <substrate.h>

// CVE-2014-4377

extern "C" void *x_calloc(size_t times, size_t size);

MSHook(void *, x_calloc, size_t times, size_t size)
{
	unsigned int total = (size * times + 31) & 0xFFFFFFF0;
	unsigned long long check = size * (unsigned long long)times;

	if ((size | times) < 0x10000 || total >= check) {
		return _x_calloc(times, size);
	} else {
		NSLog(@"[CVE-2014-4377]: Integer overflow detected in x_calloc. Returning NULL.");
		return NULL;
	}
}

// CVE-2014-4378

extern "C" bool CGPDFArrayGetStream(CGPDFArrayRef array, size_t index, CGPDFStreamRef *value);

void *_cg_build_colorspace;
int (*_CGPDFArrayGetCount)(CGPDFArrayRef);
bool (*_CGPDFArrayGetInteger)(CGPDFArrayRef array, size_t index, CGPDFInteger *integer);
bool (*_CGPDFArrayGetName)(CGPDFArrayRef array, size_t index, const char **value);

MSHook(bool, CGPDFArrayGetStream, CGPDFArrayRef array, size_t index, CGPDFStreamRef *stream)
{
	CGPDFInteger N = 0;
	const char *name = NULL;

	if (index == 3 &&
		((unsigned)__builtin_return_address(0) - (unsigned)_cg_build_colorspace) < 0x1000 &&
		_CGPDFArrayGetCount(array) == 4 &&
		_CGPDFArrayGetName(array, 0, &name) != 0 &&
		strcmp(name, "Indexed") == 0 &&
		_CGPDFArrayGetInteger(array, 2, &N) != 0 &&
		N > 0xFF) {

		NSLog(@"CVE-2014-4377 or CVE-2014-4378 are most likely trying to be exploited. You have been successfully protected.\n");
		*stream = NULL;
		return 0;
	}

	return _CGPDFArrayGetStream(array, index, stream);
}

MSInitialize
{
	// Only proceed if the process uses CoreGraphics.
	MSImageRef image = MSGetImageByName("/System/Library/Frameworks/CoreGraphics.framework/CoreGraphics");
	if (!image)
		return;

	// CVE-2014-4377
	MSHookFunction(MSFindSymbol(image, "_x_calloc"), MSHake(x_calloc));

	// CVE-2014-4378
	MSHookFunction(MSFindSymbol(image, "_CGPDFArrayGetStream"), MSHake(CGPDFArrayGetStream));

	_cg_build_colorspace = MSFindSymbol(image, "_cg_build_colorspace");
	_CGPDFArrayGetCount = (int (*)(CGPDFArrayRef))MSFindSymbol(image, "_CGPDFArrayGetCount");
	_CGPDFArrayGetInteger = (bool (*)(CGPDFArrayRef, size_t, CGPDFInteger *))MSFindSymbol(image, "_CGPDFArrayGetInteger");
	_CGPDFArrayGetName = (bool (*)(CGPDFArrayRef, size_t, const char **))MSFindSymbol(image, "_CGPDFArrayGetName");
}
