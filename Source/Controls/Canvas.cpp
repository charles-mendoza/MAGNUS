#include "Magnus/Magnus.h"
#include "Magnus/Controls/Canvas.h"
#include "Magnus/Skin.h"

using namespace Magnus;
using namespace Magnus::Controls;

Canvas::Canvas(Magnus::Skin* pSkin) : BaseClass(NULL), m_bAnyDelete(false)
{
	SetBounds(0, 0, 10000, 10000);
	SetScale(1.0f);
	SetBackgroundColor(Color(255, 255, 255, 255));
	SetDrawBackground(false);
	m_bNeedsUpdateBounds = true;

	if (pSkin) { SetSkin(pSkin); }
}

Canvas::~Canvas()
{
	ReleaseChildren();
}

void Canvas::RenderCanvas()
{
	if (m_bNeedsUpdateBounds)
	{
		SetBounds(m_Skin->GetRenderer()->GetClientBounds());
	}

	DoThink();
	Magnus::Rendering::Renderer* render = m_Skin->GetRenderer();
	//render->Begin();
	RecurseLayout(m_Skin);
	render->SetClipRegion(GetBounds());
	render->SetRenderOffset(Magnus::Point(0, 0));
	render->SetScale(Scale());
	render->SetOpacity(Opacity());

	if (m_bDrawBackground)
	{
		render->SetDrawColor(m_BackgroundColor);
		render->DrawFilledRect(GetRenderBounds());
	}

	DoRender(m_Skin);
	render->SetClipRegion(GetBounds());
	render->SetRenderOffset(Magnus::Point(0, 0));
	render->SetScale(Scale());
	render->SetOpacity(100.0f);
	//render->End();
}

void Canvas::Render(Magnus::Skin* /*skin*/)
{
	m_bNeedsRedraw = false;
}

void Canvas::OnBoundsChanged(Magnus::Rect oldBounds)
{
	BaseClass::OnBoundsChanged(oldBounds);
	InvalidateChildren(true);
	m_bNeedsUpdateBounds = false;
}

void Canvas::DoThink()
{
	ProcessDelayedDeletes();

	if (Hidden()) { return; }

	// Reset tabbing
	{
		NextTab = NULL;
		FirstTab = NULL;
	}
	ProcessDelayedDeletes();
	// Check has focus etc..
	RecurseLayout(m_Skin);

	// If we didn't have a next tab, cycle to the start.
	if (NextTab == NULL)
	{
		NextTab = FirstTab;
	}

	Magnus::Input::OnCanvasThink(this);
}

void Canvas::SetScale(float fScale)
{
	if (m_fScale == fScale) { return; }

	m_fScale = fScale;

	if (m_Skin && m_Skin->GetRenderer())
	{
		m_Skin->GetRenderer()->SetScale(m_fScale);
	}

	OnScaleChanged();
	Redraw();
}

void Canvas::AddDelayedDelete(Controls::Control* pControl)
{
	if (!m_bAnyDelete || m_DeleteSet.find(pControl) == m_DeleteSet.end())
	{
		m_bAnyDelete = true;
		m_DeleteSet.insert(pControl);
		m_DeleteList.push_back(pControl);
	}
}

void Canvas::PreDeleteCanvas(Controls::Control* pControl)
{
	if (m_bAnyDelete)
	{
		std::set<Controls::Control*>::iterator itFind;

		if ((itFind = m_DeleteSet.find(pControl)) != m_DeleteSet.end())
		{
			m_DeleteList.remove(pControl);
			m_DeleteSet.erase(pControl);
			m_bAnyDelete = !m_DeleteSet.empty();
		}
	}
}

void Canvas::ProcessDelayedDeletes()
{
	while (m_bAnyDelete)
	{
		m_bAnyDelete = false;
		Controls::Control::List deleteList = m_DeleteList;
		m_DeleteList.clear();
		m_DeleteSet.clear();

		for (Magnus::Controls::Control::List::iterator it = deleteList.begin(); it != deleteList.end(); ++it)
		{
			Magnus::Controls::Control* pControl = *it;
			pControl->PreDelete(GetSkin());
			delete pControl;
			Redraw();
		}
	}
}

void Canvas::ReleaseChildren()
{
	Control::List::iterator iter = Children.begin();

	while (iter != Children.end())
	{
		Control* pChild = *iter;
		iter = Children.erase(iter);
		delete pChild;
	}
}

bool Canvas::InputMouseMoved(int x, int y, int deltaX, int deltaY)
{
	if (Hidden()) { return false; }

	// Todo: Handle scaling here..
	//float fScale = 1.0f / Scale();
	Magnus::Input::OnMouseMoved(this, x, y, deltaX, deltaY);

	if (!Magnus::HoveredControl) { return false; }

	if (Magnus::HoveredControl == this) { return false; }

	if (Magnus::HoveredControl->GetCanvas() != this) { return false; }

	Magnus::HoveredControl->OnMouseMoved(x, y, deltaX, deltaY);

	return true;
}

bool Canvas::InputMouseButton(int iButton, bool bDown)
{
	if (Hidden()) { return false; }

	return Magnus::Input::OnMouseClicked(this, iButton, bDown);
}

bool Canvas::InputKey(int iKey, bool bDown)
{
	if (Hidden()) { return false; }

	return Magnus::Input::OnKeyEvent(this, iKey, bDown);
}

bool Canvas::InputCharacter(Magnus::UnicodeChar chr)
{
	if (Hidden()) { return false; }

	return Magnus::Input::OnKeyCharEvent(this, chr);
}

bool Canvas::InputMouseWheel(int val)
{
	if (Hidden()) { return false; }

	return Magnus::Input::OnMouseWheeled(this, val);
}