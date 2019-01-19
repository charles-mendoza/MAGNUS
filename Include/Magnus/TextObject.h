#pragma once
#ifndef MAGNUS_TEXTOBJECT_H
#define MAGNUS_TEXTOBJECT_H

#include "Magnus/Utility.h"

namespace Magnus
{
	/*
		
		TextObject can be either a UnicodeString or a String

		Just to make things easier instead of having a function taking both.

	*/
	class TextObject
	{
	public:

		TextObject() {}

		TextObject(const Magnus::UnicodeString & text)
		{
			m_Unicode = text;
			m_String = Magnus::Utility::UnicodeToString(m_Unicode);
		}

		TextObject(const wchar_t* text)
		{
			m_Unicode = text;
			m_String = Magnus::Utility::UnicodeToString(m_Unicode);
		}

		TextObject(const Magnus::String & text)
		{
			m_String = text;
			m_Unicode = Magnus::Utility::StringToUnicode(m_String);
		}

		TextObject(const char* text)
		{
			m_String = text;
			m_Unicode = Magnus::Utility::StringToUnicode(m_String);
		}

		operator const Magnus::String & () { return m_String; }
		operator const Magnus::UnicodeString & () { return m_Unicode; }

		void operator = (const Magnus::UnicodeString & text)
		{
			m_Unicode = text;
			m_String = Magnus::Utility::UnicodeToString(m_Unicode);
		}

		void operator = (const wchar_t* text)
		{
			m_Unicode = text;
			m_String = Magnus::Utility::UnicodeToString(m_Unicode);
		}

		void operator = (const Magnus::String & text)
		{
			m_String = text;
			m_Unicode = Magnus::Utility::StringToUnicode(m_String);
		}

		void operator = (const char* text)
		{
			m_String = text;
			m_Unicode = Magnus::Utility::StringToUnicode(m_String);
		}

		bool operator == (const TextObject & to) const
		{
			return m_Unicode == to.m_Unicode;
		}

		const wchar_t* c_str() const
		{
			return m_Unicode.c_str();
		}

		wchar_t operator [] (int index) const
		{
			return m_Unicode[index];
		}

		const Magnus::String & Get() const
		{
			return m_String;
		}

		const Magnus::UnicodeString & GetUnicode() const
		{
			return m_Unicode;
		}

		size_t length() const { return m_Unicode.length(); }
		bool empty() const { return m_Unicode.empty(); }

		Magnus::UnicodeString	m_Unicode;
		Magnus::String			m_String;
	};
}

#endif