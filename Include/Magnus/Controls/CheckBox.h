#pragma once
#ifndef MAGNUS_CONTROLS_CHECKBOX_H
#define MAGNUS_CONTROLS_CHECKBOX_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/LabelClickable.h"

namespace Magnus
{
	namespace Controls
	{
		class CheckBox : public Button
		{
		public:

			MAGNUS_CONTROL(CheckBox, Button);

			virtual void Render(Magnus::Skin* skin);
			virtual void OnPress();

			virtual void SetChecked(bool bChecked);
			virtual void Toggle() { SetChecked(!IsChecked()); }
			virtual bool IsChecked() { return m_bChecked; }

			Magnus::Event::Caller onChecked;
			Magnus::Event::Caller onUnChecked;
			Magnus::Event::Caller onCheckChanged;

		private:

			// For derived controls
			virtual bool AllowUncheck() { return true; }

			void OnCheckStatusChanged();

			bool m_bChecked;
		};

		class LabeledCheckBox : public Control
		{
		public:
			MAGNUS_CONTROL_INLINE(LabeledCheckBox, Control)
			{
				SetSize(150, 19);
				m_Checkbox = new CheckBox(this);
				m_Checkbox->Dock(Pos::Left);
				m_Checkbox->SetMargin(Margin(0, 2, 2, 2));
				m_Checkbox->SetTabable(false);
				m_Label = new LabelClickable(this);
				m_Label->Dock(Pos::Fill);
				m_Label->onPress.Add(m_Checkbox, &CheckBox::OnPress);
				m_Label->SetTabable(false);
				SetTabable(false);
			}

			virtual void SetText(const TextObject & text) { m_Label->SetText(text); }
			virtual CheckBox* GetCheckbox() { return m_Checkbox; }
			virtual LabelClickable* GetLabel() { return m_Label; }
			virtual bool OnKeySpace(bool bDown) { if (!bDown) { m_Checkbox->SetChecked(!m_Checkbox->IsChecked()); } return true; }

		private:

			CheckBox*		m_Checkbox;
			LabelClickable*	m_Label;
		};
	}
}

#endif