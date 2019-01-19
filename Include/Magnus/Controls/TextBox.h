#pragma once
#ifndef MAGNUS_CONTROLS_TEXTBOX_H
#define MAGNUS_CONTROLS_TEXTBOX_H

#include "Magnus/Renderer.h"
#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Label.h"

namespace Magnus
{
	namespace Controls
	{
		class TextBox : public Label
		{
			MAGNUS_CONTROL(TextBox, Label);

			virtual void Render(Magnus::Skin* skin);
			virtual void RenderFocus(Magnus::Skin* /*skin*/) {}
			virtual void Layout(Magnus::Skin* skin);
			virtual void PostLayout(Magnus::Skin* skin);

			virtual void Think();

			virtual bool OnChar(Magnus::UnicodeChar c);

			virtual void InsertText(const Magnus::UnicodeString & str);
			virtual void DeleteText(int iStartPos, int iLength);

			virtual void RefreshCursorBounds();

			virtual bool OnKeyReturn(bool bDown);
			virtual bool OnKeyBackspace(bool bDown);
			virtual bool OnKeyDelete(bool bDown);
			virtual bool OnKeyRight(bool bDown);
			virtual bool OnKeyLeft(bool bDown);
			virtual bool OnKeyHome(bool bDown);
			virtual bool OnKeyEnd(bool bDown);

			virtual bool AccelOnlyFocus() { return true; }

			virtual void OnPaste(Magnus::Controls::Control* pCtrl);
			virtual void OnCopy(Magnus::Controls::Control* pCtrl);
			virtual void OnCut(Magnus::Controls::Control* pCtrl);
			virtual void OnSelectAll(Magnus::Controls::Control* pCtrl);

			virtual void OnMouseDoubleClickLeft(int x, int y);

			virtual void EraseSelection();
			virtual bool HasSelection();
			virtual UnicodeString GetSelection();

			virtual void SetCursorPos(int i);
			virtual void SetCursorEnd(int i);

			virtual void OnMouseClickLeft(int x, int y, bool bDown);
			virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY);

			virtual void SetEditable(bool b) { m_bEditable = b; }

			virtual void SetSelectAllOnFocus(bool b) { m_bSelectAll = b; if (b) { OnSelectAll(this); } }

			virtual void MakeCaretVisible();

			virtual void OnEnter();

			virtual bool NeesInputChars() { return true; }

			virtual void MoveCaretToEnd();
			virtual void MoveCaretToStart();

			Event::Caller	onTextChanged;
			Event::Caller	onReturnPressed;

		protected:

			virtual void OnTextChanged();
			virtual bool IsTextAllowed(const Magnus::UnicodeString & /*str*/, int /*iPos*/) { return true; }

			bool m_bEditable;
			bool m_bSelectAll;

			int m_iCursorPos;
			int m_iCursorEnd;
			int m_iCursorLine;

			Magnus::Rect m_SelectionBounds;
			Magnus::Rect m_CaretBounds;

			float m_fNextCaretColorChange;
			Magnus::Color m_CaretColor;
		};

		class TextBoxNumeric : public TextBox
		{
		public:

			MAGNUS_CONTROL(TextBoxNumeric, TextBox);

			virtual float GetFloatFromText();

		private:

			virtual bool IsTextAllowed(const Magnus::UnicodeString & str, int iPos);
		};
	}
}

#endif