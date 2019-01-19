#pragma once
#ifndef MAGNUS_CONTROLS_SCROLLBAR_H
#define MAGNUS_CONTROLS_SCROLLBAR_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/ScrollThumb.h"
#include "Magnus/Controls/ScrollButton.h"
#include "Magnus/Magnus.h"

#define SCROLL_BUTTON_UP 0
#define SCROLL_BUTTON_LEFT 0
#define SCROLL_BUTTON_DOWN 1
#define SCROLL_BUTTON_RIGHT 1
#define NUDGE_DIST 10

namespace Magnus
{
	namespace Controls
	{
		class BaseScrollBar : public Control
		{
		public:

			MAGNUS_CONTROL(BaseScrollBar, Control);

			virtual void Render(Magnus::Skin* skin);

			virtual void SetBarSize(int size) = 0;
			virtual int GetBarSize() = 0;
			virtual int GetBarPos() = 0;

			virtual void OnBarMoved(Controls::Control* control);
			virtual void OnMouseClickLeft(int /*x*/, int /*y*/, bool /*bDown*/) {}

			virtual void ScrollToLeft() {}
			virtual void ScrollToRight() {}
			virtual void ScrollToTop() {}
			virtual void ScrollToBottom() {}

			virtual float GetNudgeAmount() { return m_fNudgeAmount / m_fContentSize; }
			virtual void SetNudgeAmount(float nudge) { m_fNudgeAmount = nudge; }

			virtual void BarMovedNotification();

			virtual float CalculateScrolledAmount() { return 0; }
			virtual int CalculateBarSize() { return 0; }
			virtual bool SetScrolledAmount(float amount, bool forceUpdate);

			virtual void SetContentSize(float size);
			virtual void SetViewableContentSize(float size);

			virtual int GetButtonSize() { return 0; }
			virtual float GetScrolledAmount() { return m_fScrolledAmount; }

			virtual bool IsHorizontal() { return false; }

			Magnus::Event::Caller onBarMoved;

		protected:

			ControlsInternal::ScrollButton* m_ScrollButton[2];
			ControlsInternal::ScrollThumb* m_Thumb;

			bool m_bDepressed;
			float m_fScrolledAmount;
			float m_fContentSize;
			float m_fViewableContentSize;
			float m_fNudgeAmount;
		};
	}
}

#endif