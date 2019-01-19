#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/LabelClickable.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(LabelClickable)
{
	SetIsToggle(false);
	SetAlignment(Magnus::Pos::Left | Magnus::Pos::CenterV);
}

void LabelClickable::Render(Magnus::Skin* /*skin*/)
{
}