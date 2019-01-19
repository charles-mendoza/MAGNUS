#pragma once
#ifndef MAGNUS_CONTROLS_GROUPBOX_H
#define MAGNUS_CONTROLS_GROUPBOX_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Label.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace Controls
	{
		class GroupBox : public Label
		{
		public:

			MAGNUS_CONTROL(GroupBox, Label);

			virtual void Render(Magnus::Skin* skin);
			virtual void Layout(Magnus::Skin* skin);

			virtual void SetInnerMargin(int i) { m_InnerMargin = i; }

		protected:

			int m_InnerMargin;
		};
	}
}

#endif