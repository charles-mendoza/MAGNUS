#include "Magnus/Controls/ScrollBar.h"
#include "Magnus/Controls/ScrollButton.h"
#include "Magnus/Controls/ScrollThumb.h"

using namespace Magnus;
using namespace Magnus::Controls;
using namespace Magnus::ControlsInternal;

MAGNUS_CONTROL_CONSTRUCTOR(BaseScrollBar)
{
	m_ScrollButton[0] = new ScrollButton(this);
	m_ScrollButton[1] = new ScrollButton(this);
	m_Thumb = new ScrollThumb(this);
	SetBounds(0, 0, 15, 15);
	m_bDepressed = false;
	m_fScrolledAmount = 0;
	m_fContentSize = 0;
	m_fViewableContentSize = 0;
	SetNudgeAmount(20);
}

void BaseScrollBar::Render(Magnus::Skin* skin)
{
	skin->DrawScrollTrack(GetRenderBounds(), IsHorizontal(), m_bDepressed);
}

void BaseScrollBar::OnBarMoved(Controls::Control* control)
{
	onBarMoved.Call(this);
}

void BaseScrollBar::BarMovedNotification()
{
	OnBarMoved(this);
}

void BaseScrollBar::SetContentSize(float size)
{
	if (m_fContentSize != size)
	{
		Invalidate();
	}

	m_fContentSize = size;
}

void BaseScrollBar::SetViewableContentSize(float size)
{
	if (m_fViewableContentSize != size)
	{
		Invalidate();
	}

	m_fViewableContentSize = size;
}

bool BaseScrollBar::SetScrolledAmount(float amount, bool forceUpdate)
{
	if (m_fScrolledAmount == amount && !forceUpdate) { return false; }

	m_fScrolledAmount = amount;
	Invalidate();
	BarMovedNotification();
	return true;
}