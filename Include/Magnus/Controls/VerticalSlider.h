#pragma once
#ifndef MAGNUS_CONTROLS_VERTICALSLIDER_H
#define MAGNUS_CONTROLS_VERTICALSLIDER_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Controls/Dragger.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/Slider.h"

namespace Magnus
{
	namespace Controls
	{
		class VerticalSlider : public Slider
		{
			MAGNUS_CONTROL(VerticalSlider, Slider);

			virtual void Layout(Magnus::Skin* skin);
			virtual void Render(Magnus::Skin* skin);

			virtual float CalculateValue();
			virtual void UpdateBarFromValue();
			virtual void OnMouseClickLeft(int x, int y, bool bDown);
		};
	}
}

#endif