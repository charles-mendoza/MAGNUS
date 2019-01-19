#include "Magnus/Controls/CheckBox.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(CheckBox)
{
	m_bChecked = false;
	SetSize(15, 15);
}

void CheckBox::Render(Magnus::Skin* skin)
{
	skin->DrawCheckBox(GetRenderBounds(), m_bChecked, IsDepressed(), IsDisabled());
}

void CheckBox::OnPress()
{
	if (IsDisabled())
		return;

	if (IsChecked() && !AllowUncheck())
		return;

	Toggle();
}

void CheckBox::OnCheckStatusChanged()
{
	onCheckChanged.Call(this);

	if (IsChecked())
		onChecked.Call(this);
	else
		onUnChecked.Call(this);
}

void CheckBox::SetChecked(bool bChecked)
{
	if (m_bChecked == bChecked) { return; }

	m_bChecked = bChecked;
	OnCheckStatusChanged();
}