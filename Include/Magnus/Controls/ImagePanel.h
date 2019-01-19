#pragma once
#ifndef MAGNUS_CONTROLS_IMAGEPANEL_H
#define MAGNUS_CONTROLS_IMAGEPANEL_H

#include "Magnus/Magnus.h"
#include "Magnus/Controls/Control.h"
#include "Magnus/Renderer.h"
#include "Magnus/Texture.h"

namespace Magnus
{
	namespace Controls
	{
		class ImagePanel : public Controls::Control
		{
		public:

			MAGNUS_CONTROL_INLINE(ImagePanel, Controls::Control)
			{
				SetUV(0, 0, 1, 1);
				SetMouseInputEnabled(false);
				m_DrawColor = Colors::White;
				SetStretch(true);
			}

			virtual ~ImagePanel()
			{
				m_Texture.Release(GetSkin()->GetRenderer());
			}

			virtual void SetUV(float u1, float v1, float u2, float v2)
			{
				m_uv[0] = u1;
				m_uv[1] = v1;
				m_uv[2] = u2;
				m_uv[3] = v2;
			}

			virtual void SetImage(const TextObject & imageName)
			{
				m_Texture.Load(imageName, GetSkin()->GetRenderer());
			}

			virtual TextObject & GetImage()
			{
				return m_Texture.name;
			}

			virtual int TextureWidth()
			{
				return m_Texture.width;
			}

			virtual int TextureHeight()
			{
				return m_Texture.height;
			}

			virtual const TextObject & GetImageName()
			{
				return m_Texture.name;
			}

			virtual void Render(Magnus::Skin* skin)
			{
				skin->GetRenderer()->SetDrawColor(m_DrawColor);

				if (m_bStretch)
				{
					skin->GetRenderer()->DrawTexturedRect(&m_Texture, GetRenderBounds(), m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
				}
				else
				{
					skin->GetRenderer()->DrawTexturedRect(&m_Texture, Magnus::Rect(0, 0, m_Texture.width, m_Texture.height), m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
				}
			}

			virtual void SizeToContents()
			{
				SetSize(m_Texture.width, m_Texture.height);
			}

			virtual void SetDrawColor(Magnus::Color color)
			{
				m_DrawColor = color;
			}

			virtual bool FailedToLoad()
			{
				return m_Texture.FailedToLoad();
			}

			virtual bool GetStretch() { return m_bStretch; }
			virtual void SetStretch(bool b) { m_bStretch = b; }

		protected:

			Texturing::Texture	m_Texture;
			float				m_uv[4];
			Magnus::Color		m_DrawColor;

			bool				m_bStretch;
		};
	}
}

#endif