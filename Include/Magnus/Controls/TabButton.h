#pragma once
#ifndef MAGNUS_CONTROLS_TABBUTTON_H
#define MAGNUS_CONTROLS_TABBUTTON_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"

namespace Magnus
{
	namespace Controls
	{
		class TabControl;

		class TabButton : public Button
		{
		public:

			MAGNUS_CONTROL(TabButton, Button);

			virtual void Render(Magnus::Skin* skin);
			virtual void Layout(Magnus::Skin* skin);

			void SetPage(Control* page) { m_Page = page; }
			Control* GetPage() { return m_Page; }

			void SetTabControl(TabControl* control);
			TabControl* GetTabControl() { return m_Control; }

			bool IsActive() { return m_Page && m_Page->Visible(); }

			virtual bool OnKeyLeft(bool bDown);
			virtual bool OnKeyRight(bool bDown);
			virtual bool OnKeyUp(bool bDown);
			virtual bool OnKeyDown(bool bDown);

			virtual void UpdateColours();

			virtual bool ShouldClip() { return false; }

		private:

			Control*	m_Page;
			TabControl*	m_Control;
		};
	}
}

#endif