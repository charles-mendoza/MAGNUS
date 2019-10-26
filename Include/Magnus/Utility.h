#pragma once
#ifndef MAGNUS_UTILITY_H
#define MAGNUS_UTILITY_H

#include <sstream>
#include <vector>

#include "Magnus/Structures.h"

namespace Magnus
{
	class TextObject;

	namespace Utility
	{
		template <typename T>
		inline T Min(T a, T b)
		{
			return a < b ? a : b;
		}

		template <typename T>
		inline T Max(T a, T b)
		{
			return a > b ? a : b;
		}

		template <typename T>
		inline T Clamp(T current, T vmin, T vmax)
		{
			if (current >= vmax) { return vmax; }

			if (current <= vmin) { return vmin; }

			return current;
		}

		template <typename T, typename T2>
		inline T Approach(T fCurrent, T fTarget, T2 fDelta)
		{
			if (fCurrent < fTarget)
			{
				fCurrent += fDelta;

				if (fCurrent > fTarget) { return fTarget; }
			}
			else if (fCurrent > fTarget)
			{
				fCurrent -= fDelta;

				if (fCurrent < fTarget) { return fTarget; }
			}

			return fCurrent;
		}

		inline Magnus::String UnicodeToString(const Magnus::UnicodeString & strIn)
		{
			if (!strIn.length()) { return ""; }

			Magnus::String strOut(strIn.length(), (char)0);
			std::use_facet<std::ctype<wchar_t> >(std::locale()).narrow(&strIn[0], &strIn[0] + strIn.length(), ' ', &strOut[0]);
			return strOut;
		}

		inline Magnus::UnicodeString StringToUnicode(const Magnus::String & strIn)
		{
			if (!strIn.length()) { return L""; }

			Magnus::UnicodeString strOut(strIn.length(), (wchar_t)0);
			std::use_facet<std::ctype<wchar_t> >(std::locale()).widen(&strIn[0], &strIn[0] + strIn.length(), &strOut[0]);
			return strOut;
		}

		inline float DecToPercent(float dec, float min, float max)
		{
			if (max < min)
				return 100.0f - ((dec - max) * 100.0f) / (min - max);

			return ((dec - min) * 100.0f) / (max - min);
		}

		inline float PercentToDec(float percent, float min, float max)
		{
			if (max < min)
				return ((min + max) - (percent * (min - max) * 0.01f) - max);

			return (((percent * (max - min)) * 0.01f) + min);
		}

		template <class T>
		String ToString(const T & object)
		{
			std::ostringstream os;
			os << object;
			return os.str();
		}

		Magnus::String Format(const char* format, ...);
		Magnus::UnicodeString FormatUnicode(const wchar_t* format, ...);

		namespace Strings
		{
			void ToUpper(Magnus::UnicodeString & str);
			void Strip(Magnus::UnicodeString & str, const Magnus::UnicodeString & chars);

			float ToFloat(const Magnus::TextObject & str);
		}

		namespace Colors
		{
			Magnus::Color LerpColor(Magnus::Color & toColor, Magnus::Color & fromColor, float amount);

			Magnus::Color HSVToColor(float h, float s, float v);
			Magnus::HSV RGBToHSV(int r, int g, int b);
		}
	}
}

#endif