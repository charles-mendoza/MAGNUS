#pragma once
#ifndef MAGNUS_CONTROLS_CONTROL_H
#define MAGNUS_CONTROLS_CONTROL_H

#include <list>
#include <map>
#include <algorithm>

#include "Magnus/Structures.h"
#include "Magnus/Renderer.h"
#include "Magnus/Events.h"
#include "Magnus/Utility.h"
#include "Magnus/TextObject.h"
#include "Magnus/Skin.h"
#include "Magnus/ControlList.h"

namespace Magnus
{
	namespace Pos
	{
		enum
		{
			None		= 0,
			Left		= (1 << 1),
			Right		= (1 << 2),
			Top			= (1 << 3),
			Bottom		= (1 << 4),
			CenterV		= (1 << 5),
			CenterH		= (1 << 6),
			Fill		= (1 << 7),
			Center		= CenterV | CenterH
		};
	}

	namespace Controls
	{
		class Canvas;

		/*
			
			Defines the base class for controls, which are components with visual representation.

		*/
		class Control : public Event::Handler
		{
		public:

			typedef std::list<Control*> List;

			typedef std::map<Magnus::UnicodeString, Magnus::Event::Caller*> AccelMap;

			Control(Control* pParent, const Magnus::String & Name = "");
			virtual ~Control();

			virtual const char* GetTypeName() { return "Control"; }

			virtual void DelayedDelete();
			virtual void PreDelete(Magnus::Skin* skin) {};

			virtual void SetParent(Control* pParent);
			virtual Controls::Control* GetParent() const { return m_Parent; }
			virtual Controls::Canvas* GetCanvas();

			virtual Control::List & GetChildren() { if (m_InnerPanel) { return m_InnerPanel->GetChildren(); } return Children; }
			virtual bool IsChild(Controls::Control* pChild);
			virtual unsigned int NumChildren();
			virtual Controls::Control* GetChild(unsigned int i);
			virtual bool SizeToChildren(bool w = true, bool h = true);
			virtual Magnus::Size ChildrenSize();
			virtual Controls::Control* FindChildByName(const Magnus::String & name, bool bRecursive = false);

			template <typename T> T* FindChild(const Magnus::String & name, bool bRecursive = false);

			virtual void SetName(const Magnus::String & name) { m_Name = name; }
			virtual const Magnus::String & GetName() { return m_Name; }

			virtual void Think() {}

		protected:

			virtual void AddChild(Controls::Control* pChild);
			virtual void RemoveChild(Controls::Control* pParent);
			virtual void OnChildAdded(Controls::Control* pChild);
			virtual void OnChildRemoved(Controls::Control* pChild);

		public:

			virtual void RemoveAllChildren();

			virtual void SendToBack();
			virtual void BringToFront();
			virtual void BringNextToControl(Controls::Control* pChild, bool bBehind);

			virtual Magnus::Point LocalPosToCanvas(const Magnus::Point & in = Magnus::Point(0, 0));
			virtual Magnus::Point CanvasPosToLocal(const Magnus::Point & in);

			virtual void Dock(int iDock);
			virtual int GetDock();

			virtual void RestrictToParent(bool restrict) { m_bRestrictToParent = restrict; }
			virtual bool ShouldRestrictToParent() { return m_bRestrictToParent; }

			virtual int X() const { return m_Bounds.x; }
			virtual int Y() const { return m_Bounds.y; }
			virtual int Width() const { return m_Bounds.w; }
			virtual int Height() const { return m_Bounds.h; }
			virtual int Bottom() const { return m_Bounds.y + m_Bounds.h + m_Margin.bottom; }
			virtual int Right() const { return m_Bounds.x + m_Bounds.w + m_Margin.right; }

			virtual const Magnus::Margin & GetMargin() const { return m_Margin; }
			virtual const Magnus::Padding & GetPadding() const { return m_Padding; }

			virtual void SetPos(int x, int y);
			virtual void SetPos(const Magnus::Point & p) { SetPos(p.x, p.y); }
			virtual Magnus::Point GetPos() { return Magnus::Point(X(), Y()); }
			virtual void SetWidth(int w) { SetSize(w, Height()); }
			virtual void SetHeight(int h) { SetSize(Width(), h); }
			virtual bool SetSize(int w, int h);
			virtual bool SetSize(const Magnus::Size & size);
			virtual Magnus::Size GetSize() { return Magnus::Size(Width(), Height()); }
			virtual bool SetBounds(int x, int y, int w, int h);
			virtual bool SetBounds(const Magnus::Rect & bounds);

			virtual void SetPadding(const Magnus::Padding & padding);
			virtual void SetMargin(const Magnus::Margin & margin);

			// MoveTo is identical to SetPos except it uses ShouldRestrictToParent()
			virtual void MoveTo(int x, int y);
			virtual void MoveBy(int x, int y);

			virtual const Magnus::Rect & GetBounds() const { return m_Bounds; }

			virtual Controls::Control* GetControlAt(int x, int y, bool bOnlyIfMouseEnabled = true);

		protected:

			virtual void OnBoundsChanged(Magnus::Rect oldBounds);
			virtual void OnChildBoundsChanged(Magnus::Rect oldChildBounds, Controls::Control* pChild);

			virtual void OnScaleChanged();

		public:

			// InnerBounds is the area inside the control that
			// doesn't have child controls docked to it.
			virtual const Rect & GetInnerBounds() const { return m_InnerBounds; }

		protected:

			Magnus::Rect m_InnerBounds;

		public:

			virtual const Magnus::Rect & GetRenderBounds() const { return m_RenderBounds; }

		protected:

			virtual void UpdateRenderBounds();

		public:

			virtual void DoRender(Magnus::Skin* skin);
			virtual void DoCacheRender(Magnus::Skin* skin, Controls::Control* pMaster);
			virtual void RenderRecursive(Magnus::Skin* skin, const Magnus::Rect & cliprect);

			virtual bool ShouldClip() { return true; }

		protected:

				virtual void Render(Magnus::Skin* skin);
				virtual void RenderUnder(Magnus::Skin* /*skin*/) {};
				virtual void RenderOver(Magnus::Skin* /*skin*/) {};
				virtual void RenderFocus(Magnus::Skin* /*skin*/);

		public:

			virtual void SetHidden(bool hidden) { if (m_bHidden == hidden) { return; } m_bHidden = hidden; Invalidate(); Redraw(); }
			virtual bool Hidden() const; // Returns true only if this control is hidden
			virtual bool Visible() const; // Return false if this control or its parents are hidden
			virtual void Hide() { SetHidden(true); }
			virtual void Show() { SetHidden(false); }

		public:

			// Skin
			virtual void SetSkin(Magnus::Skin* skin, bool doChildren = false);
			virtual Magnus::Skin* GetSkin();

			virtual void SetOpacity(float fOpacity, bool doChildren = true);
			virtual float Opacity() { return m_fOpacity; }

			// Background drawing
			virtual bool ShouldDrawBackground() { return m_bDrawBackground; }
			virtual void SetShouldDrawBackground(bool b) { m_bDrawBackground = b; }

		protected:

			virtual void OnSkinChanged(Magnus::Skin* newSkin);
			virtual void OnOpacityChanged();

		public:

			virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY);
			virtual bool OnMouseWheeled(int iDelta);
			virtual void OnMouseClickLeft(int /*x*/, int /*y*/, bool /*bDown*/) {}
			virtual void OnMouseClickRight(int /*x*/, int /*y*/, bool /*bDown*/) {}
			virtual void OnMouseDoubleClickLeft(int x, int y) { OnMouseClickLeft(x, y, true); }
			virtual void OnMouseDoubleClickRight(int x, int y) { OnMouseClickRight(x, y, true); }
			virtual void OnLostKeyboardFocus() {}
			virtual void OnKeyboardFocus() {}

			virtual void SetMouseInputEnabled(bool b) { m_bMouseInputEnabled = b; }
			virtual bool GetMouseInputEnabled() const { return m_bMouseInputEnabled; }

			virtual void SetKeyboardInputEnabled(bool b) { m_bKeyboardInputEnabled = b; }
			virtual bool GetKeyboardInputEnabled() const { return m_bKeyboardInputEnabled; }
			virtual bool NeedsInputChars() { return false; }

			virtual bool OnChar(Magnus::UnicodeChar /*c*/) { return false; }

			virtual bool OnKeyPress(int iKey, bool bPress = true);
			virtual bool OnKeyRelease(int iKey);

			virtual void OnPaste(Controls::Control* /*pFrom*/) {}
			virtual void OnCopy(Controls::Control* /*pFrom*/) {}
			virtual void OnCut(Controls::Control* /*pFrom*/) {}
			virtual void OnSelectAll(Controls::Control* /*pFrom*/) {}

			virtual bool OnKeyTab(bool bDown);
			virtual bool OnKeyBackspace(bool /*bDown*/) { return false; }
			virtual bool OnKeyReturn(bool /*bDown*/) { return false; }
			virtual bool OnKeyEscape(bool /*bDown*/) { return false; }
			virtual bool OnKeySpace(bool /*bDown*/) { return false; }
			virtual bool OnKeyEnd(bool /*bDown*/) { return false; }
			virtual bool OnKeyHome(bool /*bDown*/) { return false; }
			virtual bool OnKeyLeft(bool /*bDown*/) { return false; }
			virtual bool OnKeyUp(bool /*bDown*/) { return false; }
			virtual bool OnKeyRight(bool /*bDown*/) { return false; }
			virtual bool OnKeyDown(bool /*bDown*/) { return false; }
			virtual bool OnKeyDelete(bool /*bDown*/) { return false; }

			virtual void OnMouseEnter();
			virtual void OnMouseLeave();
			virtual bool IsHovered();
			virtual bool ShouldDrawHover();

			virtual void Touch();
			virtual void OnChildTouched(Controls::Control* pChild);

			virtual bool IsOnTop();

			virtual bool HasFocus();
			virtual void Focus();
			virtual void Blur();

			// Other
			virtual void SetDisabled(bool active) { if (m_bDisabled == active) { return; } m_bDisabled = active; Redraw(); }
			virtual bool IsDisabled() { return m_bDisabled; }

			virtual void Redraw() { UpdateColours(); m_bCacheTextureDirty = true; if (m_Parent) { m_Parent->Redraw(); } }
			virtual void UpdateColours() {}
			virtual void SetCacheToTexture() { m_bCacheToTexture = true; }
			virtual bool ShouldCacheToTexture() { return m_bCacheToTexture; }

			virtual Magnus::Point GetMinimumSize() { return Magnus::Point(1, 1); }
			virtual Magnus::Point GetMaximumSize() { return Magnus::Point(4096, 4096); }

			virtual bool IsMenuComponent();
			virtual void CloseMenus();

			virtual bool IsTabable() { return m_Tabable; }
			virtual void SetTabable(bool isTabable) { m_Tabable = isTabable; }

			// Accelerator functionality
			void DefaultAccel(Controls::Control* /*pCtrl*/) { AcceleratePressed(); }
			virtual void AcceleratePressed() {}
			virtual bool AccelOnlyFocus() { return false; }
			virtual bool HandleAccelerator(Magnus::UnicodeString & accelerator);

			template <typename T>
			void AddAccelerator(const TextObject & accelerator, T func, Magnus::Event::Handler* handler = NULL)
			{
				if (handler == NULL)
				{
					handler = this;
				}

				Magnus::Event::Caller* caller = new Magnus::Event::Caller();
				caller->Add(handler, func);
				Magnus::UnicodeString str = accelerator.GetUnicode();
				Magnus::Utility::Strings::ToUpper(str);
				Magnus::Utility::Strings::Strip(str, L" ");
				m_Accelerators[str] = caller;
			}

			template <typename T>
			void AddAccelerator(const TextObject & accelerator, void (T::*func)(Magnus::Event::Info), Magnus::Event::Handler* handler = NULL, void* data = NULL)
			{
				if (handler == NULL)
				{
					handler = this;
				}

				Magnus::Event::Caller* caller = new Magnus::Event::Caller();
				caller->Add(handler, func, data);
				Magnus::UnicodeString str = accelerator.GetUnicode();
				Magnus::Utility::Strings::ToUpper(str);
				Magnus::Utility::Strings::Strip(str, L" ");
				m_Accelerators[str] = caller;
			}

			void AddAccelerator(const TextObject & accelerator)
			{
				AddAccelerator(accelerator, &Control::DefaultAccel, this);
			}

			template <typename T>
			void GlobalAddAccelerator(const TextObject & accelerator, T func, Magnus::Event::Handler* handler = NULL)
			{
				if (handler == NULL)
				{
					handler = this;
				}

				Magnus::Event::Caller* caller = new Magnus::Event::Caller();
				caller->GlobalAdd(handler, func);
				Magnus::UnicodeString str = accelerator.GetUnicode();
				Magnus::Utility::Strings::ToUpper(str);
				Magnus::Utility::Strings::Strip(str, L" ");
				m_Accelerators[str] = caller;
			}

			template <typename T>
			void GlobalAddAccelerator(const TextObject & accelerator, void (T::*func)(Magnus::Event::Info), Magnus::Event::Handler* handler = NULL, void* data = NULL)
			{
				if (handler == NULL)
				{
					handler = this;
				}

				Magnus::Event::Caller* caller = new Magnus::Event::Caller();
				caller->GlobalAdd(handler, func, data);
				Magnus::UnicodeString str = accelerator.GetUnicode();
				Magnus::Utility::Strings::ToUpper(str);
				Magnus::Utility::Strings::Strip(str, L" ");
				m_Accelerators[str] = caller;
			}

			AccelMap m_Accelerators;

			// Default Events

			Magnus::Event::Caller onHoverEnter;
			Magnus::Event::Caller onHoverLeave;

			// Childrens List

			Control::List Children;

		protected:

			// The logical parent
			// It's usually what you expect, the control you've parented it to.
			Control* m_Parent;

			// If the innerpanel exists our children will automatically
			// become children of that instead of us - allowing us to move
			// them all around by moving that panel (useful for scrolling etc)
			Control* m_InnerPanel;
			virtual Control* Inner() { return m_InnerPanel; }

			// This is the panel's actual parent- most likely the logical
			// parent's InnerPanel (if it has one). You should rarely need this.
			Control* m_ActualParent;

			Magnus::Skin* m_Skin;
			
			Magnus::Rect m_Bounds;
			Magnus::Rect m_RenderBounds;

			Magnus::Padding m_Padding;
			Magnus::Margin m_Margin;

			Magnus::String m_Name;

			bool m_bRestrictToParent;
			bool m_bDisabled;
			bool m_bHidden;
			bool m_bMouseInputEnabled;
			bool m_bKeyboardInputEnabled;
			bool m_bDrawBackground;

			int m_iDock;

			bool m_Tabable;

			float m_fOpacity;

		public:

			bool NeedsLayout() { return m_bNeedsLayout; }
			void Invalidate();
			void InvalidateParent() { if (m_Parent) { m_Parent->Invalidate(); } }
			void InvalidateChildren(bool bRecursive = false);
			void Position(int pos, int xpadding = 0, int ypadding = 0);

		protected:

			virtual void RecurseLayout(Magnus::Skin* skin);
			virtual void Layout(Magnus::Skin* skin);
			virtual void PostLayout(Magnus::Skin* /*skin*/) {}

			bool m_bNeedsLayout;
			bool m_bCacheTextureDirty;
			bool m_bCacheToTexture;

			//
			// Dynamic casting, see magnus_cast below
			//
		public:

			static const char* GetIdentifier()
			{
				static const char* ident = "Control";
				return ident;
			}

			virtual Controls::Control* DynamicCast(const char* Variable)
			{
				return NULL;
			}

		public:

			void DoNotIncludeInSize() { m_bIncludeInSize = false; }
			bool ShouldIncludeInSize() { return m_bIncludeInSize; }

		protected:

			bool m_bIncludeInSize;

		public:

			virtual TextObject GetChildValue(const Magnus::String & strName);
			virtual TextObject GetValue();
			virtual void SetValue(const TextObject & strValue);
			virtual void DoAction() {}
			virtual void SetAction(Event::Handler* pObject, Handler::FunctionWithInformation pFunction, const Magnus::Event::Packet & packet) {}

			virtual int GetNamedChildren(Magnus::ControlList & list, const Magnus::String & strName, bool bDeep = true);
			virtual Magnus::ControlList GetNamedChildren(const Magnus::String & strName, bool bDeep = true);
		};
	}
}
/*
	To avoid using dynamic_cast we have magnus_cast.

	Each class in Magnus includes MAGNUS_DYNAMIC. You don't have to include this macro anywhere as it's
	automatically included in the MAGNUS_CONTROL macro.

	MAGNUS_DYNAMIC adds 2 functions:

	GetIdentifier - a static function with a static variable inside, which returns
					the address of the static variable. The variable is defined as a
					string containing "BASECLASSNAME:CLASSNAME". This string should be
					as unique as possible - or the compiler might optimize the variables
					together - which means that when this function return the address it
					could be the same on one or more variable. Something to bear in mind.

	DynamicCast -	non static, takes an address returned by GetIdentifier. Will return an
					address of a control if the control can safely be cast to the class from
					which the identifier was taken.

	Really you shouldn't actually have to concern yourself with that stuff. The only thing you
	should use in theory is magnus_cast - which is used just the same as dynamic cast - except for
	one difference. We pass in the class name, not a pointer to the class.

	magnus_cast<MyControl>(control)
	dynamic_cast<MyControl*>(control)
*/

template <class T>
T* magnus_cast(Magnus::Controls::Control* p)
{
	if (!p) { return NULL; }

	Magnus::Controls::Control* pReturn = p->DynamicCast(T::GetIdentifier());

	if (!pReturn) { return NULL; }

	return static_cast<T*>(pReturn);
}

template <typename T>
T* Magnus::Controls::Control::FindChild(const Magnus::String & name, bool bRecursive)
{
	return magnus_cast<T>(FindChildByName(name, bRecursive));
}

#define MAGNUS_DYNAMIC(ThisName, BaseName)										\
																				\
	static const char* GetIdentifier()											\
	{																			\
		static const char* ident = #BaseName ":" #ThisName;				\
		return ident;															\
	}																			\
	virtual Magnus::Controls::Control* DynamicCast(const char* Variable)		\
	{																			\
		if (GetIdentifier() == Variable)										\
		return this;															\
																				\
		return BaseClass::DynamicCast(Variable);								\
	}

#define MAGNUS_CLASS(ThisName, BaseName)\
		typedef BaseName BaseClass;\
		typedef ThisName ThisClass;

// To be placed in the controls .h definition.
#define MAGNUS_CONTROL(ThisName, BaseName)\
	public:\
	MAGNUS_CLASS(ThisName, BaseName)\
	MAGNUS_DYNAMIC(ThisName, BaseName)\
	virtual const char* GetTypeName() { return #ThisName; }\
	virtual const char* GetBaseTypeName() { return BaseClass::GetTypeName(); }\
	ThisName(Magnus::Controls::Control* pParent, const Magnus::String& pName = "")

#define MAGNUS_CONTROL_INLINE(ThisName, BaseName)\
	MAGNUS_CONTROL(ThisName, BaseName) : BaseClass(pParent, pName)

#define MAGNUS_CONTROL_CONSTRUCTOR(ThisName)\
	ThisName::ThisName(Magnus::Controls::Control* pParent, const Magnus::String& pName) : BaseClass(pParent, pName)

#endif