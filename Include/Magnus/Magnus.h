#pragma once
#ifndef MAGNUS_MAGNUS_H
#define MAGNUS_MAGNUS_H

#include <stdlib.h>
#include <memory.h>
#include <set>
#include <list>
#include <algorithm>

#include "Magnus/Structures.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Canvas.h"

namespace Magnus
{
	namespace Controls
	{
		class Control;
		class Canvas;
	}

	namespace Rendering
	{
		class Renderer;
	}

	namespace Colors
	{
		static const Color Black(255, 0, 0, 0);
		static const Color Red(255, 255, 0, 0);
		static const Color Yellow(255, 255, 255, 0);
		static const Color Orange(255, 255, 165, 0);
		static const Color Green(255, 0, 255, 0);
		static const Color Blue(255, 0, 0, 255);
		static const Color White(255, 255, 255, 255);
	}

	extern Controls::Control* HoveredControl;
	extern Controls::Control* KeyboardFocus;
	extern Controls::Control* MouseFocus;

	namespace Input
	{
		inline void Blur()
		{
			if (KeyboardFocus)
			{
				KeyboardFocus->Blur();
			}
		}
	}
}

#endif