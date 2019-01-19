#include "Magnus/Controls/RadioButtonManager.h"
#include "Magnus/Controls/RadioButton.h"
#include "Magnus/Utility.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(RadioButtonManager)
{
	m_Selected = NULL;
	SetTabable(false);
	SetKeyboardInputEnabled(false);
}

void RadioButtonManager::OnRadioClicked(Magnus::Controls::Control* pFromPanel)
{
	RadioButton* pCheckedRadioButton = magnus_cast<RadioButton>(pFromPanel);

	// Iterate through all other buttons and set them to false.
	for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
	{
		Control* pChild = *iter;
		LabeledRadioButton* pLRB = magnus_cast<LabeledRadioButton>(pChild);

		if (pLRB)
		{
			RadioButton* pChildRadioButton = pLRB->GetRadioButton();

			if (pChildRadioButton == pCheckedRadioButton)
			{
				m_Selected = pLRB;
			}
			else
			{
				pLRB->GetRadioButton()->SetChecked(false);
			}
		}
	}

	OnChange();
}

void RadioButtonManager::OnChange()
{
	onSelectionChange.Call(this);
}

LabeledRadioButton* RadioButtonManager::AddOption(const Magnus::String & strText, const Magnus::String & strOptionName)
{
	return AddOption(Magnus::Utility::StringToUnicode(strText), strOptionName);
}

LabeledRadioButton* RadioButtonManager::AddOption(const Magnus::UnicodeString & strText, const Magnus::String & strOptionName)
{
	LabeledRadioButton* lrb = new LabeledRadioButton(this);
	lrb->SetName(strOptionName);
	lrb->GetLabel()->SetText(strText);
	lrb->GetRadioButton()->onChecked.Add(this, &RadioButtonManager::OnRadioClicked);
	lrb->Dock(Pos::Top);
	lrb->SetMargin(Margin(0, 1, 0, 1));
	lrb->SetKeyboardInputEnabled(false);
	lrb->SetTabable(false);
	Invalidate();
	return lrb;
}