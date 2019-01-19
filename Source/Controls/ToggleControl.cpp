#include "Magnus\Controls\ToggleControl.h"

using namespace Magnus;
using namespace Magnus::Controls;
using namespace Magnus::ControlsInternal;

MAGNUS_CONTROL_CONSTRUCTOR(ToggleBar)
{
	m_bOn = false;
	SetTarget(this);
	RestrictToParent(true);
}

void ToggleBar::OnMouseClickLeft(int x, int y, bool bDown)
{
	GetParent()->OnMouseClickLeft(x, y, bDown);
	BaseClass::OnMouseClickLeft(x, y, bDown);
}

void ToggleBar::Render(Magnus::Skin* skin)
{
	skin->DrawToggleBar(GetRenderBounds(), IsDepressed(), IsHovered(), IsDisabled());
}

MAGNUS_CONTROL_CONSTRUCTOR(ToggleControl)
{
	m_Bar = new ToggleBar(this);
	m_Bar->onDragged.Add(this, &ToggleControl::OnMoved);
	SetBounds(0, 0, 32, 15);
	m_Bar->SetSize(15, Height());
	SetTabable(true);
}

void ToggleControl::SetChecked(bool bChecked)
{
	BaseClass::SetChecked(bChecked);
	m_Bar->SetOn(bChecked);
	UpdateBar();
}

void ToggleControl::OnMoved(Controls::Control* /*control*/)
{
	m_Bar->SetPos(m_Bar->X(), 0);
}

void ToggleControl::UpdateBar()
{
	if (IsChecked())
		m_Bar->SetPos(Width() - m_Bar->Width(), 0);
	else
		m_Bar->SetPos(0, 0);
}

void ToggleControl::OnMouseClickLeft(int x, int y, bool bDown)
{
	if (bDown) { return; }

	Magnus::Point p = CanvasPosToLocal(Magnus::Point(x, y));

	if (p.x < Width() * 0.5)
		SetChecked(false);

	if (p.x >= Width() * 0.5)
		SetChecked(true);

	OnMoved(m_Bar);
}

void ToggleControl::OnBoundsChanged(Magnus::Rect oldBounds)
{
	BaseClass::OnBoundsChanged(oldBounds);
	UpdateBar();
}

void ToggleControl::Layout(Magnus::Skin* /*skin*/)
{
	m_Bar->SetSize(15, Height());
}

void ToggleControl::Render(Magnus::Skin* skin)
{
	skin->DrawToggleTrack(GetRenderBounds(), IsChecked(), IsDisabled());
}