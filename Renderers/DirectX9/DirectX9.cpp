#include "Magnus/Renderers/DirectX9.h"
#include "Magnus/Utility.h"
#include "Magnus/Font.h"
#include "Magnus/Texture.h"

#include <d3dx9core.h>

#define MITER_LIMIT 0.75f

#define D3DFVF_VERTEXFORMAT2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

struct FontData
{
	ID3DXFont*	pFont;
	int			iSpaceWidth;
};

namespace Magnus
{
	namespace Rendering
	{
		DirectX9::DirectX9(IDirect3DDevice9* pDevice)
		{
			m_pDevice = pDevice;
			m_iVertNum = 0;

			for (int i = 0; i < MaxVerts; i++)
			{
				m_pVerts[i].z = 0.5f;
				m_pVerts[i].rhw = 1.0f;
			}
		}

		DirectX9::~DirectX9()
		{
		}

		void DirectX9::Begin()
		{
			m_pDevice->GetTexture(0, (IDirect3DBaseTexture9**)&m_pCurrentTexture);
			m_pDevice->GetPixelShader(&m_pShader);
			m_pDevice->GetRenderState(D3DRS_CULLMODE, &m_CullMode);
			m_pDevice->GetRenderState(D3DRS_ZENABLE, &m_ZEnable);
			m_pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &m_AlphaBlend);
			m_pDevice->GetRenderState(D3DRS_SRCBLEND, &m_SrcBlend);
			m_pDevice->GetRenderState(D3DRS_DESTBLEND, &m_DestBlend);
			m_pDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &m_MagFilter);
			m_pDevice->GetSamplerState(0, D3DSAMP_MINFILTER, &m_MinFilter);
			m_pDevice->GetSamplerState(0, D3DSAMP_MIPFILTER, &m_MipFilter);
			m_pDevice->GetSamplerState(0, D3DSAMP_ADDRESSU, &m_Addressu);
			m_pDevice->GetSamplerState(0, D3DSAMP_ADDRESSV, &m_Addressv);
			m_pDevice->GetTextureStageState(0, D3DTSS_COLOROP, &m_ColorOp);
			m_pDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &m_ColorArg1);
			m_pDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &m_ColorArg2);
			m_pDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &m_AlphaOp);
			m_pDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &m_AlphaArg1);
			m_pDevice->GetTextureStageState(0, D3DTSS_ALPHAARG2, &m_AlphaArg2);
			m_pDevice->GetTextureStageState(1, D3DTSS_ALPHAOP, &m_AlphaOp1);
			m_pDevice->GetTextureStageState(1, D3DTSS_COLOROP, &m_ColorOp1);
			m_pDevice->GetFVF(&m_FVF);

			m_pDevice->SetPixelShader(NULL);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
			m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			m_pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			m_pDevice->SetFVF(D3DFVF_VERTEXFORMAT2D);
		}

		void DirectX9::End()
		{
			Flush();
			m_pDevice->SetTexture(0, NULL);
			m_pCurrentTexture = NULL;

			m_pDevice->SetPixelShader(m_pShader);
			m_pDevice->SetRenderState(D3DRS_CULLMODE, m_CullMode);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, m_ZEnable);
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, m_AlphaBlend);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, m_SrcBlend);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, m_DestBlend);
			m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, m_MagFilter);
			m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, m_MinFilter);
			m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, m_MipFilter);
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, m_Addressu);
			m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, m_Addressv);
			m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP, m_ColorOp);
			m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, m_ColorArg1);
			m_pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, m_ColorArg2);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, m_AlphaOp);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, m_AlphaArg1);
			m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, m_AlphaArg2);
			m_pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, m_AlphaOp1);
			m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, m_ColorOp1);
			m_pDevice->SetFVF(m_FVF);
		}

		void DirectX9::Flush()
		{
			if (m_iVertNum > 0)
			{
				m_pDevice->SetFVF(D3DFVF_VERTEXFORMAT2D);
				m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_iVertNum / 3, &m_pVerts[0], sizeof(Vertex));
				m_iVertNum = 0;
			}
		}

		void DirectX9::AddVert(int x, int y)
		{
			if (m_iVertNum >= MaxVerts - 1)
			{
				Flush();
			}

			m_pVerts[m_iVertNum].x = (float)x;
			m_pVerts[m_iVertNum].y = (float)y;
			m_pVerts[m_iVertNum].color = m_Color;
			m_iVertNum++;
		}

		void DirectX9::AddVert(int x, int y, float u, float v)
		{
			if (m_iVertNum >= MaxVerts - 1)
			{
				Flush();
			}

			m_pVerts[m_iVertNum].x = -0.5f + (float)x;
			m_pVerts[m_iVertNum].y = -0.5f + (float)y;
			m_pVerts[m_iVertNum].u = u;
			m_pVerts[m_iVertNum].v = v;
			m_pVerts[m_iVertNum].color = m_Color;
			m_iVertNum++;
		}

		void DirectX9::DrawFilledRect(Magnus::Rect rect)
		{
			if (m_pCurrentTexture != NULL)
			{
				Flush();
				m_pDevice->SetTexture(0, NULL);
				m_pCurrentTexture = NULL;
			}

			Translate(rect);
			AddVert(rect.x, rect.y);
			AddVert(rect.x + rect.w, rect.y);
			AddVert(rect.x, rect.y + rect.h);
			AddVert(rect.x + rect.w, rect.y);
			AddVert(rect.x + rect.w, rect.y + rect.h);
			AddVert(rect.x, rect.y + rect.h);
		}

		Magnus::Rect DirectX9::GetClientBounds()
		{
			D3DVIEWPORT9 viewport;
			m_pDevice->GetViewport(&viewport);
			return Magnus::Rect(viewport.X, viewport.Y, viewport.Width, viewport.Height);
		}

		void DirectX9::Draw(Magnus::Vector2* pVertexList, float fWidth, int iVertexCount, bool bClosedShape)
		{
			if (fWidth < 1.0f) { return; }

			if (iVertexCount < 2) { return; }

			Flush();

			if (m_pCurrentTexture != NULL)
			{
				m_pDevice->SetTexture(0, NULL);
				m_pCurrentTexture = NULL;
			}

			/*for (int i = 0; i < iVertexCount; i++)
			{
				pVertexList[i].x += m_RenderOffset.x;
				pVertexList[i].y += m_RenderOffset.y;
				pVertexList[i].x = pVertexList[i].x * m_fScale;
				pVertexList[i].y = pVertexList[i].y * m_fScale;
			}*/

			int iCurVert = 0;

			if (fWidth == 1.0f)
			{
				for (; iCurVert < iVertexCount; iCurVert++)
					m_pLineVerts[iCurVert] = { pVertexList[iCurVert].x, pVertexList[iCurVert].y, 1.0f, 1.0f, m_Color };

				if (bClosedShape)
					m_pLineVerts[iCurVert++] = m_pLineVerts[0];

				m_pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, iCurVert - 1, &m_pLineVerts[0], sizeof(Vertex));

				return;
			}

			float length;
			Magnus::Vector2 a, b;
			Magnus::Vector2 p0, p1, p2;
			Magnus::Vector2 v0, v1, n0, n1, miter;

			p0 = pVertexList[bClosedShape ? iVertexCount - 1 : 0];

			// Actual width in pixels
			fWidth *= 0.5f;
			
			for (int i = 0; i < iVertexCount; i++)
			{
				p1 = pVertexList[i];
				p2 = pVertexList[i != iVertexCount - 1 ? i + 1 : bClosedShape ? 0 : i];

				v0 = (p1 - p0).Normalized();
				v1 = (p2 - p1).Normalized();

				n0 = Magnus::Vector2(-v0.y, v0.x);
				n1 = Magnus::Vector2(-v1.y, v1.x);

				miter = (n0 + n1).Normalized();

				length = fWidth / n1.Dot(miter);

				if (v0.Dot(v1) < -MITER_LIMIT)
				{
					a = p1 - n0 * fWidth;
					b = p1 + n0 * fWidth;
					m_pLineVerts[iCurVert++] = { a.x, a.y, 1.0f, 1.0f, m_Color };
					m_pLineVerts[iCurVert++] = { b.x, b.y, 1.0f, 1.0f, m_Color };
					//m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, iCurVert - 2, &m_LineVerts, sizeof(VertexFormat));
					//iCurVert = 0;
					a = p1 - n1 * fWidth;
					b = p1 + n1 * fWidth;
					m_pLineVerts[iCurVert++] = { a.x, a.y, 1.0f, 1.0f, m_Color };
					m_pLineVerts[iCurVert++] = { b.x, b.y, 1.0f, 1.0f, m_Color };
				}
				else
				{
					a = p1 - miter * length;
					b = p1 + miter * length;
					m_pLineVerts[iCurVert++] = { a.x, a.y, 1.0f, 1.0f, m_Color };
					m_pLineVerts[iCurVert++] = { b.x, b.y, 1.0f, 1.0f, m_Color };
				}

				if (!bClosedShape && i == iVertexCount - 2)
				{
					a = p2 - n1 * fWidth;
					b = p2 + n1 * fWidth;
					m_pLineVerts[iCurVert++] = { a.x, a.y, 1.0f, 1.0f, m_Color };
					m_pLineVerts[iCurVert++] = { b.x, b.y, 1.0f, 1.0f, m_Color };
					break;
				}

				p0 = p1;
			}

			if (bClosedShape)
			{
				if (v0.Dot(v1) < -MITER_LIMIT)
				{
					a = p1 - n1 * fWidth;
					b = p1 + n1 * fWidth;
				}

				m_pLineVerts[iCurVert++] = { a.x, a.y, 1.0f, 1.0f, m_Color };
				m_pLineVerts[iCurVert++] = { b.x, b.y, 1.0f, 1.0f, m_Color };

				p0 = p1;
				p1 = p2;
				p2 = pVertexList[1];

				v0 = (p1 - p0).Normalized();
				v1 = (p2 - p1).Normalized();

				n0 = Magnus::Vector2(-v0.y, v0.x);
				n1 = Magnus::Vector2(-v1.y, v1.x);

				miter = (n0 + n1).Normalized();

				length = fWidth / n1.Dot(miter);

				if (v0.Dot(v1) < -MITER_LIMIT)
				{
					a = p1 - n0 * fWidth;
					b = p1 + n0 * fWidth;
				}
				else
				{
					a = p1 - miter * length;
					b = p1 + miter * length;
				}
				
				m_pLineVerts[iCurVert++] = { a.x, a.y, 1.0f, 1.0f, m_Color };
				m_pLineVerts[iCurVert++] = { b.x, b.y, 1.0f, 1.0f, m_Color };
			}

			m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, iCurVert - 2, &m_pLineVerts[0], sizeof(Vertex));
		}

		void DirectX9::DrawLine(Magnus::Vector2 p1, Magnus::Vector2 p2, float fWidth)
		{
			Magnus::Vector2 line[2] = { { p1.x, p1.y }, { p2.x, p2.y } };
			Draw(line, fWidth, 2, false);
		}

		void DirectX9::DrawCircle(Magnus::Vector2 pCenter, float fRadius, int iSides, float fWidth, int iSpacing)
		{
			if (iSpacing == iSides) { return; }

			if (fWidth < 1.0f) { return; }

			iSides = Utility::Clamp(iSides, 2, 360);

			float angle = D3DX_PI * 2 / iSides;
			float _cos = cos(angle);
			float _sin = sin(angle);
			float x1 = fRadius, y1 = 0, x2, y2;
			int skip = 0;

			Magnus::Vector2 circle[360];
			
			for (int i = 0; i < iSides; i++)
			{
				x2 = _cos * x1 - _sin * y1 + pCenter.x;
				y2 = _sin * x1 + _cos * y1 + pCenter.y;
				x1 += pCenter.x;
				y1 += pCenter.y;

				if (!skip)
				{
					if (iSpacing > 0)
						DrawLine(Magnus::Vector2(x1, y1), Magnus::Vector2(x2, y2), fWidth);
					else
						circle[i] = { x1, y1 };
				}

				if (iSpacing > 0)
					skip = skip != iSpacing - 1 ? skip + 1 : 0;

				x1 = x2 - pCenter.x;
				y1 = y2 - pCenter.y;
			}
			
			if (iSpacing <= 0)
				Draw(circle, fWidth, iSides, true);
		}

		void DirectX9::SetDrawColor(Color color)
		{
			color.a *= Opacity();
			m_Color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
		}

		void DirectX9::LoadFont(Font* pFont)
		{
			m_FontList.push_back(pFont);
			// Scale the font according to canvas
			pFont->realsize = pFont->size * Scale();
			D3DXFONT_DESC fd;
			ZeroMemory(&fd, sizeof(fd));
			wcscpy_s(fd.FaceName, LF_FACESIZE, pFont->facename.c_str());
			fd.Width = 0;
			fd.MipLevels = 1;
			fd.CharSet = DEFAULT_CHARSET;
			fd.Height = pFont->realsize * -1.0f;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.Italic = 0;
			fd.Weight = pFont->bold ? FW_BOLD : FW_NORMAL;
#ifdef CLEARTYPE_QUALITY
			fd.Quality = pFont->realsize < 14.0f ? DEFAULT_QUALITY : CLEARTYPE_QUALITY;
#else
			fd.Quality = PROOF_QUALITY;
#endif
			fd.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
			LPD3DXFONT pD3DXFont;
			HRESULT hr = D3DXCreateFontIndirect(m_pDevice, &fd, &pD3DXFont);
			FontData* pFontData = new FontData();
			pFontData->pFont = pD3DXFont;
			// ID3DXFont doesn't measure trailing spaces, so we measure the width of a space here and store it
			// in the font data - then we can add it to the width when we measure text with trailing spaces.
			{
				RECT rctA = { 0 };
				pFontData->pFont->DrawText(NULL, L"A", -1, &rctA, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE, 0);
				RECT rctSpc = { 0 };
				pFontData->pFont->DrawText(NULL, L"A A", -1, &rctSpc, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE, 0);
				pFontData->iSpaceWidth = rctSpc.right - rctA.right * 2;
			}
			pFont->data = pFontData;
		}

		void DirectX9::FreeFont(Font* pFont)
		{
			m_FontList.remove(pFont);

			if (!pFont->data) { return; }

			FontData* pFontData = (FontData*)pFont->data;

			if (pFontData->pFont)
			{
				pFontData->pFont->Release();
				pFontData->pFont = NULL;
			}

			delete pFontData;
			pFont->data = NULL;
		}

		void DirectX9::RenderText(const Magnus::TextObject & text, Magnus::Point pos, Magnus::Font* pFont)
		{
			Flush();

			// If the font doesn't exist, or the font size should be changed
			if (!pFont->data || fabs(pFont->realsize - pFont->size * Scale()) > 2)
			{
				FreeFont(pFont);
				LoadFont(pFont);
			}

			FontData* pFontData = (FontData*)pFont->data;
			Translate(pos.x, pos.y);
			RECT ClipRect = { pos.x, pos.y, 0, 0 };
			pFontData->pFont->DrawTextW(NULL, text.c_str(), -1, &ClipRect, DT_LEFT | DT_TOP | DT_NOCLIP | DT_SINGLELINE, m_Color);
		}

		Size DirectX9::MeasureText(const Magnus::TextObject & text, Magnus::Font* pFont)
		{
			// If the font doesn't exist, or the font size should be changed
			if (!pFont->data || fabs(pFont->realsize - pFont->size * Scale()) > 2)
			{
				FreeFont(pFont);
				LoadFont(pFont);
			}

			FontData* pFontData = (FontData*)pFont->data;
			Magnus::Size size;

			if (text.empty())
			{
				RECT rct = { 0 };
				pFontData->pFont->DrawTextW(NULL, L"W", -1, &rct, DT_CALCRECT, 0);
				return Magnus::Size(0, rct.bottom);
			}

			RECT rct = { 0 };
			pFontData->pFont->DrawTextW(NULL, text.c_str(), -1, &rct, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE, 0);
			
			for (int i = text.length() - 1; i >= 0 && text[i] == L' '; i--)
			{
				rct.right += pFontData->iSpaceWidth;
			}

			return Size(rct.right / Scale(), rct.bottom / Scale());
		}

		void DirectX9::StartClip()
		{
			Flush();
			m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
			const Rect & rect = ClipRegion();
			RECT r;
			r.left = ceil(rect.x * Scale());
			r.top = ceil(rect.y * Scale());
			r.right = ceil((rect.x + rect.w) * Scale());
			r.bottom = ceil((rect.y + rect.h) * Scale());
			m_pDevice->SetScissorRect(&r);
		}

		void DirectX9::EndClip()
		{
			Flush();
			m_pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		}

		void DirectX9::DrawTexturedRect(Texturing::Texture* pTexture, Magnus::Rect rect, float u1, float v1, float u2, float v2)
		{
			IDirect3DTexture9* pD3DTexture = (IDirect3DTexture9*)pTexture->data;

			// Missing image, not loaded properly?
			if (!pD3DTexture)
			{
				return DrawMissingImage(rect);
			}

			Translate(rect);

			if (m_pCurrentTexture != pD3DTexture)
			{
				Flush();
				m_pDevice->SetTexture(0, pD3DTexture);
				m_pCurrentTexture = pD3DTexture;
			}

			AddVert(rect.x, rect.y, u1, v1);
			AddVert(rect.x + rect.w, rect.y, u2, v1);
			AddVert(rect.x, rect.y + rect.h, u1, v2);
			AddVert(rect.x + rect.w, rect.y, u2, v1);
			AddVert(rect.x + rect.w, rect.y + rect.h, u2, v2);
			AddVert(rect.x, rect.y + rect.h, u1, v2);
		}

		void DirectX9::LoadTexture(Texturing::Texture* pTexture)
		{
			IDirect3DTexture9* pD3DTexture = NULL;
			D3DXIMAGE_INFO ImageInfo;
			HRESULT hr = D3DXCreateTextureFromFileExW(m_pDevice, pTexture->name.GetUnicode().c_str(), 0, 0, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &ImageInfo, NULL, &pD3DTexture);

			if (hr != S_OK)
			{
				return;
			}

			pTexture->data = pD3DTexture;
			pTexture->width = ImageInfo.Width;
			pTexture->height = ImageInfo.Height;
		}

		void DirectX9::LoadTextureInMemory(Texturing::Texture* pTexture, void* pSrcData, unsigned int size)
		{
			IDirect3DTexture9* pD3DTexture = NULL;
			D3DXIMAGE_INFO ImageInfo;
			HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(m_pDevice, pSrcData, size, 0, 0, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &ImageInfo, NULL, &pD3DTexture);

			if (hr != S_OK)
			{
				return;
			}

			pTexture->data = pD3DTexture;
			pTexture->width = ImageInfo.Width;
			pTexture->height = ImageInfo.Height;
		}

		void DirectX9::FreeTexture(Texturing::Texture* pTexture)
		{
			IDirect3DTexture9* pD3DTexture = (IDirect3DTexture9*)pTexture->data;

			if (!pD3DTexture) { return; }

			pD3DTexture->Release();
			pTexture->data = NULL;
		}

		Color DirectX9::PixelColor(Texturing::Texture* pTexture, unsigned int x, unsigned int y, const Magnus::Color & defaultColor)
		{
			IDirect3DTexture9* pD3DTexture = (IDirect3DTexture9*)pTexture->data;

			if (!pD3DTexture) { return defaultColor; }

			IDirect3DSurface9* pSurface = NULL;

			if (pD3DTexture->GetSurfaceLevel(0, &pSurface) != S_OK) { return defaultColor; }

			if (!pSurface) { return defaultColor; }

			D3DLOCKED_RECT lockedRect;
			pSurface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY);
			DWORD* pixels = (DWORD*)lockedRect.pBits;
			D3DXCOLOR color = pixels[lockedRect.Pitch / sizeof(DWORD) * y + x];
			pSurface->UnlockRect();
			pSurface->Release();
			return Color(color.a * 255, color.r * 255, color.g * 255, color.b * 255);
		}

		void DirectX9::ResetDevice()
		{
			for (Font::List::iterator it = m_FontList.begin(); it != m_FontList.end(); ++it)
			{
				FontData* pFontData = (FontData*)((*it)->data);

				if (!pFontData->pFont) { continue; }

				pFontData->pFont->OnLostDevice();
				pFontData->pFont->OnResetDevice();
			}
		}

		void DirectX9::Release()
		{
			Font::List::iterator it = m_FontList.begin();

			while (it != m_FontList.end())
			{
				FreeFont(*it);
				it = m_FontList.begin();
			}
		}
	}
}