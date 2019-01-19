#pragma once
#ifndef MAGNUS_INPUT_WINDOWS_H
#define MAGNUS_INPUT_WINDOWS_H

#include "Magnus/InputHandler.h"
#include "Magnus/Magnus.h"
#include "Magnus/Platform.h"
#include "Magnus/Controls/Canvas.h"

#include <Windows.h>

namespace Magnus
{
	namespace Input
	{
		class Windows
		{
		public:

			Windows()
			{
				m_Canvas = NULL;
				m_MouseX = 0;
				m_MouseY = 0;
				m_LastKey = 0;
				m_NextKeyRepeat = 0;
			}

			void Init(Controls::Canvas* pCanvas)
			{
				m_Canvas = pCanvas;
			}

			bool ProcessMessage(MSG msg)
			{
				if (!m_Canvas) { return false; }

				switch (msg.message)
				{
					case WM_SYSCOMMAND:
					{
						if (msg.message == WM_SYSCOMMAND && msg.wParam != SC_CLOSE)
						{
							return false;
						}

						return m_Canvas->InputQuit();
					}

					case WM_MOUSEMOVE:
					{
						int x = (signed short)LOWORD(msg.lParam);
						int y = (signed short)HIWORD(msg.lParam);
						int dx = x - m_MouseX;
						int dy = y - m_MouseY;
						m_MouseX = x;
						m_MouseY = y;
						return m_Canvas->InputMouseMoved(x, y, dx, dy);
					}

					case WM_CHAR:
					{
						Magnus::UnicodeChar chr = (Magnus::UnicodeChar)msg.wParam;
						return m_Canvas->InputCharacter(chr);
					}

#ifdef WM_MOUSEWHEEL
					case WM_MOUSEWHEEL:
					{
						return m_Canvas->InputMouseWheel((short)HIWORD(msg.wParam));
					}
#endif

					case WM_LBUTTONDOWN:
					{
						bool ret = m_Canvas->InputMouseButton(0, true);
						if (ret) { SetCapture(msg.hwnd); }
						return ret;
					}

					case WM_LBUTTONUP:
					{
						bool ret = m_Canvas->InputMouseButton(0, false);
						if (ret) { ReleaseCapture(); }
						return ret;
					}

					case WM_RBUTTONDOWN:
					{
						bool ret = m_Canvas->InputMouseButton(1, true);
						if (ret) { SetCapture(msg.hwnd); }
						return ret;
					}

					case WM_RBUTTONUP:
					{
						bool ret = m_Canvas->InputMouseButton(1, false);
						if (ret) { ReleaseCapture(); }
						return ret;
					}

					case WM_MBUTTONDOWN:
					{
						bool ret = m_Canvas->InputMouseButton(2, true);
						if (ret) { SetCapture(msg.hwnd); }
						return ret;
					}

					case WM_MBUTTONUP:
					{
						bool ret = m_Canvas->InputMouseButton(2, false);
						if (ret) { ReleaseCapture(); }
						return ret;
					}

					case WM_LBUTTONDBLCLK:
					case WM_RBUTTONDBLCLK:
					case WM_MBUTTONDBLCLK:
					{
						// Filter out those events from the application
						return true;
					}

					case WM_KEYDOWN:
					case WM_KEYUP:
					{
						bool bDown = msg.message == WM_KEYDOWN;
						int iKey = -1;

						if (!bDown)
							m_LastKey = 0;

						// These aren't sent by WM_CHAR when CTRL is down - but we need
						// them internally for copy and past etc..
						/*if (bDown && GetKeyState(VK_CONTROL) & 0x80 && msg.wParam >= 'A' && msg.wParam <= 'Z')
						{
							Magnus::UnicodeChar chr = (Magnus::UnicodeChar)msg.wParam;
							return m_Canvas->InputCharacter(chr);
						}*/

						if (msg.wParam == VK_TAB) { iKey = Magnus::Key::Tab; }
						else if (msg.wParam == VK_BACK) { iKey = Magnus::Key::Backspace; }
						else if (msg.wParam == VK_RETURN) { iKey = Magnus::Key::Return; }
						else if (msg.wParam == VK_SHIFT) { iKey = Magnus::Key::Shift; }
						else if (msg.wParam == VK_CONTROL) { iKey = Magnus::Key::Control; }
						else if (msg.wParam == VK_PAUSE) { iKey = Magnus::Key::Pause; }
						else if (msg.wParam == VK_CAPITAL) { iKey = Magnus::Key::CapsLock; }
						else if (msg.wParam == VK_ESCAPE) { iKey = Magnus::Key::Escape; }
						else if (msg.wParam == VK_SPACE) { iKey = Magnus::Key::Space; }
						else if (msg.wParam == VK_PRIOR) { iKey = Magnus::Key::PageUp; }
						else if (msg.wParam == VK_NEXT) { iKey = Magnus::Key::PageDown; }
						else if (msg.wParam == VK_END) { iKey = Magnus::Key::End; }
						else if (msg.wParam == VK_HOME) { iKey = Magnus::Key::Home; }
						else if (msg.wParam == VK_LEFT) { iKey = Magnus::Key::Left; }
						else if (msg.wParam == VK_UP) { iKey = Magnus::Key::Up; }
						else if (msg.wParam == VK_RIGHT) { iKey = Magnus::Key::Right; }
						else if (msg.wParam == VK_DOWN) { iKey = Magnus::Key::Down; }
						else if (msg.wParam == VK_INSERT) { iKey = Magnus::Key::Insert; }
						else if (msg.wParam == VK_DELETE) { iKey = Magnus::Key::Delete; }
						else if (msg.wParam >= '0' && msg.wParam <= '9') { iKey = msg.wParam - '0' + Magnus::Key::D0; }
						else if (msg.wParam >= 'A' && msg.wParam <= 'Z') { iKey = msg.wParam - 'A' + Magnus::Key::A; }
						else if (msg.wParam >= VK_NUMPAD0 && msg.wParam <= VK_NUMPAD9) { iKey = msg.wParam - VK_NUMPAD0 + Magnus::Key::Numpad0; }
						else if (msg.wParam >= VK_F1 && msg.wParam <= VK_F12) { iKey = msg.wParam - VK_F1 + Magnus::Key::F1; }

						if (iKey != -1)
						{
							return m_Canvas->InputKey(iKey, bDown);
						}

						break;
					}

					default:
					{
						break;
					}
				}

				return false;
			}

		protected:

			Controls::Canvas* m_Canvas;
			int m_MouseX;
			int m_MouseY;

			UnicodeChar m_LastKey;
			float m_NextKeyRepeat;
		};
	}
}

#endif