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

class Radar : public Magnus::ControlsInternal::Dragger
{
public:

	MAGNUS_CONTROL_INLINE(Radar, Magnus::ControlsInternal::Dragger)
	{
		m_fov = 60.0f;
		SetTarget(this);
		RestrictToParent(true);
	}

	virtual void Radar::Render(Magnus::Skin* skin)
	{
		Rendering::Renderer* render = skin->GetRenderer();

		render->SetDrawColor(Color(255, 34, 34, 34));
		render->DrawFilledRect(GetRenderBounds());
		render->SetOpacity(100.0f);
		render->SetDrawColor(Colors::Black);

		Point center = Point(Width() * 0.5, Height() * 0.5);

		Point pos = LocalPosToCanvas();
		render->DrawLine(Vector2(pos.x + center.x, pos.y + center.y), Vector2(pos.x + center.x + fovLeft.x, pos.y + center.y + fovLeft.y), 1.0f);
		render->DrawLine(Vector2(pos.x + center.x, pos.y + center.y), Vector2(pos.x + center.x + fovRight.x, pos.y + center.y + fovRight.y), 1.0f);

		render->DrawFilledRect(Rect(0, center.y, Width(), 1));
		render->DrawFilledRect(Rect(center.x, 0, 1, Height()));

		render->DrawLinedRect(GetRenderBounds());

		render->SetDrawColor(Colors::White);
		render->DrawFilledRect(Rect(center.x - 3, center.y - 3, 6, 6));
		render->SetDrawColor(Colors::Black);
		render->DrawLinedRect(Rect(center.x - 3, center.y - 3, 6, 6));
	}

	void SetFov(float fov)
	{
		if (m_fov == fov) { return; }

		m_fov = fov;
		OnFovChanged();
	}

protected:

	virtual void Radar::OnBoundsChanged(Magnus::Rect oldBounds)
	{
		BaseClass::OnBoundsChanged(oldBounds);
		OnFovChanged();
	}

	void OnFovChanged()
	{
		float radius = Height() * sqrt(2);
		float yaw = 3.141f * 1.5f; // yaw * c - c is some constant to adjust the yaw
		float a = yaw + (m_fov * 3.141f / 180.0f) * 0.5f;
		float b = yaw - (m_fov * 3.141f / 180.0f) * 0.5f;
		fovLeft.x = cos(a) * radius;
		fovLeft.y = sin(a) * radius;
		fovRight.x = cos(b) * radius;
		fovRight.y = sin(b) * radius;
	}

	float m_fov;
	Magnus::Vector2 fovLeft, fovRight;
};

class UnitTest : public Window
{
public:

	MAGNUS_CONTROL_INLINE(UnitTest, Window)
	{
		SetSize(350, 450);
		SetTitle("Menu");
		RestrictToParent(true);
		onWindowClosed.Add(this, &ThisClass::OnWindowClosed);
		Initialize();
	}

private:

	struct Vars_t
	{
		struct Aimbot_t
		{
			bool Enabled;
			bool AutoAim;
			bool AutoFire;
			bool Friendly;
			bool Enemy;
			bool VisibilityCheck;
			bool HardLock;
			bool DrawLocked;
			int AimStyle;
			int AimBone;
			int AutoMelee;
			int FireDelay;
			int LimitAngle;
			int SmoothAim;
			int MaxDistance;

		} Aimbot;

		struct Triggerbot_t
		{
			bool Enabled;
			bool Friendly;
			bool Enemy;
			int FireDelay;
			int MaxDistance;

		} Triggerbot;

		struct Radar3D_t
		{
			bool Enabled;
			bool Line2D;
			bool Bone;
			bool Name;
			bool Distance;
			bool Health;
			bool Weapon;
			bool AimLaser;
			bool Friendly;
			bool Enemy;
			int BoundingBox;
			int LaserSize;
			int MaxDistance;

		} Radar3D;

		struct Radar2D_t
		{
			bool Enabled;
			bool Cross;
			bool Border;
			bool FOV;
			bool Friendly;
			bool Enemy;
			bool StickToWindow;
			int Scale;
			int Size;
			int DotSize;
			int Opacity;

		} Radar2D;

		struct Warning_t
		{
			bool Enabled;
			bool Beep;
			int AimAngle;
			int VisionAngle;
			int MaxDistance;

		} Warning;

		struct Misc_t
		{
			bool FrameRate;
			bool UnliAmmo;
			bool UnliAmmoRounds;
			bool LongRangeMelee;
			bool GatlingOverheat;
			bool GatlingWarmUp;
			bool Fly;
			bool WalkThroughWalls;
			bool AntiAFK;
			bool AutoReady;
			bool AutoStart;
			int ResetMatchKey;
			int RespawnKey;
			int Crosshair;
			int CrosshairSize;
			int AutoReadyDelay;
			int AutoStartDelay;

		} Misc;

		struct Colors_t
		{
			unsigned long EnemyInvisible;
			unsigned long EnemyVisible;
			unsigned long FriendlyInvisible;
			unsigned long FriendlyVisible;
			unsigned long Crosshair;
			unsigned long IsAimingAtYou;
			unsigned long CanSeeYou;
			unsigned long IsVisible;

		} Colors;

		struct Settings_t
		{
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

		config.AddSection("Aimbot");
		config.AddOption("Aimbot", "Enabled", Utility::ToString(Vars.Aimbot.Enabled));
		config.AddOption("Aimbot", "AutoAim", Utility::ToString(Vars.Aimbot.AutoAim));
		config.AddOption("Aimbot", "AutoFire", Utility::ToString(Vars.Aimbot.AutoFire));
		config.AddOption("Aimbot", "AimOnFriendly", Utility::ToString(Vars.Aimbot.Friendly));
		config.AddOption("Aimbot", "AimOnEnemy", Utility::ToString(Vars.Aimbot.Enemy));
		config.AddOption("Aimbot", "VisibilityCheck", Utility::ToString(Vars.Aimbot.VisibilityCheck));
		config.AddOption("Aimbot", "HardLock", Utility::ToString(Vars.Aimbot.HardLock));
		config.AddOption("Aimbot", "DrawLocked", Utility::ToString(Vars.Aimbot.DrawLocked));
		config.AddOption("Aimbot", "AimStyle", Utility::ToString(Vars.Aimbot.AimStyle));
		config.AddOption("Aimbot", "AimBone", Utility::ToString(Vars.Aimbot.AimBone));
		config.AddOption("Aimbot", "AutoMelee", Utility::ToString(Vars.Aimbot.AutoMelee));
		config.AddOption("Aimbot", "FireDelay", Utility::ToString(Vars.Aimbot.FireDelay));
		config.AddOption("Aimbot", "LimitAngle", Utility::ToString(Vars.Aimbot.LimitAngle));
		config.AddOption("Aimbot", "SmoothAim", Utility::ToString(Vars.Aimbot.SmoothAim));
		config.AddOption("Aimbot", "MaxDistance", Utility::ToString(Vars.Aimbot.MaxDistance));

		config.AddSection("Triggerbot");
		config.AddOption("Triggerbot", "Enabled", Utility::ToString(Vars.Triggerbot.Enabled));
		config.AddOption("Triggerbot", "TriggerOnFriendly", Utility::ToString(Vars.Triggerbot.Friendly));
		config.AddOption("Triggerbot", "TriggerOnEnemy", Utility::ToString(Vars.Triggerbot.Enemy));
		config.AddOption("Triggerbot", "FireDelay", Utility::ToString(Vars.Triggerbot.FireDelay));
		config.AddOption("Triggerbot", "MaxDistance", Utility::ToString(Vars.Triggerbot.MaxDistance));

		config.AddSection("Radar3D");
		config.AddOption("Radar3D", "Enabled", Utility::ToString(Vars.Radar3D.Enabled));
		config.AddOption("Radar3D", "Line2D", Utility::ToString(Vars.Radar3D.Line2D));
		config.AddOption("Radar3D", "PlayerBone", Utility::ToString(Vars.Radar3D.Bone));
		config.AddOption("Radar3D", "PlayerName", Utility::ToString(Vars.Radar3D.Name));
		config.AddOption("Radar3D", "PlayerDistance", Utility::ToString(Vars.Radar3D.Distance));
		config.AddOption("Radar3D", "PlayerHealth", Utility::ToString(Vars.Radar3D.Health));
		config.AddOption("Radar3D", "PlayerWeapon", Utility::ToString(Vars.Radar3D.Weapon));
		config.AddOption("Radar3D", "AimLaser", Utility::ToString(Vars.Radar3D.AimLaser));
		config.AddOption("Radar3D", "ShowFriendly", Utility::ToString(Vars.Radar3D.Friendly));
		config.AddOption("Radar3D", "ShowEnemy", Utility::ToString(Vars.Radar3D.Enemy));
		config.AddOption("Radar3D", "BoundingBox", Utility::ToString(Vars.Radar3D.BoundingBox));
		config.AddOption("Radar3D", "LaserSize", Utility::ToString(Vars.Radar3D.LaserSize));
		config.AddOption("Radar3D", "MaxDistance", Utility::ToString(Vars.Radar3D.MaxDistance));

		config.AddSection("Radar2D");
		config.AddOption("Radar2D", "Enabled", Utility::ToString(Vars.Radar2D.Enabled));
		config.AddOption("Radar2D", "DrawCross", Utility::ToString(Vars.Radar2D.Cross));
		config.AddOption("Radar2D", "DrawBorder", Utility::ToString(Vars.Radar2D.Border));
		config.AddOption("Radar2D", "DrawFOV", Utility::ToString(Vars.Radar2D.FOV));
		config.AddOption("Radar2D", "ShowFriendly", Utility::ToString(Vars.Radar2D.Friendly));
		config.AddOption("Radar2D", "ShowEnemy", Utility::ToString(Vars.Radar2D.Enemy));
		config.AddOption("Radar2D", "StickToWindow", Utility::ToString(Vars.Radar2D.StickToWindow));
		config.AddOption("Radar2D", "Scale", Utility::ToString(Vars.Radar2D.Scale));
		config.AddOption("Radar2D", "Size", Utility::ToString(Vars.Radar2D.Size));
		config.AddOption("Radar2D", "DotSize", Utility::ToString(Vars.Radar2D.DotSize));
		config.AddOption("Radar2D", "Opacity", Utility::ToString(Vars.Radar2D.Opacity));

		config.AddSection("Warning");
		config.AddOption("Warning", "Enabled", Utility::ToString(Vars.Warning.Enabled));
		config.AddOption("Warning", "Beep", Utility::ToString(Vars.Warning.Beep));
		config.AddOption("Warning", "AimAngle", Utility::ToString(Vars.Warning.AimAngle));
		config.AddOption("Warning", "VisionAngle", Utility::ToString(Vars.Warning.VisionAngle));
		config.AddOption("Warning", "MaxDistance", Utility::ToString(Vars.Warning.MaxDistance));

		config.AddSection("Misc");
		config.AddOption("Misc", "FrameRate", Utility::ToString(Vars.Misc.FrameRate));
		config.AddOption("Misc", "UnliAmmo", Utility::ToString(Vars.Misc.UnliAmmo));
		config.AddOption("Misc", "UnliAmmoRounds", Utility::ToString(Vars.Misc.UnliAmmoRounds));
		config.AddOption("Misc", "LongRangeMelee", Utility::ToString(Vars.Misc.LongRangeMelee));
		config.AddOption("Misc", "GatlingOverheat", Utility::ToString(Vars.Misc.GatlingOverheat));
		config.AddOption("Misc", "GatlingWarmUp", Utility::ToString(Vars.Misc.GatlingWarmUp));
		config.AddOption("Misc", "Fly", Utility::ToString(Vars.Misc.Fly));
		config.AddOption("Misc", "WalkThroughWalls", Utility::ToString(Vars.Misc.WalkThroughWalls));
		config.AddOption("Misc", "AntiAFK", Utility::ToString(Vars.Misc.AntiAFK));
		config.AddOption("Misc", "AutoReady", Utility::ToString(Vars.Misc.AutoReady));
		config.AddOption("Misc", "AutoStart", Utility::ToString(Vars.Misc.AutoStart));
		config.AddOption("Misc", "ResetMatchKey", Utility::ToString(Vars.Misc.ResetMatchKey));
		config.AddOption("Misc", "RespawnKey", Utility::ToString(Vars.Misc.RespawnKey));
		config.AddOption("Misc", "Crosshair", Utility::ToString(Vars.Misc.Crosshair));
		config.AddOption("Misc", "CrosshairSize", Utility::ToString(Vars.Misc.CrosshairSize));
		config.AddOption("Misc", "AutoReadyDelay", Utility::ToString(Vars.Misc.AutoReadyDelay));
		config.AddOption("Misc", "AutoStartDelay", Utility::ToString(Vars.Misc.AutoStartDelay));

		config.AddSection("Colors");
		config.AddOption("Colors", "EnemyInvisible", Utility::Format("0x%X", Vars.Colors.EnemyInvisible));
		config.AddOption("Colors", "EnemyVisible", Utility::Format("0x%X", Vars.Colors.EnemyVisible));
		config.AddOption("Colors", "FriendlyInvisible", Utility::Format("0x%X", Vars.Colors.FriendlyInvisible));
		config.AddOption("Colors", "FriendlyVisible", Utility::Format("0x%X", Vars.Colors.FriendlyVisible));
		config.AddOption("Colors", "Crosshair", Utility::Format("0x%X", Vars.Colors.Crosshair));
		config.AddOption("Colors", "IsAimingAtYou", Utility::Format("0x%X", Vars.Colors.IsAimingAtYou));
		config.AddOption("Colors", "CanSeeYou", Utility::Format("0x%X", Vars.Colors.CanSeeYou));
		config.AddOption("Colors", "IsVisible", Utility::Format("0x%X", Vars.Colors.IsVisible));

		config.AddSection("Settings");
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

		Vars.Aimbot.Enabled = config.GetBoolean("Aimbot", "Enabled", true);
		Vars.Aimbot.AutoAim = config.GetBoolean("Aimbot", "AutoAim", false);
		Vars.Aimbot.AutoFire = config.GetBoolean("Aimbot", "AutoFire", false);
		Vars.Aimbot.Friendly = config.GetBoolean("Aimbot", "AimOnFriendly", false);
		Vars.Aimbot.Enemy = config.GetBoolean("Aimbot", "AimOnEnemy", true);
		Vars.Aimbot.VisibilityCheck = config.GetBoolean("Aimbot", "VisibilityCheck", true);
		Vars.Aimbot.HardLock = config.GetBoolean("Aimbot", "HardLock", true);
		Vars.Aimbot.DrawLocked = config.GetBoolean("Aimbot", "DrawLocked", true);
		Vars.Aimbot.AimStyle = config.GetInteger("Aimbot", "AimStyle", 0);
		Vars.Aimbot.AimBone = config.GetInteger("Aimbot", "AimBone", 0);
		Vars.Aimbot.AutoMelee = config.GetInteger("Aimbot", "AutoMelee", 0);
		Vars.Aimbot.FireDelay = config.GetInteger("Aimbot", "FireDelay", 0);
		Vars.Aimbot.LimitAngle = config.GetInteger("Aimbot", "LimitAngle", 60);
		Vars.Aimbot.SmoothAim = config.GetInteger("Aimbot", "SmoothAim", 0);
		Vars.Aimbot.MaxDistance = config.GetInteger("Aimbot", "MaxDistance", 0);

		Vars.Triggerbot.Enabled = config.GetBoolean("Triggerbot", "Enabled", false);
		Vars.Triggerbot.Friendly = config.GetBoolean("Triggerbot", "TriggerbotOnFriendly", false);
		Vars.Triggerbot.Enemy = config.GetBoolean("Triggerbot", "TriggerbotOnEnemy", true);
		Vars.Triggerbot.FireDelay = config.GetInteger("Triggerbot", "FireDelay", 0);
		Vars.Triggerbot.MaxDistance = config.GetInteger("Triggerbot", "MaxDistance", 0);

		Vars.Radar3D.Enabled = config.GetBoolean("Radar3D", "Enabled", true);
		Vars.Radar3D.Bone = config.GetBoolean("Radar3D", "PlayerBone", true);
		Vars.Radar3D.Line2D = config.GetBoolean("Radar3D", "Line2D", true);
		Vars.Radar3D.Name = config.GetBoolean("Radar3D", "PlayerName", true);
		Vars.Radar3D.Distance = config.GetBoolean("Radar3D", "PlayerDistance", true);
		Vars.Radar3D.Health = config.GetBoolean("Radar3D", "PlayerHealth", true);
		Vars.Radar3D.Weapon = config.GetBoolean("Radar3D", "PlayerWeapon", true);
		Vars.Radar3D.AimLaser = config.GetBoolean("Radar3D", "AimLaser", true);
		Vars.Radar3D.Friendly = config.GetBoolean("Radar3D", "ShowFriendly", false);
		Vars.Radar3D.Enemy = config.GetBoolean("Radar3D", "ShowEnemy", true);
		Vars.Radar3D.BoundingBox = config.GetInteger("Radar3D", "BoundingBox", 0);
		Vars.Radar3D.LaserSize = config.GetInteger("Radar3D", "LaserSize", 300);
		Vars.Radar3D.MaxDistance = config.GetInteger("Radar3D", "MaxDistance", 0);

		Vars.Radar2D.Enabled = config.GetBoolean("Radar2D", "Enabled", false);
		Vars.Radar2D.Cross = config.GetBoolean("Radar2D", "DrawCross", true);
		Vars.Radar2D.Border = config.GetBoolean("Radar2D", "DrawBorder", true);
		Vars.Radar2D.FOV = config.GetBoolean("Radar2D", "DrawFOV", true);
		Vars.Radar2D.Friendly = config.GetBoolean("Radar2D", "ShowFriendly", false);
		Vars.Radar2D.Enemy = config.GetBoolean("Radar2D", "ShowEnemy", true);
		Vars.Radar2D.StickToWindow = config.GetBoolean("Radar2D", "StickToWindow", false);
		Vars.Radar2D.Scale = config.GetInteger("Radar2D", "Scale", 1);
		Vars.Radar2D.Size = config.GetInteger("Radar2D", "Size", 200);
		Vars.Radar2D.DotSize = config.GetInteger("Radar2D", "DotSize", 6);
		Vars.Radar2D.Opacity = config.GetInteger("Radar2D", "Opacity", 100);

		Vars.Warning.Enabled = config.GetBoolean("Warning", "Enabled", true);
		Vars.Warning.Beep = config.GetBoolean("Warning", "Beep", true);
		Vars.Warning.AimAngle = config.GetInteger("Warning", "AimAngle", 12);
		Vars.Warning.VisionAngle = config.GetInteger("Warning", "VisionAngle", 60);
		Vars.Warning.MaxDistance = config.GetInteger("Warning", "MaxDistance", 0);

		Vars.Misc.FrameRate = config.GetBoolean("Misc", "FrameRate", true);
		Vars.Misc.UnliAmmo = config.GetBoolean("Misc", "UnliAmmo", false);
		Vars.Misc.UnliAmmoRounds = config.GetBoolean("Misc", "UnliAmmoRounds", false);
		Vars.Misc.LongRangeMelee = config.GetBoolean("Misc", "LongRangeMelee", false);
		Vars.Misc.GatlingOverheat = config.GetBoolean("Misc", "GatlingOverheat", false);
		Vars.Misc.GatlingWarmUp = config.GetBoolean("Misc", "GatlingWarmUp", false);
		Vars.Misc.Fly = config.GetBoolean("Misc", "Fly", false);
		Vars.Misc.WalkThroughWalls = config.GetBoolean("Misc", "WalkThroughWalls", false);
		Vars.Misc.AntiAFK = config.GetBoolean("Misc", "AntiAFK", false);
		Vars.Misc.AutoReady = config.GetBoolean("Misc", "AutoReady", false);
		Vars.Misc.AutoStart = config.GetBoolean("Misc", "AutoStart", false);
		Vars.Misc.ResetMatchKey = config.GetInteger("Misc", "ResetMatchKey", 69);
		Vars.Misc.RespawnKey = config.GetInteger("Misc", "RespawnKey", 71);
		Vars.Misc.Crosshair = config.GetInteger("Misc", "Crosshair", 0);
		Vars.Misc.CrosshairSize = config.GetInteger("Misc", "CrosshairSize", 20);
		Vars.Misc.AutoReadyDelay = config.GetInteger("Misc", "AutoReadyDelay", 0);
		Vars.Misc.AutoStartDelay = config.GetInteger("Misc", "AutoStartDelay", 0);

		Vars.Colors.EnemyInvisible = config.GetInteger("Colors", "EnemyInvisible", Color::ToHex(Color(255, 255, 0, 0)));
		Vars.Colors.EnemyVisible = config.GetInteger("Colors", "EnemyVisible", Color::ToHex(Color(255, 255, 165, 0)));
		Vars.Colors.FriendlyInvisible = config.GetInteger("Colors", "FriendlyInvisible", Color::ToHex(Color(255, 0, 255, 0)));
		Vars.Colors.FriendlyVisible = config.GetInteger("Colors", "FriendlyVisible", Color::ToHex(Color(255, 0, 255, 255)));
		Vars.Colors.Crosshair = config.GetInteger("Colors", "Crosshair", Color::ToHex(Color(255, 255, 255, 0)));
		Vars.Colors.IsAimingAtYou = config.GetInteger("Colors", "IsAimingAtYou", Color::ToHex(Color(255, 255, 0, 0)));
		Vars.Colors.CanSeeYou = config.GetInteger("Colors", "CanSeeYou", Color::ToHex(Color(255, 255, 255, 0)));
		Vars.Colors.IsVisible = config.GetInteger("Colors", "IsVisible", Color::ToHex(Color(255, 0, 255, 0)));

		Vars.Settings.ShowMenu = config.GetBoolean("Settings", "ShowMenuOnStartUp", true);
		Vars.Settings.SaveOnClose = config.GetBoolean("Settings", "SaveOnClose", true);
		Vars.Settings.MenuKey = config.GetInteger("Settings", "MenuKey", 12);
		Vars.Settings.MenuOpacity = config.GetInteger("Settings", "MenuOpacity", 100);
		Vars.Settings.FontSize = config.GetInteger("Settings", "FontSize", 11);
	}

private:

	Font*		m_Font;
	FrameRate*	m_FrameRate;
	Radar*		m_Radar;

	void Initialize()
	{
		LoadConfig();

		m_Font = new Font();
		m_Font->facename = L"Microsoft Sans Serif";
		m_Font->size = 11.0f;
		m_Font->bold = true;

		m_FrameRate = new FrameRate(GetCanvas());
		m_FrameRate->SetFont(m_Font);
		m_FrameRate->SetTextColor(Colors::Yellow);

		m_Radar = new Radar(GetCanvas());

		SetHidden(!Vars.Settings.ShowMenu);

		TabControl* pTab = new TabControl(this);
		pTab->SetTabStripPosition(Pos::Left);
		pTab->Dock(Pos::Fill);
		{
			TabButton* pButton = pTab->AddPage("Aimbot");
			Control* pPage = pButton->GetPage();
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Aimbot");
				pToggle->SetVar((void*)&Vars.Aimbot.Enabled);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Auto Aim");
				pToggle->SetVar((void*)&Vars.Aimbot.AutoAim);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Auto Fire");
				pToggle->SetVar((void*)&Vars.Aimbot.AutoFire);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Aim on Friendly");
				pToggle->SetVar((void*)&Vars.Aimbot.Friendly);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Aim on Enemy");
				pToggle->SetVar((void*)&Vars.Aimbot.Enemy);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Visibility Check");
				pToggle->SetVar((void*)&Vars.Aimbot.VisibilityCheck);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Hard Lock");
				pToggle->SetVar((void*)&Vars.Aimbot.HardLock);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Draw Locked");
				pToggle->SetVar((void*)&Vars.Aimbot.DrawLocked);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledComboBox* pCombo = new LabeledComboBox(pPage);
				pCombo->SetText("Aim Style");
				pCombo->GetComboBox()->AddItem("Angle");
				pCombo->GetComboBox()->AddItem("Distance");
				pCombo->GetComboBox()->AddItem("Health");
				pCombo->GetComboBox()->AddItem("Random");
				pCombo->SetVar((void*)&Vars.Aimbot.AimStyle);
				pCombo->Dock(Pos::Top);
			}
			{
				LabeledComboBox* pCombo = new LabeledComboBox(pPage);
				pCombo->SetText("Aim Bone");
				pCombo->GetComboBox()->AddItem("Bone Scan");
				pCombo->GetComboBox()->AddItem("Head");
				pCombo->GetComboBox()->AddItem("Chest");
				pCombo->SetVar((void*)&Vars.Aimbot.AimBone);
				pCombo->Dock(Pos::Top);
			}
			{
				LabeledComboBox* pCombo = new LabeledComboBox(pPage);
				pCombo->SetText("Auto Melee");
				pCombo->GetComboBox()->AddItem("None");
				pCombo->GetComboBox()->AddItem("Aim on Fire");
				pCombo->GetComboBox()->AddItem("Auto Aim");
				pCombo->SetVar((void*)&Vars.Aimbot.AutoMelee);
				pCombo->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Fire Delay");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Aimbot.FireDelay);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Limit Angle");
				pNumeric->GetNumericUpDown()->SetMax(360);
				pNumeric->SetVar((void*)&Vars.Aimbot.LimitAngle);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Smooth Aim");
				pNumeric->SetVar((void*)&Vars.Aimbot.SmoothAim);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Max Distance");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Aimbot.MaxDistance);
				pNumeric->Dock(Pos::Top);
			}
		}
		{
			TabButton* pButton = pTab->AddPage("Triggerbot");
			Control* pPage = pButton->GetPage();
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Triggerbot");
				pToggle->SetVar((void*)&Vars.Triggerbot.Enabled);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Trigger on Friendly");
				pToggle->SetVar((void*)&Vars.Triggerbot.Friendly);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Trigger on Enemy");
				pToggle->SetVar((void*)&Vars.Triggerbot.Enemy);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Fire Delay");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Triggerbot.FireDelay);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Max Distance");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Triggerbot.MaxDistance);
				pNumeric->Dock(Pos::Top);
			}
		}
		{
			TabButton* pButton = pTab->AddPage("3D Radar");
			Control* pPage = pButton->GetPage();
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("3D Radar");
				//pToggle->GetToggle()->onCheckChanged.Add(this, &ThisClass::OnToggleRadar3DName);
				pToggle->SetVar((void*)&Vars.Radar3D.Enabled);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("2D Line");
				pToggle->SetVar((void*)&Vars.Radar3D.Line2D);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Player Bone ESP");
				pToggle->SetVar((void*)&Vars.Radar3D.Bone);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Player Name ESP");
				//pToggle->GetToggle()->onCheckChanged.Add(g_pHacks, &HackManager::ToggleESPName);
				pToggle->SetVar((void*)&Vars.Radar3D.Name);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Player Distance ESP");
				pToggle->SetVar((void*)&Vars.Radar3D.Distance);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Player Health ESP");
				pToggle->SetVar((void*)&Vars.Radar3D.Health);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Player Weapon ESP");
				pToggle->SetVar((void*)&Vars.Radar3D.Weapon);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Aim Laser");
				pToggle->SetVar((void*)&Vars.Radar3D.AimLaser);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Show Friendly");
				pToggle->SetVar((void*)&Vars.Radar3D.Friendly);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Show Enemy");
				pToggle->SetVar((void*)&Vars.Radar3D.Enemy);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledComboBox* pCombo = new LabeledComboBox(pPage);
				pCombo->SetText("Bounding Box");
				pCombo->GetComboBox()->AddItem("None");
				pCombo->GetComboBox()->AddItem("2D");
				pCombo->GetComboBox()->AddItem("3D");
				pCombo->SetVar((void*)&Vars.Radar3D.BoundingBox);
				pCombo->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Laser Size");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Radar3D.MaxDistance);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Max Distance");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Radar3D.MaxDistance);
				pNumeric->Dock(Pos::Top);
			}
		}
		{
			TabButton* pButton = pTab->AddPage("2D Radar");
			Control* pPage = pButton->GetPage();
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("2D Radar");
				pToggle->GetToggle()->onCheckChanged.Add(this, &ThisClass::UpdateRadar);
				pToggle->SetVar((void*)&Vars.Radar2D.Enabled);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Draw Cross");
				pToggle->SetVar((void*)&Vars.Radar2D.Cross);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Draw FOV");
				pToggle->SetVar((void*)&Vars.Radar2D.FOV);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Show Friendly");
				pToggle->SetVar((void*)&Vars.Radar2D.Friendly);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Show Enemy");
				pToggle->SetVar((void*)&Vars.Radar2D.Enemy);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Stick To Window");
				pToggle->SetVar((void*)&Vars.Radar2D.StickToWindow);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Radar Scale");
				pNumeric->GetNumericUpDown()->SetMin(1);
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Radar2D.Scale);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Radar Size");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->GetNumericUpDown()->onChanged.Add(this, &ThisClass::UpdateRadar);
				pNumeric->SetVar((void*)&Vars.Radar2D.Size);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Dot Size");
				pNumeric->SetVar((void*)&Vars.Radar2D.DotSize);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Radar Opacity");
				pNumeric->GetNumericUpDown()->SetMax(100);
				pNumeric->GetNumericUpDown()->onChanged.Add(this, &ThisClass::UpdateRadar);
				pNumeric->SetVar((void*)&Vars.Radar2D.Opacity);
				pNumeric->Dock(Pos::Top);
			}
		}
		{
			TabButton* pButton = pTab->AddPage("Warning");
			Control* pPage = pButton->GetPage();
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Warning System");
				pToggle->SetVar((void*)&Vars.Warning.Enabled);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Beep System");
				pToggle->SetVar((void*)&Vars.Warning.Beep);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Aiming At You Angle");
				pNumeric->GetNumericUpDown()->SetMax(360);
				pNumeric->SetVar((void*)&Vars.Warning.AimAngle);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Can See You Angle");
				pNumeric->GetNumericUpDown()->SetMax(360);
				pNumeric->SetVar((void*)&Vars.Warning.VisionAngle);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Max Distance");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Warning.MaxDistance);
				pNumeric->Dock(Pos::Top);
			}
		}
		{
			TabButton* pButton = pTab->AddPage("Misc");
			Control* pPage = pButton->GetPage();
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Show Frame Rate");
				pToggle->GetToggle()->onCheckChanged.Add(this, &ThisClass::UpdateFPSControl);
				pToggle->SetVar((void*)&Vars.Misc.FrameRate);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Unlimited Ammo");
				pToggle->SetVar((void*)&Vars.Misc.UnliAmmo);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Unlimited Ammo Rounds");
				pToggle->SetVar((void*)&Vars.Misc.UnliAmmoRounds);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Long Range Melee");
				pToggle->SetVar((void*)&Vars.Misc.LongRangeMelee);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Anti-Gatling Overheat");
				pToggle->SetVar((void*)&Vars.Misc.GatlingOverheat);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Anti-Gatling Warm-Up");
				pToggle->SetVar((void*)&Vars.Misc.GatlingWarmUp);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Fly");
				pToggle->SetVar((void*)&Vars.Misc.Fly);
				pToggle->Dock(Pos::Top);
			}

			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Walk Through Walls");
				pToggle->SetVar((void*)&Vars.Misc.WalkThroughWalls);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Anti-AFK");
				pToggle->SetVar((void*)&Vars.Misc.AntiAFK);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Auto Ready");
				pToggle->SetVar((void*)&Vars.Misc.AutoReady);
				pToggle->Dock(Pos::Top);
			}
			{
				LabeledToggleControl* pToggle = new LabeledToggleControl(pPage);
				pToggle->SetText("Auto Start");
				pToggle->SetVar((void*)&Vars.Misc.AutoStart);
				pToggle->Dock(Pos::Top);
			}
			{
				ComboBoxKey* pCombo = new ComboBoxKey(pPage);
				pCombo->SetText("Reset Match Key");
				pCombo->GetComboBox()->onSelection.Add(this, &ThisClass::OnResetMatchKeyChanged);
				pCombo->SetVar((void*)&Vars.Misc.ResetMatchKey);
				pCombo->Dock(Pos::Top);
			}
			{
				ComboBoxKey* pCombo = new ComboBoxKey(pPage);
				pCombo->SetText("Respawn Key");
				pCombo->GetComboBox()->onSelection.Add(this, &ThisClass::OnRespawnKeyChanged);
				pCombo->SetVar((void*)&Vars.Misc.RespawnKey);
				pCombo->Dock(Pos::Top);
			}
			{
				LabeledComboBox* pCombo = new LabeledComboBox(pPage);
				pCombo->SetText("Crosshair");
				pCombo->AddItem("None");
				pCombo->AddItem("Cross");
				pCombo->AddItem("Dot");
				pCombo->AddItem("T-Cross");
				pCombo->SetVar((void*)&Vars.Misc.Crosshair);
				pCombo->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Crosshair Size");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Misc.CrosshairSize);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Auto Ready Delay");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Misc.AutoReadyDelay);
				pNumeric->Dock(Pos::Top);
			}
			{
				LabeledNumricUpDown* pNumeric = new LabeledNumricUpDown(pPage);
				pNumeric->SetText("Auto Start Delay");
				pNumeric->GetNumericUpDown()->SetMax(INT_MAX);
				pNumeric->SetVar((void*)&Vars.Misc.AutoStartDelay);
				pNumeric->Dock(Pos::Top);
			}
		}
		{
			TabButton* pButton = pTab->AddPage("Colors");
			Control* pPage = pButton->GetPage();
			{
				LabeledColorPicker* pColor = new LabeledColorPicker(pPage);
				pColor->SetText("Enemy Invisible");
				pColor->SetVar((void*)&Vars.Colors.EnemyInvisible);
				pColor->Dock(Pos::Top);
			}
			{
				LabeledColorPicker* pColor = new LabeledColorPicker(pPage);
				pColor->SetText("Enemy Visible");
				pColor->SetVar((void*)&Vars.Colors.EnemyVisible);
				pColor->Dock(Pos::Top);
			}
			{
				LabeledColorPicker* pColor = new LabeledColorPicker(pPage);
				pColor->SetText("Friendly Invisible");
				pColor->SetVar((void*)&Vars.Colors.FriendlyInvisible);
				pColor->Dock(Pos::Top);
			}
			{
				LabeledColorPicker* pColor = new LabeledColorPicker(pPage);
				pColor->SetText("Friendly Visible");
				pColor->SetVar((void*)&Vars.Colors.FriendlyVisible);
				pColor->Dock(Pos::Top);
			}
			{
				LabeledColorPicker* pColor = new LabeledColorPicker(pPage);
				pColor->SetText("Crosshair");
				pColor->SetVar((void*)&Vars.Colors.Crosshair);
				pColor->Dock(Pos::Top);
			}
			{
				LabeledColorPicker* pColor = new LabeledColorPicker(pPage);
				pColor->SetText("Is Aiming At You");
				pColor->SetVar((void*)&Vars.Colors.IsAimingAtYou);
				pColor->Dock(Pos::Top);
			}
			{
				LabeledColorPicker* pColor = new LabeledColorPicker(pPage);
				pColor->SetText("Can See You");
				pColor->SetVar((void*)&Vars.Colors.CanSeeYou);
				pColor->Dock(Pos::Top);
			}
			{
				LabeledColorPicker* pColor = new LabeledColorPicker(pPage);
				pColor->SetText("Is Visible");
				pColor->SetVar((void*)&Vars.Colors.IsVisible);
				pColor->Dock(Pos::Top);
			}
		}
		{
			TabButton* pButton = pTab->AddPage("Settings");
			Control* pPage = pButton->GetPage();
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

	void UpdateRadar()
	{
		m_Radar->SetHidden(!Vars.Radar2D.Enabled);
		m_Radar->SetSize(Vars.Radar2D.Size, Vars.Radar2D.Size);
		m_Radar->SetOpacity(Vars.Radar2D.Opacity);
	}

	void UpdateFPSControl()
	{
		m_FrameRate->SetHidden(!Vars.Misc.FrameRate);
	}

	void OnFontSizeChanged()
	{
		//g_pHacks->m_Font.size = Vars.Settings.FontSize;
		//GetCanvas()->GetSkin()->ReleaseFont(&g_pHacks->m_Font);
		m_Font->size = Vars.Settings.FontSize;
		GetCanvas()->GetSkin()->ReleaseFont(m_Font);
	}

	void OnMenuOpacityChanged()
	{
		SetOpacity(Vars.Settings.MenuOpacity);
	}

	void OnResetMatchKeyChanged()
	{
		//Magnus::Input::AddKeyBind(Vars.Misc.ResetMatchKey + 1, &HackManager::ResetMatch, g_pHacks);
	}

	void OnRespawnKeyChanged()
	{
		//Magnus::Input::AddKeyBind(Vars.Misc.RespawnKey + 1, &HackManager::Respawn, g_pHacks);
	}

	void OnMenuKeyChanged()
	{
		Magnus::Input::AddKeyBind(Vars.Settings.MenuKey + 1, &ThisClass::Show, this);
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