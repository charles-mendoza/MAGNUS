#pragma once
#ifndef MAGNUS_CONTROLS_SCROLLBARTHUMB_H
#define MAGNUS_CONTROLS_SCROLLBARTHUMB_H

#include "Magnus/Controls/Dragger.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace ControlsInternal
	{
		class ScrollThumb : public ControlsInternal::Dragger
		{
		public:

			MAGNUS_CONTROL(ScrollThumb, ControlsInternal::Dragger);

			virtual void Render(Magnus::Skin* skin);
			virtual void Layout(Magnus::Skin* skin);

			virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY);
			virtual void OnMouseClickLeft(int x, int y, bool bDown);

			virtual void MoveTo(int x, int y);

			virtual void SetHorizontal() { m_bHorizontal = true; }
			virtual void SetVertical() { m_bHorizontal = false; }
			virtual bool IsVertical() { return !m_bHorizontal; }
			virtual bool IsHorizontal() { return m_bHorizontal; }
			virtual bool IsDepressed() { return m_bDepressed; }

		protected:

			bool m_bHorizontal;
		};
	}
}

#endif