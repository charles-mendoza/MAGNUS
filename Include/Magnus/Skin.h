#pragma once
#ifndef MAGNUS_SKIN_H
#define MAGNUS_SKIN_H

#include "Magnus/Font.h"
#include "Magnus/Texture.h"

namespace Magnus
{
	class Skin
	{
	public:

		Skin(Magnus::Rendering::Renderer* pRenderer = NULL);
		virtual ~Skin();

		virtual void ReleaseFont(Magnus::Font* fnt);

		virtual void Load(const Magnus::TextObject & TextureName);
		virtual void LoadInMemory(void* pSrcData, unsigned int size);

		virtual void Process();

		virtual void DrawWindow(Magnus::Rect rect, bool bFocused);
		virtual void DrawWindowCloseButton(Magnus::Rect rect, bool bPressed, bool bHovered, bool bDisabled);
		virtual void DrawWindowMaximizeButton(Magnus::Rect rect, bool bMaximized, bool bPressed, bool bHovered, bool bDisabled);
		virtual void DrawWindowMinimizeButton(Magnus::Rect rect, bool bPressed, bool bHovered, bool bDisabled);
		virtual void DrawButton(Magnus::Rect rect, bool bPressed, bool bHovered, bool bDisabled);
		virtual void DrawMenuItem(Magnus::Rect rect, bool bSubmenuOpen, bool bHovered, bool bChecked);
		virtual void DrawMenuStrip(Magnus::Rect rect);
		virtual void DrawMenu(Magnus::Rect rect, bool bPaddingDisabled);
		virtual void DrawMenuRightArrow(Magnus::Rect rect);
		virtual void DrawMenuDivider(Magnus::Rect rect);
		virtual void DrawShadow(Magnus::Rect rect);
		virtual void DrawRadioButton(Magnus::Rect rect, bool bChecked, bool bPressed, bool bDisabled);
		virtual void DrawCheckBox(Magnus::Rect rect, bool bChecked, bool bPressed, bool bDisabled);
		virtual void DrawToggleTrack(Magnus::Rect rect, bool bChecked, bool bDisabled);
		virtual void DrawToggleBar(Magnus::Rect rect, bool bPressed, bool bHovered, bool bDisabled);
		virtual void DrawGroupBox(Magnus::Rect rect, int textStart, int textHeight, int textWidth);
		virtual void DrawTextBox(Magnus::Rect rect, bool bFocused, bool bDisabled);
		virtual void DrawTabButton(Magnus::Rect rect, bool bActive, int align);
		virtual void DrawTabControl(Magnus::Rect rect);
		virtual void DrawTabTitleBar(Magnus::Rect rect);
		virtual void DrawGenericPanel(Magnus::Rect rect);
		virtual void DrawHighlight(Magnus::Rect rect);
		virtual void DrawScrollTrack(Magnus::Rect rect, bool bHorizontal, bool bPressed);
		virtual void DrawScrollThumb(Magnus::Rect rect, bool bHorizontal, bool bPressed, bool bHovered, bool bDisabled);
		virtual void DrawScrollButton(Magnus::Rect rect, int iDirection, bool bPressed, bool bHovered, bool bDisabled);
		virtual void DrawProgressBar(Magnus::Rect rect, bool bHorizontal, float progress);
		virtual void DrawListBox(Magnus::Rect rect);
		virtual void DrawListBoxLine(Magnus::Rect rect, bool bSelected, bool bHovered, bool bEven);
		virtual void DrawSliderNotchesH(Magnus::Rect rect, int numNotches, int dist);
		virtual void DrawSliderNotchesV(Magnus::Rect rect, int numNotches, int dist);
		virtual void DrawSlider(Magnus::Rect rect, bool bHorizontal, int numNotches, int barSize);
		virtual void DrawSlideButton(Magnus::Rect rect, bool bHorizontal, bool bPressed, bool bHovered, bool bDisabled);
		virtual void DrawComboBox(Magnus::Rect rect, bool bDown, bool bMenuOpen, bool bHovered, bool bDisabled);
		virtual void DrawKeyboardHighlight(Magnus::Rect rect, int iOffset);
		virtual void DrawToolTip(Magnus::Rect rect);
		virtual void DrawComboDownArrow(Magnus::Rect rect, bool bDown, bool bMenuOpen, bool bHovered, bool bDisabled);
		virtual void DrawNumericUpDownButton(Magnus::Rect rect, bool bUp, bool bPressed, bool bHovered, bool bDisabled);
		virtual void DrawStatusBar(Magnus::Rect rect);
		virtual void DrawTreeButton(Magnus::Rect rect, bool bOpen);
		virtual void DrawColorDisplay(Magnus::Rect rect, Color color);
		virtual void DrawModalControl(Magnus::Rect rect);
		virtual void DrawTreeControl(Magnus::Rect rect);
		virtual void DrawTreeNode(bool bOpen, bool bSelected, int iLabelHeight, int iLabelWidth, int iHalfWay, int iLastBranch, bool bIsRoot);
		virtual void DrawCategoryHolder(Magnus::Rect rect);
		virtual void DrawCategoryInner(Magnus::Rect rect, bool bCollapsed);

		struct Colors_t
		{
			struct Window_t
			{
				Magnus::Color TitleActive;
				Magnus::Color TitleInactive;

			} Window;

			struct Button_t
			{
				Magnus::Color Normal;
				Magnus::Color Hover;
				Magnus::Color Down;
				Magnus::Color Disabled;

			} Button;

			struct Tab_t
			{
				struct Active_t
				{
					Magnus::Color Normal;
					Magnus::Color Hover;
					Magnus::Color Down;
					Magnus::Color Disabled;

				} Active;

				struct Inactive_t
				{
					Magnus::Color Normal;
					Magnus::Color Hover;
					Magnus::Color Down;
					Magnus::Color Disabled;

				} Inactive;

			} Tab;

			struct Label_t
			{
				Magnus::Color Default;
				Magnus::Color Bright;
				Magnus::Color Dark;
				Magnus::Color Highlight;

			} Label;

			struct Tree_t
			{
				Magnus::Color Lines;
				Magnus::Color Normal;
				Magnus::Color Hover;
				Magnus::Color Selected;

			} Tree;

			struct Properties_t
			{
				Magnus::Color Line_Normal;
				Magnus::Color Line_Selected;
				Magnus::Color Line_Hover;
				Magnus::Color Column_Normal;
				Magnus::Color Column_Selected;
				Magnus::Color Column_Hover;
				Magnus::Color Label_Normal;
				Magnus::Color Label_Selected;
				Magnus::Color Label_Hover;
				Magnus::Color Border;
				Magnus::Color Title;

			} Properties;

			struct Category_t
			{
				Magnus::Color Header;
				Magnus::Color Header_Closed;

				struct Line_t
				{
					Magnus::Color Text;
					Magnus::Color Text_Hover;
					Magnus::Color Text_Selected;
					Magnus::Color Button;
					Magnus::Color Button_Hover;
					Magnus::Color Button_Selected;

				} Line;

				struct LineAlt_t
				{
					Magnus::Color Text;
					Magnus::Color Text_Hover;
					Magnus::Color Text_Selected;
					Magnus::Color Button;
					Magnus::Color Button_Hover;
					Magnus::Color Button_Selected;

				} LineAlt;

			} Category;

			Magnus::Color ModalBackground;
			Magnus::Color TooltipText;

		} Colors;

		struct Textures_t
		{
			Texturing::Bordered StatusBar;
			Texturing::Bordered Selection;
			Texturing::Bordered Shadow;
			Texturing::Bordered Tooltip;

			struct Panel_t
			{
				Texturing::Bordered Normal;
				Texturing::Bordered Bright;
				Texturing::Bordered Dark;
				Texturing::Bordered Highlight;

			} Panel;

			struct Window_t
			{
				Texturing::Bordered Normal;
				Texturing::Bordered Inactive;
				Texturing::Single Close;
				Texturing::Single Close_Hover;
				Texturing::Single Close_Down;
				Texturing::Single Maxi;
				Texturing::Single Maxi_Hover;
				Texturing::Single Maxi_Down;
				Texturing::Single Mini;
				Texturing::Single Mini_Hover;
				Texturing::Single Mini_Down;
				Texturing::Single Restore;
				Texturing::Single Restore_Hover;
				Texturing::Single Restore_Down;

			} Window;

			struct CheckBox_t
			{
				struct Active_t
				{
					Texturing::Single Normal;
					Texturing::Single Checked;

				} Active;

				struct Disabled_t
				{
					Texturing::Single Normal;
					Texturing::Single Checked;

				} Disabled;

			} CheckBox;

			struct RadioButton_t
			{
				struct Active_t
				{
					Texturing::Single Normal;
					Texturing::Single Checked;

				} Active;

				struct Disabled_t
				{
					Texturing::Single Normal;
					Texturing::Single Checked;

				} Disabled;

			} RadioButton;

			struct ToggleControl_t
			{
				struct Track_t
				{
					Texturing::Bordered Normal;
					Texturing::Bordered Checked;
					Texturing::Bordered Disabled;

				} Track;

				struct Bar_t
				{
					Texturing::Bordered Normal;
					Texturing::Bordered Checked;
					Texturing::Bordered Hover;
					Texturing::Bordered Down;
					Texturing::Bordered Disabled;

				} Bar;

			} ToggleControl;

			struct TextBox_t
			{
				Texturing::Bordered Normal;
				Texturing::Bordered Focus;
				Texturing::Bordered Disabled;

			} TextBox;

			struct Tree_t
			{
				Texturing::Bordered Background;
				Texturing::Single Minus;
				Texturing::Single Plus;

			} Tree;

			struct ProgressBar_t
			{
				Texturing::Bordered Back;
				Texturing::Bordered Front;

			} ProgressBar;

			struct Scroller_t
			{
				Texturing::Bordered TrackV;
				Texturing::Bordered ButtonV_Normal;
				Texturing::Bordered ButtonV_Hover;
				Texturing::Bordered ButtonV_Down;
				Texturing::Bordered ButtonV_Disabled;
				Texturing::Bordered TrackH;
				Texturing::Bordered ButtonH_Normal;
				Texturing::Bordered ButtonH_Hover;
				Texturing::Bordered ButtonH_Down;
				Texturing::Bordered ButtonH_Disabled;

				struct Button_t
				{
					Texturing::Bordered Normal[4];
					Texturing::Bordered Hover[4];
					Texturing::Bordered Down[4];
					Texturing::Bordered Disabled[4];

				} Button;

			} Scroller;

			struct Menu_t
			{
				Texturing::Single RightArrow;
				Texturing::Single Check;

				Texturing::Bordered Strip;
				Texturing::Bordered Background;
				Texturing::Bordered BackgroundWithMargin;
				Texturing::Bordered Hover;

			} Menu;

			struct Input_t
			{
				struct Button_t
				{
					Texturing::Bordered Normal;
					Texturing::Bordered Hovered;
					Texturing::Bordered Disabled;
					Texturing::Bordered Pressed;

				} Button;

				struct ListBox_t
				{
					Texturing::Bordered Background;
					Texturing::Bordered Hovered;

					Texturing::Bordered EvenLine;
					Texturing::Bordered OddLine;
					Texturing::Bordered EvenLineSelected;
					Texturing::Bordered OddLineSelected;

				} ListBox;

				struct UpDown_t
				{
					struct Up_t
					{
						Texturing::Single Normal;
						Texturing::Single Hover;
						Texturing::Single Down;
						Texturing::Single Disabled;

					} Up;

					struct Down_t
					{
						Texturing::Single Normal;
						Texturing::Single Hover;
						Texturing::Single Down;
						Texturing::Single Disabled;

					} Down;

				} UpDown;

				struct ComboBox_t
				{
					Texturing::Bordered Normal;
					Texturing::Bordered Hover;
					Texturing::Bordered Down;
					Texturing::Bordered Disabled;

					struct Button_t
					{
						Texturing::Single Normal;
						Texturing::Single Hover;
						Texturing::Single Down;
						Texturing::Single Disabled;

					} Button;

				} ComboBox;

				struct Slider_t
				{
					struct H_t
					{
						Texturing::Single Normal;
						Texturing::Single Hover;
						Texturing::Single Down;
						Texturing::Single Disabled;

					} H;

					struct V_t
					{
						Texturing::Single Normal;
						Texturing::Single Hover;
						Texturing::Single Down;
						Texturing::Single Disabled;

					} V;

				} Slider;

			} Input;

			struct Tab_t
			{
				struct Left_t
				{
					Texturing::Bordered Active;
					Texturing::Bordered Inactive;

				} Left;

				struct Top_t
				{
					Texturing::Bordered Active;
					Texturing::Bordered Inactive;

				} Top;

				struct Right_t
				{
					Texturing::Bordered Active;
					Texturing::Bordered Inactive;

				} Right;

				struct Bottom_t
				{
					Texturing::Bordered Active;
					Texturing::Bordered Inactive;

				} Bottom;

				Texturing::Bordered Control;
				Texturing::Bordered HeaderBar;

			} Tab;

			struct CategoryList_t
			{
				Texturing::Bordered Outer;
				Texturing::Bordered Inner;
				Texturing::Bordered Header;

			} CategoryList;

			Texturing::Bordered GroupBox;

		} Textures;

	public:

		virtual void SetRenderer(Magnus::Rendering::Renderer* pRenderer)
		{
			m_pRenderer = pRenderer;
		}

		virtual Magnus::Rendering::Renderer* GetRenderer()
		{
			return m_pRenderer;
		}

		virtual Magnus::Font* GetDefaultFont()
		{
			return &m_DefaultFont;
		}

		virtual void SetDefaultFont(Magnus::UnicodeString strFacename, float fSize = 10.0f, bool bBold = false)
		{
			m_DefaultFont.facename = strFacename;
			m_DefaultFont.size = fSize;
			m_DefaultFont.bold = bBold;
		}

	protected:

		Magnus::Font					m_DefaultFont;
		Magnus::Texturing::Texture		m_Texture;
		Magnus::Rendering::Renderer*	m_pRenderer;
	};
}

#endif