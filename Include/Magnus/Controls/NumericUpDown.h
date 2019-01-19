#pragma once
#ifndef MAGNUS_CONTROLS_NUMERICUPDOWN_H
#define MAGNUS_CONTROLS_NUMERICUPDOWN_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Controls/TextBox.h"

namespace Magnus
{
	namespace Controls
	{
		class NumericUpDownButton_Up : public Button
		{
			MAGNUS_CONTROL_INLINE(NumericUpDownButton_Up, Button)
			{
				SetSize(7, 7);
				SetText("");
			}

			virtual void Render(Magnus::Skin* skin)
			{
				skin->DrawNumericUpDownButton(GetRenderBounds(), true, IsDepressed(), IsHovered(), IsDisabled());
			}
		};

		class NumericUpDownButton_Down : public Button
		{
			MAGNUS_CONTROL_INLINE(NumericUpDownButton_Down, Button)
			{
				SetSize(7, 7);
				SetText("");
			}

			virtual void Render(Magnus::Skin* skin)
			{
				skin->DrawNumericUpDownButton(GetRenderBounds(), false, IsDepressed(), IsHovered(), IsDisabled());
			}
		};

		class NumericUpDown : public TextBoxNumeric
		{
		public:

			MAGNUS_CONTROL(NumericUpDown, TextBoxNumeric);

			virtual void SetMin(int i);
			virtual void SetMax(int i);
			virtual void SetValue(int i);
			virtual int GetMin() { return m_iMin; }
			virtual int GetMax() { return m_iMax; }
			virtual int GetNumber() { return m_iNumber; }

			Event::Caller onChanged;

		private:

			virtual void OnEnter();
			virtual void OnChange();
			virtual void OnTextChanged();

			virtual void OnButtonUp(Control* control);
			virtual void OnButtonDown(Control* control);

			virtual bool OnKeyUp(bool bDown) { if (bDown) { OnButtonUp(NULL); } return true; }
			virtual bool OnKeyDown(bool bDown) { if (bDown) { OnButtonDown(NULL); } return true; }

			virtual void OnLostKeyboardFocus();

			virtual void SyncTextFromNumber();
			virtual void SyncNumberFromText();

			int m_iNumber;
			int m_iMax;
			int m_iMin;
		};

		class LabeledNumricUpDown : public Control
		{
		public:
			MAGNUS_CONTROL_INLINE(LabeledNumricUpDown, Control)
			{
				m_pVar = NULL;
				SetSize(200, 24);
				m_Numeric = new NumericUpDown(this);
				m_Numeric->SetWidth(100);
				m_Numeric->Dock(Pos::Right);
				m_Numeric->SetMargin(Margin(2, 2, 0, 2));
				m_Numeric->SetTabable(false);
				m_Numeric->onChanged.Add(this, &ThisClass::OnChanged);
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

				m_Numeric->SetValue(*(int*)m_pVar);
			}

			virtual void OnChanged()
			{
				if (!m_pVar) { return; }

				*(int*)m_pVar = m_Numeric->GetNumber();
			}

			virtual void PostLayout(Magnus::Skin* /*skin*/)
			{
				if (!m_pVar) { return; }

				if (*(int*)m_pVar != m_Numeric->GetNumber())
					m_Numeric->SetValue(*(int*)m_pVar);
			}

			virtual void SetText(const TextObject & text) { m_Label->SetText(text); }
			virtual NumericUpDown* GetNumericUpDown() { return m_Numeric; }
			virtual Label* GetLabel() { return m_Label; }

		private:

			void* m_pVar;

			NumericUpDown*	m_Numeric;
			Label*			m_Label;
		};
	}
}

#endif