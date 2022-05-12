#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, Alex
	All rights reserved.
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	   contributors may be used to endorse or promote products derived from
	   this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***/
#pragma endregion

#pragma region sample
/**
* Example:
	#include "defGameEngine.h"

	class Sample : public def::GameEngine
	{
	public:
		Sample()
		{
			SetTitle("Sample");
		}

	protected:
		bool OnUserCreate() override
		{
			return true;
		}

		bool OnUserUpdate(float fDeltaTime) override
		{
			for (int i = 0; i < GetScreenWidth(); i++)
				for (int j = 0; j < GetScreenHeight(); j++)
					Draw(i, j, def::Pixel(rand() % 255, rand() % 255, rand() % 255, 255));

			return true;
		}

	};

	int main(int argc, char* argv[]) // argc and argv are necessary
	{
		Sample demo;
		def::rcode err = demo.Construct(256, 240, 4, 4);

		if (err.ok)
			demo.Run();
		else
			std::cerr << err.info << "\n";

		return 0;
	}
**/
#pragma endregion

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

namespace def
{
	template <class T>
	class vec2d_basic
	{
	public:
		vec2d_basic()
		{
			this->x = 0;
			this->y = 0;
		}

		vec2d_basic(T x, T y)
		{
			this->x = x;
			this->y = y;
		}

		T x;
		T y;

		friend vec2d_basic<T> operator+(const vec2d_basic<T>& v1, const vec2d_basic<T>& v2)
		{
			return { v1.x + v2.x, v1.y + v2.y };
		}

		friend vec2d_basic<T> operator-(const vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			return { v1.x - v2.x, v1.y - v2.y };
		}

		friend vec2d_basic<T> operator*(const vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			return { v1.x * v2.x, v1.y * v2.y };
		}

		friend vec2d_basic<T> operator/(const vec2d_basic<T> v1, const vec2d_basic<T>& v2)
		{
			return { v1.x / v2.x, v1.y / v2.y };
		}

		friend vec2d_basic<T> operator+(const vec2d_basic<T> v1, const T v)
		{
			return { v1.x + v, v1.y + v };
		}

		friend vec2d_basic<T> operator-(const vec2d_basic<T> v1, const T v)
		{
			return { v1.x - v, v1.y - v };
		}

		friend vec2d_basic<T> operator*(const vec2d_basic<T> v1, const T v)
		{
			return { v1.x * v, v1.y * v };
		}

		friend vec2d_basic<T> operator/(const vec2d_basic<T> v1, const T v)
		{
			return { v1.x / v, v1.y / v };
		}

		float mag()
		{
			return sqrtf(this->x * this->x + this->y * this->y);
		}

		vec2d_basic<T> norm()
		{
			return { this->x / mag(), this->y / mag() };
		}

		// linear interpolation
		vec2d_basic<T> lerp(float x, float y, float t)
		{
			return { this->x + t * (x - this->x), this->y + t * (y - this->y) };
		}

		float dot(vec2d_basic<T> v, float angle)
		{
			return (this->x * v.x + this->y * v.y);
		}

		vec2d_basic<T> reflect()
		{
			return vec2d_basic<T>(-this->x, -this->y);
		}

		float distance()
		{
			return sqrtf(this->x * this->x + this->y * this->y);
		}
	};

	using vf2d = vec2d_basic<float>;
	using vi2d = vec2d_basic<int>;
	using vd2d = vec2d_basic<double>;

	struct rcode
	{
		bool ok;
		std::string info;
	};

	struct KeyState
	{
		bool bHeld;
		bool bReleased;
		bool bPressed;
	};

	struct Pixel
	{
		Pixel()
		{
			r = 0;
			g = 0;
			b = 0;
			a = 255;
		}

		Pixel(uint8_t cr, uint8_t cg, uint8_t cb, uint8_t ca) : r(cr), g(cg), b(cb), a(ca)
		{
			
		}

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		friend Pixel operator+(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r + rhs, lhs.g + rhs, lhs.b + rhs, lhs.a);
		}

		friend Pixel operator-(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r - rhs, lhs.g - rhs, lhs.b - rhs, lhs.a);
		}

		friend Pixel operator*(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a);
		}

		friend Pixel operator/(Pixel& lhs, float& rhs)
		{
			return Pixel(lhs.r / rhs, lhs.g / rhs, lhs.b / rhs, lhs.a);
		}
	};

	Pixel DARK_BLUE = Pixel(0, 55, 218, 255);
	Pixel DARK_GREEN = Pixel(19, 161, 14, 255);
	Pixel DARK_CYAN = Pixel(59, 120, 255, 255);
	Pixel DARK_RED = Pixel(197, 15, 31, 255);
	Pixel DARK_MAGENTA = Pixel(136, 32, 152, 255);
	Pixel DARK_GREY = Pixel(118, 118, 118, 255);
	Pixel ORANGE = Pixel(255, 165, 0, 255);
	Pixel GREY = Pixel(204, 204, 204, 255);
	Pixel BLUE = Pixel(0, 0, 255, 255);
	Pixel GREEN = Pixel(0, 255, 0, 255);
	Pixel CYAN = Pixel(58, 150, 221, 255);
	Pixel RED = Pixel(255, 0, 0, 255);
	Pixel MAGENTA = Pixel(180, 0, 158, 255);
	Pixel YELLOW = Pixel(255, 255, 0, 255);
	Pixel WHITE = Pixel(255, 255, 255, 255);
	Pixel BLACK = Pixel(0, 0, 0, 0);

#define RANDOM_PIXEL def::Pixel(rand() % 255, rand() % 255, rand() % 255, 255)
#define RANDOM_PIXEL_ALPHA def::Pixel(rand() % 255, rand() % 255, rand() % 255, rand() % 255)

	class Sprite
	{
	public:
		Sprite()
		{
		}

		Sprite(std::string filename)
		{
			rcode rc = LoadTexture(filename);
			
			if (!rc.ok)
				std::cerr << rc.info << "\n";
			
			m_sFilename = filename;
		}

		~Sprite()
		{
			delete m_sdlSurface;
		}

		SDL_Surface* m_sdlSurface;

		SDL_Rect m_sdlFileRect;
		SDL_Rect m_sdlCoordRect;

	private:
		uint32_t m_nWidth;
		uint32_t m_nHeight;

		uint32_t m_nTexId;

		std::string m_sFilename;

	public:
		rcode LoadTexture(std::string filename)
		{
			rcode rc;
			rc.ok = false;
			rc.info = "Ok";

			m_sdlSurface = IMG_Load(filename.c_str());

			if (!m_sdlSurface)
			{
				rc.info += "SDL: ";
				rc.info += SDL_GetError();
				return rc;
			}

			m_nWidth = m_sdlSurface->w;
			m_nHeight = m_sdlSurface->h;

			rc.ok = true;
			return rc;
		}
		
		void SetTexId(uint32_t id)
		{
			m_nTexId = id;
		}

		uint32_t GetTexId() const
		{
			return m_nTexId;
		}

		uint32_t GetWidth() const
		{
			return m_nWidth;
		}

		uint32_t GetHeight() const
		{
			return m_nHeight;
		}

		std::string GetFilename() const
		{
			return m_sFilename;
		}
	};

	class GameEngine
	{
	public:
		GameEngine()
		{
			m_sAppName = "Undefined";
		}

		virtual ~GameEngine()
		{
			SDL_DestroyRenderer(m_sdlRenderer);
			SDL_DestroyWindow(m_sdlWindow);

			SDL_Quit();
		}

	private:
		std::string m_sAppName;
		
		int32_t m_nScreenWidth;
		int32_t m_nScreenHeight;

		int32_t m_nPixelWidth;
		int32_t m_nPixelHeight;

		SDL_Window* m_sdlWindow = nullptr;
		SDL_Renderer* m_sdlRenderer = nullptr;

		bool m_bAppThreadActive;

		KeyState m_sKeys[512];
		KeyState m_sMouse[5];
		
		uint8_t m_nKeyOldState[512];
		uint8_t* m_nKeyNewState;

		uint8_t m_nMouseOldState[5];
		uint8_t m_nMouseNewState[5];
		
		int32_t m_nMouseX;
		int32_t m_nMouseY;

		std::vector<SDL_Texture*> m_vecTextures;

		SDL_Rect* m_sdlRect;

	public:
		virtual bool OnUserCreate() = 0;
		virtual bool OnUserUpdate(float fDeltaTime) = 0;
		virtual void OnUserDestroy() { return; }

		rcode Construct(int nWidth, int nHeight, int nPixelWidth, int nPixelHeight, bool bFullScreen = false)
		{
			rcode rc;
			rc.ok = false;

			auto get_sdl_err = [&]()
			{
				rc.info += "SDL: ";
				rc.info += SDL_GetError();
				return rc;
			};

			auto set_err = [&](std::string text)
			{
				rc.info = text;
				return rc;
			};

			if (nWidth < 0 && nHeight < 0)
				return set_err("Width or height less than zero");

			m_nScreenWidth = nWidth;
			m_nScreenHeight = nHeight;

			m_nPixelWidth = nPixelWidth;
			m_nPixelHeight = nPixelHeight;

			if (SDL_Init(SDL_INIT_VIDEO) > 0)
				return get_sdl_err();

			m_sdlWindow = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_nScreenWidth * m_nPixelWidth, m_nScreenHeight * m_nPixelHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

			if (!m_sdlWindow)
				return get_sdl_err();

			SDL_SetWindowFullscreen(m_sdlWindow, bFullScreen);

			m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, SDL_RENDERER_ACCELERATED);

			rc.ok = true;
			rc.info = "Ok";
			
			return rc;
		}

		void Run()
		{
			m_bAppThreadActive = true;
			AppThread();
		}

	private:
		void AppThread()
		{
			if (!OnUserCreate())
				m_bAppThreadActive = false;

			if (m_bAppThreadActive)
			{
				auto tp1 = std::chrono::system_clock::now();
				auto tp2 = tp1;

				for (int i = 0; i < 512; i++)
					m_sKeys[i] = { false, false, false };

				for (int i = 0; i < 512; i++)
					m_nKeyOldState[i] = 0;

				SDL_Event evt;

				while (m_bAppThreadActive)
				{
					tp2 = std::chrono::system_clock::now();

					std::chrono::duration<float> elapsedTime = tp2 - tp1;
					tp1 = tp2;
					
					float fDeltaTime = elapsedTime.count();

					char s[256];
					sprintf_s(s, 256, "github.com/defini7 - %s - FPS: %3.2f", m_sAppName.c_str(), 1.0f / fDeltaTime);
					SDL_SetWindowTitle(m_sdlWindow, s);

					while (SDL_PollEvent(&evt))
					{
						switch (evt.type)
						{
						case SDL_QUIT:
							m_bAppThreadActive = false;
							break;

						case SDL_MOUSEBUTTONDOWN:
						{
							switch (evt.button.button)
							{
							case SDL_BUTTON_LEFT:
								m_nMouseNewState[0] = 1;
								break;
							case SDL_BUTTON_RIGHT:
								m_nMouseNewState[1] = 1;
								break;
							case SDL_BUTTON_MIDDLE:
								m_nMouseNewState[2] = 1;
								break;
							case SDL_BUTTON_X1:
								m_nMouseNewState[3] = 1;
								break;
							case SDL_BUTTON_X2:
								m_nMouseNewState[4] = 1;
								break;
							}

							for (int m = 0; m < 5; m++)
							{
								m_sMouse[m].bPressed = false;
								m_sMouse[m].bReleased = false;

								if (m_nMouseNewState[m] != m_nMouseOldState[m])
								{
									if (m_nMouseNewState[m])
									{
										m_sMouse[m].bPressed = true;
										m_sMouse[m].bHeld = true;
									}
									else
									{
										m_sMouse[m].bReleased = true;
										m_sMouse[m].bHeld = false;
									}
								}

								m_nMouseOldState[m] = m_nMouseNewState[m];
							}
						}
						break;

						case SDL_MOUSEBUTTONUP:
						{
							switch (evt.button.button)
							{
							case SDL_BUTTON_LEFT:
								m_nMouseNewState[0] = 0;
								break;
							case SDL_BUTTON_RIGHT:
								m_nMouseNewState[1] = 0;
								break;
							case SDL_BUTTON_MIDDLE:
								m_nMouseNewState[2] = 0;
								break;
							case SDL_BUTTON_X1:
								m_nMouseNewState[3] = 0;
								break;
							case SDL_BUTTON_X2:
								m_nMouseNewState[4] = 0;
								break;
							}

							for (int m = 0; m < 5; m++)
							{
								m_sMouse[m].bPressed = false;
								m_sMouse[m].bReleased = false;

								if (m_nMouseNewState[m] != m_nMouseOldState[m])
								{
									if (m_nMouseNewState[m])
									{
										m_sMouse[m].bPressed = true;
										m_sMouse[m].bHeld = true;
									}
									else
									{
										m_sMouse[m].bReleased = true;
										m_sMouse[m].bHeld = false;
									}
								}

								m_nMouseOldState[m] = m_nMouseNewState[m];
							}
						}
						break;

						case SDL_MOUSEMOTION:
						{
							uint32_t mouse_mask = SDL_GetMouseState(&m_nMouseX, &m_nMouseY);

							m_nMouseX /= m_nPixelWidth;
							m_nMouseY /= m_nPixelHeight;
						}
						break;

						case SDL_KEYDOWN: case SDL_KEYUP:
						{
							m_nKeyNewState = (uint8_t*)SDL_GetKeyboardState(NULL);

							for (int i = 0; i < 512; i++)
							{
								m_sKeys[i].bPressed = false;
								m_sKeys[i].bReleased = false;

								if (m_nKeyNewState[i] != m_nKeyOldState[i])
								{
									if (m_nKeyNewState[i])
									{
										m_sKeys[i].bPressed = !m_sKeys[i].bHeld;
										m_sKeys[i].bHeld = true;
									}
									else
									{
										m_sKeys[i].bReleased = true;
										m_sKeys[i].bHeld = false;
									}
								}

								m_nKeyOldState[i] = m_nKeyNewState[i];
							}
						}
						break;

						}
					}

					SDL_RenderSetScale(m_sdlRenderer, m_nPixelWidth, m_nPixelHeight);

					if (!OnUserUpdate(fDeltaTime))
						m_bAppThreadActive = false;
					
					SDL_RenderPresent(m_sdlRenderer);
				}
			}
		}

		SDL_Rect* GetPixelRect(int32_t x, int32_t y)
		{
			SDL_Rect* rct;
			rct->x = x * m_nPixelWidth;
			rct->y = y * m_nPixelHeight;
			rct->w = m_nPixelWidth;
			rct->h = m_nPixelHeight;

			return rct;
		}

	public:
		void SetTitle(std::string title)
		{
			m_sAppName = title;
		}

		void Draw(int32_t x, int32_t y, Pixel p)
		{
			SDL_SetRenderDrawColor(m_sdlRenderer, p.r, p.g, p.b, p.a);
			SDL_RenderDrawPoint(m_sdlRenderer, x, y);
		}

		void Clear(Pixel p)
		{
			SDL_SetRenderDrawColor(m_sdlRenderer, p.r, p.g, p.b, p.a);
			SDL_RenderClear(m_sdlRenderer);
		}

		void FillRectangle(int32_t x, int32_t y, int32_t sx, int32_t sy, Pixel p)
		{
			m_sdlRect = GetPixelRect(x, y);
			SDL_RenderFillRect(m_sdlRenderer, m_sdlRect);
		}

		void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p)
		{
			SDL_SetRenderDrawColor(m_sdlRenderer, p.r, p.g, p.b, p.a);
			SDL_RenderDrawLine(m_sdlRenderer, x1, y1, x2, y2);
		}

		void DrawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, Pixel p)
		{
			DrawLine(x, y, x + width, y, p);
			DrawLine(x + width, y, x + width, y + height, p);
			DrawLine(x + width, y + height, x, y + height, p);
			DrawLine(x, y + height, x, y, p);
		}

		void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p)
		{
			DrawLine(x1, y1, x2, y2, p);
			DrawLine(x2, y2, x3, y3, p);
			DrawLine(x3, y3, x1, y1, p);
		}

		// https://www.avrfreaks.net/sites/default/files/triangles.c
		void FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p)
		{
			auto drawline = [&](int32_t sx, int32_t ex, int32_t ny) { for (int32_t i = sx; i <= ex; i++) Draw(i, ny, p); };

			int32_t t1x, t2x, y, minx, maxx, t1xp, t2xp;
			bool changed1 = false;
			bool changed2 = false;
			int32_t signx1, signx2, dx1, dy1, dx2, dy2;
			int32_t e1, e2;
			
			// Sort vertices
			if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
			if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
			if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

			t1x = t2x = x1;
			y = y1;   // Starting points
			dx1 = (int32_t)(x2 - x1);

			if (dx1 < 0) 
			{
				dx1 = -dx1; 
				signx1 = -1; 
			}
			else 
				signx1 = 1;

			dy1 = (int32_t)(y2 - y1);
			dx2 = (int32_t)(x3 - x1);

			if (dx2 < 0) 
			{ 
				dx2 = -dx2; 
				signx2 = -1; 
			}
			else 
				signx2 = 1;
			
			dy2 = (int32_t)(y3 - y1);

			if (dy1 > dx1) 
			{
				std::swap(dx1, dy1);
				changed1 = true;
			}
			if (dy2 > dx2) 
			{
				std::swap(dy2, dx2);
				changed2 = true;
			}

			e2 = (int32_t)(dx2 >> 1);

			// Flat top, just process the second half
			if (y1 == y2) 
				goto next;

			e1 = (int32_t)(dx1 >> 1);

			for (int32_t i = 0; i < dx1;)
			{
				t1xp = 0; 
				t2xp = 0;
				
				if (t1x < t2x) 
				{ 
					minx = t1x; 
					maxx = t2x; 
				}
				else 
				{ 
					minx = t2x; 
					maxx = t1x; 
				}
				
				// process first line until y value is about to change
				while (i < dx1) 
				{
					i++;
					e1 += dy1;
					while (e1 >= dx1) 
					{
						e1 -= dx1;
						if (changed1) 
							t1xp = signx1;
						else 
							goto next1;
					}
					if (changed1) break;
					else t1x += signx1;
				}
				// Move line
			next1:
				// process second line until y value is about to change
				while (1) 
				{
					e2 += dy2;
					
					while (e2 >= dx2) 
					{
						e2 -= dx2;

						if (changed2) 
							t2xp = signx2;//t2x += signx2;
						else          
							goto next2;
					}

					if (changed2)     
						break;
					else              
						t2x += signx2;
				}
			next2:
				if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
				if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
				drawline(minx, maxx, y);    // Draw line from min to max points found on the y
												// Now increase y
				if (!changed1) t1x += signx1;
				t1x += t1xp;
				if (!changed2) t2x += signx2;
				t2x += t2xp;
				y += 1;
				if (y == y2) break;

			}
		next:
			// Second half
			dx1 = (int32_t)(x3 - x2);

			if (dx1 < 0) 
			{ 
				dx1 = -dx1; 
				signx1 = -1;
			}
			else 
				signx1 = 1;

			dy1 = (int32_t)(y3 - y2);
			t1x = x2;

			if (dy1 > dx1) 
			{   // swap values
				std::swap(dy1, dx1);
				changed1 = true;
			}
			else 
				changed1 = false;

			e1 = (int32_t)(dx1 >> 1);

			for (int32_t i = 0; i <= dx1; i++)
			{
				t1xp = 0; t2xp = 0;
				if (t1x < t2x) 
				{ 
					minx = t1x; 
					maxx = t2x; 
				}
				else 
				{ 
					minx = t2x;
					maxx = t1x;
				}

				// process first line until y value is about to change
				while (i < dx1) 
				{
					e1 += dy1;
					while (e1 >= dx1) 
					{
						e1 -= dx1;

						if (changed1) 
						{ 
							t1xp = signx1; 
							break; 
						}
						else          
							goto next3;
					}
					
					if (changed1) 
						break;
					else   	   	  
						t1x += signx1;
					
					if (i < dx1) 
						i++;
				}

			next3:
				// process second line until y value is about to change
				while (t2x != x3) 
				{
					e2 += dy2;

					while (e2 >= dx2) 
					{
						e2 -= dx2;

						if (changed2) 
							t2xp = signx2;
						else          
							goto next4;
					}

					if (changed2)     
						break;
					else              
						t2x += signx2;
				}
				
			next4:
				if (minx > t1x)
					minx = t1x;

				if (minx > t2x)
					minx = t2x;

				if (maxx < t1x)
					maxx = t1x;

				if (maxx < t2x)
					maxx = t2x;

				drawline(minx, maxx, y);

				if (!changed1) 
					t1x += signx1;

				t1x += t1xp;

				if (!changed2) 
					t2x += signx2;

				t2x += t2xp;
				y += 1;
				
				if (y > y3) 
					return;
			}
		}

		void DrawCircle(int32_t x, int32_t y, uint32_t r, Pixel p)
		{
			if (!r) return;

			int32_t x1 = 0;
			int32_t y1 = r;
			int32_t p1 = 3 - 2 * r;

			while (y1 >= x1)
			{
				Draw(x - x1, y - y1, p);	// upper left left
				Draw(x - y1, y - x1, p);	// upper upper left
				Draw(x + y1, y - x1, p);	// upper upper right
				Draw(x + x1, y - y1, p);	// upper right right
				Draw(x - x1, y + y1, p);	// lower left left
				Draw(x - y1, y + x1, p);	// lower lower left
				Draw(x + y1, y + x1, p);	// lower lower right
				Draw(x + x1, y + y1, p);	// lower right right

				if (p1 < 0)
					p1 += 4 * x1++ + 6;
				else
					p1 += 4 * (x1++ - y1--) + 10;
			}
		}
		
		void FillCircle(int32_t x, int32_t y, uint32_t r, Pixel p)
		{
			if (!r) return;

			int32_t x1 = 0;
			int32_t y1 = r;
			int32_t p1 = 3 - 2 * r;

			auto drawline = [&](int32_t sx, int32_t ex, int32_t ny)
			{
				for (int i = sx; i <= ex; i++)
					Draw(i, ny, p);
			};

			while (y1 >= x1)
			{
				drawline(x - x1, x + x1, y - y1);
				drawline(x - y1, x + y1, y - x1);
				drawline(x - x1, x + x1, y + y1);
				drawline(x - y1, x + y1, y + x1);
				
				if (p1 < 0)
					p1 += 4 * x1++ + 6;
				else
					p1 += 4 * (x1++ - y1--) + 10;
			}
		}

		void DrawWireFrameModel(std::vector<std::pair<float, float>>& vecModelCoordinates, int32_t x, int32_t y, uint32_t r, float s, Pixel p)
		{
			std::vector<std::pair<float, float>> vecTransformedCoordinates;
			int verts = vecModelCoordinates.size();
			vecTransformedCoordinates.resize(verts);

			// Rotate
			for (int i = 0; i < verts; i++)
			{
				vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
				vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
			}

			// Scale
			for (int i = 0; i < verts; i++)
			{
				vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
				vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
			}

			// Translate
			for (int i = 0; i < verts; i++)
			{
				vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
				vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
			}

			for (int i = 0; i < verts + 1; i++)
			{
				int j = (i + 1);
				DrawLine((int)vecTransformedCoordinates[i % verts].first, (int)vecTransformedCoordinates[i % verts].second,
					(int)vecTransformedCoordinates[j % verts].first, (int)vecTransformedCoordinates[j % verts].second, p);
			}
		}

		Sprite* CreateSprite(std::string filename)
		{
			Sprite* spr = new Sprite(filename);

			spr->SetTexId(m_vecTextures.size());

			m_vecTextures.push_back(SDL_CreateTextureFromSurface(m_sdlRenderer, spr->m_sdlSurface));

			spr->m_sdlCoordRect.w = m_nPixelWidth * spr->GetWidth();
			spr->m_sdlCoordRect.h = m_nPixelHeight * spr->GetHeight();

			spr->m_sdlFileRect = spr->m_sdlCoordRect;

			return spr;
		}

		Sprite* RecreateSprite(Sprite* spr)
		{
			m_vecTextures.erase(m_vecTextures.begin() + spr->GetTexId());
			
			delete spr;

			spr = CreateSprite(spr->GetFilename());
			
			return spr;
		}

		void DrawSprite(int32_t x, int32_t y, Sprite* spr)
		{
			spr->m_sdlCoordRect.x = x * m_nPixelWidth;
			spr->m_sdlCoordRect.y = y * m_nPixelHeight;

			SDL_RenderCopy(m_sdlRenderer, m_vecTextures[spr->GetTexId()], &spr->m_sdlFileRect, &spr->m_sdlCoordRect);
		}

		KeyState GetKey(SDL_Scancode keyCode) const
		{
			return m_sKeys[keyCode];
		}

		KeyState GetMouse(uint8_t btnCode) const
		{
			return m_sMouse[btnCode];
		}

		uint32_t GetMouseX() const
		{
			return m_nMouseX;
		}

		uint32_t GetMouseY() const
		{
			return m_nMouseY;
		}

		uint32_t GetScreenWidth() const
		{
			return m_nScreenWidth;
		}

		uint32_t GetScreenHeight() const
		{
			return m_nScreenHeight;
		}
	};
}
