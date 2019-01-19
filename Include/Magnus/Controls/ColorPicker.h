#pragma once
#ifndef MAGNUS_CONTROLS_COLORPICKER_H
#define MAGNUS_CONTROLS_COLORPICKER_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Controls/Button.h"
#include "Magnus/Controls/Menu.h"
#include "Magnus/Controls/Label.h"
#include "Magnus/Controls/TextBox.h"
#include "Magnus/Controls/ColorControls.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace ControlsInternal
	{
		class ColorDisplay : public Magnus::Controls::Control
		{
		public:

			MAGNUS_CONTROL_INLINE(ColorDisplay, Magnus::Controls::Control)
			{
				SetSize(32, 32);
			}

			virtual void Render(Magnus::Skin* skin)
			{
				skin->DrawColorDisplay(GetRenderBounds(), m_Color);
			}

			virtual void SetColor(Magnus::Color color) { m_Color = color; }
			virtual Magnus::Color GetColor() { return m_Color; }

			virtual void SetRed(int red)		{ m_Color.r = red; }
			virtual void SetGreen(int green)	{ m_Color.g = green; }
			virtual void SetBlue(int blue)		{ m_Color.b = blue; }
			virtual void SetAlpha(int alpha)	{ m_Color.a = alpha; }

		protected:

			Magnus::Color m_Color;
		};
	}

	namespace Controls
	{
		class ColorPicker : public Controls::Control
		{
		public:

			MAGNUS_CONTROL(ColorPicker, Controls::Control);

			Magnus::Color GetColor() { return m_Color; }
			void SetColor(Magnus::Color color, bool onlyHue = false);

			void ColorBoxChanged(Controls::Control* pControl);
			void ColorSliderChanged(Controls::Control* pControl);
			void NumericTyped(Controls::Control* pControl);

			void UpdateControls(Magnus::Color newColor);

			Event::Caller onColorChanged;

		protected:

			Color m_Color;

			TextBoxNumeric* m_RedBox;
			TextBoxNumeric* m_GreenBox;
			TextBoxNumeric* m_BlueBox;
			TextBoxNumeric* m_AlphaBox;

			ColorLerpBox* m_LerpBox;
			ColorSlider* m_ColorSlider;
			ControlsInternal::ColorDisplay* m_Display;
		};

		class ColorButton : public Controls::Button
		{
		public:

			MAGNUS_CONTROL_INLINE(ColorButton, Controls::Button)
			{
				SetSize(20, 15);
				m_Color = Magnus::Colors::Red;
				onPress.Add(this, &ThisClass::OnButtonPress);
			}

			void Render(Magnus::Skin* skin)
			{
				skin->DrawColorDisplay(GetRenderBounds(), m_Color);
			}

			virtual Magnus::Color GetColor() { return m_Color; }

			virtual void SetColor(const Magnus::Color & color)
			{
				m_Color = color;
				onColorChanged.Call(this);
			}

			void OnButtonPress(Controls::Control* control)
			{
				Magnus::Controls::Menu* pMenu = new Magnus::Controls::Menu(GetCanvas());
				pMenu->SetSize(256, 180);
				pMenu->SetDeleteOnClose(true);
				pMenu->SetDisableIconMargin(true);
				Magnus::Controls::ColorPicker* picker = new Magnus::Controls::ColorPicker(pMenu);
				picker->Dock(Pos::Fill);
				picker->SetSize(256, 128);
				picker->SetColor(GetColor());
				picker->onColorChanged.Add(this, &ThisClass::ColorChanged);
				pMenu->SetOpacity(Opacity());
				pMenu->Open(Pos::Right | Pos::Top);
				pMenu->SetPos(pMenu->X() - pMenu->Width(), pMenu->Y());
			}

			void ColorChanged(Controls::Control* pControl)
			{
				Magnus::Controls::ColorPicker* picker = magnus_cast<Magnus::Controls::ColorPicker>(pControl);
				SetColor(picker->GetColor());
			}

			Event::Caller onColorChanged;

		protected:

			Magnus::Color m_Color;
		};

		class LabeledColorPicker: public Control
		{
		public:
			MAGNUS_CONTROL_INLINE(LabeledColorPicker, Control)
			{
				m_pVar = NULL;
				SetSize(150, 19);
				SetMargin(Margin(0, 2, 0, 2));
				m_ColorButton = new ColorButton(this);
				m_ColorButton->Dock(Pos::Right);
				m_ColorButton->SetMargin(Margin(2, 2, 0, 2));
				m_ColorButton->SetTabable(false);
				m_ColorButton->onColorChanged.Add(this, &ThisClass::OnColorChanged);
				m_Label = new Label(this);
				m_Label->SetAlignment(Magnus::Pos::Left | Magnus::Pos::CenterV);
				m_Label->SetTextPadding(Padding(3, 0, 3, 0));
				m_Label->Dock(Pos::Fill);
				m_Label->SetTabable(false);
				SetTabable(false);
			}

			virtual void SetVar(void* pVar)
			{
				m_pVar = pVar;

				if (!m_pVar) { return; }

				m_ColorButton->SetColor(Color::ToRGB(*(unsigned long*)m_pVar));
			}

			virtual void OnColorChanged()
			{
				if (!m_pVar) { return; }

				*(unsigned long*)m_pVar = Color::ToHex(m_ColorButton->GetColor());
			}

			virtual void PostLayout(Magnus::Skin* /*skin*/)
			{
				if (!m_pVar) { return; }

				if (*(int*)m_pVar != Color::ToHex(m_ColorButton->GetColor()))
					m_ColorButton->SetColor(Color::ToRGB(*(unsigned long*)m_pVar));
			}

			virtual void SetText(const TextObject & text) { m_Label->SetText(text); }
			virtual ColorButton* GetColorButton() { return m_ColorButton; }
			virtual Label* GetLabel() { return m_Label; }

		private:

			void* m_pVar;

			ColorButton* m_ColorButton;
			Label*	m_Label;
		};
	}
}

#endif