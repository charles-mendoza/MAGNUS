#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"

using namespace Magnus;

Skin::Skin(Magnus::Rendering::Renderer* pRenderer)
{
	m_pRenderer = pRenderer;
	SetDefaultFont(L"Microsoft Sans Serif", 11.0f, true);
}

Skin::~Skin()
{
	ReleaseFont(&m_DefaultFont);
	m_Texture.Release(m_pRenderer);
}

void Skin::ReleaseFont(Magnus::Font* fnt)
{
	if (!fnt) { return; }

	if (!m_pRenderer) { return; }

	m_pRenderer->FreeFont(fnt);
}

void Skin::Load(const Magnus::TextObject & TextureName)
{
	m_Texture.Load(TextureName, GetRenderer());
	Process();
}

void Skin::LoadInMemory(void* pSrcData, unsigned int size)
{
	m_Texture.LoadInMemory(pSrcData, size, GetRenderer());
	Process();
}

void Skin::Process()
{
	Colors.Window.TitleActive = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 0, 508, Color(255, 255, 0, 0));
	Colors.Window.TitleInactive = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 1, 508, Color(255, 255, 255, 0));
	Colors.Button.Normal = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 2, 508, Color(255, 255, 255, 0));
	Colors.Button.Hover = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 3, 508, Color(255, 255, 255, 0));
	Colors.Button.Down = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 2, 508, Color(255, 255, 255, 0));
	Colors.Button.Disabled = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 3, 508, Color(255, 255, 255, 0));
	Colors.Tab.Active.Normal = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 4, 508, Color(255, 255, 255, 0));
	Colors.Tab.Active.Hover = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 5, 508, Color(255, 255, 255, 0));
	Colors.Tab.Active.Down = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 4, 508, Color(255, 255, 255, 0));
	Colors.Tab.Active.Disabled = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 5, 508, Color(255, 255, 255, 0));
	Colors.Tab.Inactive.Normal = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 6, 508, Color(255, 255, 255, 0));
	Colors.Tab.Inactive.Hover = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 7, 508, Color(255, 255, 255, 0));
	Colors.Tab.Inactive.Down = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 6, 508, Color(255, 255, 255, 0));
	Colors.Tab.Inactive.Disabled = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 7, 508, Color(255, 255, 255, 0));
	Colors.Label.Default = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 8, 508, Color(255, 255, 255, 0));
	Colors.Label.Bright = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 9, 508, Color(255, 255, 255, 0));
	Colors.Label.Dark = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 8, 508, Color(255, 255, 255, 0));
	Colors.Label.Highlight = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 9, 508, Color(255, 255, 255, 0));
	Colors.Tree.Lines = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 10, 508, Color(255, 255, 255, 0));
	Colors.Tree.Normal = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 11, 508, Color(255, 255, 255, 0));
	Colors.Tree.Hover = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 10, 508, Color(255, 255, 255, 0));
	Colors.Tree.Selected = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 11, 508, Color(255, 255, 255, 0));
	Colors.Properties.Line_Normal = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 12, 508, Color(255, 255, 255, 0));
	Colors.Properties.Line_Selected = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 13, 508, Color(255, 255, 255, 0));
	Colors.Properties.Line_Hover = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 12, 508, Color(255, 255, 255, 0));
	Colors.Properties.Title = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 13, 508, Color(255, 255, 255, 0));
	Colors.Properties.Column_Normal = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 14, 508, Color(255, 255, 255, 0));
	Colors.Properties.Column_Selected = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 15, 508, Color(255, 255, 255, 0));
	Colors.Properties.Column_Hover = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 14, 508, Color(255, 255, 255, 0));
	Colors.Properties.Border = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 15, 508, Color(255, 255, 255, 0));
	Colors.Properties.Label_Normal = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 16, 508, Color(255, 255, 255, 0));
	Colors.Properties.Label_Normal = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 17, 508, Color(255, 255, 255, 0));
	Colors.Properties.Label_Normal = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 16, 508, Color(255, 255, 255, 0));
	Colors.ModalBackground = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 18, 508, Color(255, 255, 255, 0));
	Colors.TooltipText = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 19, 508, Color(255, 255, 255, 0));
	Colors.Category.Header = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 18, 508, Color(255, 255, 255, 0));
	Colors.Category.Header_Closed = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 19, 508, Color(255, 255, 255, 0));
	Colors.Category.Line.Text = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 20, 508, Color(255, 255, 255, 0));
	Colors.Category.Line.Text_Hover = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 21, 508, Color(255, 255, 255, 0));
	Colors.Category.Line.Text_Selected = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 20, 508, Color(255, 255, 255, 0));
	Colors.Category.Line.Button = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 21, 508, Color(255, 255, 255, 0));
	Colors.Category.Line.Button_Hover = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 22, 508, Color(255, 255, 255, 0));
	Colors.Category.Line.Button_Selected = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 23, 508, Color(255, 255, 255, 0));
	Colors.Category.LineAlt.Text = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 22, 508, Color(255, 255, 255, 0));
	Colors.Category.LineAlt.Text_Hover = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 23, 508, Color(255, 255, 255, 0));
	Colors.Category.LineAlt.Text_Selected = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 24, 508, Color(255, 255, 255, 0));
	Colors.Category.LineAlt.Button = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 25, 508, Color(255, 255, 255, 0));
	Colors.Category.LineAlt.Button_Hover = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 24, 508, Color(255, 255, 255, 0));
	Colors.Category.LineAlt.Button_Selected = GetRenderer()->PixelColor(&m_Texture, 4 + 8 * 25, 508, Color(255, 255, 255, 0));
	Textures.Shadow.Init(&m_Texture, 448, 0, 31, 31, Margin(8, 8, 8, 8));
	Textures.Tooltip.Init(&m_Texture, 128, 320, 127, 31, Margin(8, 8, 8, 8));
	Textures.StatusBar.Init(&m_Texture, 128, 288, 127, 31, Margin(8, 8, 8, 8));
	Textures.Selection.Init(&m_Texture, 384, 32, 31, 31, Margin(4, 4, 4, 4));
	Textures.Panel.Normal.Init(&m_Texture, 256, 0, 63, 63, Margin(16, 16, 16, 16));
	Textures.Panel.Bright.Init(&m_Texture, 320, 0, 63, 63, Margin(16, 16, 16, 16));
	Textures.Panel.Dark.Init(&m_Texture, 256, 64, 63, 63, Margin(16, 16, 16, 16));
	Textures.Panel.Highlight.Init(&m_Texture, 320, 0, 63, 63, Margin(16, 16, 16, 16));
	Textures.Window.Normal.Init(&m_Texture, 0, 0, 127, 127, Margin(8, 32, 8, 8));
	Textures.Window.Inactive.Init(&m_Texture, 128, 0, 127, 127, Margin(8, 82, 8, 8));
	Textures.CheckBox.Active.Checked.Init(&m_Texture, 448, 32, 15, 15);
	Textures.CheckBox.Active.Normal.Init(&m_Texture, 464, 32, 15, 15);
	Textures.CheckBox.Disabled.Checked.Init(&m_Texture, 448, 48, 15, 15);
	Textures.CheckBox.Disabled.Normal.Init(&m_Texture, 464, 48, 15, 15);
	Textures.RadioButton.Active.Checked.Init(&m_Texture, 448, 64, 15, 15);
	Textures.RadioButton.Active.Normal.Init(&m_Texture, 464, 64, 15, 15);
	Textures.RadioButton.Disabled.Checked.Init(&m_Texture, 448, 80, 15, 15);
	Textures.RadioButton.Disabled.Normal.Init(&m_Texture, 464, 80, 15, 15);
	Textures.ToggleControl.Track.Normal.Init(&m_Texture, 400, 464, 15, 15, Margin(2, 2, 2, 2));
	Textures.ToggleControl.Track.Checked.Init(&m_Texture, 416, 464, 15, 15, Margin(2, 2, 2, 2));
	Textures.ToggleControl.Track.Disabled.Init(&m_Texture, 432, 464, 15, 15, Margin(2, 2, 2, 2));
	Textures.ToggleControl.Bar.Normal.Init(&m_Texture, 448, 464, 15, 15, Margin(2, 2, 2, 2));
	Textures.ToggleControl.Bar.Hover.Init(&m_Texture, 464, 464, 15, 15, Margin(2, 2, 2, 2));
	Textures.ToggleControl.Bar.Down.Init(&m_Texture, 480, 464, 15, 15, Margin(2, 2, 2, 2));
	Textures.ToggleControl.Bar.Disabled.Init(&m_Texture, 496, 464, 15, 15, Margin(2, 2, 2, 2));
	Textures.TextBox.Normal.Init(&m_Texture, 0, 150, 127, 21, Margin(4, 4, 4, 4));
	Textures.TextBox.Focus.Init(&m_Texture, 0, 172, 127, 21, Margin(4, 4, 4, 4));
	Textures.TextBox.Disabled.Init(&m_Texture, 0, 193, 127, 21, Margin(4, 4, 4, 4));
	Textures.Menu.Strip.Init(&m_Texture, 0, 128, 127, 21, Margin(1, 1, 1, 1));
	Textures.Menu.BackgroundWithMargin.Init(&m_Texture, 128, 128, 127, 63, Margin(24, 8, 8, 8));
	Textures.Menu.Background.Init(&m_Texture, 128, 192, 127, 63, Margin(8, 8, 8, 8));
	Textures.Menu.Hover.Init(&m_Texture, 128, 256, 127, 31, Margin(8, 8, 8, 8));
	Textures.Menu.RightArrow.Init(&m_Texture, 464, 112, 15, 15);
	Textures.Menu.Check.Init(&m_Texture, 448, 112, 15, 15);
	Textures.Tab.Control.Init(&m_Texture, 0, 256, 127, 127, Margin(8, 8, 8, 8));
	Textures.Tab.Left.Active.Init(&m_Texture, 64, 384, 31, 63, Margin(8, 8, 8, 8));
	Textures.Tab.Left.Inactive.Init(&m_Texture, 192, 384, 31, 63, Margin(8, 8, 8, 8));
	Textures.Tab.Top.Active.Init(&m_Texture, 0, 384, 63, 31, Margin(8, 8, 8, 8));
	Textures.Tab.Top.Inactive.Init(&m_Texture, 128, 384, 63, 31, Margin(8, 8, 8, 8));
	Textures.Tab.Right.Active.Init(&m_Texture, 96, 384, 31, 63, Margin(8, 8, 8, 8));
	Textures.Tab.Right.Inactive.Init(&m_Texture, 224, 384, 31, 63, Margin(8, 8, 8, 8));
	Textures.Tab.Bottom.Active.Init(&m_Texture, 0, 416, 63, 31, Margin(8, 8, 8, 8));
	Textures.Tab.Bottom.Inactive.Init(&m_Texture, 128, 416, 63, 31, Margin(8, 8, 8, 8));
	Textures.Tab.HeaderBar.Init(&m_Texture, 128, 352, 127, 31, Margin(4, 4, 4, 4));
	Textures.Window.Close.Init(&m_Texture, 41, 456, 13, 8);
	Textures.Window.Close_Hover.Init(&m_Texture, 73, 456, 13, 8);
	Textures.Window.Close_Down.Init(&m_Texture, 105, 456, 13, 8);
	Textures.Window.Maxi.Init(&m_Texture, 224, 448, 31, 31);
	Textures.Window.Maxi_Hover.Init(&m_Texture, 256, 448, 31, 31);
	Textures.Window.Maxi_Down.Init(&m_Texture, 288, 448, 31, 31);
	Textures.Window.Restore.Init(&m_Texture, 224, 480, 31, 31);
	Textures.Window.Restore_Hover.Init(&m_Texture, 256, 480, 31, 31);
	Textures.Window.Restore_Down.Init(&m_Texture, 288, 480, 31, 31);
	Textures.Window.Mini.Init(&m_Texture, 128, 448, 31, 31);
	Textures.Window.Mini_Hover.Init(&m_Texture, 160, 448, 31, 31);
	Textures.Window.Mini_Down.Init(&m_Texture, 192, 448, 31, 31);
	Textures.Tree.Background.Init(&m_Texture, 256, 128, 127, 127, Margin(16, 16, 16, 16));
	Textures.Tree.Plus.Init(&m_Texture, 448, 96, 15, 15);
	Textures.Tree.Minus.Init(&m_Texture, 464, 96, 15, 15);
	Textures.Input.Button.Normal.Init(&m_Texture, 480, 0, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.Button.Hovered.Init(&m_Texture, 480, 32, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.Button.Disabled.Init(&m_Texture, 480, 64, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.Button.Pressed.Init(&m_Texture, 480, 96, 31, 31, Margin(8, 8, 8, 8));

	for (int i = 0; i < 4; i++)
	{
		Textures.Scroller.Button.Normal[i].Init(&m_Texture, 464, 208 + i * 16, 15, 15, Margin(2, 2, 2, 2));
		Textures.Scroller.Button.Hover[i].Init(&m_Texture, 480, 208 + i * 16, 15, 15, Margin(2, 2, 2, 2));
		Textures.Scroller.Button.Down[i].Init(&m_Texture, 464, 272 + i * 16, 15, 15, Margin(2, 2, 2, 2));
		Textures.Scroller.Button.Disabled[i].Init(&m_Texture, 528, 272 + i * 16, 15, 15, Margin(2, 2, 2, 2));
	}

	Textures.Scroller.TrackV.Init(&m_Texture, 384, 208, 15, 127, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonV_Normal.Init(&m_Texture, 400, 208, 15, 127, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonV_Hover.Init(&m_Texture, 416, 208, 15, 127, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonV_Down.Init(&m_Texture, 432, 208, 15, 127, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonV_Disabled.Init(&m_Texture, 448, 208, 15, 127, Margin(4, 4, 4, 4));
	Textures.Scroller.TrackH.Init(&m_Texture, 384, 128, 127, 15, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonH_Normal.Init(&m_Texture, 384, 144, 127, 15, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonH_Hover.Init(&m_Texture, 384, 160, 127, 15, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonH_Down.Init(&m_Texture, 384, 176, 127, 15, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonH_Disabled.Init(&m_Texture, 384, 192, 127, 15, Margin(4, 4, 4, 4));
	Textures.Input.ListBox.Background.Init(&m_Texture, 256, 256, 63, 127, Margin(8, 8, 8, 8));
	Textures.Input.ListBox.Hovered.Init(&m_Texture, 320, 320, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.ListBox.EvenLine.Init(&m_Texture, 352, 256, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.ListBox.OddLine.Init(&m_Texture, 352, 288, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.ListBox.EvenLineSelected.Init(&m_Texture, 320, 256, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.ListBox.OddLineSelected.Init(&m_Texture, 320, 288, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.ComboBox.Normal.Init(&m_Texture, 384, 336, 127, 31, Margin(8, 8, 32, 8));
	Textures.Input.ComboBox.Hover.Init(&m_Texture, 384, 368, 127, 31, Margin(8, 8, 32, 8));
	Textures.Input.ComboBox.Down.Init(&m_Texture, 384, 400, 127, 31, Margin(8, 8, 32, 8));
	Textures.Input.ComboBox.Disabled.Init(&m_Texture, 384, 432, 127, 31, Margin(8, 8, 32, 8));
	Textures.Input.ComboBox.Button.Normal.Init(&m_Texture, 496, 272, 15, 15);
	Textures.Input.ComboBox.Button.Hover.Init(&m_Texture, 496, 288, 15, 15);
	Textures.Input.ComboBox.Button.Down.Init(&m_Texture, 496, 304, 15, 15);
	Textures.Input.ComboBox.Button.Disabled.Init(&m_Texture, 496, 320, 15, 15);
	Textures.Input.UpDown.Up.Normal.Init(&m_Texture, 384, 112, 7, 7);
	Textures.Input.UpDown.Up.Hover.Init(&m_Texture, 392, 112, 7, 7);
	Textures.Input.UpDown.Up.Down.Init(&m_Texture, 400, 112, 7, 7);
	Textures.Input.UpDown.Up.Disabled.Init(&m_Texture, 408, 112, 7, 7);
	Textures.Input.UpDown.Down.Normal.Init(&m_Texture, 384, 120, 7, 7);
	Textures.Input.UpDown.Down.Hover.Init(&m_Texture, 392, 120, 7, 7);
	Textures.Input.UpDown.Down.Down.Init(&m_Texture, 400, 120, 7, 7);
	Textures.Input.UpDown.Down.Disabled.Init(&m_Texture, 408, 120, 7, 7);
	Textures.ProgressBar.Back.Init(&m_Texture, 384, 0, 31, 31, Margin(8, 8, 8, 8));
	Textures.ProgressBar.Front.Init(&m_Texture, 416, 0, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.Slider.H.Normal.Init(&m_Texture, 416, 32, 15, 15);
	Textures.Input.Slider.H.Hover.Init(&m_Texture, 416, 48, 15, 15);
	Textures.Input.Slider.H.Down.Init(&m_Texture, 416, 64, 15, 15);
	Textures.Input.Slider.H.Disabled.Init(&m_Texture, 416, 80, 15, 15);
	Textures.Input.Slider.V.Normal.Init(&m_Texture, 432, 32, 15, 15);
	Textures.Input.Slider.V.Hover.Init(&m_Texture, 432, 48, 15, 15);
	Textures.Input.Slider.V.Down.Init(&m_Texture, 432, 64, 15, 15);
	Textures.Input.Slider.V.Disabled.Init(&m_Texture, 432, 80, 15, 15);
	Textures.CategoryList.Outer.Init(&m_Texture, 256, 384, 63, 63, Margin(8, 8, 8, 8));
	Textures.CategoryList.Inner.Init(&m_Texture, 320, 384, 63, 63, Margin(8, 8, 8, 8));
	Textures.CategoryList.Header.Init(&m_Texture, 320, 352, 63, 63, Margin(8, 8, 8, 8));
	Textures.GroupBox.Init(&m_Texture, 0, 448, 31, 31, Margin(8, 8, 8, 8));
}

void Skin::DrawButton(Magnus::Rect rect, bool bDepressed, bool bHovered, bool bDisabled)
{
	if (bDisabled) { return Textures.Input.Button.Disabled.Draw(rect, GetRenderer()); }

	if (bDepressed) { return Textures.Input.Button.Pressed.Draw(rect, GetRenderer()); }

	if (bHovered) { return Textures.Input.Button.Hovered.Draw(rect, GetRenderer()); }

	Textures.Input.Button.Normal.Draw(rect, GetRenderer());
}

void Skin::DrawMenuItem(Magnus::Rect rect, bool bSubmenuOpen, bool bHovered, bool bChecked)
{
	if (bSubmenuOpen || bHovered) { Textures.Menu.Hover.Draw(rect, GetRenderer()); }

	if (bChecked) { Textures.Menu.Check.Draw(Rect(rect.x + 4, rect.y + 3, 15, 15), GetRenderer()); }
}

void Skin::DrawMenuStrip(Magnus::Rect rect)
{
	Textures.Menu.Strip.Draw(rect, GetRenderer());
}

void Skin::DrawMenu(Magnus::Rect rect, bool bPaddingDisabled)
{
	if (!bPaddingDisabled)
	{
		return Textures.Menu.BackgroundWithMargin.Draw(rect, GetRenderer());
	}

	Textures.Menu.Background.Draw(rect, GetRenderer());
}

void Skin::DrawMenuRightArrow(Magnus::Rect rect)
{
	Textures.Menu.RightArrow.Draw(rect, GetRenderer());
}

void Skin::DrawMenuDivider(Magnus::Rect rect)
{
	GetRenderer()->SetDrawColor(Magnus::Color(100, 0, 0, 0));
	GetRenderer()->DrawFilledRect(rect);
}

void Skin::DrawShadow(Magnus::Rect rect)
{
	Textures.Shadow.Draw(rect + Magnus::Rect(-4, -4, 10, 10), GetRenderer());
}

void Skin::DrawRadioButton(Magnus::Rect rect, bool bChecked, bool bPressed, bool bDisabled)
{
	if (bChecked)
	{
		if (bDisabled)
			Textures.RadioButton.Disabled.Checked.Draw(rect, GetRenderer());
		else
			Textures.RadioButton.Active.Checked.Draw(rect, GetRenderer());
	}
	else
	{
		if (bDisabled)
			Textures.RadioButton.Disabled.Normal.Draw(rect, GetRenderer());
		else
			Textures.RadioButton.Active.Normal.Draw(rect, GetRenderer());
	}
}

void Skin::DrawCheckBox(Magnus::Rect rect, bool bChecked, bool bPressed, bool bDisabled)
{
	if (bChecked)
	{
		if (bDisabled)
			Textures.CheckBox.Disabled.Checked.Draw(rect, GetRenderer());
		else
			Textures.CheckBox.Active.Checked.Draw(rect, GetRenderer());
	}
	else
	{
		if (bDisabled)
			Textures.CheckBox.Disabled.Normal.Draw(rect, GetRenderer());
		else
			Textures.CheckBox.Active.Normal.Draw(rect, GetRenderer());
	}
}

void Skin::DrawToggleTrack(Magnus::Rect rect, bool bChecked, bool bDisabled)
{
	if (bChecked) { return Textures.ToggleControl.Track.Checked.Draw(rect, GetRenderer()); }

	if (bDisabled) { return Textures.ToggleControl.Track.Disabled.Draw(rect, GetRenderer()); }

	Textures.ToggleControl.Track.Normal.Draw(rect, GetRenderer());
}

void Skin::DrawToggleBar(Magnus::Rect rect, bool bPressed, bool bHovered, bool bDisabled)
{
	if (bPressed) { return Textures.ToggleControl.Bar.Down.Draw(rect, GetRenderer()); }

	if (bHovered) { return Textures.ToggleControl.Bar.Hover.Draw(rect, GetRenderer()); }

	if (bDisabled) { return Textures.ToggleControl.Bar.Disabled.Draw(rect, GetRenderer()); }

	Textures.ToggleControl.Bar.Normal.Draw(rect, GetRenderer());
}

void Skin::DrawGroupBox(Magnus::Rect rect, int textStart, int textHeight, int textWidth)
{
	Magnus::Rect r = rect;
	r.y += textHeight * 0.5;
	r.h -= textHeight * 0.5;
	Textures.GroupBox.Draw(r, GetRenderer(), Magnus::Colors::White, true, false);
	r.x += textStart + textWidth - 4;
	r.w -= textStart + textWidth - 4;
	Textures.GroupBox.Draw(r, GetRenderer(), Magnus::Colors::White, false, true, false, false, false, false, false, false, false);
}

void Skin::DrawTextBox(Magnus::Rect rect, bool bFocused, bool bDisabled)
{
	if (bDisabled) { return Textures.TextBox.Disabled.Draw(rect, GetRenderer()); }

	if (bFocused)
		Textures.TextBox.Focus.Draw(rect, GetRenderer());
	else
		Textures.TextBox.Normal.Draw(rect, GetRenderer());
}

void Skin::DrawTabButton(Magnus::Rect rect, bool bActive, int align)
{
	if (bActive)
	{
		if (align == Pos::Bottom) { return Textures.Tab.Bottom.Active.Draw(rect + Magnus::Rect(0, -8, 0, 8), GetRenderer()); }

		if (align == Pos::Top) { return Textures.Tab.Top.Active.Draw(rect + Magnus::Rect(0, 0, 0, 8), GetRenderer()); }

		if (align == Pos::Left) { return Textures.Tab.Left.Active.Draw(rect + Magnus::Rect(0, 0, 8, 0), GetRenderer()); }

		if (align == Pos::Right) { return Textures.Tab.Right.Active.Draw(rect + Magnus::Rect(-8, 0, 8, 0), GetRenderer()); }
	}

	if (align == Pos::Bottom) { return Textures.Tab.Bottom.Inactive.Draw(rect, GetRenderer()); }

	if (align == Pos::Top) { return Textures.Tab.Top.Inactive.Draw(rect, GetRenderer()); }

	if (align == Pos::Left) { return Textures.Tab.Left.Inactive.Draw(rect, GetRenderer()); }

	if (align == Pos::Right) { return Textures.Tab.Right.Inactive.Draw(rect, GetRenderer()); }
}

void Skin::DrawTabControl(Magnus::Rect rect)
{
	Textures.Tab.Control.Draw(rect, GetRenderer());
}

void Skin::DrawTabTitleBar(Magnus::Rect rect)
{
	Textures.Tab.HeaderBar.Draw(rect, GetRenderer());
}

void Skin::DrawGenericPanel(Magnus::Rect rect)
{
	Textures.Panel.Normal.Draw(rect, GetRenderer());
}

void Skin::DrawWindow(Magnus::Rect rect, bool bFocused)
{
	if (bFocused)
		Textures.Window.Normal.Draw(rect, GetRenderer());
	else
		Textures.Window.Inactive.Draw(rect, GetRenderer());
}

void Skin::DrawWindowCloseButton(Magnus::Rect rect, bool bPressed, bool bHovered, bool bDisabled)
{
	if (bDisabled) { return Textures.Window.Close.Draw(rect, GetRenderer(), Color(255, 200, 200, 200)); }

	if (bPressed) { return Textures.Window.Close_Down.Draw(rect, GetRenderer()); }

	if (bHovered) { return Textures.Window.Close_Hover.Draw(rect, GetRenderer()); }

	Textures.Window.Close.Draw(rect, GetRenderer());
}

void Skin::DrawWindowMaximizeButton(Magnus::Rect rect, bool bMaximized, bool bPressed, bool bHovered, bool bDisabled)
{
	Magnus::Rect r = Magnus::Rect(rect.x, rect.y, 31, 31);

	if (!bMaximized)
	{
		if (bDisabled) { return Textures.Window.Maxi.Draw(r, GetRenderer(), Color(255, 200, 200, 200)); }

		if (bPressed) { return Textures.Window.Maxi_Down.Draw(r, GetRenderer()); }

		if (bHovered) { return Textures.Window.Maxi_Hover.Draw(r, GetRenderer()); }

		return Textures.Window.Maxi.Draw(r, GetRenderer());
	}

	if (bDisabled) { return Textures.Window.Restore.Draw(r, GetRenderer(), Color(255, 200, 200, 200)); }

	if (bPressed) { return Textures.Window.Restore_Down.Draw(r, GetRenderer()); }

	if (bHovered) { return Textures.Window.Restore_Hover.Draw(r, GetRenderer()); }

	return Textures.Window.Restore.Draw(r, GetRenderer());
}

void Skin::DrawWindowMinimizeButton(Magnus::Rect rect, bool bPressed, bool bHovered, bool bDisabled)
{
	Magnus::Rect r = Magnus::Rect(rect.x, rect.y, 31, 31);

	if (bDisabled) { return Textures.Window.Mini.Draw(r, GetRenderer(), Color(255, 200, 200, 200)); }

	if (bPressed) { return Textures.Window.Mini_Down.Draw(r, GetRenderer()); }

	if (bHovered) { return Textures.Window.Mini_Hover.Draw(r, GetRenderer()); }

	Textures.Window.Mini.Draw(r, GetRenderer());
}

void Skin::DrawHighlight(Magnus::Rect rect)
{
	GetRenderer()->SetDrawColor(Colors.Label.Highlight);
	GetRenderer()->DrawFilledRect(rect);
}

void Skin::DrawScrollTrack(Magnus::Rect rect, bool bHorizontal, bool bPressed)
{
	if (bHorizontal)
		Textures.Scroller.TrackH.Draw(rect, GetRenderer());
	else
		Textures.Scroller.TrackV.Draw(rect, GetRenderer());
}

void Skin::DrawScrollThumb(Magnus::Rect rect, bool bHorizontal, bool bPressed, bool bHovered, bool bDisabled)
{
	if (bHorizontal)
	{
		if (bDisabled) { return Textures.Scroller.ButtonH_Disabled.Draw(rect, GetRenderer()); }

		if (bPressed) { return Textures.Scroller.ButtonH_Down.Draw(rect, GetRenderer()); }

		if (bHovered) { return Textures.Scroller.ButtonH_Hover.Draw(rect, GetRenderer()); }

		return Textures.Scroller.ButtonH_Normal.Draw(rect, GetRenderer());
	}

	if (bDisabled) { return Textures.Scroller.ButtonV_Disabled.Draw(rect, GetRenderer()); }

	if (bPressed) { return Textures.Scroller.ButtonV_Down.Draw(rect, GetRenderer()); }

	if (bHovered) { return Textures.Scroller.ButtonV_Hover.Draw(rect, GetRenderer()); }

	return Textures.Scroller.ButtonV_Normal.Draw(rect, GetRenderer());
}

void Skin::DrawScrollButton(Magnus::Rect rect, int iDirection, bool bPressed, bool bHovered, bool bDisabled)
{
	int i = 0;

	if (iDirection == Pos::Top) { i = 1; }

	if (iDirection == Pos::Right) { i = 2; }

	if (iDirection == Pos::Bottom) { i = 3; }

	if (bDisabled) { return Textures.Scroller.Button.Disabled[i].Draw(rect, GetRenderer()); }

	if (bPressed) { return Textures.Scroller.Button.Down[i].Draw(rect, GetRenderer()); }

	if (bHovered) { return Textures.Scroller.Button.Hover[i].Draw(rect, GetRenderer()); }

	return Textures.Scroller.Button.Normal[i].Draw(rect, GetRenderer());
}

void Skin::DrawProgressBar(Magnus::Rect rect, bool bHorizontal, float progress)
{
	if (bHorizontal)
	{
		Textures.ProgressBar.Back.Draw(rect, GetRenderer());
		rect.w *= progress;

		if (rect.w > 0) { Textures.ProgressBar.Front.Draw(rect, GetRenderer()); }
	}
	else
	{
		Textures.ProgressBar.Back.Draw(rect, GetRenderer());
		int inv_progress = rect.h * (1 - progress);
		rect.y += inv_progress;
		rect.h -= inv_progress;
		Textures.ProgressBar.Front.Draw(rect, GetRenderer());
	}
}

void Skin::DrawListBox(Magnus::Rect rect)
{
	Textures.Input.ListBox.Background.Draw(rect, GetRenderer());
}

void Skin::DrawListBoxLine(Magnus::Rect rect, bool bSelected, bool bHovered, bool bEven)
{
	if (bSelected)
	{
		if (bEven)
			return Textures.Input.ListBox.EvenLineSelected.Draw(rect, GetRenderer());
		else
			return Textures.Input.ListBox.OddLineSelected.Draw(rect, GetRenderer());
	}
	else
	{
		if (bHovered) { return Textures.Input.ListBox.Hovered.Draw(rect, GetRenderer()); }

		if (bEven)
			return Textures.Input.ListBox.EvenLineSelected.Draw(rect, GetRenderer());
		else
			return Textures.Input.ListBox.OddLineSelected.Draw(rect, GetRenderer());
	}
}

void Skin::DrawSliderNotchesH(Magnus::Rect rect, int numNotches, int dist)
{
	if (numNotches == 0) { return; }

	float iSpacing = (float)rect.w / (float)numNotches;

	for (int i = 0; i < numNotches + 1; i++)
	{
		GetRenderer()->DrawFilledRect(Magnus::Rect(rect.x + iSpacing * i, rect.y + dist - 2, 1, 5));
	}
}

void Skin::DrawSliderNotchesV(Magnus::Rect rect, int numNotches, int dist)
{
	if (numNotches == 0) { return; }

	float iSpacing = (float)rect.h / (float)numNotches;

	for (int i = 0; i < numNotches + 1; i++)
	{
		GetRenderer()->DrawFilledRect(Magnus::Rect(rect.x + dist - 1, rect.y + iSpacing * i, 5, 1));
	}
}

void Skin::DrawSlider(Magnus::Rect rect, bool bHorizontal, int numNotches, int barSize)
{
	if (bHorizontal)
	{
		Magnus::Rect r = rect;
		r.x += barSize * 0.5;
		r.y += rect.h * 0.5 - 1;
		r.w -= barSize;
		r.h = 1;
		GetRenderer()->SetDrawColor(Magnus::Color(100, 0, 0, 0));
		DrawSliderNotchesH(r, numNotches, barSize * 0.5);
		return GetRenderer()->DrawFilledRect(r);
	}

	Magnus::Rect r = rect;
	r.x += rect.w * 0.5 - 1;
	r.y += barSize * 0.5;
	r.w = 1;
	r.h -= barSize;
	GetRenderer()->SetDrawColor(Magnus::Color(100, 0, 0, 0));
	DrawSliderNotchesV(r, numNotches, barSize * 0.4);
	return GetRenderer()->DrawFilledRect(r);
}

void Skin::DrawSlideButton(Magnus::Rect rect, bool bHorizontal, bool bPressed, bool bHovered, bool bDisabled)
{
	if (!bHorizontal)
	{
		if (bDisabled) { return Textures.Input.Slider.V.Disabled.DrawCenter(rect, GetRenderer()); }

		if (bPressed) { return Textures.Input.Slider.V.Down.DrawCenter(rect, GetRenderer()); }

		if (bHovered) { return Textures.Input.Slider.V.Hover.DrawCenter(rect, GetRenderer()); }

		return Textures.Input.Slider.V.Normal.DrawCenter(rect, GetRenderer());
	}

	if (bDisabled) { return Textures.Input.Slider.H.Disabled.DrawCenter(rect, GetRenderer()); }

	if (bPressed) { return Textures.Input.Slider.H.Down.DrawCenter(rect, GetRenderer()); }

	if (bHovered) { return Textures.Input.Slider.H.Hover.DrawCenter(rect, GetRenderer()); }

	Textures.Input.Slider.H.Normal.DrawCenter(rect, GetRenderer());
}

void Skin::DrawComboBox(Magnus::Rect rect, bool bDown, bool bMenuOpen, bool bHovered, bool bDisabled)
{
	if (bDisabled) { return Textures.Input.ComboBox.Disabled.Draw(rect, GetRenderer()); }

	if (bDown || bMenuOpen) { return Textures.Input.ComboBox.Down.Draw(rect, GetRenderer()); }

	if (bHovered) { return Textures.Input.ComboBox.Hover.Draw(rect, GetRenderer()); }

	Textures.Input.ComboBox.Normal.Draw(rect, GetRenderer());
}

void Skin::DrawKeyboardHighlight(Magnus::Rect rect, int iOffset)
{
	Magnus::Rect r = rect;
	r.x += iOffset;
	r.y += iOffset;
	r.w -= iOffset * 2;
	r.h -= iOffset * 2;
	// draw the top and bottom
	bool skip = true;

	for (int i = 0; i < r.w * 0.5; i++)
	{
		GetRenderer()->SetDrawColor(Magnus::Color(255, 0, 0, 0));

		if (!skip)
		{
			GetRenderer()->DrawPixel(r.x + (i * 2), r.y);
			GetRenderer()->DrawPixel(r.x + (i * 2), r.y + r.h - 1);
		}
		else
		{
			skip = !skip;
		}
	}

	skip = false;

	for (int i = 0; i < r.h * 0.5; i++)
	{
		GetRenderer()->SetDrawColor(Magnus::Color(255, 0, 0, 0));

		if (!skip)
		{
			GetRenderer()->DrawPixel(r.x, r.y + i * 2);
			GetRenderer()->DrawPixel(r.x + r.w - 1, r.y + i * 2);
		}
		else
		{
			skip = !skip;
		}
	}
}

void Skin::DrawToolTip(Magnus::Rect rect)
{
	Textures.Tooltip.Draw(rect, GetRenderer());
}

void Skin::DrawComboDownArrow(Magnus::Rect rect, bool bDown, bool bMenuOpen, bool bHovered, bool bDisabled)
{
	if (bDisabled) { return Textures.Input.ComboBox.Button.Disabled.Draw(rect, GetRenderer()); }

	if (bDown || bMenuOpen) { return Textures.Input.ComboBox.Button.Down.Draw(rect, GetRenderer()); }

	if (bHovered) { return Textures.Input.ComboBox.Button.Hover.Draw(rect, GetRenderer()); }

	Textures.Input.ComboBox.Button.Normal.Draw(rect, GetRenderer());
}

void Skin::DrawNumericUpDownButton(Magnus::Rect rect, bool bUp, bool bPressed, bool bHovered, bool bDisabled)
{
	if (bUp)
	{
		if (bDisabled) { return Textures.Input.UpDown.Up.Disabled.DrawCenter(rect, GetRenderer()); }

		if (bPressed) { return Textures.Input.UpDown.Up.Down.DrawCenter(rect, GetRenderer()); }

		if (bHovered) { return Textures.Input.UpDown.Up.Hover.DrawCenter(rect, GetRenderer()); }

		return Textures.Input.UpDown.Up.Normal.DrawCenter(rect, GetRenderer());
	}

	if (bDisabled) { return Textures.Input.UpDown.Down.Disabled.DrawCenter(rect, GetRenderer()); }

	if (bPressed) { return Textures.Input.UpDown.Down.Down.DrawCenter(rect, GetRenderer()); }

	if (bHovered) { return Textures.Input.UpDown.Down.Hover.DrawCenter(rect, GetRenderer()); }

	return Textures.Input.UpDown.Down.Normal.DrawCenter(rect, GetRenderer());
}

void Skin::DrawStatusBar(Magnus::Rect rect)
{
	Textures.StatusBar.Draw(rect, GetRenderer());
}

void Skin::DrawTreeButton(Magnus::Rect rect, bool bOpen)
{
	if (bOpen)
		Textures.Tree.Minus.Draw(rect, GetRenderer());
	else
		Textures.Tree.Plus.Draw(rect, GetRenderer());
}

void Skin::DrawColorDisplay(Magnus::Rect rect, Color color)
{
	if (color.a != 255)
	{
		GetRenderer()->SetDrawColor(Magnus::Color(255, 255, 255, 255));
		GetRenderer()->DrawFilledRect(rect);
		GetRenderer()->SetDrawColor(Color(128, 128, 128, 128));
		GetRenderer()->DrawFilledRect(Magnus::Rect(0, 0, rect.w * 0.5, rect.h * 0.5));
		GetRenderer()->DrawFilledRect(Magnus::Rect(rect.w * 0.5, rect.h * 0.5, rect.w * 0.5, rect.h * 0.5));
	}

	GetRenderer()->SetDrawColor(color);
	GetRenderer()->DrawFilledRect(rect);
	GetRenderer()->SetDrawColor(Magnus::Color(255, 0, 0, 0));
	GetRenderer()->DrawLinedRect(rect);
}

void Skin::DrawModalControl(Magnus::Rect rect)
{
	GetRenderer()->SetDrawColor(Colors.ModalBackground);
	GetRenderer()->DrawFilledRect(rect);
}

void Skin::DrawTreeControl(Magnus::Rect rect)
{
	Textures.Tree.Background.Draw(rect, GetRenderer());
}

void Skin::DrawTreeNode(bool bOpen, bool bSelected, int iLabelHeight, int iLabelWidth, int iHalfWay, int iLastBranch, bool bIsRoot)
{
	if (bSelected) { Textures.Selection.Draw(Magnus::Rect(17, 0, iLabelWidth + 2, iLabelHeight - 1), GetRenderer()); }

	DrawTreeNode(bOpen, bSelected, iLabelHeight, iLabelWidth, iHalfWay, iLastBranch, bIsRoot);
}

void Skin::DrawCategoryHolder(Magnus::Rect rect)
{
	Textures.CategoryList.Outer.Draw(rect, GetRenderer());
}

void Skin::DrawCategoryInner(Magnus::Rect rect, bool bCollapsed)
{
	if (bCollapsed) { return Textures.CategoryList.Header.Draw(rect, GetRenderer()); }

	Textures.CategoryList.Inner.Draw(rect, GetRenderer());
}