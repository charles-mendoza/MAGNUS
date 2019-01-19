#pragma once
#ifndef MAGNUS_CONTROLS_VERTICALSCROLLBAR_H
#define MAGNUS_CONTROLS_VERTICALSCROLLBAR_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/ScrollBar.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace Controls
	{
		class VerticalScrollBar : public BaseScrollBar
		{
		public:

			MAGNUS_CONTROL(VerticalScrollBar, BaseScrollBar);

			virtual void Layout(Magnus::Skin* skin);

			virtual void OnMouseClickLeft(int x, int y, bool bDown);
			virtual void OnBarMoved(Controls::Control* control);

			virtual int GetBarSize() { return m_Thumb->Height(); }
			virtual int GetBarPos() { return m_Thumb->Y() - Width(); }
			virtual void SetBarSize(int size) { m_Thumb->SetHeight(size); }
			virtual int GetButtonSize() { return Width(); }

			virtual void ScrollToTop();
			virtual void ScrollToBottom();
			virtual void NudgeUp(Control* control);
			virtual void NudgeDown(Control* control);
			virtual float GetNudgeAmount();

			virtual float CalculateScrolledAmount();
			virtual bool SetScrolledAmount(float amount, bool forceUpdate);
		};
	}
}

#endif