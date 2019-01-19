#pragma once
#ifndef MAGNUS_CONTROLS_MENUITEM_H
#define MAGNUS_CONTROLS_MENUITEM_H

#include "Magnus/Renderer.h"
#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"

namespace Magnus
{
	namespace Controls
	{
		class Menu;

		class MenuItem : public Button
		{
		public:

			MAGNUS_CONTROL(MenuItem, Button);

			virtual ~MenuItem();

			virtual void Render(Magnus::Skin* skin);
			virtual void Layout(Magnus::Skin* skin);

			virtual void SizeToContents();

			virtual void OnPress();

			Menu* GetMenu();

			bool IsMenuOpen();
			void OpenMenu();
			void CloseMenu();
			void ToggleMenu();

			void SetOnStrip(bool b) { m_bOnStrip = b; }
			bool OnStrip() { return m_bOnStrip; }

			virtual void SetCheckable(bool bCheck) { m_bCheckable = bCheck; }
			virtual void SetChecked(bool bCheck);
			virtual bool GetChecked() { return m_bChecked; }

			template <typename T>
			MenuItem* SetAction(Magnus::Event::Handler* pHandler, T fn)
			{
				if (m_Accelerator) { AddAccelerator(m_Accelerator->GetText(), fn, pHandler); }

				onMenuItemSelected.Add(pHandler, fn);
				return this;
			}

			void SetAccelerator(const TextObject & strAccelerator);

			Magnus::Event::Caller onMenuItemSelected;
			Magnus::Event::Caller onChecked;
			Magnus::Event::Caller onUnChecked;
			Magnus::Event::Caller onCheckChange;

		private:

			Menu* m_Menu;
			bool m_bOnStrip;
			bool m_bCheckable;
			bool m_bChecked;

			Label* m_Accelerator;

			Controls::Control* m_SubmenuArrow;
		};
	}
}

#endif