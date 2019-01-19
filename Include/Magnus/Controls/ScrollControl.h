#pragma once
#ifndef MAGNUS_CONTROLS_SCROLLCONTROL_H
#define MAGNUS_CONTROLS_SCROLLCONTROL_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/ScrollBar.h"
#include "Magnus/Controls/VerticalScrollBar.h"
#include "Magnus/Controls/HorizontalScrollBar.h"

namespace Magnus
{
	namespace Controls
	{
		class ScrollControl : public Control
		{
		public:

			MAGNUS_CONTROL(ScrollControl, Control);

			virtual void Render(Magnus::Skin* skin);
			virtual void Layout(Magnus::Skin* skin);

			virtual void SetScroll(bool h, bool v);
			virtual void SetAutoHideBars(bool should) { m_bAutoHideBars = should; }
			virtual bool CanScrollH() { return m_bCanScrollH; }
			virtual bool CanScrollV() { return m_bCanScrollV; }
			virtual void OnChildBoundsChanged(Magnus::Rect oldChildBounds, Control* pChild);
			virtual void UpdateScrollBars();

			virtual void SetVScrollRequired(bool req);
			virtual void SetHScrollRequired(bool req);

			virtual void SetInnerSize(int w, int h);

			virtual void VBarMoved(Controls::Control* control);
			virtual void HBarMoved(Controls::Control* control);

			virtual bool OnMouseWheeled(int iDelta);

			virtual void ScrollToLeft();
			virtual void ScrollToRight();
			virtual void ScrollToTop();
			virtual void ScrollToBottom();

			virtual void Clear();

		protected:

			virtual bool ContentsAreDocked();

			bool m_bCanScrollH;
			bool m_bCanScrollV;

			bool m_bAutoHideBars;

			Controls::BaseScrollBar* m_VerticalScrollBar;
			Controls::BaseScrollBar* m_HorizontalScrollBar;
		};
	}
}

#endif