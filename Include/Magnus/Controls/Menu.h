#pragma once
#ifndef MAGNUS_CONTROLS_MENU_H
#define MAGNUS_CONTROLS_MENU_H

#include "Magnus/Renderer.h"
#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/MenuItem.h"
#include "Magnus/Controls/ScrollControl.h"

namespace Magnus
{
	namespace Controls
	{
		class Menu : public ScrollControl
		{
		public:

			MAGNUS_CONTROL(Menu, ScrollControl);

			virtual void Render(Magnus::Skin* skin);
			virtual void RenderUnder(Magnus::Skin* skin);

			virtual void Layout(Magnus::Skin* skin);

			virtual MenuItem* AddItem(const TextObject & strName, const TextObject & strIconName = L"", const TextObject & strAccelerator = L"");
			virtual MenuItem* GetItemAt(unsigned int index);

			virtual void AddDivider();

			void OnHoverItem(Magnus::Controls::Control* pControl);
			void CloseAll();
			bool IsMenuOpen();
			void ClearItems();

			virtual void Open(unsigned int iPos);
			virtual void Close();

			virtual bool IsMenuComponent() { return true; }
			virtual void CloseMenus();

			virtual int MaxContentHeight() { return m_iMaxContentHeight; }
			virtual void SetMaxContentHeight(int height) { m_iMaxContentHeight = height; }

			bool IconMarginDisabled() { return m_bDisableIconMargin; }
			void SetDisableIconMargin(bool bDisable) { m_bDisableIconMargin = bDisable; }

			bool DeleteOnClose() { return m_bDeleteOnClose; }
			void SetDeleteOnClose(bool b) { m_bDeleteOnClose = b; }

			Magnus::Event::Caller onMenuClosed;

		protected:

			virtual bool ShouldHoverOpenMenu() { return true; }
			virtual void OnAddItem(MenuItem* item);

			int m_iMaxContentHeight;
			bool m_bDisableIconMargin;
			bool m_bDeleteOnClose;
		};

		class MenuDivider : public Control
		{
		public:

			MAGNUS_CONTROL_INLINE(MenuDivider, Control)
			{
				SetHeight(1);
			}

			void Render(Magnus::Skin* skin);
		};
	}
}

#endif