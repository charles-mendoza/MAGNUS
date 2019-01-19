#pragma once
#ifndef MAGNUS_CONTROLS_TABCONTROL_H
#define MAGNUS_CONTROLS_TABCONTROL_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Label.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Controls/TabButton.h"
#include "Magnus/Controls/TabStrip.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace ControlsInternal
	{
		class ScrollButton;
	}

	namespace Controls
	{
		class TabControl : public Control
		{
			MAGNUS_CONTROL(TabControl, Control);

			virtual TabButton* AddPage(const TextObject & strText, Controls::Control* pPage = NULL);

			virtual void AddPage(TabButton* pButton);
			virtual void RemovePage(TabButton* pButton);

			virtual void OnTabPressed(Controls::Control* control);
			virtual void OnLoseTab(TabButton* pButton);

			virtual int TabCount();
			virtual TabButton* GetTab(int iNum);
			virtual TabButton* GetCurrentButton() { return m_pCurrentButton; }
			virtual TabStrip* GetTabStrip() { return m_TabStrip; }

			virtual void SetTabStripPosition(int iDock);

			Magnus::Event::Caller onLoseTab;
			Magnus::Event::Caller onAddTab;

		private:

			virtual void PostLayout(Magnus::Skin* skin);
			void HandleOverflow();

			void ScrollPressLeft(Control* pFrom);
			void ScrollPressRight(Control* pFrom);

			TabStrip* m_TabStrip;
			TabButton* m_pCurrentButton;

			ControlsInternal::ScrollButton* m_pScroll[2];
			int m_iScrollOffset;
		};
	}
}

#endif