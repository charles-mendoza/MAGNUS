#pragma once
#ifndef MAGNUS_TEXTURING_H
#define MAGNUS_TEXTURING_H

#include "Magnus/Renderer.h"
#include "Magnus/TextObject.h"

namespace Magnus
{
	namespace Texturing
	{
		struct Texture
		{
			typedef std::list<Texture*> List;

			TextObject	name;
			void*		data;
			bool		failed;
			int			width;
			int			height;

			Texture()
			{
				data = NULL;
				width = 4;
				height = 4;
				failed = false;
			}

			~Texture()
			{
			}

			void Load(const TextObject & str, Rendering::Renderer* pRenderer)
			{
				name = str;
				if (!pRenderer) { return; }
				pRenderer->LoadTexture(this);
			}

			void LoadInMemory(void* pSrcData, unsigned int size, Rendering::Renderer* pRenderer)
			{
				if (!pRenderer) { return; }
				pRenderer->LoadTextureInMemory(this, pSrcData, size);
			}

			void Release(Rendering::Renderer* pRenderer)
			{
				pRenderer->FreeTexture(this);
			}

			bool FailedToLoad() const
			{
				return failed;
			}
		};

		struct Single
		{
			Single()
			{
				texture = NULL;
			}

			void Init(Texture* pTexture, int x, int y, int w, int h)
			{
				texture = pTexture;
				float texw = (float)texture->width;
				float texh = (float)texture->height;
				uv[0] = x / texw;
				uv[1] = y / texh;
				uv[2] = (x + w) / texw;
				uv[3] = (y + h) / texh;
				iWidth = w;
				iHeight = h;
			}

			void Draw(Magnus::Rect rect, Rendering::Renderer* pRenderer, const Magnus::Color & color = Magnus::Color(255, 255, 255, 255))
			{
				if (!texture) { return; }

				pRenderer->SetDrawColor(color);
				pRenderer->DrawTexturedRect(texture, rect, uv[0], uv[1], uv[2], uv[3]);
			}

			void DrawCenter(Magnus::Rect rect, Rendering::Renderer* pRenderer, const Magnus::Color & color = Magnus::Color(255, 255, 255, 255))
			{
				if (!texture) { return; }

				rect.x += (int)((rect.w - iWidth) * 0.5);
				rect.y += (int)((rect.h - iHeight) * 0.5);
				rect.w = iWidth;
				rect.h = iHeight;
				Draw(rect, pRenderer, color);
			}

			Texture*	texture;
			float		uv[4];
			int			iWidth;
			int			iHeight;
		};

		struct Bordered
		{
			Bordered()
			{
				texture = NULL;
			}

			void Init(Texture* pTexture, int x, int y, int w, int h, Margin m, float mScale = 1.0f)
			{
				texture = pTexture;
				margin = m;
				SetRect(0, x, y, margin.left, margin.top);
				SetRect(1, x + margin.left, y, w - margin.left - margin.right, margin.top);
				SetRect(2, (x + w) - margin.right, y, margin.right, margin.top);
				SetRect(3, x, y + margin.top, margin.left, h - margin.top - margin.bottom);
				SetRect(4, x + margin.left, y + margin.top, w - margin.left - margin.right, h - margin.top - margin.bottom);
				SetRect(5, (x + w) - margin.right, y + margin.top, margin.right, h - margin.top - margin.bottom);
				SetRect(6, x, (y + h) - margin.bottom, margin.left, margin.bottom);
				SetRect(7, x + margin.left, (y + h) - margin.bottom, w - margin.left - margin.right, margin.bottom);
				SetRect(8, (x + w) - margin.right, (y + h) - margin.bottom, margin.right, margin.bottom);
				margin.left = (int)(margin.left * mScale);
				margin.top = (int)(margin.top * mScale);
				margin.right = (int)(margin.right * mScale);
				margin.bottom = (int)(margin.bottom * mScale);
				width = (float)(w - x);
				height = (float)(h - y);
			}

			void SetRect(int iNum, int x, int y, int w, int h)
			{
				float texw = (float)texture->width;
				float texh = (float)texture->height;
				rects[iNum].uv[0] = x / texw;
				rects[iNum].uv[1] = y / texh;
				rects[iNum].uv[2] = (x + w) / texw;
				rects[iNum].uv[3] = (y + h) / texh;
			}

			void Draw(Magnus::Rect rect, Rendering::Renderer* pRenderer, const Magnus::Color & color = Magnus::Color(255, 255, 255, 255), bool b1 = true, bool b2 = true, bool b3 = true, bool b4 = true, bool b5 = true, bool b6 = true, bool b7 = true, bool b8 = true, bool b9 = true)
			{
				if (!texture) { return; }

				pRenderer->SetDrawColor(color);

				if (rect.w < width && rect.h < height)
				{
					pRenderer->DrawTexturedRect(texture, rect, rects[0].uv[0], rects[0].uv[1], rects[8].uv[2], rects[8].uv[3]);
					return;
				}

				if (b1) { DrawRect(0, rect.x, rect.y, margin.left, margin.top, pRenderer); }

				if (b2) { DrawRect(1, rect.x + margin.left, rect.y, rect.w - margin.left - margin.right, margin.top, pRenderer); }

				if (b3) { DrawRect(2, (rect.x + rect.w) - margin.right, rect.y, margin.right, margin.top, pRenderer); }

				if (b4) { DrawRect(3, rect.x, rect.y + margin.top, margin.left, rect.h - margin.top - margin.bottom, pRenderer); }

				if (b5) { DrawRect(4, rect.x + margin.left, rect.y + margin.top, rect.w - margin.left - margin.right, rect.h - margin.top - margin.bottom, pRenderer); }

				if (b6) { DrawRect(5, (rect.x + rect.w) - margin.right, rect.y + margin.top, margin.right, rect.h - margin.top - margin.bottom, pRenderer); }

				if (b7) { DrawRect(6, rect.x, (rect.y + rect.h) - margin.bottom, margin.left, margin.bottom, pRenderer); }

				if (b8) { DrawRect(7, rect.x + margin.left, (rect.y + rect.h) - margin.bottom, rect.w - margin.left - margin.right, margin.bottom, pRenderer); }

				if (b9) { DrawRect(8, (rect.x + rect.w) - margin.right, (rect.y + rect.h) - margin.bottom, margin.right, margin.bottom, pRenderer); }
			}

			void DrawRect(int i, int x, int y, int w, int h, Rendering::Renderer* pRenderer)
			{
				pRenderer->DrawTexturedRect(texture, Magnus::Rect(x, y, w, h), rects[i].uv[0], rects[i].uv[1], rects[i].uv[2], rects[i].uv[3]);
			}

			Texture* texture;

			struct SubRect
			{
				float uv[4];
			};

			SubRect rects[9];
			Margin margin;

			float width;
			float height;
		};
	}
}

#endif