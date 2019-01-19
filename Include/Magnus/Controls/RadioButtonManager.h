#pragma once
#ifndef MAGNUS_CONTROLS_RADIOBUTTONMANAGER_H
#define MAGNUS_CONTROLS_RADIOBUTTONMANAGER_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Label.h"
#include "Magnus/Controls/RadioButton.h"

namespace Magnus
{
	namespace Controls
	{
		class RadioButtonManager : public Control
		{
		public:

			MAGNUS_CONTROL(RadioButtonManager, Control);

			virtual void Render(Magnus::Skin /*skin*/) {}
			virtual void OnRadioClicked(Control* pFromPanel);

			virtual void OnChange();

			virtual LabeledRadioButton* AddOption(const Magnus::String & strText, const Magnus::String & strOptionName = "");
			virtual LabeledRadioButton* AddOption(const Magnus::UnicodeString & strText, const Magnus::String & strOptionName = "");

			virtual LabeledRadioButton* GetSelected() { return m_Selected; }

			virtual const Magnus::String & GetSelectedName() { return m_Selected->GetName(); }
			virtual const TextObject & GetSelectedLabel() { return m_Selected->GetLabel()->GetText(); }

			Event::Caller onSelectionChange;

		private:

			LabeledRadioButton* m_Selected;
		};
	}
}

#endif