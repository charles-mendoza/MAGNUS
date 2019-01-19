#pragma once
#ifndef MAGNUS_CONTROLS_TRACKBAR_H
#define MAGNUS_CONTROLS_TRACKBAR_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Controls/Dragger.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace ControlsInternal
	{
		class SliderBar : public ControlsInternal::Dragger
		{
			MAGNUS_CONTROL(SliderBar, ControlsInternal::Dragger);

			virtual void Render(Magnus::Skin* skin);
			virtual void SetHorizontal(bool b) { m_bHorizontal = b; }
			virtual bool IsHorizontal() { return m_bHorizontal; }

		protected:

			bool m_bHorizontal;
		};
	}

	namespace Controls
	{
		class Slider : public Control
		{
			MAGNUS_CONTROL(Slider, Controls::Control);

			virtual void Render(Magnus::Skin* skin) = 0;
			virtual void Layout(Magnus::Skin* skin);

			virtual void SetClampToNotches(bool bClamp) { m_bClampToNotches = bClamp; }

			virtual void SetNotchCount(int num) { m_iNumNotches = num; }
			virtual int GetNotchCount() { return m_iNumNotches; }

			virtual void SetRange(float fMin, float fMax);
			virtual float GetFloatValue();
			virtual void SetFloatValue(float val, bool forceUpdated = true);

			virtual float CalculateValue();
			virtual void OnMoved(Control* control);

			virtual void OnMouseClickLeft(int x, int y, bool bDown) {}

			virtual bool OnKeyRight(bool bDown)		{ if (bDown) { SetFloatValue(GetFloatValue() + 1, true); } return true; }
			virtual bool OnKeyLeft(bool bDown)		{ if (bDown) { SetFloatValue(GetFloatValue() - 1, true); } return true; }
			virtual bool OnKeyUp(bool bDown)		{ if (bDown) { SetFloatValue(GetFloatValue() + 1, true); } return true; }
			virtual bool OnKeyDown(bool bDown)		{ if (bDown) { SetFloatValue(GetFloatValue() - 1, true); } return true; }

			virtual void RenderFocus(Magnus::Skin* skin);

			Magnus::Event::Caller onValueChanged;

			virtual float GetMin() { return m_fMin; }
			virtual float GetMax() { return m_fMax; }

		protected:

			virtual void SetValueInternal(float fVal);
			virtual void UpdateBarFromValue() {}

			ControlsInternal::SliderBar* m_SliderBar;
			void OnBoundsChanged(Magnus::Rect oldBounds);
			bool m_bClampToNotches;
			int m_iNumNotches;
			float m_fValue;

			float m_fMin;
			float m_fMax;
		};
	}
}

#endif