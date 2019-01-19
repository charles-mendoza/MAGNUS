#include "Magnus/Magnus.h"
#include "Magnus/Utility.h"
#include "Magnus/Skin.h"
#include "Magnus/Controls/NumericUpDown.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(NumericUpDown)
{
	SetSize(100, 20);
	Controls::Control* pSplitter = new Controls::Control(this);
	pSplitter->Dock(Pos::Right);
	pSplitter->SetWidth(13);
	NumericUpDownButton_Up* pButtonUp = new NumericUpDownButton_Up(pSplitter);
	pButtonUp->onPress.Add(this, &NumericUpDown::OnButtonUp);
	pButtonUp->SetTabable(false);
	pButtonUp->Dock(Pos::Top);
	pButtonUp->SetHeight(10);
	NumericUpDownButton_Down* pButtonDown = new NumericUpDownButton_Down(pSplitter);
	pButtonDown->onPress.Add(this, &NumericUpDown::OnButtonDown);
	pButtonDown->SetTabable(false);
	pButtonDown->Dock(Pos::Fill);
	pButtonUp->SetPadding(Padding(0, 1, 1, 0));
	m_iMax = 100;
	m_iMin = 0;
	m_iNumber = 0;
	SetText("0");
}

void NumericUpDown::OnButtonUp(Control* /*control*/)
{
	SyncNumberFromText();
	SetValue(m_iNumber + 1);
}

void NumericUpDown::OnButtonDown(Control* /*control*/)
{
	SyncNumberFromText();
	SetValue(m_iNumber - 1);
}

void NumericUpDown::SyncTextFromNumber()
{
	SetText(Utility::ToString(m_iNumber));
}

void NumericUpDown::SyncNumberFromText()
{
	SetValue((int)GetFloatFromText());
}

void NumericUpDown::SetMin(int i)
{
	m_iMin = i;
}

void NumericUpDown::SetMax(int i)
{
	m_iMax = i;
}

void NumericUpDown::SetValue(int i)
{
	i = Utility::Clamp(i, m_iMin, m_iMax);

	if (m_iNumber == i) { return; }

	m_iNumber = i;
	
	SyncTextFromNumber();

	OnChange();
}

void NumericUpDown::OnChange()
{
	onChanged.Call(this);
}

void NumericUpDown::OnTextChanged()
{
	BaseClass::OnTextChanged();
	SyncNumberFromText();
}

void NumericUpDown::OnEnter()
{
	SyncNumberFromText();
	SyncTextFromNumber();
	BaseClass::OnEnter();
}

void NumericUpDown::OnLostKeyboardFocus()
{
	SyncNumberFromText();
	SyncTextFromNumber();
	BaseClass::OnLostKeyboardFocus();
}