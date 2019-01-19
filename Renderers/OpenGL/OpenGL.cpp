#include "Magnus/Renderers/OpenGL.h"
#include "Magnus/Utility.h"
#include "Magnus/Font.h"
#include "Magnus/Texture.h"

#include "GL/glew.h"
#include "FreeImage/FreeImage.h"
#include "FontData/FontData.h"

namespace Magnus
{
	namespace Rendering
	{
		OpenGL::OpenGL()
		{
			m_fLetterSpacing = 1.0f / 16.0f;
			m_fFontScale[0] = 1.5f;
			m_fFontScale[1] = 1.5f;
			m_pFontTexture = NULL;

			m_iVertNum = 0;
			::FreeImage_Initialise();

			for (int i = 0; i < MaxVerts; i++)
			{
				m_Vertices[i].z = 0.5f;
			}
		}

		void OpenGL::Init()
		{
			BuildFont();
		}

		OpenGL::~OpenGL()
		{
			DestroyFont();
			::FreeImage_DeInitialise();
		}

		void OpenGL::BuildFont()
		{
			if (m_pFontTexture) { return; }

			m_pFontTexture = new Texturing::Texture();
			// Create a little texture pointer..
			GLuint* pglTexture = new GLuint;
			// Sort out our GWEN texture
			m_pFontTexture->data = pglTexture;
			m_pFontTexture->width = 256;
			m_pFontTexture->height = 256;
			// Create the opengl texture
			glGenTextures(1, pglTexture);
			glBindTexture(GL_TEXTURE_2D, *pglTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			unsigned char* texdata = new unsigned char[256 * 256 * 4];

			for (int i = 0; i < 256 * 256; i++)
			{
				texdata[i * 4] = sFontData[i];
				texdata[i * 4 + 1] = sFontData[i];
				texdata[i * 4 + 2] = sFontData[i];
				texdata[i * 4 + 3] = sFontData[i];
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pFontTexture->width, m_pFontTexture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)texdata);
			delete[]texdata;
		}

		void OpenGL::DestroyFont()
		{
			if (!m_pFontTexture) { return; }

			GLuint* tex = (GLuint*)m_pFontTexture->data;

			if (!tex) { return; }

			glDeleteTextures(1, tex);
			delete tex;
			m_pFontTexture->data = NULL;
			delete m_pFontTexture;
			m_pFontTexture = NULL;
		}

		void OpenGL::Begin()
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glAlphaFunc(GL_GREATER, 1.0f);
			glEnable(GL_BLEND);
		}

		void OpenGL::End()
		{
			Flush();
		}

		void OpenGL::Flush()
		{
			if (m_iVertNum == 0) { return; }

			glVertexPointer(3, GL_FLOAT,  sizeof(Vertex), (void*)&m_Vertices[0].x);
			glEnableClientState(GL_VERTEX_ARRAY);
			glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)&m_Vertices[0].r);
			glEnableClientState(GL_COLOR_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)&m_Vertices[0].u);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_iVertNum);
			m_iVertNum = 0;
			glFlush();
		}

		void OpenGL::AddVert(int x, int y, float u, float v)
		{
			if (m_iVertNum >= MaxVerts - 1)
			{
				Flush();
			}

			m_Vertices[m_iVertNum].x = (float)x;
			m_Vertices[m_iVertNum].y = (float)y;
			m_Vertices[m_iVertNum].u = u;
			m_Vertices[m_iVertNum].v = v;
			m_Vertices[m_iVertNum].r = m_Color.r;
			m_Vertices[m_iVertNum].g = m_Color.g;
			m_Vertices[m_iVertNum].b = m_Color.b;
			m_Vertices[m_iVertNum].a = m_Color.a;
			m_iVertNum++;
		}

		void OpenGL::DrawFilledRect(Magnus::Rect rect)
		{
			GLboolean texturesOn;
			glGetBooleanv(GL_TEXTURE_2D, &texturesOn);

			if (texturesOn)
			{
				Flush();
				glDisable(GL_TEXTURE_2D);
			}

			Translate(rect);
			AddVert(rect.x, rect.y);
			AddVert(rect.x + rect.w, rect.y);
			AddVert(rect.x, rect.y + rect.h);
			AddVert(rect.x + rect.w, rect.y);
			AddVert(rect.x + rect.w, rect.y + rect.h);
			AddVert(rect.x, rect.y + rect.h);
		}

		void OpenGL::SetDrawColor(Magnus::Color color)
		{
			color.a *= Opacity();
			glColor4ubv((GLubyte*)&color);
			m_Color = color;
		}

		void OpenGL::StartClip()
		{
			Flush();
			Magnus::Rect rect = ClipRegion();
			// OpenGL's coords are from the bottom left
			// so we need to translate them here.
			{
				GLint view[4];
				glGetIntegerv(GL_VIEWPORT, &view[0]);
				rect.y = view[3] - (rect.y + rect.h);
			}
			glScissor(rect.x * Scale(), rect.y * Scale(), rect.w * Scale(), rect.h * Scale());
			glEnable(GL_SCISSOR_TEST);
		};

		void OpenGL::EndClip()
		{
			Flush();
			glDisable(GL_SCISSOR_TEST);
		};

		void OpenGL::DrawTexturedRect(Texturing::Texture* pTexture, Magnus::Rect rect, float u1, float v1, float u2, float v2)
		{
			GLuint* tex = (GLuint*)pTexture->data;

			// Missing image, not loaded properly?
			if (!tex)
			{
				return DrawMissingImage(rect);
			}

			Translate(rect);
			GLuint boundtex;
			GLboolean texturesOn;
			glGetBooleanv(GL_TEXTURE_2D, &texturesOn);
			glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundtex);

			if (!texturesOn || *tex != boundtex)
			{
				Flush();
				glBindTexture(GL_TEXTURE_2D, *tex);
				glEnable(GL_TEXTURE_2D);
			}

			AddVert(rect.x, rect.y, u1, v1);
			AddVert(rect.x + rect.w, rect.y, u2, v1);
			AddVert(rect.x, rect.y + rect.h, u1, v2);
			AddVert(rect.x + rect.w, rect.y, u2, v1);
			AddVert(rect.x + rect.w, rect.y + rect.h, u2, v2);
			AddVert(rect.x, rect.y + rect.h, u1, v2);
		}

		void OpenGL::LoadTexture(Texturing::Texture* pTexture)
		{
			const wchar_t* wFileName = pTexture->name.GetUnicode().c_str();
			FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileTypeU(wFileName);

			if (imageFormat == FIF_UNKNOWN)
			{
				imageFormat = FreeImage_GetFIFFromFilenameU(wFileName);
			}

			// Image failed to load..
			if (imageFormat == FIF_UNKNOWN)
			{
				pTexture->failed = true;
				return;
			}

			// Try to load the image..
			FIBITMAP* bits = FreeImage_LoadU(imageFormat, wFileName);

			if (!bits)
			{
				pTexture->failed = true;
				return;
			}

			// Convert to 32bit
			FIBITMAP* bits32 = FreeImage_ConvertTo32Bits(bits);
			FreeImage_Unload(bits);

			if (!bits32)
			{
				pTexture->failed = true;
				return;
			}

			// Flip
			::FreeImage_FlipVertical(bits32);
			// Create a little texture pointer..
			GLuint* pglTexture = new GLuint;
			// Sort out our Magnus texture
			pTexture->data = pglTexture;
			pTexture->width = FreeImage_GetWidth(bits32);
			pTexture->height = FreeImage_GetHeight(bits32);
			// Create the opengl texture
			glGenTextures(1, pglTexture);
			glBindTexture(GL_TEXTURE_2D, *pglTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef FREEIMAGE_BIGENDIAN
			GLenum format = GL_RGBA;
#else
			GLenum format = GL_BGRA;
#endif
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pTexture->width, pTexture->height, 0, format, GL_UNSIGNED_BYTE, (const GLvoid*)FreeImage_GetBits(bits32));
			FreeImage_Unload(bits32);
		}

		void OpenGL::LoadTextureInMemory(Texturing::Texture* pTexture, void* pSrcData, unsigned int size)
		{
			FIMEMORY* stream = FreeImage_OpenMemory((BYTE*)pSrcData, size);
			FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileTypeFromMemory(stream, size);

			// Image failed to load..
			if (imageFormat == FIF_UNKNOWN)
			{
				pTexture->failed = true;
				return;
			}

			// Try to load the image..
			FIBITMAP* bits = FreeImage_LoadFromMemory(imageFormat, stream);

			if (!bits)
			{
				pTexture->failed = true;
				return;
			}

			// Convert to 32bit
			FIBITMAP* bits32 = FreeImage_ConvertTo32Bits(bits);
			FreeImage_Unload(bits);

			if (!bits32)
			{
				pTexture->failed = true;
				return;
			}

			// Flip
			::FreeImage_FlipVertical(bits32);
			// Create a little texture pointer..
			GLuint* pglTexture = new GLuint;
			// Sort out our Magnus texture
			pTexture->data = pglTexture;
			pTexture->width = FreeImage_GetWidth(bits32);
			pTexture->height = FreeImage_GetHeight(bits32);
			// Create the opengl texture
			glGenTextures(1, pglTexture);
			glBindTexture(GL_TEXTURE_2D, *pglTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef FREEIMAGE_BIGENDIAN
			GLenum format = GL_RGBA;
#else
			GLenum format = GL_BGRA;
#endif
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pTexture->width, pTexture->height, 0, format, GL_UNSIGNED_BYTE, (const GLvoid*)FreeImage_GetBits(bits32));
			FreeImage_Unload(bits32);
		}

		void OpenGL::FreeTexture(Texturing::Texture* pTexture)
		{
			GLuint* tex = (GLuint*)pTexture->data;

			if (!tex) { return; }

			glDeleteTextures(1, tex);
			delete tex;
			pTexture->data = NULL;
		}

		Magnus::Color OpenGL::PixelColor(Texturing::Texture* pTexture, unsigned int x, unsigned int y, const Magnus::Color & col_default)
		{
			GLuint* tex = (GLuint*)pTexture->data;

			if (!tex) { return col_default; }

			unsigned int iPixelSize = sizeof(unsigned char) * 4;
			glBindTexture(GL_TEXTURE_2D, *tex);
			unsigned char* data = (unsigned char*)malloc(iPixelSize * pTexture->width * pTexture->height);
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			unsigned int iOffset = (y * pTexture->width + x) * 4;
			Magnus::Color c;
			c.r = data[0 + iOffset];
			c.g = data[1 + iOffset];
			c.b = data[2 + iOffset];
			c.a = data[3 + iOffset];
			//
			// Retrieving the entire texture for a single pixel read
			// is kind of a waste - maybe cache this pointer in the texture
			// data and then release later on? It's never called during runtime
			// - only during initialization.
			//
			free(data);
			return c;
		}

		void OpenGL::RenderText(const Magnus::TextObject & text, Magnus::Point pos, Magnus::Font* pFont)
		{
			float fSize = pFont->size * Scale();

			if (!text.length())
			{
				return;
			}

			Magnus::String converted_string = text.Get();
			float yOffset = 0.0f;

			for (unsigned int i = 0; i < text.length(); i++)
			{
				char ch = converted_string[i];
				float curSpacing = sFontSpacing[ch] * m_fLetterSpacing * fSize * m_fFontScale[0];
				Magnus::Rect r(pos.x + yOffset, pos.y - fSize * 0.5, (fSize * m_fFontScale[0]), fSize * m_fFontScale[1]);

				if (m_pFontTexture)
				{
					float uv_texcoords[8] = { 0., 0., 1., 1. };

					if (ch >= 0)
					{
						float cx = (ch % 16) / 16.0;
						float cy = (ch / 16) / 16.0;
						uv_texcoords[0] = cx;
						uv_texcoords[1] = cy;
						uv_texcoords[4] = float(cx + 1.0f / 16.0f);
						uv_texcoords[5] = float(cy + 1.0f / 16.0f);
					}

					DrawTexturedRect(m_pFontTexture, r, uv_texcoords[0], uv_texcoords[5], uv_texcoords[4], uv_texcoords[1]);
					yOffset += curSpacing;
				}
				else
				{
					DrawFilledRect(r);
					yOffset += curSpacing;
				}
			}
		}

		Magnus::Size OpenGL::MeasureText(const Magnus::TextObject & text, Magnus::Font* pFont)
		{
			Magnus::Size s;
			float fSize = pFont->size * Scale();
			Magnus::String converted_string = text.Get();
			float spacing = 0.0f;

			for (unsigned int i = 0; i < text.length(); i++)
			{
				char ch = converted_string[i];
				spacing += sFontSpacing[ch];
			}

			s.w = spacing * m_fLetterSpacing * fSize * m_fFontScale[0];
			s.h = pFont->size * Scale();
			return s;
		}
	}
}