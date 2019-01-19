#pragma once
#ifndef MAGNUS_PLATFORM_H
#define MAGNUS_PLATFORM_H

#include "Magnus/Structures.h"

namespace Magnus
{
	namespace Platform
	{
		//
		// Used by copy/paste
		//
		UnicodeString GetClipboardText();
		bool SetClipboardText(const UnicodeString & str);

		//
		// Needed for things like keyboard input
		//
		float GetTimeInSeconds();
	}
}

#endif