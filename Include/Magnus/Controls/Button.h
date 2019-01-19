#pragma once
#ifndef MAGNUS_CONTROLS_BUTTON_H
#define MAGNUS_CONTROLS_BUTTON_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Label.h"

namespace Magnus
{
	namespace Controls
	{
		class ImagePanel;

		class Button : public Label
		{
		public:

			MAGNUS_CONTROL(Button, Label);

			virtual void Render(Magnus::Skin* skin);
			virtual void OnMouseClickLeft(int x, int y, bool bDown);
			virtual void OnMouseClickRight(int x, int y, bool bDown);
			virtual void OnMouseDoubleClickLeft(int x, int y);
			virtual bool OnKeySpace(bool bDown);

			virtual void OnPress();
			virtual void OnRightPress();

			virtual void AcceleratePressed();

			virtual bool IsDepressed() const { return m_bDepressed; }
			virtual void SetDepressed(bool b);

			//
			// Button can be toggle type, which means that it is
			// toggled on and off. Its toggle statis is in IsDepressed.
			//
			virtual void SetIsToggle(bool b) { m_bToggle = b; }
			virtual bool IsToggle() const { return m_bToggle; }
			virtual bool GetToggleState() const { return m_bToggleStatus; }
			virtual void SetToggleState(bool b);
			virtual void Toggle() { SetToggleState(!GetToggleState()); }

			virtual void SetImage(const TextObject & strName, bool bCenter = false);

			virtual void SizeToContents();
			virtual void PostLayout(Magnus::Skin* pSkin);
			virtual void UpdateColours();

			virtual void SetImageAlpha(float fMultiply);

			virtual void DoAction() { OnPress(); }
			virtual void SetAction(Event::Handler* pObject, Handler::FunctionWithInformation pFunction, void* data);

		public:

			Magnus::Event::Caller onPress;
			Magnus::Event::Caller onRightPress;
			Magnus::Event::Caller onDown;
			Magnus::Event::Caller onUp;
			Magnus::Event::Caller onDoubleClick;
			Magnus::Event::Caller onToggle;
			Magnus::Event::Caller onToggleOn;
			Magnus::Event::Caller onToggleOff;

		protected:

			ImagePanel* m_Image;

			bool m_bDepressed;
			bool m_bToggle;
			bool m_bToggleStatus;

			bool m_bCenterImage;
		};

		class LabeledButton : public Control
		{
		public:
			MAGNUS_CONTROL_INLINE(LabeledButton, Control)
			{
				SetSize(200, 24);
				m_Button = new Button(this);
				m_Button->SetWidth(100);
				m_Button->Dock(Pos::Right);
				m_Button->SetMargin(Margin(2, 2, 0, 2));
				m_Button->SetTabable(false);
				m_Label = new Label(this);
				m_Label->SetAlignment(Magnus::Pos::Left | Magnus::Pos::CenterV);
				m_Label->Dock(Pos::Fill);
				m_Label->SetTextPadding(Padding(3, 0, 3, 0));
				m_Label->SetTabable(false);
				SetTabable(false);
			}

			virtual void SetText(const TextObject & text) { m_Label->SetText(text); }
			virtual Button* GetButton() { return m_Button; }
			virtual Label* GetLabel() { return m_Label; }

		private:

			Button*	m_Button;
			Label*	m_Label;
		};
	}
}

#endif