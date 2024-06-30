// zipTest.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include <zpp.h>

int _tmain (int argc, _TCHAR* argv[])
{
	zppZipArchive::openAll ("*.zip");
	zppZipArchive::dumpGlobalMap ();

	// Reading and printing one file...
	char c;
	std::string nF1 ("readme.htm");
	izppstream fp1 (nF1.c_str ());
	if (fp1.fail ()) 
		return (1);
	while (!fp1.eof () && !fp1.fail ()) 
	{
		fp1.read (&c,1);
		std::cout << c;
	}

	// Copying a file...
	std::string nF2 ("troll.png");
	izppstream fp2 (nF2.c_str ());
	std::string nFC2 ("troll.png");
	std::ofstream fpC2 (nFC2.c_str (), std::ios::binary);
	fpC2 << fp2.rdbuf ();
	fpC2.close ();
	fp2.close ();

    return 0;
}

