#pragma once
#ifndef MAGNUS_CONTROLS_DRAGGER_H
#define MAGNUS_CONTROLS_DRAGGER_H

#include "Magnus/Controls/Control.h"
#include "Magnus/Magnus.h"
#include "Magnus/Skin.h"

namespace Magnus
{
	namespace ControlsInternal
	{
		class Dragger : public Controls::Control
		{
		public:

			MAGNUS_CONTROL(Dragger, Controls::Control);

			virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY);

			virtual void OnMouseClickLeft(int x, int y, bool bDown);
			virtual void Render(Magnus::Skin* skin);

			virtual void SetTarget(Controls::Control* pControl) { m_pTarget = pControl; }
			virtual bool IsDepressed() { return m_bDepressed; }

			virtual void SetDoMove(bool b) { m_bDoMove = b; }

			virtual void OnMouseDoubleClickLeft(int x, int y);

			Magnus::Event::Caller onDragged;
			Magnus::Event::Caller onDragStart;

			Magnus::Event::Caller onDoubleClickLeft;

		protected:

			bool				m_bDepressed;
			Magnus::Point		m_HoldPos;
			Controls::Control*	m_pTarget;
			bool				m_bDoMove;
		};
	}
}

#endif