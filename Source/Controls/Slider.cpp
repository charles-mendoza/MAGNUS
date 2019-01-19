#include <math.h>

#include "Magnus/Controls/Slider.h"
#include "Magnus/Utility.h"

using namespace Magnus;
using namespace Magnus::Controls;
using namespace Magnus::ControlsInternal;

MAGNUS_CONTROL_CONSTRUCTOR(SliderBar)
{
	SetTarget(this);
	RestrictToParent(true);
}

void SliderBar::Render(Magnus::Skin* skin)
{
	skin->DrawSlideButton(GetRenderBounds(), IsHorizontal(), IsDepressed(), IsHovered(), IsDisabled());
}

MAGNUS_CONTROL_CONSTRUCTOR(Slider)
{
	SetBounds(0, 0, 100, 32);
	m_SliderBar = new SliderBar(this);
	m_SliderBar->onDragged.Add(this, &Slider::OnMoved);
	m_fMin = 0.0f;
	m_fMax = 10.0f;
	m_bClampToNotches = false;
	m_iNumNotches = 5;
	m_fValue = 0.0f;
	SetTabable(true);
}

void Slider::OnMoved(Controls::Control* /*control*/)
{
	SetValueInternal(CalculateValue());
}

void Slider::Layout(Magnus::Skin* skin)
{
	BaseClass::Layout(skin);
}

float Slider::CalculateValue()
{
	return 0;
}

void Slider::SetFloatValue(float val, bool /*forceUpdate*/)
{
	val = Utility::Clamp(val, m_fMin, m_fMax);

	// Normalize value
	val = (val - m_fMin) / (m_fMax - m_fMin);
	SetValueInternal(val);
	Redraw();
}

void Slider::SetValueInternal(float val)
{
	if (m_bClampToNotches)
	{
		val = floorf((val * (float)m_iNumNotches) + 0.5f);
		val /= (float)m_iNumNotches;
	}

	if (m_fValue != val)
	{
		m_fValue = val;
		onValueChanged.Call(this);
	}

	UpdateBarFromValue();
}

float Slider::GetFloatValue()
{
	return m_fMin + (m_fValue * (m_fMax - m_fMin));
}

void Slider::SetRange(float fMin, float fMax)
{
	m_fMin = fMin;
	m_fMax = fMax;
}

void Slider::RenderFocus(Magnus::Skin* skin)
{
	if (Magnus::KeyboardFocus != this) { return; }

	if (!IsTabable()) { return; }

	skin->DrawKeyboardHighlight(GetRenderBounds(), 0);
}

void Slider::OnBoundsChanged(Magnus::Rect oldBounds)
{
	BaseClass::OnBoundsChanged(oldBounds);
	UpdateBarFromValue();
}