#pragma once

#include "INIParser.h"

#include "Magnus/Controls.h"
#include "Magnus/Platform.h"
#include "Magnus/Controls/Text.h"

using namespace Magnus;
using namespace Magnus::Controls;
using namespace cppiniparser;

class ComboBoxKey : public LabeledComboBox
{
	MAGNUS_CONTROL_INLINE(ComboBoxKey, LabeledComboBox)
	{
		AddItem("");
		AddItem("Tab");
		AddItem("Backspace");
		AddItem("Return");
		AddItem("Shift");
		AddItem("Control");
		AddItem("Pause");
		AddItem("CapsLock");
		AddItem("Escape");
		AddItem("Space");
		AddItem("PageUp");
		AddItem("PageDown");
		AddItem("End");
		AddItem("Home");
		AddItem("Left");
		AddItem("Up");
		AddItem("Right");
		AddItem("Down");
		AddItem("Insert");
		AddItem("Delete");
		AddItem("0");
		AddItem("1");
		AddItem("2");
		AddItem("3");
		AddItem("4");
		AddItem("5");
		AddItem("6");
		AddItem("7");
		AddItem("8");
		AddItem("9");
		AddItem("A");
		AddItem("B");
		AddItem("C");
		AddItem("D");
		AddItem("E");
		AddItem("F");
		AddItem("G");
		AddItem("H");
		AddItem("I");
		AddItem("J");
		AddItem("K");
		AddItem("L");
		AddItem("M");
		AddItem("N");
		AddItem("O");
		AddItem("P");
		AddItem("Q");
		AddItem("R");
		AddItem("S");
		AddItem("T");
		AddItem("U");
		AddItem("V");
		AddItem("W");
		AddItem("X");
		AddItem("Y");
		AddItem("Z");
		AddItem("Numpad0");
		AddItem("Numpad1");
		AddItem("Numpad2");
		AddItem("Numpad3");
		AddItem("Numpad4");
		AddItem("Numpad5");
		AddItem("Numpad6");
		AddItem("Numpad7");
		AddItem("Numpad8");
		AddItem("Numpad9");
		AddItem("F1");
		AddItem("F2");
		AddItem("F3");
		AddItem("F4");
		AddItem("F5");
		AddItem("F6");
		AddItem("F7");
		AddItem("F8");
		AddItem("F9");
		AddItem("F10");
		AddItem("F11");
		AddItem("F12");
	}
};

class FrameRate : public Label
{
	MAGNUS_CONTROL_INLINE(FrameRate, Label)
	{
		SetBounds(10, 10, 400, 100);
		m_Text->SetPadding(Padding(0, 0, 100, 100));
		m_iFrames = 0;
		m_fLastSecond = Magnus::Platform::GetTimeInSeconds();
	}

	virtual void Render(Skin* skin)
	{
		m_iFrames++;

		if (m_fLastSecond < Magnus::Platform::GetTimeInSeconds())
		{
			SetText(Magnus::Utility::Format("FPS : %i", m_iFrames * 2));
			m_fLastSecond = Magnus::Platform::GetTimeInSeconds() + 0.5f;
			m_iFrames = 0;
		}

		BaseClass::Render(skin);
	}

private:

	int		m_iFrames;
	float	m_fLastSecond;
};

class UnitTest : public Window
{
public:

	MAGNUS_CONTROL_INLINE(UnitTest, Window)
	{
		SetSize(400, 300);
		SetTitle("Menu");
		RestrictToParent(true);
		onWindowClosed.Add(this, &ThisClass::OnWindowClosed);
		Initialize();
	}

private:

	struct Vars_t
	{
		struct Colors_t
		{
			unsigned long Background;
			unsigned long TextOverlay;

		} Colors;

		struct Settings_t
		{
			bool FrameRate;
			bool ShowMenu;
			bool SaveOnClose;
			int MenuKey;
			int MenuOpacity;
			int FontSize;
			int SettingsFile;

		} Settings;

	} Vars;

	void SaveConfig()
	{
		INIConfig config;

		config.AddSection("Colors");
		config.AddOption("Colors", "TextOverlay", Utility::Format("0x%X", Vars.Colors.TextOverlay));
		config.AddOption("Colors", "BackgroundColor", Utility::Format("0x%X", Vars.Colors.Background));

		config.AddSection("Settings");
		config.AddOption("Settings", "FrameRate", Utility::ToString(Vars.Settings.FrameRate));
		config.AddOption("Settings", "ShowMenuOnStartUp", Utility::ToString(Vars.Settings.ShowMenu));
		config.AddOption("Settings", "SaveOnClose", Utility::ToString(Vars.Settings.SaveOnClose));
		config.AddOption("Settings", "MenuKey", Utility::ToString(Vars.Settings.MenuKey));
		config.AddOption("Settings", "MenuOpacity", Utility::ToString(Vars.Settings.MenuOpacity));
		config.AddOption("Settings", "FontSize", Utility::ToString(Vars.Settings.FontSize));

		switch (Vars.Settings.SettingsFile)
		{
		case 1:
			INIParser::Write(config, "config_001.ini");
			break;
		case 2:
			INIParser::Write(config, "config_002.ini");
			break;
		default:
			INIParser::Write(config, "config_def.ini");
		}
	}

	void LoadConfig()
	{
		INIConfig config;
		INIParser::Read(config, "config_def.ini");

		Vars.Settings.SettingsFile = config.GetInteger("Settings", "SettingsFile", 0);

		switch (Vars.Settings.SettingsFile)
		{
		case 1:
			config.Clear();
			INIParser::Read(config, "config_001.ini");
			break;
		case 2:
			config.Clear();
			INIParser::Read(config, "config_002.ini");
			break;
		}

		Vars.Colors.TextOverlay = config.GetInteger("Colors", "TextOverlay", Color::ToHex(Colors::Yellow));
		Vars.Colors.Background = config.GetInteger("Colors", "Background", Color::ToHex(Color(255, 51, 51, 51)));

		Vars.Settings.FrameRate = config.GetBoolean("Settings", "FrameRate", true);
		Vars.Settings.ShowMenu = config.GetBoolean("Settings", "ShowMenuOnStartUp", true);
		Vars.Settings.SaveOnClose = config.GetBoolean("Settings", "SaveOnClose", true);
		Vars.Settings.MenuKey = config.GetInteger("Settings", "MenuKey", 13);
		Vars.Settings.MenuOpacity = config.GetInteger("Settings", "MenuOpacity", 100);
		Vars.Settings.FontSize = config.GetInteger("Settings", "FontSize", 11);
	}

private:

	Font*		m_Font;
	FrameRate*	m_FrameRate;

	void Initialize()
	{
		LoadConfig();
		OnBackgroundColorChanged();

		m_Font = new Font();
		m_Font->facename = L"Microsoft Sans Serif";
		m_Font->size = 11.0f;
		m_Font->bold = true;

		m_FrameRate = new FrameRate(GetCanvas());
		m_FrameRate->SetFont(m_Font);
		m_FrameRate->SetTextColor(Colors::Yellow);

		SetHidden(!Vars.Settings.ShowMenu);

		TabControl* pTab = new TabControl(this);
		pTab->SetTabStripPosition(Pos::Left);
		pTab->Dock(Pos::Fill);
		{
			TabButton* pButton = pTab->AddPage("Colors");
			Control* pPage = pButton->GetPage();
			{
				LabeledColorPicker* pColor = new LabeledColorPicker(pPage);
				pColor->SetText("Text Overlay");
				pColor->SetVar((void*)&Vars.Colors.TextOverlay);
				pColor->GetColorButton()->onColorChanged.Add(this, &ThisClass::OnTextOverlayColorChanged);
				pColor->Dock(Pos::Top);
			}
			{
				LabeledColorPicker* pColor = new LabeledColorPicker(pPage);
				pColor->SetText("Background");
				pColor->SetVar((void*)&Vars.Colors.Background);
				pColor->GetColorButton()->onColorChanged.Add(this, &ThisClass::OnBackgroundColorChanged);
				pColor->Dock(Pos::Top);
			}
		}
		{
			TabButton* pButton = pTab->AddPage("Settings");
			Control* pPage = pButton->GetPage();
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Frame Rate");
				pToggle->SetVar((void*)&Vars.Settings.FrameRate);
				pToggle->GetToggle()->onCheckChanged.Add(this, &ThisClass::UpdateFPSControl);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Show Menu on Startup");
				pToggle->SetVar((void*)&Vars.Settings.ShowMenu);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Save on Menu Close");
				pToggle->SetVar((void*)&Vars.Settings.SaveOnClose);
				pToggle->Dock(Pos::Top);
			}
			{
				ComboBoxKey* pCombo = new ComboBoxKey(pPage);
				pCombo->SetText("Menu Key");
				pCombo->GetComboBox()->onSelection.Add(this, &ThisClass::OnMenuKeyChanged);
				pCombo->SetVar((void*)&Vars.Settings.MenuKey);
				pCombo->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Menu Opacity");
				pNumeric->GetNumericUpDown()->SetMin(10);
				pNumeric->GetNumericUpDown()->SetMax(100);
				pNumeric->SetVar((void*)&Vars.Settings.MenuOpacity);
				pNumeric->GetNumericUpDown()->onChanged.Add(this, &ThisClass::OnMenuOpacityChanged);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Text Overlay Size");
				pNumeric->GetNumericUpDown()->SetMin(8);
				pNumeric->GetNumericUpDown()->SetMax(72);
				pNumeric->GetNumericUpDown()->SetValue(11);
				pNumeric->GetNumericUpDown()->onChanged.Add(this, &ThisClass::OnFontSizeChanged);
				pNumeric->SetVar((void*)&Vars.Settings.FontSize);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledButton* pButton = new LabeledButton(pPage);
				pButton->GetButton()->SetText("Save");
				pButton->GetButton()->onPress.Add(this, &ThisClass::SaveConfig);
				pButton->Dock(Pos::Bottom);
			}
			{
				LabeledComboBox* pCombo = new LabeledComboBox(pPage);
				pCombo->SetText("Settings File");
				pCombo->AddItem("Default");
				pCombo->AddItem("Custom 1");
				pCombo->AddItem("Custom 2");
				pCombo->SetVar((void*)&Vars.Settings.SettingsFile);
				pCombo->GetComboBox()->onSelection.Add(this, &ThisClass::OnConfigFileChanged);
				pCombo->Dock(Pos::Bottom);
			}
		}

		OnMenuOpacityChanged();
		m_FrameRate->BringToFront();
	}

	virtual void Show()
	{
		BaseClass::Show();
		m_FrameRate->BringToFront();
	}

	virtual void Touch()
	{
		BaseClass::Touch();
		m_FrameRate->BringToFront();
	}

private:

	void UpdateFPSControl()
	{
		m_FrameRate->SetHidden(!Vars.Settings.FrameRate);
	}

	void OnBackgroundColorChanged()
	{
		GetCanvas()->SetBackgroundColor(Color::ToRGB(Vars.Colors.Background));
	}

	void OnTextOverlayColorChanged()
	{
		m_FrameRate->SetTextColor(Color::ToRGB(Vars.Colors.TextOverlay));
	}

	void OnFontSizeChanged()
	{
		m_Font->size = Vars.Settings.FontSize;
		GetCanvas()->GetSkin()->ReleaseFont(m_Font);
	}

	void OnMenuOpacityChanged()
	{
		SetOpacity(Vars.Settings.MenuOpacity);
	}

	void OnMenuKeyChanged()
	{
		Magnus::Input::AddKeyBind(Vars.Settings.MenuKey, &ThisClass::Show, this);
	}

	void OnConfigFileChanged()
	{
		INIConfig config;
		config.AddSection("Settings");
		config.AddOption("Settings", "SettingsFile", Utility::ToString(Vars.Settings.SettingsFile));
		INIParser::Modify(config, "config_def.ini");
		LoadConfig();
	}

	void OnWindowClosed()
	{
		if (Vars.Settings.SaveOnClose)
		{
			SaveConfig();
		}
	}
};