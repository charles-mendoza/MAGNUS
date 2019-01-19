#include "Magnus\Controls\ScrollButton.h"

using namespace Magnus;
using namespace Magnus::Controls;
using namespace Magnus::ControlsInternal;

MAGNUS_CONTROL_CONSTRUCTOR(ScrollButton)
{
	SetDirectionUp();
}

void ScrollButton::SetDirectionUp()
{
	m_iDirection = Pos::Top;
}

void ScrollButton::SetDirectionDown()
{
	m_iDirection = Pos::Bottom;
}

void ScrollButton::SetDirectionLeft()
{
	m_iDirection = Pos::Left;
}

void ScrollButton::SetDirectionRight()
{
	m_iDirection = Pos::Right;
}

void ScrollButton::Render(Magnus::Skin* skin)
{
	skin->DrawScrollButton(GetRenderBounds(), m_iDirection, IsDepressed(), IsHovered(), IsDisabled());
}