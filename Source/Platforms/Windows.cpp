#include "Magnus/Platform.h"

#include <Windows.h>

using namespace Magnus;
using namespace Magnus::Platform;

Magnus::UnicodeString Magnus::Platform::GetClipboardText()
{
	if (!OpenClipboard(NULL)) { return L""; }

	HANDLE hData = GetClipboardData(CF_UNICODETEXT);

	if (hData == NULL)
	{
		CloseClipboard();
		return L"";
	}

	wchar_t* buffer = (wchar_t*)GlobalLock(hData);
	UnicodeString str = buffer;
	GlobalUnlock(hData);
	CloseClipboard();
	return str;
}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996) // _CRT_SECURE_NO_WARNINGS
#endif

bool Magnus::Platform::SetClipboardText(const Magnus::UnicodeString & str)
{
	if (!OpenClipboard(NULL)) { return false; }

	EmptyClipboard();
	// Create a buffer to hold the string
	size_t iDataSize = (str.length() + 1) * sizeof(wchar_t);
	HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE, iDataSize);
	// Copy the string into the buffer
	wchar_t* buffer = (wchar_t*)GlobalLock(clipbuffer);
	wcscpy(buffer, str.c_str());
	GlobalUnlock(clipbuffer);
	// Place it on the clipboard
	SetClipboardData(CF_UNICODETEXT, clipbuffer);
	CloseClipboard();
	return true;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

double GetPerformanceFrequency()
{
	static double Frequency = 0.0f;

	if (Frequency == 0.0f)
	{
		__int64 perfFreq;
		QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq);
		Frequency = 1.0 / (double)perfFreq;
	}

	return Frequency;
}

float Magnus::Platform::GetTimeInSeconds()
{
	static float fCurrentTime = 0.0f;
	static __int64 iLastTime = 0;
	__int64 thistime;
	QueryPerformanceCounter((LARGE_INTEGER*)&thistime);
	float fSecondsDifference = (float)((double)(thistime - iLastTime) * GetPerformanceFrequency());

	if (fSecondsDifference > 0.1f) { fSecondsDifference = 0.1f; }

	fCurrentTime += fSecondsDifference;
	iLastTime = thistime;
	return fCurrentTime;
}