#pragma once
#ifndef MAGNUS_CONTROLS_WINDOWBUTTONS_H
#define MAGNUS_CONTROLS_WINDOWBUTTONS_H

#include "Magnus/Magnus.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace Controls
	{
		class WindowCloseButton : public Button
		{
			MAGNUS_CONTROL_INLINE(WindowCloseButton, Button)
			{
				m_pWindow = NULL;
				SetSize(13, 8);
				SetText("");
			}

			virtual void Render(Magnus::Skin* skin)
			{
				if (!m_pWindow) { return; }

				skin->DrawWindowCloseButton(GetRenderBounds(), IsDepressed() && IsHovered(), IsHovered() && ShouldDrawHover(), IsDisabled());
			}

			void SetWindow(Magnus::Controls::Control* p)
			{
				m_pWindow = p;
			}

		protected:

			Controls::Control* m_pWindow;
		};

		class WindowMaximizeButton : public WindowCloseButton
		{
			MAGNUS_CONTROL_INLINE(WindowMaximizeButton, WindowCloseButton)
			{
				m_bMaximized = false;
			}

			virtual void Render(Magnus::Skin* skin)
			{
				if (!m_pWindow) { return; }

				skin->DrawWindowMaximizeButton(GetRenderBounds(), m_bMaximized, IsDepressed() && IsHovered(), IsHovered() && ShouldDrawHover(), IsDisabled());
			}

			virtual void SetMaximized(bool b)
			{
				m_bMaximized = b;
			}

		protected:

			bool m_bMaximized;
		};

		class WindowMinimizeButton : public WindowCloseButton
		{
			MAGNUS_CONTROL_INLINE(WindowMinimizeButton, WindowCloseButton) {}

			virtual void Render(Magnus::Skin* skin)
			{
				if (!m_pWindow) { return; }

				skin->DrawWindowMinimizeButton(GetRenderBounds(), IsDepressed() && IsHovered(), IsHovered() && ShouldDrawHover(), IsDisabled());
			}
		};
	}
}

#endif