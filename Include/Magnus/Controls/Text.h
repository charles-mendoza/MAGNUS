#pragma once
#ifndef MAGNUS_CONTROLS_TEXT_H
#define MAGNUS_CONTROLS_TEXT_H

#include "Magnus/Renderer.h"
#include "Magnus/Controls/Control.h"

namespace Magnus
{
	namespace ControlsInternal
	{
		class Text : public Controls::Control
		{
		public:

			MAGNUS_CONTROL(Text, Controls::Control);

			virtual ~Text();
			Magnus::Font* GetFont();

			void SetString(const TextObject & str);

			void Render(Magnus::Skin* skin);
			void Layout(Magnus::Skin* skin);

			void RefreshSize();

			void SetFont(Magnus::Font* pFont);

			const TextObject & GetText() const { return m_String; }

			Magnus::Rect GetCharacterPosition(int iChar);
			int GetClosestCharacter(Magnus::Point p);

			/// return the encapsulating box of the given line
			// @remark this function is necessary to implement multiline selection
			virtual Magnus::Rect GetLineBox(int i);

			int Length() const { return (int)m_String.GetUnicode().size(); }

			virtual void SetTextColor(const Magnus::Color & col) { m_Color = col; }
			virtual void SetTextColorOverride(const Magnus::Color & col) { m_ColorOverride = col; }
			
			virtual void OnScaleChanged();

			inline const Magnus::Color & TextColor() const { return m_Color; }

			virtual void TextChanged() { m_bTextChanged = true; }
			virtual bool Wrap() { return m_bWrap; }
			virtual void SetWrap(bool b) { if (m_bWrap == b) { return; } m_bWrap = b; m_bTextChanged = true; Invalidate(); }

			virtual Text* GetLine(int i);
			virtual int GetLineFromChar(int i);
			virtual int GetStartCharFromLine(int i);
			virtual int GetEndCharFromLine(int i);
			virtual int GetCharPosOnLine(int i);
			virtual int NumLines();

		protected:

			virtual void Text::SplitWords(const Magnus::UnicodeString & s, std::vector<Magnus::UnicodeString> & elems);

		private:

			virtual void RefreshSizeWrap();

			Magnus::TextObject	m_String;
			Magnus::Font*		m_Font;
			Magnus::Color		m_Color;
			Magnus::Color		m_ColorOverride;

			bool				m_bWrap;
			bool				m_bTextChanged;

			typedef std::list<Text*> TextLines;
			TextLines			m_Lines;
		};
	}
}

#endif