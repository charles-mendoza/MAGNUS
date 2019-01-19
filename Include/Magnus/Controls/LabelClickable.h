#pragma once
#ifndef MAGNUS_CONTROLS_LABELCLICKABLE_H
#define MAGNUS_CONTROLS_LABELCLICKABLE_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"

namespace Magnus
{
	namespace Controls
	{
		class LabelClickable : public Button
		{
		public:

			MAGNUS_CONTROL(LabelClickable, Button);

			virtual void Render(Magnus::Skin* skin);
		};
	}
}

#endif