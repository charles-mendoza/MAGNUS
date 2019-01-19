#pragma once
#ifndef MAGNUS_CONTROLS_HORIZONTALSLIDER_H
#define MAGNUS_CONTROLS_HORIZONTALSLIDER_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Controls/Dragger.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/Slider.h"
#include "Magnus/Controls/Label.h"

namespace Magnus
{
	namespace Controls
	{
		class HorizontalSlider : public Slider
		{
			MAGNUS_CONTROL(HorizontalSlider, Slider);

			virtual void Layout(Magnus::Skin* skin);
			virtual void Render(Magnus::Skin* skin);

			virtual float CalculateValue();
			virtual void UpdateBarFromValue();
			virtual void OnMouseClickLeft(int x, int y, bool bDown);
		};

		class LabeledHSlider : public Control
		{
		public:
			MAGNUS_CONTROL_INLINE(LabeledHSlider, Control)
			{
				m_pVar = NULL;
				SetSize(200, 36);
				m_Slider = new HorizontalSlider(this);
				m_Slider->Dock(Pos::Right);
				m_Slider->SetMargin(Margin(2, 2, 0, 2));
				m_Slider->SetTabable(false);
				m_Slider->onValueChanged.Add(this, &ThisClass::OnValueChanged);
				m_Label = new Label(this);
				m_Label->SetAlignment(Magnus::Pos::Left | Magnus::Pos::CenterV);
				m_Label->Dock(Pos::Fill);
				m_Label->SetTextPadding(Padding(3, 0, 3, 0));
				m_Label->SetTabable(false);
				SetTabable(false);
			}

			virtual void SetVar(void* pVar)
			{
				m_pVar = pVar;

				if (!m_pVar) { return; }

				m_Slider->SetFloatValue(*(float*)m_pVar, true);
			}

			virtual void OnValueChanged()
			{
				if (!m_pVar) { return; }

				*(float*)m_pVar = m_Slider->GetFloatValue();
			}

			virtual void PostLayout(Magnus::Skin* /*skin*/)
			{
				if (!m_pVar) { return; }

				if (*(float*)m_pVar != m_Slider->GetFloatValue())
					m_Slider->SetFloatValue(*(float*)m_pVar, true);
			}

			virtual void SetText(const TextObject & text) { m_Label->SetText(text); }
			virtual HorizontalSlider* GetSlider() { return m_Slider; }
			virtual Label* GetLabel() { return m_Label; }

		private:

			void* m_pVar;

			HorizontalSlider*	m_Slider;
			Label*				m_Label;
		};
	}
}

#endif