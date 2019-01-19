#include "Magnus/Controls/Slider.h"
#include "Magnus/Controls/HorizontalSlider.h"

using namespace Magnus;
using namespace Magnus::Controls;
using namespace Magnus::ControlsInternal;

MAGNUS_CONTROL_CONSTRUCTOR(HorizontalSlider)
{
	m_SliderBar->SetHorizontal(true);
	Layout(GetSkin());
}

float HorizontalSlider::CalculateValue()
{
	return (float)m_SliderBar->X() / (float)(Width() - m_SliderBar->Width());
}

void HorizontalSlider::UpdateBarFromValue()
{
	m_SliderBar->MoveTo((Width() - m_SliderBar->Width()) * (m_fValue), m_SliderBar->Y());
}

void HorizontalSlider::OnMouseClickLeft(int x, int y, bool bDown)
{
	m_SliderBar->MoveTo(CanvasPosToLocal(Magnus::Point(x, y)).x - m_SliderBar->Width() * 0.5, m_SliderBar->Y());
	m_SliderBar->OnMouseClickLeft(x, y, bDown);
	OnMoved(m_SliderBar);
}

void HorizontalSlider::Layout(Magnus::Skin* /*skin*/)
{
	m_SliderBar->SetSize(15, Height());
}

void HorizontalSlider::Render(Magnus::Skin* skin)
{
	skin->DrawSlider(GetRenderBounds(), true, m_bClampToNotches ? m_iNumNotches : 0, m_SliderBar->Width());
}