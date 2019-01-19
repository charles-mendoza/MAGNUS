#pragma once
#ifndef MAGNUS_INPUTHANDLER_H
#define MAGNUS_INPUTHANDLER_H

#include <map>
#include <list>

#include "Magnus/Magnus.h"
#include "Magnus/Events.h"

#define MAX_MOUSE_BUTTONS	5
#define DOUBLE_CLICK_SPEED	0.5f
#define SCROLL_SPEED		0.03f

#define KEY_REPEAT_RATE		0.03f
#define KEY_REPEAT_DELAY	0.3f

namespace Magnus
{
	namespace Controls
	{
		class Control;
	}

	namespace Key
	{
		enum
		{
			Invalid,

			Tab,
			Backspace,
			Return,
			Shift,
			Control,
			Pause,
			CapsLock,
			Escape,
			Space,
			PageUp,
			PageDown,
			End,
			Home,
			Left,
			Up,
			Right,
			Down,
			Insert,
			Delete,

			D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,

			A, B, C, D, E, F, G, H, I, J, K, L, M,
			N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

			Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, 
			Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,

			F1, F2, F3, F4, F5, F6,
			F7, F8, F9, F10, F11, F12,

			Count
		};
	}

	namespace Input
	{
		// For use in panels
		bool IsKeyDown(int iKey);
		bool IsLeftMouseDown();
		bool IsRightMouseDown();
		bool IsMiddleMouseDown();
		Magnus::Point GetMousePosition();

		inline bool IsShiftDown() { return IsKeyDown(Magnus::Key::Shift); }
		inline bool IsControlDown() { return IsKeyDown(Magnus::Key::Control); }

		// Does copy, past etc
		bool HandleAccelerator(Controls::Control* pCanvas, Magnus::UnicodeChar chr);

		// Send input to canvas for study
		void OnMouseMoved(Controls::Control* pCanvas, int x, int y, int deltaX, int deltaY);
		bool OnMouseClicked(Controls::Control* pCanvas, int iButton, bool bDown);
		bool OnMouseWheeled(Controls::Control* pCanvas, int val);
		bool OnKeyCharEvent(Controls::Control* pCanvas, Magnus::UnicodeChar chr);
		bool OnKeyEvent(Controls::Control* pCanvas, int iKey, bool bDown);
		void OnCanvasThink(Controls::Control* pControl);

		// Key binding
		template <typename T> void AddKeyBind(int iKey, void (T::*f)(), Event::Handler* ob) { AddKeyBindInternal(iKey, static_cast<Event::Handler::FunctionBlank>(f), ob); }
		void AddKeyBindInternal(int iKey, Event::Handler::FunctionBlank pFunction, Event::Handler* pObject);
	}
}

#endif