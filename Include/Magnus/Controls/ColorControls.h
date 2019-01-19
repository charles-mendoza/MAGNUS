#pragma once
#ifndef MAGNUS_CONTROLS_CONTROLCONTROLS_H
#define MAGNUS_CONTROLS_CONTROLCONTROLS_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace Controls
	{
		class ColorLerpBox : public Controls::Control
		{
		public:

			MAGNUS_CONTROL(ColorLerpBox, Controls::Control);

			virtual void Render(Magnus::Skin* skin);

			Magnus::Color GetColorAtPos(int x, int y);
			void SetColor(Magnus::Color color, bool onlyHue = true, bool bDoEvents = true);

			virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY);
			virtual void OnMouseClickLeft(int x, int y, bool bDown);

			Magnus::Color GetSelectedColor();

			Event::Caller onSelectionChanged;

		protected:

			Magnus::Point cursorPos;
			bool m_bDepressed;
			int m_Hue;
		};

		class ColorSlider : public Controls::Control
		{
		public:

			MAGNUS_CONTROL(ColorSlider, Controls::Control);

			virtual void Render(Magnus::Skin* skin);

			virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY);
			virtual void OnMouseClickLeft(int x, int y, bool bDown);

			Magnus::Color GetSelectedColor();
			Magnus::Color GetColorAtHeight(int y);

			void SetColor(Magnus::Color color, bool bDoEvents = true);

			Event::Caller onSelectionChanged;

		protected:

			int m_iSelectedDist;
			bool m_bDepressed;
		};
	}
}

#endif