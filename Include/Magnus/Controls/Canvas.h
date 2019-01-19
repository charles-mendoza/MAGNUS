#pragma once
#ifndef MAGNUS_CONTROLS_CANVAS_H
#define MAGNUS_CONTROLS_CANVAS_H

#include <set>

#include "Magnus/Controls/Control.h"
#include "Magnus/InputHandler.h"

namespace Magnus
{
	namespace Controls
	{
		class Canvas : public Control
		{
		public:

			typedef Controls::Control BaseClass;

			Canvas(Magnus::Skin* pSkin);
			virtual ~Canvas();

			//
			// For additional initialization
			// (which is sometimes no appropriate in the constructor
			//
			virtual void Initialize() {}

			//
			// Call this when the window bounds have changed
			//
			virtual void UpdateBounds() { m_bNeedsUpdateBounds = true; }

			//
			// You should call this to render your canvas
			//
			virtual void RenderCanvas();

			//
			// Call this whenever you want to process input. This
			// is usually once a frame..
			//
			virtual void DoThink();

			//
			// In most situations you will be rendering the canvas
			// every frame. Butin some situations you will only want
			// to render when there have been changes. You can do this
			// by checking NeedsRedraw().
			//
			virtual bool NeedsRedraw() { return m_bNeedsRedraw; }
			virtual void Redraw() { m_bNeedsRedraw = true; }

			// Internal. Do not call directly.
			virtual void Render(Magnus::Skin* skin);

			// Child controls call parent->GetCanvas()
			// until they get to this top level function.
			virtual Canvas* GetCanvas() { return this; }

			virtual void SetScale(float fScale);
			virtual float Scale() const { return m_fScale; }

			virtual void OnBoundsChanged(Magnus::Rect oldBounds);

			// Delete all children (this is done called in the destructor too)
			virtual void ReleaseChildren();

			// Delayted deletes
			virtual void AddDelayedDelete(Controls::Control* pControl);
			virtual void ProcessDelayedDeletes();

			Controls::Control* FirstTab;
			Controls::Control* NextTab;

			// Input
			virtual bool InputMouseMoved(int x, int y, int deltaX, int deltaY);
			virtual bool InputMouseButton(int iButton, bool bDown);
			virtual bool InputKey(int iKey, bool bDown);
			virtual bool InputCharacter(Magnus::UnicodeChar chr);
			virtual bool InputMouseWheel(int val);
			virtual bool InputQuit() { return true; }

			// Background
			virtual void SetBackgroundColor(const Magnus::Color & color) { m_BackgroundColor = color; }
			virtual void SetDrawBackground(bool bShouldDraw) { m_bDrawBackground = bShouldDraw; }

		protected:

			bool m_bNeedsUpdateBounds;
			bool m_bNeedsRedraw;
			bool m_bAnyDelete;
			float m_fScale;

			Controls::Control::List m_DeleteList;
			std::set<Controls::Control*> m_DeleteSet;
			friend class Controls::Control;
			void PreDeleteCanvas(Controls::Control*);

			bool			m_bDrwaBackground;
			Magnus::Color	m_BackgroundColor;
		};
	}
}

#endif