#include "Magnus/Controls/ComboBox.h"
#include "Magnus/Controls/Menu.h"

using namespace Magnus;
using namespace Magnus::Controls;
using namespace Magnus::ControlsInternal;

class DownArrow : public Controls::Control
{
public:

	MAGNUS_CONTROL_INLINE(DownArrow, Controls::Control)
	{
		SetMouseInputEnabled(false);
		SetSize(15, 15);
	}

	void Render(Magnus::Skin* skin)
	{
		if (!m_ComboBox->ShouldDrawBackground())
		{
			return skin->DrawComboDownArrow(GetRenderBounds(), false, false, false, m_ComboBox->IsDisabled());
		}

		skin->DrawComboDownArrow(GetRenderBounds(), m_ComboBox->IsDepressed(), m_ComboBox->IsMenuOpen(), m_ComboBox->IsHovered(), m_ComboBox->IsDisabled());
	}

	void SetComboBox(ComboBox* pComboBox) { m_ComboBox = pComboBox; }

protected:

	ComboBox* m_ComboBox;
};

MAGNUS_CONTROL_CONSTRUCTOR(ComboBox)
{
	SetSize(100, 20);
	m_Menu = new Menu(this);
	m_Menu->SetHidden(true);
	m_Menu->SetDisableIconMargin(true);
	m_Menu->SetTabable(false);
	DownArrow* pArrow = new DownArrow(this);
	pArrow->SetComboBox(this);
	m_Button = pArrow;
	m_SelectedItem = NULL;
	SetAlignment(Pos::Left | Pos::CenterV);
	SetText(L"");
	SetMargin(Margin(3, 0, 0, 0));
	SetTabable(true);
	SetKeyboardInputEnabled(true);
}

MenuItem* ComboBox::AddItem(const TextObject & strLabel, const String & strName)
{
	MenuItem* pItem = m_Menu->AddItem(strLabel, strName);
	pItem->onMenuItemSelected.Add(this, &ComboBox::OnItemSelected);

	// Default
	if (m_SelectedItem == NULL)
	{
		OnItemSelected(pItem);
	}

	return pItem;
}

void ComboBox::Render(Magnus::Skin* skin)
{
	if (!ShouldDrawBackground()) { return; }

	skin->DrawComboBox(GetRenderBounds(), IsDepressed(), IsMenuOpen(), IsHovered(), IsDisabled());
}

void ComboBox::Layout(Magnus::Skin* skin)
{
	m_Button->Position(Pos::Right | Pos::CenterV, 4, 0);
	BaseClass::Layout(skin);
}

void ComboBox::UpdateColours()
{
	if (!ShouldDrawBackground())
	{
		return SetTextColor(GetSkin()->Colors.Button.Normal);
	}

	BaseClass::UpdateColours();
}

void ComboBox::OnPress()
{
	if (IsMenuOpen())
	{
		return GetCanvas()->CloseMenus();
	}

	bool bWasMenuHidden = m_Menu->Hidden();
	GetCanvas()->CloseMenus();

	if (bWasMenuHidden)
	{
		OpenList();
	}
}

void ComboBox::ClearItems()
{
	if (m_Menu)
	{
		m_Menu->ClearItems();
	}
}

void ComboBox::SelectItem(MenuItem* pItem, bool bFireChangeEvents)
{
	if (m_SelectedItem == pItem) { return; }

	m_SelectedItem = pItem;
	SetText(m_SelectedItem->GetText());
	m_Menu->SetHidden(true);
	Invalidate();

	if (bFireChangeEvents)
	{
		onSelection.Call(this);
		Focus();
	}
}

void ComboBox::SelectItemByIndex(unsigned int index, bool bFireChangeEvents)
{
	MenuItem* pItem = m_Menu->GetItemAt(index);

	if (!pItem) { return; }

	SelectItem(pItem, bFireChangeEvents);
}

int ComboBox::GetSelectedIndex()
{
	Control::List & children = m_Menu->GetChildren();
	Control::List::iterator it = std::find(children.begin(), children.end(), m_SelectedItem);
	int index = std::distance(children.begin(), it);
	return index;
}

void ComboBox::OnItemSelected(Controls::Control* pControl)
{
	// Convert selected to a menu item
	MenuItem* pItem = magnus_cast<MenuItem>(pControl);

	if (!pItem) { return; }

	SelectItem(pItem);
}

void ComboBox::SelectItemByName(const Magnus::String & name, bool bFireChangeEvents)
{
	Control::List & children = m_Menu->GetChildren();
	Control::List::iterator it = children.begin();

	while (it != children.end())
	{
		MenuItem* pChild = magnus_cast<MenuItem>(*it);

		if (pChild->GetName() == name)
		{
			return SelectItem(pChild, bFireChangeEvents);
		}

		++it;
	}
}

Magnus::Controls::Label* ComboBox::GetSelectedItem()
{
	return m_SelectedItem;
}

bool ComboBox::IsMenuOpen()
{
	return m_Menu && !m_Menu->Hidden();
}

void ComboBox::OpenList()
{
	if (!m_Menu) { return; }

	m_Menu->SetParent(GetCanvas());
	m_Menu->SetHidden(false);
	m_Menu->BringToFront();
	Magnus::Point p = LocalPosToCanvas(Magnus::Point(0, 0));
	m_Menu->SetBounds(Magnus::Rect(p.x, p.y + Height(), Width(), m_Menu->Height()));
}

void ComboBox::CloseList()
{
	if (!m_Menu) { return; }

	m_Menu->Hide();
}

bool ComboBox::OnKeyUp(bool bDown)
{
	if (bDown)
	{
		Control::List & children = m_Menu->GetChildren();
		Control::List::reverse_iterator it = std::find(children.rbegin(), children.rend(), m_SelectedItem);

		if (it != children.rend() && (++it != children.rend()))
		{
			Control* pUpElement = *it;
			OnItemSelected(pUpElement);
		}
	}

	return true;
}

bool ComboBox::OnKeyDown(bool bDown)
{
	if (bDown)
	{
		Control::List & children = m_Menu->GetChildren();
		Control::List::iterator it = std::find(children.begin(), children.end(), m_SelectedItem);

		if (it != children.end() && (++it != children.end()))
		{
			Control* pDownElement = *it;
			OnItemSelected(pDownElement);
		}
	}

	return true;
}

void ComboBox::OnOpacityChanged()
{
	m_Menu->SetOpacity(Opacity());
}

void ComboBox::RenderFocus(Magnus::Skin* /*skin*/)
{
}