#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/TabControl.h"
#include "Magnus/Controls/ScrollButton.h"

using namespace Magnus;
using namespace Magnus::Controls;

class TabControlInner : public Control
{
public:

	MAGNUS_CONTROL_INLINE(TabControlInner, Control) {}

	void Render(Magnus::Skin* skin)
	{
		skin->DrawTabControl(GetRenderBounds());
	}
};

MAGNUS_CONTROL_CONSTRUCTOR(TabControl)
{
	m_iScrollOffset = 0;
	m_pCurrentButton = NULL;
	m_TabStrip = new TabStrip(this);
	m_TabStrip->SetTabPosition(Pos::Top);
	// Make this some special control?
	m_pScroll[0] = new ControlsInternal::ScrollButton(this);
	m_pScroll[0]->SetDirectionLeft();
	m_pScroll[0]->onPress.Add(this, &TabControl::ScrollPressLeft);
	m_pScroll[0]->SetSize(14, 14);
	m_pScroll[1] = new ControlsInternal::ScrollButton(this);
	m_pScroll[1]->SetDirectionRight();
	m_pScroll[1]->onPress.Add(this, &TabControl::ScrollPressRight);
	m_pScroll[1]->SetSize(14, 14);
	m_InnerPanel = new TabControlInner(this);
	m_InnerPanel->Dock(Pos::Fill);
	m_InnerPanel->SendToBack();
	SetTabable(false);
}

TabButton* TabControl::AddPage(const TextObject & strText, Controls::Control* pPage)
{
	if (!pPage)
		pPage = new Control(this);
	else
		pPage->SetParent(this);

	TabButton* pButton = new TabButton(m_TabStrip);
	pButton->SetText(strText);
	pButton->SetPage(pPage);
	pButton->SetTabable(false);
	AddPage(pButton);
	return pButton;
}

void TabControl::RemovePage(TabButton* pButton)
{
	pButton->SetParent(GetCanvas());
	OnLoseTab(pButton);
}

void TabControl::AddPage(TabButton* pButton)
{
	Control* pPage = pButton->GetPage();
	pPage->SetParent(this);
	pPage->SetHidden(true);
	pPage->SetMargin(Margin(6, 6, 6, 6));
	pPage->Dock(Pos::Fill);
	pButton->SetParent(m_TabStrip);
	pButton->Dock(Pos::Left);
	pButton->SizeToContents();

	if (pButton->GetTabControl())
		pButton->onPress.RemoveHandler(pButton->GetTabControl());

	pButton->SetTabControl(this);
	pButton->onPress.Add(this, &TabControl::OnTabPressed);

	if (!m_pCurrentButton)
		pButton->OnPress();

	onAddTab.Call(this);
	Invalidate();
}

void TabControl::OnTabPressed(Controls::Control* control)
{
	TabButton* pButton = magnus_cast<TabButton>(control);

	if (!pButton) { return; }

	Control* pPage = pButton->GetPage();

	if (!pPage) { return; }

	if (m_pCurrentButton == pButton) { return; }

	if (m_pCurrentButton)
	{
		Control* pPage = m_pCurrentButton->GetPage();

		if (pPage)
			pPage->SetHidden(true);

		m_pCurrentButton->Redraw();
		m_pCurrentButton = NULL;
	}

	m_pCurrentButton = pButton;
	pPage->SetHidden(false);
	m_TabStrip->Invalidate();
	Invalidate();
}

void TabControl::PostLayout(Magnus::Skin* skin)
{
	BaseClass::PostLayout(skin);
	HandleOverflow();
}

void TabControl::OnLoseTab(TabButton* pButton)
{
	if (m_pCurrentButton == pButton)
		m_pCurrentButton = NULL;

	// TODO: Select a tab if any exist.
	onLoseTab.Call(this);
	Invalidate();
}

int TabControl::TabCount()
{
	return m_TabStrip->NumChildren();
}

TabButton* TabControl::GetTab(int iNum)
{
	return magnus_cast<TabButton>(m_TabStrip->GetChild(iNum));
}

void TabControl::SetTabStripPosition(int iDock)
{
	m_TabStrip->SetTabPosition(iDock);
}

void TabControl::HandleOverflow()
{
	Magnus::Size TabsSize = m_TabStrip->ChildrenSize();
	// Only enable the scrollers if the tabs are at the top.
	// This is a limitation we should explore.
	// Really TabControl should have derivatives for tabs placed elsewhere where we could specialize
	// some functions like this for each direction.
	bool bNeeded = TabsSize.w > Width() && m_TabStrip->GetDock() == Pos::Top;
	m_pScroll[0]->SetHidden(!bNeeded);
	m_pScroll[1]->SetHidden(!bNeeded);

	if (!bNeeded) { return; }

	m_iScrollOffset = Magnus::Utility::Clamp(m_iScrollOffset, 0, TabsSize.w - Width() + 32);
#if 0
	//
	// This isn't frame rate independent.
	// Could be better. Get rid of m_iScrollOffset and just use m_TabStrip->GetMargin().left?
	// Then get a margin animation type and do it properly!
	// TODO!
	//
	m_TabStrip->SetMargin(Margin(Magnus::Approach(m_TabStrip->GetMargin().left, m_iScrollOffset * -1, 2), 0, 0, 0));
	InvalidateParent();
#else
	m_TabStrip->SetMargin(Margin(m_iScrollOffset * -1, 0, 0, 0));
#endif
	m_pScroll[0]->SetPos(Width() - 30, 5);
	m_pScroll[1]->SetPos(m_pScroll[0]->Right(), 5);
}

void TabControl::ScrollPressLeft(Control* pFrom)
{
	m_iScrollOffset -= 120;
}

void TabControl::ScrollPressRight(Control* pFrom)
{
	m_iScrollOffset += 120;
}