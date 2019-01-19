#include "Magnus/Magnus.h"
#include "Magnus/Controls/Label.h"
#include "Magnus/Utility.h"

using namespace Magnus;
using namespace Magnus::Controls;

MAGNUS_CONTROL_CONSTRUCTOR(Label)
{
	m_CreatedFont = NULL;
	m_Text = new ControlsInternal::Text(this);
	m_Text->SetFont(GetSkin()->GetDefaultFont());
	SetMouseInputEnabled(false);
	SetBounds(0, 0, 100, 10);
	SetAlignment(Magnus::Pos::Left | Magnus::Pos::Top);
}

void Label::PreDelete(Magnus::Skin* skin)
{
	if (m_CreatedFont)
	{
		skin->ReleaseFont(m_CreatedFont);
		delete m_CreatedFont;
		m_CreatedFont = NULL;
		SetFont(NULL);
	}
}

void Label::PostLayout(Magnus::Skin* /*skin*/)
{
	m_Text->Position(m_iAlign);
}

void Label::SetAlignment(int iAlign)
{
	if (m_iAlign == iAlign) { return; }

	m_iAlign = iAlign;
	Invalidate();
}

int Label::GetAlignment()
{
	return m_iAlign;
}

void Label::SetText(const TextObject & str, bool bDoEvents)
{
	if (m_Text->GetText() == str.GetUnicode()) { return; }

	m_Text->SetString(str);
	Redraw();

	if (bDoEvents)
		OnTextChanged();
}

void Label::SizeToContents()
{
	m_Text->SetPos(m_Padding.left, m_Padding.top);
	m_Text->RefreshSize();
	SetSize(m_Text->Width() + m_Padding.left + m_Padding.right, m_Text->Height() + m_Padding.top + m_Padding.bottom);
}

Magnus::Rect Label::GetCharacterPosition(int iChar)
{
	Magnus::Rect p = m_Text->GetCharacterPosition(iChar);
	p.x += m_Text->X();
	p.y += m_Text->Y();
	return p;
}

void Label::OnBoundsChanged(Magnus::Rect oldChildBounds)
{
	BaseClass::OnBoundsChanged(oldChildBounds);

	if (m_Text->Wrap())
	{
		m_Text->RefreshSize();
		Invalidate();
	}
}

void Label::SetFont(Magnus::UnicodeString strFacename, int iSize, bool bBold)
{
	if (m_CreatedFont)
	{
		GetSkin()->ReleaseFont(m_CreatedFont);
		delete m_CreatedFont;
		m_CreatedFont = NULL;
		SetFont(NULL);
	}

	m_CreatedFont = new Magnus::Font();
	m_CreatedFont->bold = bBold;
	m_CreatedFont->facename = strFacename;
	m_CreatedFont->size = iSize;
	SetFont(m_CreatedFont);
	m_Text->RefreshSize();
}