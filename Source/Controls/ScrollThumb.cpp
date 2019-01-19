#include "Magnus/Controls/ScrollThumb.h"

using namespace Magnus;
using namespace Magnus::Controls;
using namespace Magnus::ControlsInternal;

MAGNUS_CONTROL_CONSTRUCTOR(ScrollThumb)
{
	RestrictToParent(true);
	SetTarget(this);
}

void ScrollThumb::Render(Magnus::Skin* skin)
{
	skin->DrawScrollThumb(GetRenderBounds(), m_bHorizontal, IsDepressed(), IsHovered(), IsDisabled());
	BaseClass::Render(skin);
}

void ScrollThumb::OnMouseMoved(int x, int y, int deltaX, int deltaY)
{
	BaseClass::OnMouseMoved(x, y, deltaX, deltaY);

	if (!m_bDepressed) { return; }

	InvalidateParent();
}

void ScrollThumb::OnMouseClickLeft(int x, int y, bool bDown)
{
	BaseClass::OnMouseClickLeft(x, y, bDown);
	InvalidateParent();
}

void ScrollThumb::Layout(Magnus::Skin* skin)
{
	if (!GetParent()) { return; }

	// Move to our current position to force clamping - is this a hack?
	MoveTo(X(), Y());
}

void ScrollThumb::MoveTo(int x, int y)
{
	BaseClass::MoveTo(x, y);
}