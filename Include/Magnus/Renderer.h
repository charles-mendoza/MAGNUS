#pragma once
#ifndef MAGNUS_RENDERING_RENDERER_H
#define MAGNUS_RENDERING_RENDERER_H

#include "Magnus/Structures.h"

namespace Magnus
{
	struct Font;
	class WindowProvider;

	namespace Texturing
	{
		struct Texture;
	}

	namespace Rendering
	{
		class Renderer;

		class ICacheToTexture
		{
		public:
			virtual ~ICacheToTexture() {}
			virtual void Initialize() = 0;
			virtual void ShutDown() = 0;
			virtual void SetupCacheTexture(Magnus::Controls::Control* control) = 0;
			virtual void FinishCacheTexture(Magnus::Controls::Control* control) = 0;
			virtual void DrawCachedControlTexture(Magnus::Controls::Control* control) = 0;
			virtual void CreateControlCacheTexture(Magnus::Controls::Control* control) = 0;
			virtual void UpdateControlCacheTexture(Magnus::Controls::Control* control) = 0;
			virtual void SetRenderer(Magnus::Rendering::Renderer* renderer) = 0;
		};

		/*

		Defines the base class for renderers, which is what makes an object appear on a canvas.

		*/
		class Renderer
		{
		public:

			Renderer();
			virtual ~Renderer();

			virtual void Init() {}

			virtual void Begin() {}
			virtual void End() {}

			virtual void SetDrawColor(Magnus::Color color) {}

			virtual void DrawFilledRect(Magnus::Rect rect) {}

			virtual void StartClip() {}
			virtual void EndClip() {}

			virtual void LoadTexture(Texturing::Texture* pTexture) {}
			virtual void LoadTextureInMemory(Texturing::Texture* pTexture, void* pSrcData, unsigned int size) {}
			virtual void FreeTexture(Texturing::Texture* pTexture) {}
			virtual void DrawTexturedRect(Texturing::Texture* pTexture, Magnus::Rect rect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) {}
			virtual void DrawMissingImage(Magnus::Rect rect);
			virtual Magnus::Color PixelColor(Texturing::Texture* pTexture, unsigned int x, unsigned int y, const Magnus::Color & defaultColor = Magnus::Color(255, 255, 255, 255)) { return defaultColor; }

			virtual ICacheToTexture* GetCTT() { return NULL; }

			virtual void LoadFont(Magnus::Font* pFont) {}
			virtual void FreeFont(Magnus::Font* pFont) {}
			virtual void RenderText(const Magnus::TextObject & text, Magnus::Point pos, Magnus::Font* pFont);
			virtual Magnus::Size MeasureText(const Magnus::TextObject & text, Magnus::Font* pFont);

			//
			// No need to implement these functions in your derived class, but if
			// you can do them faster than the default implementation it's a good idea to.
			//
			virtual void DrawLinedRect(Magnus::Rect rect);
			virtual void DrawPixel(int x, int y);
			virtual void DrawShavedCornerRect(Magnus::Rect rect, bool bSlight = false);

			virtual void Draw(Magnus::Vector2* pVertexList, float fWidth, int iVertexCount, bool bClosedShape = false) {}
			virtual void DrawLine(Magnus::Vector2 p1, Magnus::Vector2 p2, float fWidth) {}
			virtual void DrawCircle(Magnus::Vector2 pCenter, float fRadius, int iSides, float fWidth, int iSpacing = 0) {}

			virtual Magnus::Rect GetClientBounds() { return Magnus::Rect(0, 0, 10000, 10000); }

		public:

			//
			// Translate a panel's local drawing coordinates
			// into view space, taking offset's into account.
			//
			void Translate(int & x, int & y);
			void Translate(Magnus::Rect & rect);

			//
			// Set the rendering offset. You shouldn't have to
			// touch these, ever.
			//
			void SetRenderOffset(const Magnus::Point & offset) { m_RenderOffset = offset; }
			void AddRenderOffset(const Magnus::Rect & offset) { m_RenderOffset.x += offset.x; m_RenderOffset.y += offset.y; }
			const Magnus::Point & GetRenderOffset() const { return m_RenderOffset; }

		protected:

			Magnus::Point m_RenderOffset;

		public:

			void SetClipRegion(Magnus::Rect rect);
			void AddClipRegion(Magnus::Rect rect);
			bool ClipRegionVisible();
			const Magnus::Rect & ClipRegion() const;

		protected:

			Magnus::Rect m_ClipRegion;
			ICacheToTexture* m_RTT;

		public:

			void SetScale(float fScale) { m_fScale = fScale; }
			float Scale() const { return m_fScale; }
			void SetOpacity(float fOpacity);
			float Opacity() const { return m_fOpacity; }

		protected:

			float m_fScale;
			float m_fOpacity;
		};
	}
}

#endif