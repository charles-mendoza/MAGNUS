#include "Magnus/InputHandler.h"
#include "Magnus/Controls/Control.h"
#include "Magnus/Platform.h"

using namespace Magnus;

struct KeyData_t
{
	KeyData_t()
	{
		for (int i = 0; i < Magnus::Key::Count; i++)
		{
			KeyState[i] = false;
			NextRepeat[i] = 0;
		}

		Target = NULL;
		LeftMouseDown = false;
		RightMouseDown = false;
	}

	bool KeyState[Magnus::Key::Count];
	float NextRepeat[Magnus::Key::Count];
	float NextCharRepeat;
	float NextScroll;
	Magnus::UnicodeChar LastChar;
	Controls::Control* Target;
	bool LeftMouseDown;
	bool RightMouseDown;
	bool MiddleMouseDown;

} KeyData;

struct KeyBindCaller_t
{
	Event::Handler* pObject;
	Event::Handler::FunctionBlank fnFunction;
};

std::map<int, std::list<KeyBindCaller_t>> KeyboardMap;

Magnus::Point MousePosition;

static float g_fLastClickTime[MAX_MOUSE_BUTTONS];
static Magnus::Point g_ptLastClickPos;

void UpdateHoveredControl(Controls::Control* pInCanvas)
{
	Controls::Control* pHovered = pInCanvas->GetControlAt(MousePosition.x, MousePosition.y);

	if (pHovered != Magnus::HoveredControl)
	{
		if (Magnus::HoveredControl)
		{
			Controls::Control* oldHover = Magnus::HoveredControl;
			Magnus::HoveredControl = NULL;
			oldHover->OnMouseLeave();
		}
		
		Magnus::HoveredControl = pHovered;

		if (Magnus::HoveredControl)
		{
			Magnus::HoveredControl->OnMouseEnter();
		}
	}

	if (Magnus::MouseFocus && Magnus::MouseFocus->GetCanvas() == pInCanvas)
	{
		if (Magnus::HoveredControl)
		{
			Controls::Control* oldHover = Magnus::HoveredControl;
			Magnus::HoveredControl = NULL;
			oldHover->Redraw();
		}

		Magnus::HoveredControl = Magnus::MouseFocus;
	}
}

bool FindKeyboardFocus(Controls::Control* pControl)
{
	if (!pControl) { return false; }

	if (pControl->GetKeyboardInputEnabled())
	{
		// Make sure non of our children have keyboard focus first - todo recursive
		for (Controls::Control::List::iterator iter = pControl->Children.begin(); iter != pControl->Children.end(); ++iter)
		{
			Controls::Control* pChild = *iter;

			if (pChild == Magnus::KeyboardFocus)
			{
				return false;
			}
		}

		pControl->Focus();
		return true;
	}

	return FindKeyboardFocus(pControl->GetParent());
}

void Magnus::Input::OnCanvasThink(Controls::Control* pControl)
{
	if (Magnus::MouseFocus && !Magnus::MouseFocus->Visible())
	{
		Magnus::MouseFocus = NULL;
	}

	if (Magnus::KeyboardFocus && (!Magnus::KeyboardFocus->Visible() || !KeyboardFocus->GetKeyboardInputEnabled()))
	{
		Magnus::KeyboardFocus = NULL;
	}

	if (!KeyboardFocus) { return; }

	if (KeyboardFocus->GetCanvas() != pControl) { return; }

	float fTime = Magnus::Platform::GetTimeInSeconds();

	//
	// Simulate Key-Repeats
	//
	for (int i = 0; i < Key::Count; i++)
	{
		if (KeyData.KeyState[i] && KeyData.Target != KeyboardFocus)
		{
			KeyData.KeyState[i] = false;
			continue;
		}

		if (KeyData.KeyState[i] && fTime > KeyData.NextRepeat[i])
		{
			KeyData.NextRepeat[i] = Magnus::Platform::GetTimeInSeconds() + KEY_REPEAT_RATE;

			if (KeyboardFocus)
			{
				KeyboardFocus->OnKeyPress(i);
			}
		}
	}
}

bool Magnus::Input::IsKeyDown(int iKey)
{
	return KeyData.KeyState[iKey];
}

bool Magnus::Input::IsLeftMouseDown()
{
	return KeyData.LeftMouseDown;
}

bool Magnus::Input::IsRightMouseDown()
{
	return KeyData.RightMouseDown;
}

bool Magnus::Input::IsMiddleMouseDown()
{
	return KeyData.MiddleMouseDown;
}

Magnus::Point Magnus::Input::GetMousePosition()
{
	return MousePosition;
}

void Magnus::Input::OnMouseMoved(Controls::Control* pCanvas, int x, int y, int deltaX, int deltaY)
{
	MousePosition.x = x;
	MousePosition.y = y;
	UpdateHoveredControl(pCanvas);
}

bool Magnus::Input::OnMouseClicked(Controls::Control* pCanvas, int iMouseButton, bool bDown)
{
	if (iMouseButton >= MAX_MOUSE_BUTTONS) { return false; }

	if (iMouseButton == 0) { KeyData.LeftMouseDown = bDown; }
	else if (iMouseButton == 1) { KeyData.RightMouseDown = bDown; }
	else if (iMouseButton == 2) { KeyData.MiddleMouseDown = bDown; }

	// If we click on a control that isn't a menu we want to close
	// all the open menus. Menus are children of the canvas.
	if (bDown && (!Magnus::HoveredControl || !Magnus::HoveredControl->IsMenuComponent()))
	{
		pCanvas->CloseMenus();
	}

	if (!Magnus::HoveredControl) { return false; }

	if (Magnus::HoveredControl->GetCanvas() != pCanvas) { return false; }

	if (!Magnus::HoveredControl->Visible()) { return false; }

	if (Magnus::HoveredControl == pCanvas) { return false; }

	// Double click.
	// Todo: Shouldn't double click if mouse has moved significantly
	bool bIsDoubleClick = false;

	if (bDown &&
		g_ptLastClickPos.x == MousePosition.x &&
		g_ptLastClickPos.y == MousePosition.y &&
		(Magnus::Platform::GetTimeInSeconds() - g_fLastClickTime[iMouseButton]) < DOUBLE_CLICK_SPEED)
	{
		bIsDoubleClick = true;
	}

	if (bDown && !bIsDoubleClick)
	{
		g_fLastClickTime[iMouseButton] = Magnus::Platform::GetTimeInSeconds();
		g_ptLastClickPos = MousePosition;
	}
	
	if (bDown)
	{
		if (!FindKeyboardFocus(Magnus::HoveredControl))
		{
			if (Magnus::KeyboardFocus)
			{
				Magnus::KeyboardFocus->Blur();
			}
		}
	}

	// This tells the child it has been touched, which
	// in turn tells its parent, who tell their parents.
	// This is basically so that Windows can pop themselves
	// to the top when one of their children have been clicked.
	if (bDown)
	{
		Magnus::HoveredControl->Touch();
	}
	
	switch (iMouseButton)
	{
		case 0:
		{
			if (bIsDoubleClick)
			{
				Magnus::HoveredControl->OnMouseDoubleClickLeft(MousePosition.x, MousePosition.y);
			}
			else
			{
				Magnus::HoveredControl->OnMouseClickLeft(MousePosition.x, MousePosition.y, bDown);
			}

			return true;
		}
		case 1:
		{
			if (bIsDoubleClick)
			{
				Magnus::HoveredControl->OnMouseDoubleClickRight(MousePosition.x, MousePosition.y);
			}
			else
			{
				Magnus::HoveredControl->OnMouseClickRight(MousePosition.x, MousePosition.y, bDown);
			}

			return true;
		}
	}

	return false;
}

bool Magnus::Input::OnMouseWheeled(Controls::Control* pCanvas, int val)
{
	if (!Magnus::HoveredControl) { return false; }

	if (Magnus::HoveredControl == pCanvas) { return false; }

	if (Magnus::HoveredControl->GetCanvas() != pCanvas) { return false; }

	float fTime = Magnus::Platform::GetTimeInSeconds();

	if (fTime > KeyData.NextScroll)
	{
		KeyData.NextScroll = fTime + SCROLL_SPEED;
		return Magnus::HoveredControl->OnMouseWheeled(val);
	}

	return false;
}

bool Magnus::Input::HandleAccelerator(Controls::Control* pCanvas, Magnus::UnicodeChar chr)
{
	// Build the accelerator search string
	Magnus::UnicodeString accelString;

	if (Magnus::Input::IsControlDown())
	{
		accelString += Magnus::Utility::StringToUnicode("CTRL+");
	}

	if (Magnus::Input::IsShiftDown())
	{
		accelString += Magnus::Utility::StringToUnicode("SHIFT+");
	}

	chr = towupper(chr);
	accelString += chr;

	if (Magnus::KeyboardFocus && Magnus::KeyboardFocus->HandleAccelerator(accelString))
	{
		return true;
	}

	if (Magnus::MouseFocus && Magnus::MouseFocus->HandleAccelerator(accelString))
	{
		return true;
	}

	if (pCanvas->HandleAccelerator(accelString))
	{
		return true;
	}

	return false;
}

bool Magnus::Input::OnKeyCharEvent(Controls::Control* pCanvas, Magnus::UnicodeChar chr)
{
	if (!iswprint(chr)) { return false; }

	// Handle Accelerators
	if (Magnus::Input::HandleAccelerator(pCanvas, chr)) { return true; }

	// Handle characters
	if (!Magnus::KeyboardFocus) { return false; }

	if (Magnus::KeyboardFocus->GetCanvas() != pCanvas) { return false; }

	if (!Magnus::KeyboardFocus->Visible()) { return false; }

	if (Magnus::Input::IsControlDown()) { return false; }

	float fTime = Magnus::Platform::GetTimeInSeconds();

	if (KeyData.LastChar != chr)
	{
		KeyData.LastChar = chr;
		KeyData.NextCharRepeat = fTime + KEY_REPEAT_DELAY;
		return KeyboardFocus->OnChar(chr);
	}
	else if (fTime > KeyData.NextCharRepeat)
	{
		KeyData.NextCharRepeat = fTime + KEY_REPEAT_RATE;
		return KeyboardFocus->OnChar(chr);
	}

	return false;
}

bool Magnus::Input::OnKeyEvent(Controls::Control* pCanvas, int iKey, bool bDown)
{
	if (!bDown) { KeyData.LastChar = 0; }

	if (iKey <= Magnus::Key::Invalid) { return false; }

	if (iKey >= Magnus::Key::Count) { return false; }

	Controls::Control* pTarget = Magnus::KeyboardFocus;

	if (pTarget && pTarget->GetCanvas() != pCanvas) { pTarget = NULL; }

	if (pTarget && !pTarget->Visible()) { pTarget = NULL; }

	if (bDown)
	{
		std::list<KeyBindCaller_t>::iterator iter = KeyboardMap[iKey].begin();

		for (; iter != KeyboardMap[iKey].end(); ++iter)
		{
			KeyBindCaller_t & k = *iter;

			if (k.fnFunction)
			{
				(k.pObject->*k.fnFunction)();
			}
		}

		if (!KeyData.KeyState[iKey])
		{
			KeyData.KeyState[iKey] = true;
			KeyData.NextRepeat[iKey] = Magnus::Platform::GetTimeInSeconds() + KEY_REPEAT_DELAY;
			KeyData.Target = pTarget;

			if (pTarget)
			{
				return pTarget->OnKeyPress(iKey);
			}
		}
	}
	else
	{
		if (KeyData.KeyState[iKey])
		{
			KeyData.KeyState[iKey] = false;

			if (pTarget)
			{
				return pTarget->OnKeyRelease(iKey);
			}
		}
	}

	return false;
}

void Magnus::Input::AddKeyBindInternal(int iKey, Event::Handler::FunctionBlank pFunction, Event::Handler* pObject)
{
	if (iKey <= Magnus::Key::Invalid) { return; }

	if (iKey >= Magnus::Key::Count) { return; }

	std::list<KeyBindCaller_t>::iterator inner_iter;
	std::map<int, std::list<KeyBindCaller_t>>::iterator outer_iter;

	// Make sure that no other key is binded to the same function
	for (outer_iter = KeyboardMap.begin(); outer_iter != KeyboardMap.end(); ++outer_iter)
	{
		for (inner_iter = outer_iter->second.begin(); inner_iter != outer_iter->second.end(); ++inner_iter)
		{
			KeyBindCaller_t & k = *inner_iter;

			if (k.pObject == pObject && k.fnFunction == pFunction)
			{
				outer_iter->second.erase(inner_iter);
			}
		}
	}

	KeyBindCaller_t caller;
	caller.pObject = pObject;
	caller.fnFunction = pFunction;
	KeyboardMap[iKey].push_back(caller);
}