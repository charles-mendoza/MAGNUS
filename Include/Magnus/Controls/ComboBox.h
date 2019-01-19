#pragma once
#ifndef MAGNUS_CONTROLS_COMBOBOX_H
#define MAGNUS_CONTROLS_COMBOBOX_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/TextBox.h"
#include "Magnus/Controls/Menu.h"
#include "Magnus/Controls/Label.h"

namespace Magnus
{
	namespace Controls
	{
		class ComboBox : public Button
		{
		public:

			MAGNUS_CONTROL(ComboBox, Button);

			virtual void Render(Magnus::Skin* skin);
			virtual void Layout(Magnus::Skin* skin);
			virtual void UpdateColours();

			virtual void SelectItem(MenuItem* pItem, bool bFireChangeEvents = true);
			virtual void SelectItemByIndex(unsigned int index, bool bFireChangeEvents = true);
			virtual void SelectItemByName(const Magnus::String & name, bool bFireChangeEvents = true);
			virtual Magnus::Controls::Label* GetSelectedItem();
			virtual int GetSelectedIndex();

			virtual void OnPress();
			virtual void OnItemSelected(Controls::Control* pControl);
			virtual void OpenList();
			virtual void CloseList();

			virtual void ClearItems();

			virtual MenuItem* AddItem(const TextObject & strLabel, const String & strName = "");
			virtual bool OnKeyUp(bool bDown);
			virtual bool OnKeyDown(bool bDown);

			virtual void RenderFocus(Magnus::Skin* skin);

			virtual void OnOpacityChanged();

			virtual bool IsMenuOpen();

			virtual bool IsMenuComponent() { return true; }

			Magnus::Event::Caller onSelection;

		protected:

			Menu* m_Menu;
			MenuItem* m_SelectedItem;

			Controls::Control* m_Button;
		};

		class LabeledComboBox : public Control
		{
		public:
			MAGNUS_CONTROL_INLINE(LabeledComboBox, Control)
			{
				m_pVar = NULL;
				SetSize(200, 24);
				m_ComboBox = new ComboBox(this);
				m_ComboBox->SetWidth(100);
				m_ComboBox->Dock(Pos::Right);
				m_ComboBox->SetMargin(Margin(2, 2, 0, 2));
				m_ComboBox->SetTabable(false);
				m_ComboBox->onSelection.Add(this, &ThisClass::OnSelection);
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

				m_ComboBox->SelectItemByIndex(*(int*)m_pVar);
			}

			virtual void OnSelection()
			{
				if (!m_pVar) { return; }

				*(int*)m_pVar = m_ComboBox->GetSelectedIndex();
			}

			virtual void PostLayout(Magnus::Skin* /*skin*/)
			{
				if (!m_pVar) { return; }

				if (*(int*)m_pVar != m_ComboBox->GetSelectedIndex())
					m_ComboBox->SelectItemByIndex(*(int*)m_pVar);
			}

			virtual void SetText(const TextObject & text) { m_Label->SetText(text); }
			virtual MenuItem* AddItem(const TextObject & strLabel, const String & strName = "") { return m_ComboBox->AddItem(strLabel, strName); }

			virtual ComboBox* GetComboBox() { return m_ComboBox; }
			virtual Label* GetLabel() { return m_Label; }

		private:

			void* m_pVar;

			ComboBox*	m_ComboBox;
			Label*		m_Label;
		};
	}
}

#endif