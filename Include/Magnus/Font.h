#pragma once
#ifndef MAGNUS_FONT_H
#define MAGNUS_FONT_H

#include <string>
#include <list>

#include "Magnus/Renderer.h"

namespace Magnus
{
	/*

	Defines a particular format for text, including font face, size, and style attributes.

	*/
	struct Font
	{
		typedef std::list<Font*> List;

		Font()
		{
			data = NULL;
			facename = L"Arial";
			size = 10.0f;
			realsize = 0.0f;
			bold = false;
		}

		UnicodeString facename;
		float size;
		bool bold;

		// This should be set by the renderer
		// if it tries to use a font where it's
		// NULL.
		void* data;
		
		// This is the real font size, after it's
		// been scaled by Render->Scale()
		float realsize;
	};
}

#endif