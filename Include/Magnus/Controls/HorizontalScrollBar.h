#pragma once
#ifndef MAGNUS_CONTROLS_HORIZONTALSCROLLBAR_H
#define MAGNUS_CONTROLS_HORIZONTALSCROLLBAR_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/Dragger.h"
#include "Magnus/Controls/ScrollBar.h"

namespace Magnus
{
	namespace Controls
	{
		class HorizontalScrollBar : public BaseScrollBar
		{
		public:

			MAGNUS_CONTROL(HorizontalScrollBar, BaseScrollBar);

			virtual void Layout(Magnus::Skin* skin);

			virtual void OnMouseClickLeft(int x, int y, bool bDown);
			virtual void OnBarMoved(Controls::Control* control);

			virtual int GetBarSize() { return m_Thumb->Width(); }
			virtual int GetBarPos() { return m_Thumb->X() - Height(); }
			virtual void SetBarSize(int size) { m_Thumb->SetWidth(size); }
			virtual int GetButtonSize() { return Height(); }

			virtual void ScrollToLeft();
			virtual void ScrollToRight();
			virtual void NudgeLeft(Control* control);
			virtual void NudgeRight(Control* control);
			virtual float GetNudgeAmount();

			virtual float CalculateScrolledAmount();
			virtual bool SetScrolledAmount(float amount, bool forceUpdate);

			virtual bool IsHorizontal() { return true; }
		};
	}
}

#endif