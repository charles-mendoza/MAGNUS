#include "Magnus/Magnus.h"
#include "Magnus/Controls/TextBox.h"
#include "Magnus/Skin.h"
#include "Magnus/Utility.h"
#include "Magnus/Platform.h"

#include <math.h>

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(TextBox)
{
	SetSize(100, 20);
	SetMouseInputEnabled(true);
	SetKeyboardInputEnabled(true);
	SetAlignment(Pos::Left | Pos::CenterV);
	SetPadding(Padding(4, 2, 4, 2));
	SetTextColor(Color(255, 139, 147, 139));
	SetText("");
	m_iCursorPos = 0;
	m_iCursorEnd = 0;
	m_iCursorLine = 0;
	m_bEditable = true;
	m_bSelectAll = false;
	SetTabable(true);
	AddAccelerator("Ctrl + C", &TextBox::OnCopy);
	AddAccelerator("Ctrl + X", &TextBox::OnCut);
	AddAccelerator("Ctrl + V", &TextBox::OnPaste);
	AddAccelerator("Ctrl + A", &TextBox::OnSelectAll);
}

bool TextBox::OnChar(Magnus::UnicodeChar c)
{
	if (c == '\t') { return false; }

	Magnus::UnicodeString str;
	str += c;
	InsertText(str);
	return true;
}

void TextBox::InsertText(const Magnus::UnicodeString & strInsert)
{
	if (!m_bEditable) return;

	// TODO: Make sure fits (implement maxlength)
	if (HasSelection())
	{
		EraseSelection();
	}

	if (m_iCursorPos > TextLength()) { m_iCursorPos = TextLength(); }

	if (!IsTextAllowed(strInsert, m_iCursorPos)) { return; }

	UnicodeString str = GetText().GetUnicode();
	str.insert(m_iCursorPos, strInsert);
	SetText(str);
	m_iCursorPos += (int)strInsert.size();
	m_iCursorEnd = m_iCursorPos;
	m_iCursorLine = 0;
	RefreshCursorBounds();
}

void TextBox::Think()
{
	if (m_fNextCaretColorChange > Magnus::Platform::GetTimeInSeconds()) { return; }

	if (!HasFocus()) { m_fNextCaretColorChange = Magnus::Platform::GetTimeInSeconds() + 0.5f; }

	Magnus::Color targetcolor = Magnus::Color(0, 0, 0, 0);

	if (m_CaretColor == targetcolor)
	{
		targetcolor = Magnus::Colors::White;
	}

	m_fNextCaretColorChange = Magnus::Platform::GetTimeInSeconds() + 0.5;
	m_CaretColor = targetcolor;
	Redraw();
}

void TextBox::Render(Magnus::Skin* skin)
{
	if (ShouldDrawBackground())
	{
		skin->DrawTextBox(GetRenderBounds(), HasFocus(), IsDisabled());
	}

	if (!HasFocus()) { return; }

	// Draw selection.. if selected..
	if (m_iCursorPos != m_iCursorEnd)
	{
		skin->GetRenderer()->SetDrawColor(Magnus::Color(200, 50, 170, 255));
		skin->GetRenderer()->DrawFilledRect(m_SelectionBounds);
	}

	// Draw caret
	skin->GetRenderer()->SetDrawColor(m_CaretColor);
	skin->GetRenderer()->DrawFilledRect(m_CaretBounds);
}

void TextBox::RefreshCursorBounds()
{
	m_fNextCaretColorChange = Magnus::Platform::GetTimeInSeconds() + 1.5f;
	m_CaretColor = Magnus::Colors::White;
	MakeCaretVisible();
	Magnus::Rect pA = GetCharacterPosition(m_iCursorPos);
	Magnus::Rect pB = GetCharacterPosition(m_iCursorEnd);
	m_SelectionBounds.x = Utility::Min(pA.x, pB.x);
	m_SelectionBounds.y = m_Text->Y() - 1;
	m_SelectionBounds.w = Utility::Max(pA.x, pB.x) - m_SelectionBounds.x;
	m_SelectionBounds.h = m_Text->Height() + 2;
	m_CaretBounds.x = pA.x;
	m_CaretBounds.y = pA.y;
	m_CaretBounds.w = 1;
	m_CaretBounds.h = pA.h;
	Redraw();
}

void TextBox::OnPaste(Magnus::Controls::Control* /*pCtrl*/)
{
	InsertText(Platform::GetClipboardText());
}

void TextBox::OnCopy(Magnus::Controls::Control* /*pCtrl*/)
{
	if (!HasSelection()) { return; }

	Platform::SetClipboardText(GetSelection());
}

void TextBox::OnCut(Magnus::Controls::Control* /*pCtrl*/)
{
	if (!HasSelection()) { return; }

	Platform::SetClipboardText(GetSelection());
	EraseSelection();
}

void TextBox::OnSelectAll(Magnus::Controls::Control* /*pCtrl*/)
{
	m_iCursorEnd = 0;
	m_iCursorPos = TextLength();
	m_iCursorLine = 0;
	RefreshCursorBounds();
}

void TextBox::OnMouseDoubleClickLeft(int /*x*/, int /*y*/)
{
	OnSelectAll(this);
}

UnicodeString TextBox::GetSelection()
{
	if (!HasSelection()) { return L""; }

	int iStart = Utility::Min(m_iCursorPos, m_iCursorEnd);
	int iEnd = Utility::Max(m_iCursorPos, m_iCursorEnd);
	const UnicodeString & str = GetText().GetUnicode();
	return str.substr(iStart, iEnd - iStart);
}

bool TextBox::OnKeyReturn(bool bDown)
{
	if (bDown) { return true; }

	// Try to move the next control, as if tab had been pressed
	OnKeyTab(true);

	// If we still have focus, blur it.
	if (HasFocus())
	{
		Blur();
	}

	// This is called AFTER the blurring so you can
	// refocus in your onReturnPressed hook.
	OnEnter();
	return true;
}

bool TextBox::OnKeyBackspace(bool bDown)
{
	if (!bDown) { return true; }

	if (HasSelection())
	{
		EraseSelection();
		return true;
	}

	if (m_iCursorPos == 0) { return true; }

	DeleteText(m_iCursorPos - 1, 1);
	return true;
}

bool TextBox::OnKeyDelete(bool bDown)
{
	if (!bDown) { return true; }

	if (HasSelection())
	{
		EraseSelection();
		return true;
	}

	if (m_iCursorPos >= TextLength()) { return true; }

	DeleteText(m_iCursorPos, 1);
	return true;
}

bool TextBox::OnKeyLeft(bool bDown)
{
	if (!bDown) { return true; }

	if (m_iCursorPos > 0)
	{
		m_iCursorPos--;
	}

	if (!Magnus::Input::IsShiftDown())
	{
		m_iCursorEnd = m_iCursorPos;
	}

	RefreshCursorBounds();
	return true;
}

bool TextBox::OnKeyRight(bool bDown)
{
	if (!bDown) { return true; }

	if (m_iCursorPos < TextLength())
	{
		m_iCursorPos++;
	}

	if (!Magnus::Input::IsShiftDown())
	{
		m_iCursorEnd = m_iCursorPos;
	}

	RefreshCursorBounds();
	return true;
}

bool TextBox::OnKeyHome(bool bDown)
{
	if (!bDown) { return true; }

	m_iCursorPos = 0;

	if (!Magnus::Input::IsShiftDown())
	{
		m_iCursorEnd = m_iCursorPos;
	}

	RefreshCursorBounds();
	return true;
}

bool TextBox::OnKeyEnd(bool /*bDown*/)
{
	m_iCursorPos = TextLength();

	if (!Magnus::Input::IsShiftDown())
	{
		m_iCursorEnd = m_iCursorPos;
	}

	RefreshCursorBounds();
	return true;
}

void TextBox::SetCursorPos(int i)
{
	if (m_iCursorPos == i) { return; }

	m_iCursorPos = i;
	m_iCursorLine = 0;
	RefreshCursorBounds();
}

void TextBox::SetCursorEnd(int i)
{
	if (m_iCursorEnd == i) { return; }

	m_iCursorEnd = i;
	RefreshCursorBounds();
}

void TextBox::DeleteText(int iStartPos, int iLength)
{
	if (!m_bEditable) return;

	UnicodeString str = GetText().GetUnicode();
	str.erase(iStartPos, iLength);
	SetText(str);

	if (m_iCursorPos > iStartPos)
	{
		SetCursorPos(m_iCursorPos - iLength);
	}

	SetCursorEnd(m_iCursorPos);
}

bool TextBox::HasSelection()
{
	return m_iCursorPos != m_iCursorEnd;
 }

void TextBox::EraseSelection()
{
	int iStart = Utility::Min(m_iCursorPos, m_iCursorEnd);
	int iEnd = Utility::Max(m_iCursorPos, m_iCursorEnd);
	DeleteText(iStart, iEnd - iStart);
	// Move the cursor to the start of the selection,
	// since the end is probably outside of the string now.
	m_iCursorPos = iStart;
	m_iCursorEnd = iStart;
}

void TextBox::OnMouseClickLeft(int x, int y, bool bDown)
{
	if (m_bSelectAll)
	{
		OnSelectAll(this);
		m_bSelectAll = false;
		return;
	}

	int iChar = m_Text->GetClosestCharacter(m_Text->CanvasPosToLocal(Magnus::Point(x, y)));

	if (bDown)
	{
		SetCursorPos(iChar);

		if (!Magnus::Input::IsShiftDown())
		{
			SetCursorEnd(iChar);
		}

		Magnus::MouseFocus = this;
	}
	else
	{
		if (Magnus::MouseFocus == this)
		{
			SetCursorPos(iChar);
			Magnus::MouseFocus = NULL;
		}
	}
}

void TextBox::OnMouseMoved(int x, int y, int /*deltaX*/, int /*deltaY*/)
{
	if (Magnus::MouseFocus != this) { return; }

	int iChar = m_Text->GetClosestCharacter(m_Text->CanvasPosToLocal(Magnus::Point(x, y)));
	SetCursorPos(iChar);
}

void TextBox::MakeCaretVisible()
{
	if (m_Text->Width() < Width())
	{
		m_Text->Position(m_iAlign);
	}
	else
	{
		int iCaretPos = m_Text->GetCharacterPosition(m_iCursorPos).x;
		int iRealCaretPos = iCaretPos + m_Text->X();
		int iSlidingZone = m_Text->GetFont()->size + 1;

		// If the caret is already in a semi-good position, leave it.
		if (iRealCaretPos >= iSlidingZone && iRealCaretPos <= Width() - iSlidingZone)
		{
			return;
		}

		int x = 0;

		if (iRealCaretPos > Width() - iSlidingZone)
		{
			x = Width() - iCaretPos - iSlidingZone;
		}

		if (iRealCaretPos < iSlidingZone)
		{
			x = -iCaretPos + iSlidingZone;
		}

		// Don't show too much whitespace to the rigth
		if (x + m_Text->Width() < Width() - GetPadding().right)
		{
			x = -m_Text->Width() + (Width() - GetPadding().right);
		}

		// Or the left
		if (x > GetPadding().left)
		{
			x = GetPadding().left;
		}

		int y = 0;

		if (m_iAlign & Pos::Top) { y = GetPadding().top; }

		if (m_iAlign & Pos::Bottom) { y = Height() - m_Text->Height() - GetPadding().bottom; }

		if (m_iAlign & Pos::CenterV) { y = (Height() - m_Text->Height()) * 0.5; }

		m_Text->SetPos(x, y);
	}
}

void TextBox::Layout(Magnus::Skin* skin)
{
	BaseClass::Layout(skin);
	RefreshCursorBounds();
}

void TextBox::PostLayout(Magnus::Skin* /*skin*/)
{
}

void TextBox::OnTextChanged()
{
	if (m_iCursorPos > TextLength()) { m_iCursorPos = TextLength(); }

	if (m_iCursorEnd > TextLength()) { m_iCursorEnd = TextLength(); }

	onTextChanged.Call(this);
}

void TextBox::OnEnter()
{
	onReturnPressed.Call(this);
}

void TextBox::MoveCaretToEnd()
{
	m_iCursorPos = TextLength();
	m_iCursorEnd = TextLength();
	RefreshCursorBounds();
}

void TextBox::MoveCaretToStart()
{
	m_iCursorPos = 0;
	m_iCursorEnd = 0;
	RefreshCursorBounds();
}

MAGNUS_CONTROL_CONSTRUCTOR(TextBoxNumeric)
{
	SetText("0");
}

bool TextBoxNumeric::IsTextAllowed(const Magnus::UnicodeString & str, int iPos)
{
	const UnicodeString & strString = GetText().GetUnicode();

	if (str.length() == 0)
	{
		return true;
	}

	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == L'-')
		{
			// Has to be at the start
			if (i != 0 || iPos != 0)
			{
				return false;
			}

			// Can only be one
			if (std::count(strString.begin(), strString.end(), L'-') > 0)
			{
				return false;
			}

			continue;
		}

		if (str[i] == L'0') { continue; }

		if (str[i] == L'1') { continue; }

		if (str[i] == L'2') { continue; }

		if (str[i] == L'3') { continue; }

		if (str[i] == L'4') { continue; }

		if (str[i] == L'5') { continue; }

		if (str[i] == L'6') { continue; }

		if (str[i] == L'7') { continue; }

		if (str[i] == L'8') { continue; }

		if (str[i] == L'9') { continue; }

		if (str[i] == L'.')
		{
			// Already a fullstop
			if (std::count(strString.begin(), strString.end(), L'.') > 0)
			{
				return false;
			}

			continue;
		}

		return false;
	}

	return true;
}

float TextBoxNumeric::GetFloatFromText()
{
	double temp = Magnus::Utility::Strings::ToFloat(GetText().GetUnicode());
	return temp;
}