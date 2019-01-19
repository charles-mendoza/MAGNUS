#include "Magnus/Controls/ScrollBar.h"
#include "Magnus/Controls/VerticalScrollBar.h"
#include "Magnus/Utility.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(VerticalScrollBar)
{
	m_ScrollButton[SCROLL_BUTTON_UP]->SetDirectionUp();
	m_ScrollButton[SCROLL_BUTTON_UP]->onPress.Add(this, &VerticalScrollBar::NudgeUp);
	m_ScrollButton[SCROLL_BUTTON_DOWN]->SetDirectionDown();
	m_ScrollButton[SCROLL_BUTTON_DOWN]->onPress.Add(this, &VerticalScrollBar::NudgeDown);
	m_Thumb->SetVertical();
	m_Thumb->onDragged.Add(this, &VerticalScrollBar::OnBarMoved);
}

void VerticalScrollBar::Layout(Magnus::Skin* skin)
{
	BaseClass::Layout(skin);
	m_ScrollButton[SCROLL_BUTTON_UP]->Dock(Pos::Top);
	m_ScrollButton[SCROLL_BUTTON_UP]->SetHeight(Width());
	m_ScrollButton[SCROLL_BUTTON_DOWN]->Dock(Pos::Bottom);
	m_ScrollButton[SCROLL_BUTTON_DOWN]->SetHeight(Width());
	m_Thumb->SetWidth(GetButtonSize());
	m_Thumb->SetPadding(Padding(0, GetButtonSize(), 0, GetButtonSize()));
	float barHeight = (m_fViewableContentSize / m_fContentSize) * (Height() - GetButtonSize());

	if (barHeight < GetButtonSize() * 0.5)
	{
		barHeight = GetButtonSize() * 0.5;
	}

	m_Thumb->SetHeight(barHeight);
	m_Thumb->SetHidden(Height() - (GetButtonSize() * 2) <= barHeight);

	if (Hidden())
	{
		SetScrolledAmount(0, true);
	}

	// Based on our last scroll amount, produce a position for the bar
	if (!m_Thumb->IsDepressed())
	{
		SetScrolledAmount(GetScrolledAmount(), true);
	}
}

void VerticalScrollBar::ScrollToTop()
{
	SetScrolledAmount(0, true);
}

void VerticalScrollBar::ScrollToBottom()
{
	SetScrolledAmount(1, true);
}

void VerticalScrollBar::NudgeUp(Control* /*control*/)
{
	if (!IsDisabled())
	{
		SetScrolledAmount(GetScrolledAmount() - GetNudgeAmount(), true);
	}
}

void VerticalScrollBar::NudgeDown(Control* /*control*/)
{
	if (!IsDisabled())
	{
		SetScrolledAmount(GetScrolledAmount() + GetNudgeAmount(), true);
	}
}

float VerticalScrollBar::GetNudgeAmount()
{
	if (m_bDepressed)
	{
		return m_fViewableContentSize / m_fContentSize;
	}
	else
	{
		return BaseClass::GetNudgeAmount();
	}
}

void VerticalScrollBar::OnMouseClickLeft(int x, int y, bool bDown)
{
	if (bDown)
	{
		m_bDepressed = true;
		Magnus::MouseFocus = this;
	}
	else
	{
		Magnus::Point clickPos = CanvasPosToLocal(Magnus::Point(x, y));

		if (clickPos.y < m_Thumb->Y())
		{
			NudgeUp(this);
		}
		else if (clickPos.y > m_Thumb->Y() + m_Thumb->Height())
		{
			NudgeDown(this);
		}

		m_bDepressed = false;
		Magnus::MouseFocus = NULL;
	}
}

float VerticalScrollBar::CalculateScrolledAmount()
{
	return (float)(m_Thumb->Y() - GetButtonSize()) / (float)(Height() - m_Thumb->Height() - (GetButtonSize() * 2));
}

bool VerticalScrollBar::SetScrolledAmount(float amount, bool forceUpdate)
{
	amount = Utility::Clamp(amount, 0.f, 1.f);

	if (!BaseClass::SetScrolledAmount(amount, forceUpdate)) { return false; }

	if (forceUpdate)
	{
		int newY = GetButtonSize() + (amount * ((Height() - m_Thumb->Height()) - (GetButtonSize() * 2)));
		m_Thumb->MoveTo(m_Thumb->X(), newY);
	}

	return true;
}

void VerticalScrollBar::OnBarMoved(Controls::Control* control)
{
	if (m_Thumb->IsDepressed())
	{
		SetScrolledAmount(CalculateScrolledAmount(), false);
		BaseClass::OnBarMoved(control);
	}
	else
	{
		InvalidateParent();
	}
}