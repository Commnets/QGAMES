/*
**
** $id:$
**
** File: ex1.cpp -- example of using ZPP library to read a file from a .ZIP
** 		 archive.
**
** Copyright (C) 1999 Michael Cuddy, Fen's Ende Software. All Rights Reserved
** modifications Copyright (C) 2000-2003 Eero Pajarre
**
** check http://zpp-library.sourceforge.net for latest version
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
** Change Log
** ----------
** $Log: Ex1.cpp,v $
** Revision 1.2  2003/06/22 19:24:02  epajarre
** Integrated all changes by me, testing still to be done
**
** Revision 1.1.1.1  2003/06/21 19:09:36  epajarre
** Initial import. (0.2) version)
**
** Revision 1.2  1999/08/22 05:21:08  mcuddy
** Revision 1.1  1999/06/10 03:34:43  mcuddy
** Initial revision
**
*/
#include "zpp.h"

/////////////////////////////////////////////////////////////////////////
//
// test harness 
//
// this program assumes that a zip file called "zpp.zip" exists in
// the directory that you are running it in.  it assumes that a file
// "readme.htm" exists in that .ZIP file.  it also assumes that there's
// a file called "notinzip.txt" in the current directory which is NOT
// in the ZIP file (to test filesystem fallback)
//
// this program does not illustrate opening of all zip files, attributes,
// etc.  it's pretty basic.
//
/////////////////////////////////////////////////////////////////////////

using namespace std;

int main()
{
	int i;
#ifdef ZPP_INCLUDE_OPENALL
	zppZipArchive::openAll ("*.zip");
	zppZipArchive::dumpGlobalMap();
#endif /* ZPP_INCLUDE_OPENALL */
	try {
#ifndef ZPP_INCLUDE_OPENALL
		zppZipArchive zf1(string("zpp.zip"));
#endif /* ZPP_INCLUDE_OPENALL */

		izppstream fp;

		fp.open("readme.htm");
		if (fp.fail()) {
			cerr << "can't open readme.htm" << endl;
		}
	
		cout << "<<contents of readme.htm>>" << endl;

		while (!fp.eof() && !fp.fail()) {
			char c;
			// do raw read of file so that newlines get read.
			// alternately, we could do fp->rdbuf() and use the
			// zppstreambuf object instead.
			fp.read(&c,1);
			cout << c;
		}
		cout << "<<end of readme.htm>>" << endl;

		// EOF will be set here, so we need to clear it.
		fp.clear(ios::goodbit);

		// we can seek to the BEGINNING of a file, but nowhere
		// else, currently.
		fp.seekg(0,ios::beg);

		i = 0;
		while (i++ < 1000 && !fp.eof() && !fp.fail()) {
			char c;
			fp.read(&c,1);
			cout << c;
		}
		cout << "<<end of dump of first 1000 bytes of readme.htm>>" << endl;

		fp.close();

		fp.open("notinzip.txt");
		if (fp.fail()) {
			cerr << "can't open notinzip.txt" << endl;
			exit (1);
		}
	
		cout << "<<contents of notinzip.txt>>" << endl;

		while (!fp.fail() && !fp.eof()) {
			char c;
			// do raw read of file so that newlines get read.
			// alternately, we could do fp->rdbuf() and use the
			// zppstreambuf object instead.
			fp.read(&c,1);
			cout << c;
		}
		cout << "<<end of notinzip.txt>>" << endl;

		// EOF will be set here, so we need to clear it.
		fp.clear(ios::goodbit);

		// we can seek to the BEGINNING of a file, but nowhere
		// else, currently.
		fp.seekg(0,ios::beg);

		i = 0;
		while (i++ < 1000 && !fp.eof() && !fp.fail()) {
			char c;
			fp.read(&c,1);
			cout << c;
		}
		cout << "<<end of dump of first 1000 bytes of notinzip.txt>>" << endl;

	} catch ( zppError e ) {
		cerr << "zppError: " << e.str << endl;
	}

    return 0;
}
