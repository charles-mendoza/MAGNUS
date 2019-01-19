#include "Magnus/Controls/ColorPicker.h"
#include "Magnus/Utility.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(ColorPicker)
{
	SetSize(256, 64);
	SetCacheToTexture();
	SetMouseInputEnabled(true);
	m_LerpBox = new Magnus::Controls::ColorLerpBox(this);
	m_LerpBox->onSelectionChanged.Add(this, &ColorPicker::ColorBoxChanged);
	m_LerpBox->Dock(Pos::Left);
	m_ColorSlider = new Magnus::Controls::ColorSlider(this);
	m_ColorSlider->SetPos(m_LerpBox->Width() + 15, 5);
	m_ColorSlider->onSelectionChanged.Add(this, &ColorPicker::ColorSliderChanged);
	m_ColorSlider->Dock(Pos::Left);
	m_Display = new Magnus::ControlsInternal::ColorDisplay(this);
	m_Display->SetSize(48, 24);
	m_Display->SetPos(m_ColorSlider->X() + m_ColorSlider->Width() + 15, 5);
	int x = m_Display->X();
	int y = m_Display->Y() + 30;
	{
		Label* label = new Label(this);
		label->SetText("R:");
		label->SizeToContents();
		label->SetPos(x, y);
		m_RedBox = new TextBoxNumeric(this);
		m_RedBox->SetName("RedBox");
		m_RedBox->SetPos(x + 15, y - 1);
		m_RedBox->SetSize(26, 16);
		m_RedBox->SetSelectAllOnFocus(true);
		m_RedBox->onTextChanged.Add(this, &ColorPicker::NumericTyped);
	}
	y += 20;
	{
		Label* label = new Label(this);
		label->SetText("G:");
		label->SizeToContents();
		label->SetPos(x, y);
		m_GreenBox = new TextBoxNumeric(this);
		m_GreenBox->SetName("GreenBox");
		m_GreenBox->SetPos(x + 15, y - 1);
		m_GreenBox->SetSize(26, 16);
		m_GreenBox->SetSelectAllOnFocus(true);
		m_GreenBox->onTextChanged.Add(this, &ColorPicker::NumericTyped);
	}
	y += 20;
	{
		Label* label = new Label(this);
		label->SetText("B:");
		label->SizeToContents();
		label->SetPos(x, y);
		m_BlueBox = new TextBoxNumeric(this);
		m_BlueBox->SetName("BlueBox");
		m_BlueBox->SetPos(x + 15, y - 1);
		m_BlueBox->SetSize(26, 16);
		m_BlueBox->SetSelectAllOnFocus(true);
		m_BlueBox->onTextChanged.Add(this, &ColorPicker::NumericTyped);
	}
	y += 20;
	{
		Label* label = new Label(this);
		label->SetText("A:");
		label->SizeToContents();
		label->SetPos(x, y);
		m_AlphaBox = new TextBoxNumeric(this);
		m_AlphaBox->SetName("AlphaBox");
		m_AlphaBox->SetPos(x + 15, y - 1);
		m_AlphaBox->SetSize(26, 16);
		m_AlphaBox->SetSelectAllOnFocus(true);
		m_AlphaBox->onTextChanged.Add(this, &ColorPicker::NumericTyped);
	}
}

void ColorPicker::NumericTyped(Controls::Control* pControl)
{
	TextBoxNumeric* box = magnus_cast<TextBoxNumeric>(pControl);

	if (!box) { return; }

	if (box->GetText() == "") { return; }

	int textValue = atoi(box->GetText().Get().c_str());

	textValue = Utility::Clamp(textValue, 0, 255);

	Magnus::Color newColor = GetColor();

	if (box->GetName() == "RedBox")
		newColor.r = textValue;
	else if (box->GetName() == "GreenBox")
		newColor.g = textValue;
	else if (box->GetName() == "BlueBox")
		newColor.b = textValue;
	else if (box->GetName() == "AlphaBox")
		newColor.a = textValue;

	SetColor(newColor);
}

void ColorPicker::UpdateControls(Magnus::Color color)
{
	m_RedBox->SetText(Magnus::Utility::ToString((int)color.r), false);
	m_GreenBox->SetText(Magnus::Utility::ToString((int)color.g), false);
	m_BlueBox->SetText(Magnus::Utility::ToString((int)color.b), false);
	m_AlphaBox->SetText(Magnus::Utility::ToString((int)color.a), false);
	m_Display->SetColor(color);
}

void ColorPicker::SetColor(Magnus::Color color, bool onlyHue)
{
	m_Color = color;
	UpdateControls(color);
	m_ColorSlider->SetColor(color, false);
	m_LerpBox->SetColor(color, onlyHue, false);
	onColorChanged.Call(this);
	Invalidate();
}

void ColorPicker::ColorBoxChanged(Magnus::Controls::Control* /*pControl*/)
{
	Magnus::Color color = m_LerpBox->GetSelectedColor();
	m_Color = Color(m_Color.a, color.r, color.g, color.b);
	onColorChanged.Call(this);
	UpdateControls(GetColor());
	Invalidate();
}

void ColorPicker::ColorSliderChanged(Magnus::Controls::Control* /*pControl*/)
{
	m_LerpBox->SetColor(m_ColorSlider->GetSelectedColor(), true, true);
	Invalidate();
}