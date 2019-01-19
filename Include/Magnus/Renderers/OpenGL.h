#pragma once
#ifndef MAGNUS_RENDERING_OPENGL_H
#define MAGNUS_RENDERING_OPENGL_H

#include "Magnus/Magnus.h"
#include "Magnus/Renderer.h"

namespace Magnus
{
	namespace Rendering
	{
		class OpenGL : public Renderer
		{
		public:

			OpenGL();
			~OpenGL();

			virtual void Init();

			virtual void Begin();
			virtual void End();

			virtual void SetDrawColor(Magnus::Color color);
			virtual void DrawFilledRect(Magnus::Rect rect);

			void StartClip();
			void EndClip();

			void DrawTexturedRect(Texturing::Texture* pTexture, Magnus::Rect rect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);
			void LoadTexture(Texturing::Texture* pTexture);
			void LoadTextureInMemory(Texturing::Texture* pTexture, void* pSrcData, unsigned int size);
			void FreeTexture(Texturing::Texture* pTexture);
			Magnus::Color PixelColor(Texturing::Texture* pTexture, unsigned int x, unsigned int y, const Magnus::Color & defaultColor);

			void RenderText(const Magnus::TextObject & text, Magnus::Point pos, Magnus::Font* pFont);
			Magnus::Size MeasureText(const Magnus::TextObject & text, Magnus::Font* pFont);

			//void Draw(Magnus::Vector2* pVertexList, float fWidth, int iVertexCount, bool bClosedShape = false);
			//void DrawLine(Magnus::Vector2 p1, Magnus::Vector2 p2, float fWidth);
			//void DrawCircle(Magnus::Vector2 pCenter, float fRadius, int iSides, float fWidth, int iSpacing = 0);

		protected:

			Magnus::Color	m_Color;

			void Flush();
			void AddVert(int x, int y, float u = 0.0f, float v = 0.0f);

		protected:

			void BuildFont();
			void DestroyFont();

			Texturing::Texture* m_pFontTexture;
			float				m_fFontScale[2];
			float				m_fLetterSpacing;

		protected:

			struct Vertex
			{
				float x, y, z;
				float u, v;
				unsigned char r, g, b, a;
			};

			static const int	MaxVerts = 1024;
			int					m_iVertNum;
			Vertex				m_Vertices[MaxVerts];
		};
	}
}
#endif
