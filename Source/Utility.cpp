#include "Magnus/Utility.h"
#include "Magnus/TextObject.h"

#include <stdarg.h>
#include <algorithm>

using namespace Magnus;

Magnus::String Magnus::Utility::Format(const char* format, ...)
{
	va_list args;
	int len;
	Magnus::String buffer;

	va_start(args, format);

	len = _vscprintf(format, args);

	if (len > 0)
	{
		buffer.resize(len + 1);
		vsprintf_s(&buffer[0], buffer.size(), format, args);
		buffer.resize(len);
	}

	va_end(args);

	return buffer;
}

Magnus::UnicodeString Magnus::Utility::FormatUnicode(const wchar_t* format, ...)
{
	va_list args;
	int len;
	Magnus::UnicodeString buffer;

	va_start(args, format);

	len = _vscwprintf(format, args);

	if (len > 0)
	{
		buffer.resize(len + 1);
		vswprintf_s(&buffer[0], buffer.size(), format, args);
		buffer.resize(len);
	}

	va_end(args);

	return buffer;
}

float Magnus::Utility::Strings::ToFloat(const Magnus::TextObject & str)
{
	return wcstod(str.GetUnicode().c_str(), NULL);
}

void Magnus::Utility::Strings::ToUpper(Magnus::UnicodeString & str)
{
	transform(str.begin(), str.end(), str.begin(), towupper);
}

void Magnus::Utility::Strings::Strip(Magnus::UnicodeString & str, const Magnus::UnicodeString & chars)
{
	Magnus::UnicodeString Source = str;
	str = L"";

	for (unsigned int i = 0; i < Source.length(); i++)
	{
		if (chars.find(Source[i]) != Magnus::UnicodeString::npos)
		{
			continue;
		}

		str += Source[i];
	}
}

Magnus::Color Magnus::Utility::Colors::LerpColor(Magnus::Color & toColor, Magnus::Color & fromColor, float amount)
{
	Magnus::Color colorDelta = toColor - fromColor;
	colorDelta.r *= amount;
	colorDelta.g *= amount;
	colorDelta.b *= amount;
	Magnus::Color newColor = fromColor + colorDelta;
	return newColor;
}

Magnus::Color Magnus::Utility::Colors::HSVToColor(float h, float s, float v)
{
	if (h < 0.0f) { h += 360.0f; }

	if (h > 360.0f) { h -= 360.0f; }

	s *= 255.0f;
	v *= 255.0f;
	float r, g, b;

	if (!h && !s)
	{
		r = g = b = v;
	}

	double min, max, delta, hue;
	max = v;
	delta = (max * s) / 255.0;
	min = max - delta;
	hue = h;

	if (h > 300 || h <= 60)
	{
		r = (int)max;

		if (h > 300)
		{
			g = (int)min;
			hue = (hue - 360.0) / 60.0;
			b = (int)((hue * delta - min) * -1);
		}
		else
		{
			b = (int)min;
			hue /= 60.0;
			g = (int)(hue * delta + min);
		}
	}
	else if (h > 60 && h < 180)
	{
		g = (int)max;

		if (h < 120)
		{
			b = (int)min;
			hue = (hue / 60.0 - 2.0) * delta;
			r = (int)(min - hue);
		}
		else
		{
			r = (int)min;
			hue = (hue / 60 - 2.0) * delta;
			b = (int)(min + hue);
		}
	}
	else
	{
		b = (int)max;

		if (h < 240)
		{
			r = (int)min;
			hue = (hue / 60.0 - 4.0) * delta;
			g = (int)(min - hue);
		}
		else
		{
			g = (int)min;
			hue = (hue / 60 - 4.0) * delta;
			r = (int)(min + hue);
		}
	}

	return Magnus::Color(255, r, g, b);
}

Magnus::HSV Magnus::Utility::Colors::RGBToHSV(int r, int g, int b)
{
	double min, max, delta, temp;
	min = Magnus::Utility::Min(r, Magnus::Utility::Min(g, b));
	max = Magnus::Utility::Max(r, Magnus::Utility::Max(g, b));
	delta = max - min;
	Magnus::HSV hsv;
	hsv.v = (int)max;

	if (!delta)
	{
		hsv.h = hsv.s = 0;
	}
	else
	{
		temp = delta / max;
		hsv.s = (int)(temp * 255);

		if (r == (int)max)
		{
			temp = (double)(g - b) / delta;
		}
		else if (g == (int)max)
		{
			temp = 2.0 + ((double)(b - r) / delta);
		}
		else
		{
			temp = 4.0 + ((double)(r - g) / delta);
		}

		temp *= 60;

		if (temp < 0)
		{
			temp += 360;
		}

		if (temp == 360)
		{
			temp = 0;
		}

		hsv.h = (int)temp;
	}

	hsv.s /= 255.0f;
	hsv.v /= 255.0f;
	return hsv;
}