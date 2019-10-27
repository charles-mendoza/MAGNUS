#pragma once
#ifndef MAGNUS_RENDERING_DIRECTX9_H
#define MAGNUS_RENDERING_DIRECTX9_H

#include "Magnus/Magnus.h"
#include "Magnus/Renderer.h"

#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace Magnus
{
	namespace Rendering
	{
		class DirectX9 : public Renderer
		{
		public:

			DirectX9(IDirect3DDevice9* pDevice = NULL);
			~DirectX9();

			virtual void Begin();
			virtual void End();
			virtual void Release();

			virtual void SetDrawColor(Magnus::Color color);

			virtual void DrawFilledRect(Magnus::Rect rect);

			virtual void LoadFont(Magnus::Font* pFont);
			virtual void FreeFont(Magnus::Font* pFont);
			virtual void RenderText(const Magnus::TextObject & text, Magnus::Point pos, Magnus::Font* pFont);
			virtual Magnus::Size MeasureText(const Magnus::TextObject & text, Magnus::Font* pFont);

			void StartClip();
			void EndClip();

			void DrawTexturedRect(Texturing::Texture* pTexture, Magnus::Rect rect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);
			void LoadTexture(Texturing::Texture* pTexture);
			void LoadTextureInMemory(Texturing::Texture* pTexture, void* pSrcData, unsigned int size);
			void FreeTexture(Texturing::Texture* pTexture);
			Magnus::Color PixelColor(Texturing::Texture* pTexture, unsigned int x, unsigned int y, const Magnus::Color & defaultColor);

			// Efficient and faster implementation of ID3DXLine::Draw.
			void Draw(Magnus::Vector2* pVertexList, float fWidth, int iVertexCount, bool bClosedShape = false);
			void DrawLine(Magnus::Vector2 p1, Magnus::Vector2 p2, float fWidth);
			void DrawCircle(Magnus::Vector2 pCenter, float fRadius, float fWidth, int iSides = 360, int iSpacing = 0);
			void DrawFilledCircle(Magnus::Vector2 pCenter, float fRadius, int iSides = 360);

			Magnus::Rect GetClientBounds();

			void ResetDevice();

		protected:

			void*				m_pCurrentTexture;
			IDirect3DDevice9*	m_pDevice;
			DWORD				m_Color;
			Font::List			m_FontList;

			IDirect3DPixelShader9* m_pShader;
			DWORD m_CullMode;
			DWORD m_ZEnable;
			DWORD m_AlphaBlend;
			DWORD m_SrcBlend;
			DWORD m_DestBlend;
			DWORD m_MagFilter;
			DWORD m_MinFilter;
			DWORD m_MipFilter;
			DWORD m_Addressu;
			DWORD m_Addressv;
			DWORD m_ColorOp;
			DWORD m_ColorArg1;
			DWORD m_ColorArg2;
			DWORD m_AlphaOp;
			DWORD m_AlphaArg1;
			DWORD m_AlphaArg2;
			DWORD m_AlphaOp1;
			DWORD m_ColorOp1;
			DWORD m_FVF;

			void Flush();
			void AddVert(int x, int y);
			void AddVert(int x, int y, float u, float v);

		protected:

			struct Vertex
			{
				float x, y, z, rhw;
				DWORD color;
				float u, v;
			};

			static const int	MaxVerts = 1024;
			int					m_iVertNum;
			Vertex				m_pVerts[MaxVerts];
			Vertex				m_pLineVerts[MaxVerts];
		};
	}
}

#endif