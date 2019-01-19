#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/TabStrip.h"
#include "Magnus/Utility.h"

using namespace Magnus;
using namespace Magnus::Controls;

void TabStrip::Layout(Magnus::Skin* skin)
{
	Magnus::Point ptLargetTab(5, 5);
	int iNum = 0;

	for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
	{
		TabButton* pButton = magnus_cast<TabButton>(*iter);

		if (!pButton) { continue; }

		pButton->SizeToContents();
		Margin m;
		int iNotFirst = iNum > 0 ? -1 : 0;

		if (m_iDock == Pos::Top)
		{
			m.left = iNotFirst;
			pButton->Dock(Pos::Left);
		}

		if (m_iDock == Pos::Left)
		{
			m.top = iNotFirst;
			pButton->Dock(Pos::Top);
		}

		if (m_iDock == Pos::Right)
		{
			m.top = iNotFirst;
			pButton->Dock(Pos::Top);
		}

		if (m_iDock == Pos::Bottom)
		{
			m.left = iNotFirst;
			pButton->Dock(Pos::Left);
		}

		ptLargetTab.x = Utility::Max(ptLargetTab.x, pButton->Width());
		ptLargetTab.y = Utility::Max(ptLargetTab.y, pButton->Height());
		pButton->SetMargin(m);
		iNum++;
	}

	if (m_iDock == Pos::Top || m_iDock == Pos::Bottom)
	{
		SetSize(Width(), ptLargetTab.y);
	}

	if (m_iDock == Pos::Left || m_iDock == Pos::Right)
	{
		SetSize(ptLargetTab.x, Height());
	}

	BaseClass::Layout(skin);
}

void TabStrip::SetTabPosition(int iPos)
{
	Dock(iPos);

	if (m_iDock == Pos::Top)
	{
		SetPadding(Padding(5, 0, 0, 0));
	}

	if (m_iDock == Pos::Left)
	{
		SetPadding(Padding(0, 5, 0, 0));
	}

	if (m_iDock == Pos::Right)
	{
		SetPadding(Padding(0, 5, 0, 0));
	}

	if (m_iDock == Pos::Bottom)
	{
		SetPadding(Padding(5, 0, 0, 0));
	}

	InvalidateChildren(true);
}