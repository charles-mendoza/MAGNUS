#pragma once
#ifndef MAGNUS_CONTROLS_LABEL_H
#define MAGNUS_CONTROLS_LABEL_H

#include "Magnus/Renderer.h"
#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Text.h"

namespace Magnus
{
	namespace Controls
	{
		class Label : public Controls::Control
		{
		public:

			MAGNUS_CONTROL(Label, Controls::Control);

			virtual void PreDelete(Magnus::Skin* skin);

			virtual void SetText(const TextObject & str, bool bDoEvents = true);
			virtual const TextObject & GetText() const { return m_Text->GetText(); }

			virtual void Render(Magnus::Skin* /*skin*/) {}

			virtual void PostLayout(Magnus::Skin* skin);

			virtual void SizeToContents();

			virtual void SetAlignment(int iAlign);
			virtual int GetAlignment();

			virtual void SetFont(Magnus::UnicodeString strFacename, int iSize, bool bBold);
			virtual void SetFont(Magnus::Font* pFont) { m_Text->SetFont(pFont); }
			virtual Magnus::Font* GetFont() { return m_Text->GetFont(); }
			virtual void SetTextColor(const Magnus::Color & col) { m_Text->SetTextColor(col); }
			virtual void SetTextColorOverride(const Magnus::Color & col) { m_Text->SetTextColorOverride(col); }
			inline const Magnus::Color & TextColor() const { return m_Text->TextColor(); }

			virtual int TextWidth() { return m_Text->Width(); }
			virtual int TextRight() { return m_Text->Right(); }
			virtual int TextHeight() { return m_Text->Height(); }
			virtual int TextX() { return m_Text->X(); }
			virtual int TextY() { return m_Text->Y(); }
			virtual int TextLength() { return m_Text->Length(); }

			Magnus::Rect GetCharacterPosition(int iChar);

			virtual void SetTextPadding(const Padding & padding) { m_Text->SetPadding(padding); Invalidate(); InvalidateParent(); }
			virtual const Padding & GetTextPadding() { return m_Text->GetPadding(); }

			inline int Alignment() const { return m_iAlign; }

			virtual void MakeColorNormal() { SetTextColor(GetSkin()->Colors.Label.Default); }
			virtual void MakeColorBrigth() { SetTextColor(GetSkin()->Colors.Label.Bright); }
			virtual void MakeColorDark() { SetTextColor(GetSkin()->Colors.Label.Dark); }
			virtual void MakeColorHighlight() { SetTextColor(GetSkin()->Colors.Label.Highlight); }

			virtual TextObject GetValue() { return GetText(); }
			virtual void SetValue(const TextObject & strValue) { return SetText(strValue); }

			virtual bool Wrap() { return m_Text->Wrap(); }
			virtual void SetWrap(bool b) { m_Text->SetWrap(b); }

			virtual void OnBoundsChanged(Magnus::Rect oldChildBounds);

		protected:

			virtual void OnTextChanged() {}

			Magnus::Font* m_CreatedFont;
			ControlsInternal::Text* m_Text;
			int m_iAlign;
		};
	}
}

#endif