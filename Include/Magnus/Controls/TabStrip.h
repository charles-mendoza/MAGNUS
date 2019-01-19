#pragma once
#ifndef MAGNUS_CONTROLS_TABSTRIP_H
#define MAGNUS_CONTROLS_TABSTRIP_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Controls/TabButton.h"

namespace Magnus
{
	namespace Controls
	{
		class TabStrip : public Control
		{
		public:

			MAGNUS_CONTROL_INLINE(TabStrip, Control) {}

			virtual void Layout(Magnus::Skin* skin);

			virtual void SetTabPosition(int iPos);

			virtual bool ShouldClip() { return false; }
		};
	}
}

#endif