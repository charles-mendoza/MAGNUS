#include "Magnus/Magnus.h"
#include "Magnus/Renderer.h"
#include "Magnus/Utility.h"

#include <math.h>

namespace Magnus
{
	namespace Rendering
	{
		Renderer::Renderer()
		{
			m_RenderOffset = Point(0, 0);
			m_fScale = 1.0f;
			m_fOpacity = 1.0f;
		}

		Renderer::~Renderer()
		{
			if (GetCTT())
			{
				GetCTT()->ShutDown();
			}
		}

		void Renderer::SetOpacity(float fOpacity)
		{
			float percent = Utility::Clamp(fOpacity, 0.0f, 100.0f);
			m_fOpacity = Utility::PercentToDec(percent, 0.0f, 1.0f);
		}

		void Renderer::DrawLinedRect(Magnus::Rect rect)
		{
			DrawFilledRect(Magnus::Rect(rect.x, rect.y, rect.w, 1));
			DrawFilledRect(Magnus::Rect(rect.x, rect.y + rect.h - 1, rect.w, 1));
			DrawFilledRect(Magnus::Rect(rect.x, rect.y, 1, rect.h));
			DrawFilledRect(Magnus::Rect(rect.x + rect.w - 1, rect.y, 1, rect.h));
		}

		void Renderer::DrawPixel(int x, int y)
		{
			DrawFilledRect(Magnus::Rect(x, y, 1, 1));
		}

		void Renderer::DrawShavedCornerRect(Magnus::Rect rect, bool bSlight)
		{
			// Draw INSIDE the w/h.
			rect.w -= 1;
			rect.h -= 1;

			if (bSlight)
			{
				DrawFilledRect(Magnus::Rect(rect.x + 1, rect.y, rect.w - 1, 1));
				DrawFilledRect(Magnus::Rect(rect.x + 1, rect.y + rect.h, rect.w - 1, 1));
				DrawFilledRect(Magnus::Rect(rect.x, rect.y + 1, 1, rect.h - 1));
				DrawFilledRect(Magnus::Rect(rect.x + rect.w, rect.y + 1, 1, rect.h - 1));
				return;
			}

			DrawPixel(rect.x + 1, rect.y + 1);
			DrawPixel(rect.x + rect.w - 1, rect.y + 1);
			DrawPixel(rect.x + 1, rect.y + rect.h - 1);
			DrawPixel(rect.x + rect.w - 1, rect.y + rect.h - 1);
			DrawFilledRect(Magnus::Rect(rect.x + 2, rect.y, rect.w - 3, 1));
			DrawFilledRect(Magnus::Rect(rect.x + 2, rect.y + rect.h, rect.w - 3, 1));
			DrawFilledRect(Magnus::Rect(rect.x, rect.y + 2, 1, rect.h - 3));
			DrawFilledRect(Magnus::Rect(rect.x + rect.w, rect.y + 2, 1, rect.h - 3));
		}

		void Renderer::Translate(int & x, int & y)
		{
			x += m_RenderOffset.x;
			y += m_RenderOffset.y;
			x = ceilf(x * m_fScale);
			y = ceilf(y * m_fScale);
		}

		void Renderer::Translate(Magnus::Rect & rect)
		{
			Translate(rect.x, rect.y);
			rect.w = ceilf(rect.w * m_fScale);
			rect.h = ceilf(rect.h * m_fScale);
		}

		void Renderer::SetClipRegion(Magnus::Rect rect)
		{
			m_ClipRegion = rect;
		}

		void Renderer::AddClipRegion(Magnus::Rect rect)
		{
			rect.x = m_RenderOffset.x;
			rect.y = m_RenderOffset.y;
			Magnus::Rect out = rect;

			if (rect.x < m_ClipRegion.x)
			{
				out.w -= (m_ClipRegion.x - out.x);
				out.x = m_ClipRegion.x;
			}

			if (rect.y < m_ClipRegion.y)
			{
				out.h -= (m_ClipRegion.y - out.y);
				out.y = m_ClipRegion.y;
			}

			if (rect.x + rect.w > m_ClipRegion.x + m_ClipRegion.w)
			{
				out.w = (m_ClipRegion.x + m_ClipRegion.w) - out.x;
			}

			if (rect.y + rect.h > m_ClipRegion.y + m_ClipRegion.h)
			{
				out.h = (m_ClipRegion.y + m_ClipRegion.h) - out.y;
			}

			m_ClipRegion = out;
		}

		const Rect & Renderer::ClipRegion() const
		{
			return m_ClipRegion;
		}

		bool Renderer::ClipRegionVisible()
		{
			return (m_ClipRegion.w > 0 && m_ClipRegion.h > 0);
		}

		void Renderer::DrawMissingImage(Magnus::Rect rect)
		{
			SetDrawColor(Colors::Red);
			DrawFilledRect(rect);
		}

		/*
			If they haven't defined these font functions in their renderer code
			we just draw some rects where the letters would be to give them an idea.
		*/

		void Renderer::RenderText(const Magnus::TextObject & text, Magnus::Point pos, Magnus::Font* pFont)
		{
			float fSize = pFont->size * Scale();

			for (float i = 0; i < text.length(); i++)
			{
				wchar_t chr = text[i];

				if (chr == ' ') { continue; }

				Magnus::Rect r(pos.x + i * fSize * 0.4, pos.y, fSize * 0.4 - 1, fSize);

				/*
					This isn't important, it's just me messing around changing the
					shape of the rect based on the letter.. just for fun.
				*/
				if (chr == '1' || chr == 'i' || chr == '!' || chr == 't')
				{
					r.w = 1;
				}
				else if (chr >= 'a' && chr <= 'z')
				{
					r.y += fSize * 0.5f;
					r.h -= fSize * 0.4f;
				}
				else if (chr == '.' || chr == ',')
				{
					r.x += 2;
					r.y += r.h - 2;
					r.w = 2;
					r.h = 2;
				}
				else if (chr == '\'' || chr == '`' || chr == '"')
				{
					r.x += 3;
					r.w = 2;
					r.h = 2;
				}

				if (chr == 'o' || chr == 'O' || chr == '0')
				{
					DrawLinedRect(r);
				}
				else
				{
					DrawFilledRect(r);
				}
			}
		}

		Magnus::Size Renderer::MeasureText(const Magnus::TextObject & text, Magnus::Font* pFont)
		{
			Size s;
			s.w = pFont->size * Scale() * (float)text.length() * 0.4;
			s.h = pFont->size * Scale();
			return s;
		}
	}
}