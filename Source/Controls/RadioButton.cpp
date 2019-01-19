#include "Magnus/Controls/RadioButton.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(RadioButton)
{
	SetSize(15, 15);
	SetMouseInputEnabled(true);
	SetTabable(false);
}

void RadioButton::Render(Magnus::Skin* skin)
{
	skin->DrawRadioButton(GetRenderBounds(), IsChecked(), IsDepressed(), IsDisabled());
}