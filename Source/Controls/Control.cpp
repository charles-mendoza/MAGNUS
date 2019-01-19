#include "Magnus/Controls/Control.h"
#include "Magnus/Magnus.h"
#include "Magnus/Renderer.h"
#include "Magnus/Skin.h"
#include "Magnus/Platform.h"
#include "Magnus/Utility.h"

using namespace Magnus;
using namespace Controls;

Control::Control(Control* pParent, const String & Name)
{
	m_Parent = NULL;
	m_ActualParent = NULL;
	m_InnerPanel = NULL;
	m_Skin = NULL;
	SetName(Name);
	SetParent(pParent);
	m_bHidden = false;
	m_Bounds = Magnus::Rect(0, 0, 10, 10);
	m_Padding = Padding(0, 0, 0, 0);
	m_Margin = Margin(0, 0, 0, 0);
	m_iDock = 0;
	m_fOpacity = 100.0f;
	RestrictToParent(false);
	SetMouseInputEnabled(true);
	SetKeyboardInputEnabled(false);
	Invalidate();
	SetTabable(false);
	SetShouldDrawBackground(true);
	m_bDisabled = false;
	m_bCacheTextureDirty = true;
	m_bCacheToTexture = false;
	m_bIncludeInSize = true;
}

Control::~Control()
{
	{
		Canvas* canvas = GetCanvas();

		if (canvas)
		{
			canvas->PreDeleteCanvas(this);
		}
	}
	Control::List::iterator iter = Children.begin();

	while (iter != Children.end())
	{
		Control* pChild = *iter;
		iter = Children.erase(iter);
		delete pChild;
	}

	for (AccelMap::iterator accelIt = m_Accelerators.begin(); accelIt != m_Accelerators.end(); ++accelIt)
	{
		delete accelIt->second;
	}

	m_Accelerators.clear();
	SetParent(NULL);

	if (Magnus::HoveredControl == this) { Magnus::HoveredControl = NULL; }

	if (Magnus::KeyboardFocus == this) { Magnus::KeyboardFocus = NULL; }

	if (Magnus::MouseFocus == this) { Magnus::MouseFocus = NULL; }
}

void Control::Invalidate()
{
	m_bNeedsLayout = true;
	m_bCacheTextureDirty = true;
}

void Control::DelayedDelete()
{
	Canvas* canvas = GetCanvas();
	canvas->AddDelayedDelete(this);
}

Canvas* Control::GetCanvas()
{
	Control* pCanvas = m_Parent;

	if (!pCanvas) { return NULL; }

	return pCanvas->GetCanvas();
}

void Control::SetParent(Control* parent)
{
	if (m_Parent == parent) { return; }

	if (m_Parent)
	{
		m_Parent->RemoveChild(this);
	}

	m_Parent = parent;
	m_ActualParent = NULL;

	if (m_Parent)
	{
		m_Parent->AddChild(this);
	}
}

void Control::Dock(int iDock)
{
	if (m_iDock == iDock) { return; }

	m_iDock = iDock;
	Invalidate();
	InvalidateParent();
}

int Control::GetDock()
{
	return m_iDock;
}

bool Control::Hidden() const
{
	return m_bHidden;
}

bool Control::Visible() const
{
	if (Hidden()) { return false; }

	if (GetParent())
	{
		return GetParent()->Visible();
	}

	return true;
}

void Control::InvalidateChildren(bool bRecursive)
{
	for (Control::List::iterator it = Children.begin(); it != Children.end(); ++it)
	{
		(*it)->Invalidate();

		if (bRecursive)
		{
			(*it)->InvalidateChildren(bRecursive);
		}
	}

	if (m_InnerPanel)
	{
		for (Control::List::iterator it = m_InnerPanel->Children.begin(); it != m_InnerPanel->Children.end(); ++it)
		{
			(*it)->Invalidate();

			if (bRecursive)
			{
				(*it)->InvalidateChildren(bRecursive);
			}
		}
	}
}

void Control::Position(int pos, int xpadding, int ypadding)
{
	const Rect & bounds = GetParent()->GetInnerBounds();
	const Margin & margin = GetMargin();
	int x = X();
	int y = Y();

	if (pos & Pos::Left) { x = bounds.x + xpadding + margin.left; }

	if (pos & Pos::Right) { x = bounds.x + (bounds.w - Width() - xpadding - margin.right); }

	if (pos & Pos::CenterH) { x = bounds.x + (bounds.w - Width()) * 0.5; }

	if (pos & Pos::Top) { y = bounds.y + ypadding; }

	if (pos & Pos::Bottom) { y = bounds.y + (bounds.h - Height() - ypadding); }

	if (pos & Pos::CenterV) { y = bounds.y + (bounds.h - Height()) * 0.5; }

	SetPos(x, y);
}

void Control::SendToBack()
{
	if (!m_ActualParent) { return; }

	if (m_ActualParent->Children.front() == this) { return; }

	m_ActualParent->Children.remove(this);
	m_ActualParent->Children.push_front(this);
	InvalidateParent();
}

void Control::BringToFront()
{
	if (!m_ActualParent) { return; }

	if (m_ActualParent->Children.back() == this) { return; }

	m_ActualParent->Children.remove(this);
	m_ActualParent->Children.push_back(this);
	InvalidateParent();
	Redraw();
}

Controls::Control* Control::FindChildByName(const Magnus::String & name, bool bRecursive)
{
	Control::List::iterator iter;

	for (iter = Children.begin(); iter != Children.end(); ++iter)
	{
		Control* pChild = *iter;

		if (!pChild->GetName().empty() && pChild->GetName() == name)
		{
			return pChild;
		}

		if (bRecursive)
		{
			Controls::Control* pSubChild = pChild->FindChildByName(name, true);

			if (pSubChild)
			{
				return pSubChild;
			}
		}
	}

	return NULL;
}

void Control::BringNextToControl(Controls::Control* pChild, bool bBehind)
{
	if (!m_ActualParent) { return; }

	m_ActualParent->Children.remove(this);
	Control::List::iterator it = std::find(m_ActualParent->Children.begin(), m_ActualParent->Children.end(), pChild);

	if (it == m_ActualParent->Children.end())
	{
		return BringToFront();
	}

	if (bBehind)
	{
		++it;

		if (it == m_ActualParent->Children.end())
		{
			return BringToFront();
		}
	}

	m_ActualParent->Children.insert(it, this);
	InvalidateParent();
}

void Control::AddChild(Control* pChild)
{
	if (m_InnerPanel)
	{
		m_InnerPanel->AddChild(pChild);
		return;
	}

	Children.push_back(pChild);
	OnChildAdded(pChild);
	pChild->m_ActualParent = this;
}

void Control::RemoveChild(Control* pChild)
{
	// If we removed our innerpanel
	// remove our pointer to it
	if (m_InnerPanel == pChild)
	{
		m_InnerPanel = NULL;
	}

	if (m_InnerPanel)
	{
		m_InnerPanel->RemoveChild(pChild);
	}

	Children.remove(pChild);
	OnChildRemoved(pChild);
}

void Control::RemoveAllChildren()
{
	while (Children.size() > 0)
	{
		RemoveChild(*Children.begin());
	}
}

unsigned int Control::NumChildren()
{
	// Include m_InnerPanel's children here?
	return Children.size();
}

Controls::Control* Control::GetChild(unsigned int i)
{
	if (i >= NumChildren()) { return NULL; }

	for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
	{
		if (i == 0) { return *iter; }

		i--;
	}

	// Should never happen.
	return NULL;
}

void Control::OnChildAdded(Control* /*pChild*/)
{
	Invalidate();
}

void Control::OnChildRemoved(Control* /*pChild*/)
{
	Invalidate();
}

Skin* Control::GetSkin()
{
	if (m_Skin) { return m_Skin; }

	if (m_Parent) { return m_Parent->GetSkin(); }

	return NULL;
}

void Control::MoveBy(int x, int y)
{
	MoveTo(X() + x, Y() + y);
}

void Control::MoveTo(int x, int y)
{
	if (m_bRestrictToParent && GetParent())
	{
		Control* pParent = GetParent();

		if (x - GetPadding().left < pParent->GetMargin().left) { x = pParent->GetMargin().left + GetPadding().left; }

		if (y - GetPadding().top < pParent->GetMargin().top) { y = pParent->GetMargin().top + GetPadding().top; }

		if (x + Width() + GetPadding().right > pParent->Width() - pParent->GetMargin().right) { x = pParent->Width() - pParent->GetMargin().right - Width() - GetPadding().right; }

		if (y + Height() + GetPadding().bottom > pParent->Height() - pParent->GetMargin().bottom) { y = pParent->Height() - pParent->GetMargin().bottom - Height() - GetPadding().bottom; }
	}

	SetBounds(x, y, Width(), Height());
}

void Control::SetPos(int x, int y)
{
	SetBounds(x, y, Width(), Height());
}

bool Control::SetSize(int w, int h)
{
	return SetBounds(X(), Y(), w, h);
}

bool Control::SetSize(const Magnus::Size & s)
{
	return SetSize(s.w, s.h);
}

bool Control::SetBounds(const Magnus::Rect & bounds)
{
	return SetBounds(bounds.x, bounds.y, bounds.w, bounds.h);
}

bool Control::SetBounds(int x, int y, int w, int h)
{
	if (m_Bounds.x == x &&
		m_Bounds.y == y &&
		m_Bounds.w == w &&
		m_Bounds.h == h) {
		return false;
	}

	Magnus::Rect oldBounds = GetBounds();
	m_Bounds.x = x;
	m_Bounds.y = y;
	m_Bounds.w = w;
	m_Bounds.h = h;
	OnBoundsChanged(oldBounds);
	return true;
}

void Control::OnBoundsChanged(Magnus::Rect oldBounds)
{
	// Anything that needs to update on size changes
	// Iterate my children and tell them I've changed
	if (GetParent())
	{
		GetParent()->OnChildBoundsChanged(oldBounds, this);
	}

	if (m_Bounds.w != oldBounds.w || m_Bounds.h != oldBounds.h)
	{
		Invalidate();
	}

	Redraw();
	UpdateRenderBounds();
}

void Control::OnScaleChanged()
{
	for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
	{
		(*iter)->OnScaleChanged();
	}
}

void Control::OnChildBoundsChanged(Magnus::Rect /*oldChildBounds*/, Controls::Control* /*pChild*/)
{
}

void Control::Render(Magnus::Skin* /*skin*/)
{
}

void Control::DoCacheRender(Magnus::Skin* skin, Controls::Control* pMaster)
{
	Magnus::Rendering::Renderer* render = skin->GetRenderer();
	Magnus::Rendering::ICacheToTexture* cache = render->GetCTT();

	if (!cache) { return; }

	Magnus::Point pOldRenderOffset = render->GetRenderOffset();
	Magnus::Rect rOldRegion = render->ClipRegion();

	render->SetOpacity(Opacity());

	if (this != pMaster)
	{
		render->AddRenderOffset(GetBounds());
		render->AddClipRegion(GetBounds());
	}
	else
	{
		render->SetRenderOffset(Magnus::Point(0, 0));
		render->SetClipRegion(GetBounds());
	}

	if (m_bCacheTextureDirty && render->ClipRegionVisible())
	{
		render->StartClip();
		{
			if (ShouldCacheToTexture())
			{
				cache->SetupCacheTexture(this);
			}

			// Render myself first
			Render(skin);

			if (!Children.empty())
			{
				// Now render my kids
				for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
				{
					Control* pChild = *iter;

					if (pChild->Hidden()) { continue; }

					pChild->DoCacheRender(skin, pMaster);
				}
			}

			if (ShouldCacheToTexture())
			{
				cache->FinishCacheTexture(this);
				m_bCacheTextureDirty = false;
			}
		}
		render->EndClip();
	}

	render->SetClipRegion(rOldRegion);
	render->StartClip();
	{
		render->SetRenderOffset(pOldRenderOffset);
		cache->DrawCachedControlTexture(this);
	}
	render->EndClip();
}

void Control::DoRender(Magnus::Skin* skin)
{
	// If this control has a different skin,
	// then so does its children.
	if (m_Skin) { skin = m_Skin; }

	// Do think
	Think();
	Magnus::Rendering::Renderer* render = skin->GetRenderer();

	if (render->GetCTT() && ShouldCacheToTexture())
	{
		DoCacheRender(skin, this);
		return;
	}

	RenderRecursive(skin, GetBounds());
}

void Control::RenderRecursive(Magnus::Skin* skin, const Magnus::Rect & cliprect)
{
	Magnus::Rendering::Renderer* render = skin->GetRenderer();
	Magnus::Point pOldRenderOffset = render->GetRenderOffset();
	Magnus::Rect rOldRegion = render->ClipRegion();
	render->SetOpacity(Opacity());
	render->AddRenderOffset(cliprect);
	RenderUnder(skin);

	//
	// If this control is clipping, change the clip rect to ourselves
	// (if not then we stil clip using our parent clip rect)
	//
	if (ShouldClip())
	{
		render->AddClipRegion(cliprect);

		if (!render->ClipRegionVisible())
		{
			render->SetRenderOffset(pOldRenderOffset);
			render->SetClipRegion(rOldRegion);
			return;
		}
	}

	//
	// Render this control and its child controls
	//
	render->StartClip();
	{
		Render(skin);

		if (!Children.empty())
		{
			// Now render my kids
			for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
			{
				Control* pChild = *iter;

				if (pChild->Hidden()) { continue; }

				pChild->DoRender(skin);
			}
		}
	}
	render->EndClip();
	//
	// Render overlay/focus
	//
	{
		render->SetClipRegion(rOldRegion);
		render->StartClip();
		{
			RenderOver(skin);
			RenderFocus(skin);
		}
		render->EndClip();
		render->SetRenderOffset(pOldRenderOffset);
	}
}

void Control::SetOpacity(float fOpacity, bool doChildren)
{
	if (m_fOpacity == fOpacity) { return; }

	m_fOpacity = fOpacity;
	Invalidate();
	Redraw();
	OnOpacityChanged();

	if (doChildren)
	{
		for (Control::List::iterator it = Children.begin(); it != Children.end(); ++it)
		{
			(*it)->SetOpacity(fOpacity, true);
		}
	}
}

void Control::OnOpacityChanged()
{
}

void Control::SetSkin(Magnus::Skin* skin, bool doChildren)
{
	if (m_Skin == skin) { return; }

	m_Skin = skin;
	Invalidate();
	Redraw();
	OnSkinChanged(skin);

	if (doChildren)
	{
		for (Control::List::iterator it = Children.begin(); it != Children.end(); ++it)
		{
			(*it)->SetSkin(skin, true);
		}
	}
}

void Control::OnSkinChanged(Magnus::Skin* /*skin*/)
{
}

bool Control::OnMouseWheeled(int iDelta)
{
	if (m_ActualParent)
	{
		return m_ActualParent->OnMouseWheeled(iDelta);
	}

	return false;
}

void Control::OnMouseMoved(int /*x*/, int /*y*/, int /*deltaX*/, int /*deltaY*/)
{
}

void Control::OnMouseEnter()
{
	onHoverEnter.Call(this);
	Redraw();
}

void Control::OnMouseLeave()
{
	onHoverLeave.Call(this);
	Redraw();
}

bool Control::IsHovered()
{
	return Magnus::HoveredControl == this;
}

bool Control::ShouldDrawHover()
{
	return Magnus::MouseFocus == this || Magnus::MouseFocus == NULL;
}

bool Control::HasFocus()
{
	return Magnus::KeyboardFocus == this;
}

void Control::Focus()
{
	if (Magnus::KeyboardFocus == this) { return; }

	if (Magnus::KeyboardFocus)
	{
		Magnus::KeyboardFocus->OnLostKeyboardFocus();
	}

	Magnus::KeyboardFocus = this;
	OnKeyboardFocus();
	Redraw();
}

void Control::Blur()
{
	if (Magnus::KeyboardFocus != this) { return; }

	Magnus::KeyboardFocus = NULL;
	OnLostKeyboardFocus();
	Redraw();
}

bool Control::IsOnTop()
{
	if (!GetParent())
	{
		return false;
	}

	Control::List::iterator iter = GetParent()->Children.begin();
	Control* pChild = *iter;

	if (pChild = this)
	{
		return true;
	}

	return false;
}

void Control::Touch()
{
	if (GetParent())
	{
		GetParent()->OnChildTouched(this);
	}
}

void Control::OnChildTouched(Controls::Control* /*pChild*/)
{
	Touch();
}

Control* Control::GetControlAt(int x, int y, bool bOnlyIfMouseEnabled)
{
	if (Hidden()) { return NULL; }

	if (x < 0 || y < 0 || x >= Width() || y >= Height()) { return NULL; }

	Control::List::reverse_iterator iter;

	for (iter = Children.rbegin(); iter != Children.rend(); ++iter)
	{
		Control* pChild = *iter;
		Control* pFound = NULL;
		pFound = pChild->GetControlAt(x - pChild->X(), y - pChild->Y(), bOnlyIfMouseEnabled);

		if (pFound) { return pFound; }
	}

	if (bOnlyIfMouseEnabled && !GetMouseInputEnabled()) { return NULL; }

	return this;
}

void Control::Layout(Magnus::Skin* skin)
{
	if (skin->GetRenderer()->GetCTT() && ShouldCacheToTexture())
	{
		skin->GetRenderer()->GetCTT()->CreateControlCacheTexture(this);
	}
}

void Control::RecurseLayout(Magnus::Skin* skin)
{
	if (m_Skin) { skin = m_Skin; }

	if (Hidden()) { return; }

	if (NeedsLayout())
	{
		m_bNeedsLayout = false;
		Layout(skin);
	}

	Magnus::Rect bounds = GetRenderBounds();
	// Adjust bounds for padding
	bounds.x += m_Padding.left;
	bounds.w -= m_Padding.left + m_Padding.right;
	bounds.y += m_Padding.top;
	bounds.h -= m_Padding.top + m_Padding.bottom;

	for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
	{
		Control* pChild = *iter;

		if (pChild->Hidden()) { continue; }

		int iDock = pChild->GetDock();

		if (iDock & Pos::Fill) { continue; }

		if (iDock & Pos::Top)
		{
			const Margin & margin = pChild->GetMargin();
			pChild->SetBounds(bounds.x + margin.left, bounds.y + margin.top, bounds.w - margin.left - margin.right, pChild->Height());
			int iHeight = margin.top + margin.bottom + pChild->Height();
			bounds.y += iHeight;
			bounds.h -= iHeight;
		}

		if (iDock & Pos::Left)
		{
			const Margin & margin = pChild->GetMargin();
			pChild->SetBounds(bounds.x + margin.left, bounds.y + margin.top, pChild->Width(), bounds.h - margin.top - margin.bottom);
			int iWidth = margin.left + margin.right + pChild->Width();
			bounds.x += iWidth;
			bounds.w -= iWidth;
		}

		if (iDock & Pos::Right)
		{
			// TODO: THIS MARGIN CODE MIGHT NOT BE FULLY FUNCTIONAL
			const Margin & margin = pChild->GetMargin();
			pChild->SetBounds((bounds.x + bounds.w) - pChild->Width() - margin.right, bounds.y + margin.top, pChild->Width(), bounds.h - margin.top - margin.bottom);
			int iWidth = margin.left + margin.right + pChild->Width();
			bounds.w -= iWidth;
		}

		if (iDock & Pos::Bottom)
		{
			// TODO: THIS MARGIN CODE MIGHT NOT BE FULLY FUNCTIONAL
			const Margin & margin = pChild->GetMargin();
			pChild->SetBounds(bounds.x + margin.left, (bounds.y + bounds.h) - pChild->Height() - margin.bottom, bounds.w - margin.left - margin.right, pChild->Height());
			bounds.h -= pChild->Height() + margin.bottom + margin.top;
		}

		pChild->RecurseLayout(skin);
	}

	m_InnerBounds = bounds;

	//
	// Fill uses the left over space, so do that now.
	//
	for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
	{
		Control* pChild = *iter;
		int iDock = pChild->GetDock();

		if (!(iDock & Pos::Fill)) { continue; }

		const Margin & margin = pChild->GetMargin();
		pChild->SetBounds(bounds.x + margin.left, bounds.y + margin.top, bounds.w - margin.left - margin.right, bounds.h - margin.top - margin.bottom);
		pChild->RecurseLayout(skin);
	}

	PostLayout(skin);

	if (IsTabable() && !IsDisabled())
	{
		if (!GetCanvas()->FirstTab) { GetCanvas()->FirstTab = this; }

		if (!GetCanvas()->NextTab) { GetCanvas()->NextTab = this; }
	}

	if (Magnus::KeyboardFocus == this)
	{
		GetCanvas()->NextTab = NULL;
	}
}

bool Control::IsChild(Controls::Control* pChild)
{
	for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
	{
		if (pChild == (*iter)) { return true; }
	}

	return false;
}

Magnus::Point Control::LocalPosToCanvas(const Magnus::Point & pt)
{
	if (m_Parent)
	{
		int x = pt.x + X();
		int y = pt.y + Y();

		// If our parent has an inner panel and we're a child of it
		// add its offset onto us.
		if (m_Parent->m_InnerPanel && m_Parent->m_InnerPanel->IsChild(this))
		{
			x += m_Parent->m_InnerPanel->X();
			y += m_Parent->m_InnerPanel->Y();
		}

		return m_Parent->LocalPosToCanvas(Magnus::Point(x, y));
	}

	return pt;
}

Magnus::Point Control::CanvasPosToLocal(const Magnus::Point & pt)
{
	if (m_Parent)
	{
		int x = pt.x - X();
		int y = pt.y - Y();

		// If our parent has an inner panel and we're a child of it
		// subtract its offset onto us.
		if (m_Parent->m_InnerPanel && m_Parent->m_InnerPanel->IsChild(this))
		{
			x -= m_Parent->m_InnerPanel->X();
			y -= m_Parent->m_InnerPanel->Y();
		}

		return m_Parent->CanvasPosToLocal(Magnus::Point(x, y));
	}

	return pt;
}

bool Control::IsMenuComponent()
{
	if (!m_Parent) { return false; }

	return m_Parent->IsMenuComponent();
}

void Control::CloseMenus()
{
	for (Control::List::iterator it = Children.begin(); it != Children.end(); ++it)
	{
		(*it)->CloseMenus();
	}
}

void Control::UpdateRenderBounds()
{
	m_RenderBounds.x = 0;
	m_RenderBounds.y = 0;
	m_RenderBounds.w = m_Bounds.w;
	m_RenderBounds.h = m_Bounds.h;
}

bool Control::SizeToChildren(bool w, bool h)
{
	Magnus::Size size = ChildrenSize();
	size.w += GetPadding().right;
	size.h += GetPadding().bottom;
	return SetSize(w ? size.w : Width(), h ? size.h : Height());
}

Magnus::Size Control::ChildrenSize()
{
	Magnus::Size size;

	for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
	{
		Control* pChild = *iter;

		if (pChild->Hidden()) { continue; }

		if (!pChild->ShouldIncludeInSize()) { continue; }

		size.w = Utility::Max(size.w, pChild->Right());
		size.h = Utility::Max(size.h, pChild->Bottom());
	}

	return size;
}

void Control::SetPadding(const Padding & padding)
{
	if (m_Padding.left == padding.left &&
		m_Padding.top == padding.top &&
		m_Padding.right == padding.right &&
		m_Padding.bottom == padding.bottom) {
		return;
	}

	m_Padding = padding;
	Invalidate();
	InvalidateParent();
}

void Control::SetMargin(const Margin & margin)
{
	if (m_Margin.left == margin.left &&
		m_Margin.top == margin.top &&
		m_Margin.right == margin.right &&
		m_Margin.bottom == margin.bottom) {
		return;
	}

	m_Margin = margin;
	Invalidate();
	InvalidateParent();
}

bool Control::HandleAccelerator(Magnus::UnicodeString & accelerator)
{
	if (Magnus::KeyboardFocus == this || !AccelOnlyFocus())
	{
		AccelMap::iterator iter = m_Accelerators.find(accelerator);

		if (iter != m_Accelerators.end())
		{
			iter->second->Call(this);
			return true;
		}
	}

	for (Control::List::iterator it = Children.begin(); it != Children.end(); ++it)
	{
		if ((*it)->HandleAccelerator(accelerator))
		{
			return true;
		}
	}

	return false;
}

bool Control::OnKeyPress(int iKey, bool bPress)
{
	bool bHandled = false;

	switch (iKey)
	{
	case Key::Tab:
		bHandled = OnKeyTab(bPress);
		break;
	case Key::Backspace:
		bHandled = OnKeyBackspace(bPress);
		break;
	case Key::Return:
		bHandled = OnKeyReturn(bPress);
		break;
	case Key::Escape:
		bHandled = OnKeyEscape(bPress);
		break;
	case Key::Space:
		bHandled = OnKeySpace(bPress);
		break;
	case Key::End:
		bHandled = OnKeyEnd(bPress);
		break;
	case Key::Home:
		bHandled = OnKeyHome(bPress);
		break;
	case Key::Left:
		bHandled = OnKeyLeft(bPress);
		break;
	case Key::Up:
		bHandled = OnKeyUp(bPress);
		break;
	case Key::Right:
		bHandled = OnKeyRight(bPress);
		break;
	case Key::Down:
		bHandled = OnKeyDown(bPress);
		break;
	case Key::Delete:
		bHandled = OnKeyDelete(bPress);
		break;
	default:
		break;
	}

	if (!bHandled && GetParent())
	{
		GetParent()->OnKeyPress(iKey, bPress);
	}

	return bHandled;
}

bool Control::OnKeyRelease(int iKey)
{
	return OnKeyPress(iKey, false);
}

bool Control::OnKeyTab(bool bDown)
{
	if (!bDown) { return true; }

	if (GetCanvas()->NextTab)
	{
		GetCanvas()->NextTab->Focus();
		Redraw();
	}

	return true;
}

void Control::RenderFocus(Magnus::Skin* skin)
{
	if (Magnus::KeyboardFocus != this) { return; }

	if (!IsTabable()) { return; }

	skin->DrawKeyboardHighlight(GetRenderBounds(), 3);
}

TextObject Control::GetChildValue(const Magnus::String & strName)
{
	Control* pChild = FindChildByName(strName, true);

	if (!pChild) { return ""; }

	return pChild->GetValue();
}

TextObject Control::GetValue()
{
	// Generic value accessor should be filled in if we have a value to give.
	return "";
}

void Control::SetValue(const TextObject & strValue)
{
}

int Control::GetNamedChildren(Magnus::ControlList & list, const Magnus::String & strName, bool bDeep)
{
	int iFound = 0;

	for (Control::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
	{
		Control* pChild = *iter;

		if (!pChild->GetName().empty() && pChild->GetName() == strName)
		{
			list.Add(pChild);
			iFound++;
		}

		if (!bDeep) { continue; }

		iFound += pChild->GetNamedChildren(list, strName, bDeep);
	}

	return iFound;
}

Magnus::ControlList Control::GetNamedChildren(const Magnus::String & strName, bool bDeep)
{
	Magnus::ControlList list;
	GetNamedChildren(list, strName, bDeep);
	return list;
}