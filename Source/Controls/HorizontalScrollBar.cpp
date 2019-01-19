#include "Magnus/Controls/ScrollBar.h"
#include "Magnus/Controls/HorizontalScrollBar.h"
#include "Magnus/Utility.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(HorizontalScrollBar)
{
	m_ScrollButton[SCROLL_BUTTON_LEFT]->SetDirectionLeft();
	m_ScrollButton[SCROLL_BUTTON_LEFT]->onPress.Add(this, &HorizontalScrollBar::NudgeLeft);
	m_ScrollButton[SCROLL_BUTTON_RIGHT]->SetDirectionRight();
	m_ScrollButton[SCROLL_BUTTON_RIGHT]->onPress.Add(this, &HorizontalScrollBar::NudgeRight);
	m_Thumb->SetHorizontal();
	m_Thumb->onDragged.Add(this, &HorizontalScrollBar::OnBarMoved);
}

void HorizontalScrollBar::Layout(Magnus::Skin* skin)
{
	BaseClass::Layout(skin);
	m_ScrollButton[SCROLL_BUTTON_LEFT]->SetWidth(Height());
	m_ScrollButton[SCROLL_BUTTON_LEFT]->Dock(Pos::Left);
	m_ScrollButton[SCROLL_BUTTON_RIGHT]->SetWidth(Height());
	m_ScrollButton[SCROLL_BUTTON_RIGHT]->Dock(Pos::Right);
	m_Thumb->SetHeight(GetButtonSize());
	m_Thumb->SetPadding(Padding(GetButtonSize(), 0, GetButtonSize(), 0));
	float barWidth = (m_fViewableContentSize / m_fContentSize) * (Width() - GetButtonSize());

	if (barWidth < GetButtonSize() * 0.5)
	{
		barWidth = GetButtonSize() * 0.5;
	}

	m_Thumb->SetWidth(barWidth);
	m_Thumb->SetHidden(Width() - (GetButtonSize() * 2) <= barWidth);

	// Based on our last scroll amount, produce a position for the bar
	if (!m_Thumb->IsDepressed())
	{
		SetScrolledAmount(GetScrolledAmount(), true);
	}
}

void HorizontalScrollBar::NudgeLeft(Control* /*control*/)
{
	if (!IsDisabled())
	{
		SetScrolledAmount(GetScrolledAmount() - GetNudgeAmount(), true);
	}
}

void HorizontalScrollBar::NudgeRight(Control* /*control*/)
{
	if (!IsDisabled())
	{
		SetScrolledAmount(GetScrolledAmount() + GetNudgeAmount(), true);
	}
}

void HorizontalScrollBar::ScrollToLeft()
{
	SetScrolledAmount(0, true);
}

void HorizontalScrollBar::ScrollToRight()
{
	SetScrolledAmount(1, true);
}

float HorizontalScrollBar::GetNudgeAmount()
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

void HorizontalScrollBar::OnMouseClickLeft(int x, int y, bool bDown)
{
	if (bDown)
	{
		m_bDepressed = true;
		Magnus::MouseFocus = this;
	}
	else
	{
		Magnus::Point clickPos = CanvasPosToLocal(Magnus::Point(x, y));

		if (clickPos.x < m_Thumb->X())
		{
			NudgeLeft(this);
		}
		else if (clickPos.x > m_Thumb->X() + m_Thumb->Width())
		{
			NudgeRight(this);
		}

		m_bDepressed = false;
		Magnus::MouseFocus = NULL;
	}
}

float HorizontalScrollBar::CalculateScrolledAmount()
{
	return (float)(m_Thumb->X() - GetButtonSize()) / (float)(Width() - m_Thumb->Width() - (GetButtonSize() * 2));
}

bool HorizontalScrollBar::SetScrolledAmount(float amount, bool forceUpdate)
{
	amount = Utility::Clamp(amount, 0.f, 1.f);

	if (!BaseClass::SetScrolledAmount(amount, forceUpdate)) { return false; }

	if (forceUpdate)
	{
		int newX = GetButtonSize() + (amount * ((Width() - m_Thumb->Width()) - (GetButtonSize() * 2)));
		m_Thumb->MoveTo(newX, m_Thumb->Y());
	}

	return true;
}

void HorizontalScrollBar::OnBarMoved(Controls::Control* control)
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