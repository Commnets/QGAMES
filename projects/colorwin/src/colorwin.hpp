// The MIT License(MIT)
// Copyright(c) 2016  Jeff Rebacz
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files(the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef COLORWIN_HPP_INCLUDED
#define COLORWIN_HPP_INCLUDED

#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include <stack>

namespace colorwin
{
#ifdef _WIN32
	enum CW_COLORS
	{
		dark_red		= FOREGROUND_RED,
		dark_yellow		= FOREGROUND_RED | FOREGROUND_GREEN,
		dark_green		= FOREGROUND_GREEN,
		dark_cyan		= FOREGROUND_GREEN | FOREGROUND_BLUE,
		dark_blue		= FOREGROUND_BLUE,
		dark_magenta	= FOREGROUND_BLUE | FOREGROUND_RED,
		dark_grey		= FOREGROUND_INTENSITY,

		red				= FOREGROUND_RED | FOREGROUND_INTENSITY,
		yellow			= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		green			= FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		cyan			= FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		blue			= FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		magenta			= FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY,
		white			= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		grey			= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

		back_red		= BACKGROUND_RED | white,
		back_yellow		= BACKGROUND_RED | BACKGROUND_GREEN | white,
		back_green		= BACKGROUND_GREEN | white,
		back_cyan		= BACKGROUND_GREEN | BACKGROUND_BLUE | white,
		back_blue		= BACKGROUND_BLUE | white,
		back_magenta	= BACKGROUND_BLUE | BACKGROUND_RED | white,
		back_grey		= BACKGROUND_INTENSITY 
	};

	class color
	{
		public:
		color (CW_COLORS color) 
			: m_color (color), m_console_handle (INVALID_HANDLE_VALUE)
		{
			CONSOLE_SCREEN_BUFFER_INFO console_info;
			m_console_handle = GetStdHandle (STD_OUTPUT_HANDLE);
			if (!GetConsoleScreenBufferInfo (m_console_handle, &console_info))
			{
				m_console_handle = GetStdHandle (STD_ERROR_HANDLE);
				if (!GetConsoleScreenBufferInfo (m_console_handle, &console_info)) // maybe standard output device has been redirected, try the standard error device
				{
					m_console_handle = INVALID_HANDLE_VALUE;
				}
			}
		}

		color (color &) = delete;
		color& operator = (color) = delete;

		~color ()
		{
			if (m_console_handle != INVALID_HANDLE_VALUE)
			{
				// Restore the previous color.
				SetConsoleTextAttribute (m_console_handle, get_color_stack ().top ());
				get_color_stack ().pop ();
			}
		}

		private:
		void change_color() const
		{
			if (m_console_handle == INVALID_HANDLE_VALUE)
				return; // Can't get console info, can't change color.
			CONSOLE_SCREEN_BUFFER_INFO console_info;
			GetConsoleScreenBufferInfo (m_console_handle, &console_info);
			// save the current attributes for restoration on destruction.
			get_color_stack ().push (console_info.wAttributes);
			SetConsoleTextAttribute (m_console_handle, 0x0F & m_color | 0xf0 & console_info.wAttributes); // save the background color
		}

		static std::stack<WORD>& get_color_stack()
		{
			// Use this instead of static member to avoid multiply defined symbols.
			static std::stack <WORD> color_stack;
			return color_stack;
		}

		HANDLE m_console_handle;
		const CW_COLORS m_color;

		template <typename charT, typename traits> friend std::basic_ostream <charT, traits>&
			operator << (std::basic_ostream <charT, traits>& lhs, colorwin::color const& rhs);
	};

	template <typename charT, typename traits> std::basic_ostream <charT, traits>&
		operator << (std::basic_ostream <charT, traits>& lhs, colorwin::color const& rhs)
	{
		rhs.change_color ();
		return (lhs);
	}

#elif
	enum CW_COLORS
	{
		dark_red		= 0,
		dark_yellow		= 1,
		dark_green		= 2,
		dark_cyan		= 3,
		dark_blue		= 4,
		dark_magenta	= 5,
		dark_grey		= 6,

		red				= 7,
		yellow			= 8,
		green			= 9,
		cyan			= 10,
		blue			= 11,
		magenta			= 12,
		white			= 13,
		grey			= 14,

		back_red		= 15,
		back_yellow		= 16,
		back_green		= 17,
		back_cyan		= 18,
		back_blue		= 19,
		back_magenta	= 20,
		back_grey		= 21 
	};

	class color
	{
		public:
		color (CW_COLORS) 
		{
		}

		color (color &) = delete;
		color& operator = (color) = delete;
	};

	template<typename charT, typename traits> std::basic_ostream<charT, traits> 
		&operator << (std::basic_ostream <charT, traits>& lhs, colorwin::color const&)
	{
		return (lhs);
	}

#endif // _WIN32

}

#endif 
