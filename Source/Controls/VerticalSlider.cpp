#include "Magnus/Controls/Slider.h"
#include "Magnus/Controls/VerticalSlider.h"

using namespace Magnus;
using namespace Magnus::Controls;
using namespace Magnus::ControlsInternal;

MAGNUS_CONTROL_CONSTRUCTOR(VerticalSlider)
{
	m_SliderBar->SetHorizontal(false);
	Layout(GetSkin()); // Strangely, this fixes the SliderBar's position
}

float VerticalSlider::CalculateValue()
{
	return 1 - (float)m_SliderBar->Y() / (float)(Height() - m_SliderBar->Height());
}

void VerticalSlider::UpdateBarFromValue()
{
	m_SliderBar->MoveTo(m_SliderBar->X(), (Height() - m_SliderBar->Height()) * (1 - m_fValue));
}

void VerticalSlider::OnMouseClickLeft(int x, int y, bool bDown)
{
	m_SliderBar->MoveTo(m_SliderBar->X(), CanvasPosToLocal(Magnus::Point(x, y)).y - m_SliderBar->Height() * 0.5);
	m_SliderBar->OnMouseClickLeft(x, y, bDown);
	OnMoved(m_SliderBar);
}

void VerticalSlider::Layout(Magnus::Skin* /*skin*/)
{
	m_SliderBar->SetSize(Width(), 15);
}

void VerticalSlider::Render(Magnus::Skin* skin)
{
	skin->DrawSlider(GetRenderBounds(), false, m_bClampToNotches ? m_iNumNotches : 0, m_SliderBar->Height());
}