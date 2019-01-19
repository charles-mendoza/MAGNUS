#pragma once
#ifndef MAGNUS_CONTROLS_WINDOW_H
#define MAGNUS_CONTROLS_WINDOW_H

#include "Magnus/Magnus.h"
#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Label.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Controls/Dragger.h"
#include "Magnus/Controls/WindowButtons.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace Controls
	{
		class Window : public Controls::Control
		{
		public:

			MAGNUS_CONTROL(Window, Controls::Control);

			virtual void Render(Magnus::Skin* skin);
			virtual void RenderUnder(Magnus::Skin* skin);

			virtual void SetTitle(const Magnus::TextObject & title);
			virtual void SetClosable(bool closable);

			virtual void Touch();
			bool IsOnTop();

			virtual void SetHidden(bool hidden);

			void CloseButtonPressed();
			void RenderFocus(Magnus::Skin* skin);
			void SetDeleteOnClose(bool b) { m_bDeleteOnClose = b; }

			Magnus::Event::Caller onWindowClosed;

		protected:

			ControlsInternal::Dragger* m_TitleBar;
			Magnus::Controls::Label* m_Title;

			Magnus::Controls::WindowCloseButton* m_CloseButton;

			bool m_bDeleteOnClose;
		};
	}
}

#endif