#include "Magnus/Magnus.h"
#include "Magnus/Controls/Text.h"
#include "Magnus/Skin.h"
#include "Magnus/Utility.h"

using namespace Magnus;
using namespace Magnus::ControlsInternal;

MAGNUS_CONTROL_CONSTRUCTOR(Text)
{
	m_Font = NULL;
	m_ColorOverride = Color(0, 255, 255, 255);
	m_Color = GetSkin()->Colors.Label.Default;
	SetMouseInputEnabled(false);
	SetWrap(false);
}

Text::~Text()
{
	// NOTE: This font doesn't need to be released
	// Because it's a pointer to another font somewhere.
}

void Text::Layout(Magnus::Skin* skin)
{
	if (m_bTextChanged)
	{
		RefreshSize();
		m_bTextChanged = false;
	}
}

Magnus::Font* Text::GetFont()
{
	return m_Font;
}

void Text::SetFont(Magnus::Font* pFont)
{
	if (m_Font == pFont) { return; }

	m_Font = pFont;
	m_bTextChanged = true;
	// Change the font of multilines too!
	{
		TextLines::iterator it = m_Lines.begin();
		TextLines::iterator itEnd = m_Lines.end();

		while (it != itEnd)
		{
			(*it)->SetFont(m_Font);
			++it;
		}
	}
	Invalidate();
}

void Text::SetString(const TextObject & str)
{
	if (m_String == str) { return; }

	m_String = str.GetUnicode();
	m_bTextChanged = true;
	Invalidate();
}

void Text::Render(Magnus::Skin* skin)
{
	if (m_bWrap) { return; }

	if (Length() == 0 || !GetFont()) { return; }

	if (m_ColorOverride.a == 0)
	{
		skin->GetRenderer()->SetDrawColor(m_Color);
	}
	else
	{
		skin->GetRenderer()->SetDrawColor(m_ColorOverride);
	}

	skin->GetRenderer()->RenderText(m_String.GetUnicode(), Magnus::Point(GetPadding().left, GetPadding().top), GetFont());
}

Magnus::Rect Text::GetCharacterPosition(int iChar)
{
	if (!m_Lines.empty())
	{
		TextLines::iterator it = m_Lines.begin();
		TextLines::iterator itEnd = m_Lines.end();
		int iChars = 0;

		Text* pLine;
		while (it != itEnd)
		{
			pLine = *it;
			++it;
			iChars += pLine->Length();

			if (iChars <= iChar) { continue; }

			iChars -= pLine->Length();
			Magnus::Rect rect = pLine->GetCharacterPosition(iChar - iChars);
			rect.x += pLine->X();
			rect.y += pLine->Y();
			return rect;
		}
		// Manage special case of the last character.
		Magnus::Rect rect = pLine->GetCharacterPosition(pLine->Length());
		rect.x += pLine->X();
		rect.y += pLine->Y();
		return rect;
	}

	if (Length() == 0 || iChar == 0)
	{
		Magnus::Size s = GetSkin()->GetRenderer()->MeasureText(L" ", GetFont());
		return Magnus::Rect(0, 0, 0, s.h);
	}

	UnicodeString sub = m_String.GetUnicode().substr(0, iChar);
	Magnus::Size s = GetSkin()->GetRenderer()->MeasureText(sub, GetFont());
	return Rect(s.w, 0, 0, s.h);
}

Magnus::Rect Text::GetLineBox(int i)
{
	Text* line = GetLine(i);
	if (line != NULL)
	{
		Magnus::Size s = GetSkin()->GetRenderer()->MeasureText(line->m_String.GetUnicode(), GetFont());
		return Magnus::Rect(line->X(), line->Y(), Utility::Clamp(s.w, 1, s.w), Utility::Clamp(s.h, 1, s.h));
	}
	else
	{
		Magnus::Size s = GetSkin()->GetRenderer()->MeasureText(m_String.GetUnicode(), GetFont());
		return Magnus::Rect(0, 0, Utility::Clamp(s.w, 1, s.w), Utility::Clamp(s.h, 1, s.h));
	}
}

int Text::GetClosestCharacter(Magnus::Point p)
{
	if (!m_Lines.empty())
	{
		TextLines::iterator it = m_Lines.begin();
		TextLines::iterator itEnd = m_Lines.end();
		int iChars = 0;

		Text* pLine;
		while (it != itEnd)
		{
			pLine = *it;
			++it;
			iChars += pLine->Length();

			if (p.y < pLine->Y()) { continue; }

			if (p.y > pLine->Bottom()) { continue; }

			if (p.y < pLine->Bottom()) { break; }
		}
		iChars -= pLine->Length();
		int iLinePos = pLine->GetClosestCharacter(Magnus::Point(p.x - pLine->X(), p.y - pLine->Y()));
		return iChars + iLinePos;
	}

	int iDistance = 4096;
	int iChar = 0;

	for (size_t i = 0; i < m_String.GetUnicode().length() + 1; i++)
	{
		Magnus::Rect cp = GetCharacterPosition(i);
		int iDist = abs(cp.x - p.x) + abs(cp.y - p.y); // this isn't proper

		if (iDist > iDistance) { continue; }

		iDistance = iDist;
		iChar = i;
	}

	return iChar;
}

void Text::OnScaleChanged()
{
	Invalidate();
}

void Text::RefreshSize()
{
	if (m_bWrap)
	{
		return RefreshSizeWrap();
	}

	if (!GetFont()) { return; }

	Magnus::Size s(1, GetFont()->size);

	if (Length() > 0)
	{
		s = GetSkin()->GetRenderer()->MeasureText(m_String.GetUnicode(), GetFont());
	}

	s.w += GetPadding().left + GetPadding().right;
	s.h += GetPadding().top + GetPadding().bottom;

	if (s.w == Width() && s.h == Height())
	{
		return;
	}

	if (s.h < GetFont()->size) { s.h = GetFont()->size; }

	SetSize(s.w, s.h);
	InvalidateParent();
	Invalidate();
}

void Text::SplitWords(const Magnus::UnicodeString & s, std::vector<Magnus::UnicodeString> & elems)
{
	Magnus::UnicodeString str;

	int w = GetParent()->Width() - GetParent()->GetPadding().left - GetParent()->GetPadding().right;
	for (int i = 0; i < (int)s.length(); i++)
	{
		if (s[i] == L'\n')
		{
			if (!str.empty()) { elems.push_back(str); }

			elems.push_back(L"\n");
			str.clear();
			continue;
		}

		if (s[i] == L' ')
		{
			str += s[i];
			elems.push_back(str);
			str.clear();
			continue;
		}

		str += s[i];

		// If adding character makes the word bigger than the textbox size
		Magnus::Size s = GetSkin()->GetRenderer()->MeasureText(str, GetFont());
		if (s.w > w)
		{
			int addSum = GetPadding().left + GetPadding().right;
			// split words
			str.resize(str.size() - 1);
			elems.push_back(str);
			str.clear();
			--i;
			continue;
		}
	}

	if (!str.empty()) { elems.push_back(str); }
}

void Text::RefreshSizeWrap()
{
	RemoveAllChildren();

	for (TextLines::iterator it = m_Lines.begin(); it != m_Lines.end(); ++it)
	{
		delete *it;
	}

	m_Lines.clear();
	std::vector<Magnus::UnicodeString> words;
	SplitWords(GetText().GetUnicode(), words);
	// Adding a bullshit word to the end simplifies the code below
	// which is anything but simple.
	words.push_back(L"");

	if (!GetFont()) { return; }

	Magnus::Size fontSize = GetSkin()->GetRenderer()->MeasureText(L" ", GetFont());
	int w = GetParent()->Width() - GetParent()->GetPadding().left - GetParent()->GetPadding().right;
	int x = 0, y = 0;
	Magnus::UnicodeString strLine;

	for (std::vector<Magnus::UnicodeString>::iterator it = words.begin(); it != words.end(); ++it)
	{
		bool bFinishLine = false;
		bool bWrapped = false;

		// If this word is a newline - make a newline (we still add it to the text)
		if ((*it).c_str()[0] == L'\n') { bFinishLine = true; }

		// Does adding this word drive us over the width?
		{
			strLine += (*it);
			Magnus::Size s = GetSkin()->GetRenderer()->MeasureText(strLine, GetFont());

			if (s.w > w) { bFinishLine = true; bWrapped = true; }
		}

		// If this is the last word then finish the line
		if (--words.end() == it)
		{
			bFinishLine = true;
		}

		if (bFinishLine)
		{
			Text* t = new Text(this);
			t->SetFont(GetFont());
			if (bWrapped)
			{
				t->SetString(strLine.substr(0, strLine.length() - (*it).length()));
				// newline should start with the word that was too big
				strLine = *it;
			}
			else
			{
				t->SetString(strLine.substr(0, strLine.length()));
				// newline is empty
				strLine.clear();
			}
			t->RefreshSize();
			t->SetPos(x, y);
			m_Lines.push_back(t);

			// newline should start with the word that was too big
			// strLine = *it;

			// Position the newline
			y += fontSize.h;
			x = 0;
			//if (strLine[0] == L' ') x -= fontSize.x;
		}
	}

	// Size to children height and parent width
	{
		Size childsize = ChildrenSize();
		SetSize(w, childsize.h);
	}
	InvalidateParent();
	Invalidate();
}

int Text::NumLines()
{
	return m_Lines.size();
}

Text* Text::GetLine(int i)
{
	TextLines::iterator it = m_Lines.begin();
	TextLines::iterator itEnd = m_Lines.end();

	while (it != itEnd)
	{
		if (i == 0) { return *it; }

		++it;
		i--;
	}

	return NULL;
}

int Text::GetLineFromChar(int i)
{
	TextLines::iterator it = m_Lines.begin();
	TextLines::iterator itEnd = m_Lines.end();
	int iChars = 0;
	int iLine = 0;

	while (it != itEnd)
	{
		Text* pLine = *it;
		++it;
		iChars += pLine->Length();

		if (iChars > i) { return iLine; }

		iLine++;
	}

	if (iLine > 0) return iLine - 1;
	return iLine;
}

int Text::GetStartCharFromLine(int i)
{
	TextLines::iterator it = m_Lines.begin();
	TextLines::iterator itEnd = m_Lines.end();
	int iChars = 0;

	while (it != itEnd)
	{
		Text* pLine = *it;
		++it;

		if (i == 0) { return Magnus::Utility::Clamp(iChars, 0, Length()); }

		iChars += pLine->Length();
		i--;
	}

	return Magnus::Utility::Clamp(iChars, 0, Length());
}

int Text::GetEndCharFromLine(int i)
{
	int iStart = GetStartCharFromLine(i);
	Text* iLine = GetLine(i);

	if (iLine)
	{
		iStart += iLine->Length();
	}

	return Magnus::Utility::Clamp(iStart, 0, Length());
}

int Text::GetCharPosOnLine(int i)
{
	int iLine = GetLineFromChar(i);
	Text* line = GetLine(iLine);

	if (!line) { return 0; }

	int iStart = GetStartCharFromLine(iLine);
	return i - iStart;
}