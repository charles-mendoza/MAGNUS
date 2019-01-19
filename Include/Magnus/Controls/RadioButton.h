#pragma once
#ifndef MAGNUS_CONTROLS_RADIOBUTTON_H
#define MAGNUS_CONTROLS_RADIOBUTTON_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Label.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/CheckBox.h"
#include "Magnus/Controls/LabelClickable.h"

namespace Magnus
{
	namespace Controls
	{
		class RadioButton : public CheckBox
		{
			MAGNUS_CONTROL(RadioButton, CheckBox);

			virtual void Render(Magnus::Skin* skin);

		private:

			// From CheckBox
			virtual bool AllowUncheck() { return false; }
		};

		class LabeledRadioButton : public Control
		{
		public:

			MAGNUS_CONTROL_INLINE(LabeledRadioButton, Control)
			{
				SetSize(150, 19);
				m_RadioButton = new RadioButton(this);
				m_RadioButton->Dock(Pos::Left);
				m_RadioButton->SetMargin(Margin(0, 2, 2, 2));
				m_RadioButton->SetTabable(false);
				m_RadioButton->SetKeyboardInputEnabled(false);
				m_Label = new LabelClickable(this);
				m_Label->SetAlignment(Pos::CenterV | Pos::Left);
				m_Label->SetText("RadioButton");
				m_Label->Dock(Pos::Fill);
				m_Label->onPress.Add(m_RadioButton, &CheckBox::OnPress);
				m_Label->SetTabable(false);
				m_Label->SetKeyboardInputEnabled(false);
			}

			void RenderFocus(Magnus::Skin* skin)
			{
				if (Magnus::KeyboardFocus != this) { return; }

				if (!IsTabable()) { return; }

				skin->DrawKeyboardHighlight(GetRenderBounds(), 0);
			}

			virtual void SetText(const TextObject & text) { m_Label->SetText(text); }
			virtual RadioButton* GetRadioButton() { return m_RadioButton; }
			virtual LabelClickable* GetLabel() { return m_Label; }
			virtual bool OnKeySpace(bool bDown) { if (bDown) { m_RadioButton->SetChecked(!m_RadioButton->IsChecked()); } return true; }

			virtual void Select() { m_RadioButton->SetChecked(true); }

		private:

			RadioButton*		m_RadioButton;
			LabelClickable*		m_Label;
		};
	}
}

#endif