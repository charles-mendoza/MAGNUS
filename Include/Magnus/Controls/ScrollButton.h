#pragma once
#ifndef MAGNUS_CONTROLS_SCROLLBARBUTTON_H
#define MAGNUS_CONTROLS_SCROLLBARBUTTON_H

#include "Magnus/Controls/Button.h"

namespace Magnus
{
	namespace ControlsInternal
	{
		class ScrollButton : public Controls::Button
		{
		public:

			MAGNUS_CONTROL(ScrollButton, Controls::Button);

			void Render(Magnus::Skin* skin);

			void SetDirectionUp();
			void SetDirectionDown();
			void SetDirectionLeft();
			void SetDirectionRight();

		protected:

			int m_iDirection;
		};
	}
}

#endif