#include "Magnus/Controls/GroupBox.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(GroupBox)
{
	// Set to true, because it's likely that our
	// children will want mouse input, and they
	// can't get it without us..
	SetMouseInputEnabled(true);
	SetTextPadding(Padding(10, 0, 0, 0));
	SetTextColor(Color(255, 139, 147, 139));
	SetAlignment(Pos::Top | Pos::Left);
	Invalidate();
	m_InnerPanel = new Control(this);
	m_InnerPanel->Dock(Pos::Fill);
	m_InnerMargin = 10;
}

void GroupBox::Layout(Magnus::Skin* skin)
{
	m_InnerPanel->SetMargin(Margin(m_InnerMargin, (TextHeight() / 2) + m_InnerMargin, m_InnerMargin, m_InnerMargin));
	BaseClass::Layout(skin);
}

void GroupBox::Render(Magnus::Skin* skin)
{
	skin->DrawGroupBox(GetRenderBounds(), TextX(), TextHeight(), TextWidth());
}