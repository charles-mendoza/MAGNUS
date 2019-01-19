#include "Magnus/Controls/Window.h"

using namespace Magnus;
using namespace Magnus::Controls;
using namespace Magnus::ControlsInternal;

MAGNUS_CONTROL_CONSTRUCTOR(Window)
{
	m_bDeleteOnClose = false;
	m_TitleBar = new Dragger(this);
	m_TitleBar->SetHeight(24);
	m_TitleBar->SetPadding(Padding(0, 0, 0, 0));
	m_TitleBar->SetTarget(this);
	m_TitleBar->Dock(Pos::Top);
	m_Title = new Label(m_TitleBar);
	m_Title->SetAlignment(Pos::Center);
	m_Title->SetText("Window");
	m_Title->Dock(Pos::Fill);
	m_Title->SetPadding(Padding(8, 0, 0, 0));
	m_Title->SetTextColor(GetSkin()->Colors.Window.TitleInactive);
	m_CloseButton = new Magnus::Controls::WindowCloseButton(m_TitleBar);
	m_CloseButton->SetText("");
	m_CloseButton->SetPos(100, 3);
	m_CloseButton->Dock(Pos::Right | Pos::CenterV);
	m_CloseButton->onPress.Add(this, &Window::CloseButtonPressed);
	m_CloseButton->SetTabable(false);
	m_CloseButton->SetName("closeButton");
	m_CloseButton->SetWindow(this);
	m_CloseButton->SetMargin(Margin(0, 8, 12, 8));
	// Create a blank content control, dock it to the top - Should this be a ScrollControl?
	m_InnerPanel = new Control(this);
	m_InnerPanel->SetPadding(Padding(2, 0, 2, 2));
	m_InnerPanel->Dock(Pos::Fill);
	BringToFront();
	SetTabable(false);
	Focus();
	SetKeyboardInputEnabled(false);
}

bool Window::IsOnTop()
{
	for (Control::List::reverse_iterator iter = GetParent()->Children.rbegin(); iter != GetParent()->Children.rend(); ++iter)
	{
		Window* pWindow = magnus_cast<Window>(*iter);

		if (!pWindow) { continue; }

		if (pWindow == this) { return true; }

		return false;
	}

	return false;
}

void Window::Render(Magnus::Skin* skin)
{
	bool bHasFocus = IsOnTop();

	if (bHasFocus)
	{
		m_Title->SetTextColor(GetSkin()->Colors.Window.TitleActive);
	}
	else
	{
		m_Title->SetTextColor(GetSkin()->Colors.Window.TitleInactive);
	}

	skin->DrawWindow(GetRenderBounds(), bHasFocus);
}

void Window::RenderUnder(Magnus::Skin* skin)
{
	skin->DrawShadow(GetRenderBounds());
}

void Window::SetTitle(const Magnus::TextObject & title)
{
	m_Title->SetText(title);
}

void Window::SetClosable(bool closable)
{
	m_CloseButton->SetHidden(!closable);
}

void Window::SetHidden(bool hidden)
{
	if (!hidden) { BringToFront(); }

	BaseClass::SetHidden(hidden);
}

void Window::Touch()
{
	BaseClass::Touch();
	BringToFront();
}

void Window::CloseButtonPressed()
{
	onWindowClosed.Call(this);
	SetHidden(true);

	if (m_bDeleteOnClose)
	{
		DelayedDelete();
	}
}

void Window::RenderFocus(Magnus::Skin* /*skin*/)
{
}