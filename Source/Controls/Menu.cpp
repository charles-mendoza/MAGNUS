#include "Magnus/Magnus.h"
#include "Magnus/Controls/Menu.h"
#include "Magnus/Skin.h"
#include "Magnus/Utility.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(Menu)
{
	SetBounds(0, 0, 10, 10);
	SetPadding(Padding(2, 2, 2, 2));
	SetDisableIconMargin(false);
	SetAutoHideBars(true);
	SetScroll(false, true);
	SetMaxContentHeight(210);
	SetDeleteOnClose(false);
}

void Menu::Render(Magnus::Skin* skin)
{
	skin->DrawMenu(GetRenderBounds(), IconMarginDisabled());
}

void Menu::RenderUnder(Magnus::Skin* skin)
{
	skin->DrawShadow(GetRenderBounds());
}

void Menu::Layout(Magnus::Skin* skin)
{
	int childrenHeight = 0;

	for (Control::List::iterator it = m_InnerPanel->Children.begin(); it != m_InnerPanel->Children.end(); ++it)
	{
		Control* pChild = *it;

		if (!pChild) { continue; }

		childrenHeight += pChild->Height();
	}

	if (Y() + childrenHeight > GetCanvas()->Height())
	{
		childrenHeight = GetCanvas()->Height() - Y();
	}

	childrenHeight = Utility::Clamp(childrenHeight, 0, MaxContentHeight());

	SetSize(Width(), childrenHeight);
	BaseClass::Layout(skin);
}

MenuItem* Menu::AddItem(const TextObject & strName, const TextObject & strIconName, const TextObject & strAccelerator)
{
	MenuItem* pItem = new MenuItem(this);
	pItem->SetPadding(Padding(2, 4, 4, 4));
	pItem->SetText(strName);
	pItem->SetImage(strIconName);
	pItem->SetAccelerator(strAccelerator);
	OnAddItem(pItem);
	return pItem;
}

MenuItem* Menu::GetItemAt(unsigned int index)
{
	Control* pControl = m_InnerPanel->GetChild(index);
	MenuItem* pItem = magnus_cast<MenuItem>(pControl);
	return pItem;
}

void Menu::OnAddItem(MenuItem* item)
{
	item->SetTextPadding(Padding(IconMarginDisabled() ? 0 : 24, 0, 16, 0));
	item->Dock(Pos::Top);
	item->SizeToContents();
	item->SetAlignment(Pos::CenterV | Pos::Left);
	item->onHoverEnter.Add(this, &Menu::OnHoverItem);
	// Do this here - after Top Docking these values mean nothing in layout
	int w = item->Width() + 10 + 32;

	if (w < Width()) { w = Width(); }

	SetSize(w, Height());
}

void Menu::ClearItems()
{
	for (Control::List::iterator it = m_InnerPanel->Children.begin(); it != m_InnerPanel->Children.end(); ++it)
	{
		Control* pChild = *it;

		if (!pChild) { continue; }

		pChild->DelayedDelete();
	}
}

void Menu::CloseAll()
{
	for (Control::List::iterator it = m_InnerPanel->Children.begin(); it != m_InnerPanel->Children.end(); ++it)
	{
		Control* pChild = *it;
		MenuItem* pItem = magnus_cast<MenuItem>(pChild);

		if (!pItem) { continue; }

		pItem->CloseMenu();
	}
}

bool Menu::IsMenuOpen()
{
	for (Control::List::iterator it = m_InnerPanel->Children.begin(); it != m_InnerPanel->Children.end(); ++it)
	{
		Control* pChild = *it;
		MenuItem* pItem = magnus_cast<MenuItem>(pChild);

		if (!pItem) { continue; }

		if (pItem->IsMenuOpen())
		{
			return true;
		}
	}

	return false;
}

void Menu::OnHoverItem(Controls::Control* pControl)
{
	if (!ShouldHoverOpenMenu()) { return; }

	MenuItem* pItem = magnus_cast<MenuItem>(pControl);

	if (!pItem) { return; }

	if (pItem->IsMenuOpen()) { return; }

	CloseAll();
	pItem->OpenMenu();
}

void Menu::Open(unsigned int iPos)
{
	SetHidden(false);
	BringToFront();
	Magnus::Point MousePos = Input::GetMousePosition();
	SetPos(MousePos.x, MousePos.y);
}

void Menu::Close()
{
	SetHidden(true);

	if (DeleteOnClose())
	{
		DelayedDelete();
	}
}

void Menu::CloseMenus()
{
	BaseClass::CloseMenus();
	CloseAll();
	Close();
	onMenuClosed.Call(this);
}

void Menu::AddDivider()
{
	MenuDivider* divider = new MenuDivider(this);
	divider->Dock(Pos::Top);
	divider->SetMargin(Margin(IconMarginDisabled() ? 0 : 24, 0, 4, 0));
}

void MenuDivider::Render(Magnus::Skin* skin)
{
	skin->DrawMenuDivider(GetRenderBounds());
}