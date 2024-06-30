#// The MIT License(MIT)
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

#include "..\src\colorwin.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace colorwin;

int main()
{
    string separator(40, '=');

    cout << color(white) << "DemoAllColors()\n";
    cout << color(red) << "red\n";
    cout << color(yellow) << "yellow\n";
    cout << color(green) << "green\n";
    cout << color(cyan) << "cyan\n";
    cout << color(blue) << "blue\n";
    cout << color(magenta) << "magenta\n";
    cout << color(white) << "white\n";
    cout << color(grey) << "grey\n";
    cout << color(dark_grey) << "dark_grey\n";

    cout << "\n\nHit the enter key to exit.";
    cin.get();
    return 0;
}