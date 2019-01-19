#pragma once
#ifndef MAGNUS_CONTROLS_TOGGLEWSWITCH
#define MAGNUS_CONTROLS_TOGGLEWSWITCH

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/CheckBox.h"
#include "Magnus/Controls/Dragger.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace ControlsInternal
	{
		class ToggleBar : public ControlsInternal::Dragger
		{
		public:

			MAGNUS_CONTROL(ToggleBar, ControlsInternal::Dragger);

			virtual void Render(Magnus::Skin* skin);

			virtual void OnMouseClickLeft(int x, int y, bool bDown);

			virtual void SetOn(bool bOn) { m_bOn = bOn; }
			virtual bool IsOn() { return m_bOn; }

		private:

			bool m_bOn;
		};
	}

	namespace Controls
	{
		class ToggleControl : public CheckBox
		{
		public:

			MAGNUS_CONTROL(ToggleControl, CheckBox);

			virtual void Render(Magnus::Skin* skin);
			virtual void Layout(Magnus::Skin* skin);

			virtual void SetChecked(bool bChecked);

			virtual void OnMoved(Control* control);
			virtual void OnMouseClickLeft(int x, int y, bool bDown);

		protected:

			void UpdateBar();
			void OnBoundsChanged(Magnus::Rect oldBounds);

			Label* m_Label;
			ControlsInternal::ToggleBar* m_Bar;
		};

		class LabeledToggleControl : public Control
		{
		public:

			MAGNUS_CONTROL_INLINE(LabeledToggleControl, Control)
			{
				m_pVar = NULL;
				SetSize(150, 19);
				SetMargin(Margin(0, 2, 0, 2));
				m_Toggle = new ToggleControl(this);
				m_Toggle->Dock(Pos::Right);
				m_Toggle->SetMargin(Margin(0, 2, 0, 2));
				m_Toggle->SetTabable(false);
				m_Toggle->onCheckChanged.Add(this, &ThisClass::OnCheckChanged);
				m_Label = new LabelClickable(this);
				m_Label->SetAlignment(Magnus::Pos::Left | Magnus::Pos::CenterV);
				m_Label->Dock(Pos::Fill);
				m_Label->SetMargin(Margin(0, 0, 2, 0));
				m_Label->onPress.Add(m_Toggle, &ToggleControl::OnPress);
				m_Label->SetTabable(false);
				SetTabable(false);
			}

			virtual void SetVar(void* pVar)
			{
				m_pVar = pVar;

				if (!m_pVar) { return; }

				m_Toggle->SetChecked(*(bool*)m_pVar);
			}

			virtual void OnCheckChanged()
			{
				if (!m_pVar) { return; }

				*(bool*)m_pVar = m_Toggle->IsChecked();
			}

			virtual void PostLayout(Magnus::Skin* /*skin*/)
			{
				if (!m_pVar) { return; }

				if (*(bool*)m_pVar != m_Toggle->IsChecked())
					m_Toggle->SetChecked(*(bool*)m_pVar);
			}

			virtual void SetText(const TextObject & text) { m_Label->SetText(text); }
			virtual ToggleControl* GetToggle() { return m_Toggle; }
			virtual LabelClickable* GetLabel() { return m_Label; }
			virtual bool OnKeySpace(bool bDown) { if (!bDown) { m_Toggle->SetChecked(!m_Toggle->IsChecked()); } return true; }

		private:

			void* m_pVar;

			ToggleControl*	m_Toggle;
			LabelClickable*	m_Label;
		};
	}
}

#endif