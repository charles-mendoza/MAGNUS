#include "Magnus/Utility.h"
#include "Magnus/Controls/ColorControls.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(ColorLerpBox)
{
	SetColor(Magnus::Color(255, 255, 128, 0));
	SetSize(128, 128);
	SetMouseInputEnabled(true);
	m_bDepressed = false;
}

Magnus::Color ColorLerpBox::GetSelectedColor()
{
	return GetColorAtPos(cursorPos.x, cursorPos.y);
}

void ColorLerpBox::SetColor(Magnus::Color color, bool onlyHue, bool bDoEvents)
{
	HSV hsv = Magnus::Utility::Colors::RGBToHSV(color.r, color.g, color.b);
	m_Hue = hsv.h;

	if (!onlyHue)
	{
		cursorPos.x = hsv.s * Width();
		cursorPos.y = (1 - hsv.v) * Height();
	}

	if (bDoEvents)
	{
		onSelectionChanged.Call(this);
	}
}

void ColorLerpBox::OnMouseMoved(int x, int y, int /*deltaX*/, int /*deltaY*/)
{
	if (m_bDepressed)
	{
		cursorPos = CanvasPosToLocal(Magnus::Point(x, y));

		// Do we have clamp?
		if (cursorPos.x < 0)
		{
			cursorPos.x = 0;
		}

		if (cursorPos.x > Width())
		{
			cursorPos.x = Width();
		}

		if (cursorPos.y < 0)
		{
			cursorPos.y = 0;
		}

		if (cursorPos.y > Height())
		{
			cursorPos.y = Height();
		}

		onSelectionChanged.Call(this);
	}
}

void ColorLerpBox::OnMouseClickLeft(int x, int y, bool bDown)
{
	m_bDepressed = bDown;

	if (bDown)
	{
		Magnus::MouseFocus = this;
	}
	else
	{
		Magnus::MouseFocus = NULL;
	}

	OnMouseMoved(x, y, 0, 0);
}

Magnus::Color ColorLerpBox::GetColorAtPos(int x, int y)
{
	float xPercent = ((float)x / (float)Width());
	float yPercent = 1 - ((float)y / (float)Height());
	Magnus::Color result = Magnus::Utility::Colors::HSVToColor(m_Hue, xPercent, yPercent);
	result.a = 255;
	return result;
}

void ColorLerpBox::Render(Magnus::Skin* skin)
{
	// Is there any way to move this into skin? Not for now, no idea how we'll "actually" render these
	for (int x = 0; x < Width(); x++)
	{
		for (int y = 0; y < Height(); y++)
		{
			skin->GetRenderer()->SetDrawColor(GetColorAtPos(x, y));
			skin->GetRenderer()->DrawPixel(x, y);
		}
	}

	skin->GetRenderer()->SetDrawColor(Magnus::Color(255, 0, 0, 0));
	skin->GetRenderer()->DrawLinedRect(GetRenderBounds());
	Magnus::Color selected = GetSelectedColor();

	if ((selected.r + selected.g + selected.b) / 3 < 170)
	{
		skin->GetRenderer()->SetDrawColor(Magnus::Color(255, 255, 255, 255));
	}
	else
	{
		skin->GetRenderer()->SetDrawColor(Magnus::Color(255, 0, 0, 0));
	}

	Magnus::Rect testRect = Magnus::Rect(cursorPos.x - 3, cursorPos.y - 3, 6, 6);
	skin->GetRenderer()->DrawShavedCornerRect(testRect);
}


MAGNUS_CONTROL_CONSTRUCTOR(ColorSlider)
{
	SetSize(32, 128);
	SetMouseInputEnabled(true);
	m_bDepressed = false;
}

void ColorSlider::Render(Magnus::Skin* skin)
{
	// Is there any way to move this into skin? Not for now, no idea how we'll "actually" render these
	for (int y = 0; y < Height(); y++)
	{
		float yPercent = (float)y / (float)Height();
		skin->GetRenderer()->SetDrawColor(Magnus::Utility::Colors::HSVToColor(yPercent * 360, 1, 1));
		skin->GetRenderer()->DrawFilledRect(Magnus::Rect(5, y, Width() - 10, 1));
	}

	int drawHeight = m_iSelectedDist - 3;
	// Draw our selectors
	skin->GetRenderer()->SetDrawColor(Magnus::Color(255, 0, 0, 0));
	skin->GetRenderer()->DrawFilledRect(Magnus::Rect(0, drawHeight + 2, Width(), 1));
	skin->GetRenderer()->DrawFilledRect(Magnus::Rect(0, drawHeight, 5, 5));
	skin->GetRenderer()->DrawFilledRect(Magnus::Rect(Width() - 5, drawHeight, 5, 5));
	skin->GetRenderer()->SetDrawColor(Magnus::Color(255, 255, 255, 255));
	skin->GetRenderer()->DrawFilledRect(Magnus::Rect(1, drawHeight + 1, 3, 3));
	skin->GetRenderer()->DrawFilledRect(Magnus::Rect(Width() - 4, drawHeight + 1, 3, 3));
}

void ColorSlider::OnMouseClickLeft(int x, int y, bool bDown)
{
	m_bDepressed = bDown;

	if (bDown)
	{
		Magnus::MouseFocus = this;
	}
	else
	{
		Magnus::MouseFocus = NULL;
	}

	OnMouseMoved(x, y, 0, 0);
}

Magnus::Color ColorSlider::GetColorAtHeight(int y)
{
	float yPercent = (float)y / (float)Height();
	return Magnus::Utility::Colors::HSVToColor(yPercent * 360, 1, 1);
}

void ColorSlider::OnMouseMoved(int x, int y, int /*deltaX*/, int /*deltaY*/)
{
	if (m_bDepressed)
	{
		Magnus::Point cursorPos = CanvasPosToLocal(Magnus::Point(x, y));

		if (cursorPos.y < 0)
		{
			cursorPos.y = 0;
		}

		if (cursorPos.y > Height())
		{
			cursorPos.y = Height();
		}

		m_iSelectedDist = cursorPos.y;
		onSelectionChanged.Call(this);
	}
}

void ColorSlider::SetColor(Magnus::Color color, bool bDoEvents)
{
	HSV hsv = Magnus::Utility::Colors::RGBToHSV(color.r, color.g, color.b);
	m_iSelectedDist = hsv.h / 360 * Height();

	if (bDoEvents)
	{
		onSelectionChanged.Call(this);
	}
}

Magnus::Color ColorSlider::GetSelectedColor()
{
	return GetColorAtHeight(m_iSelectedDist);
}