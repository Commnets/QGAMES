#include <Common/genalgorithms.hpp>
#include <Common/definitions.hpp>
#include <Common/dirent.h>
#include <time.h>
#include <sstream>
#include <cmath>
#include <time.h>

// ---
std::string& QGAMES::ltrim (std::string& s) 
{ 
	s.erase (s.begin (), std::find_if (s.begin (), s.end (),
		std::not1 (std::ptr_fun <int, int> (std::isspace))));
	return (s); 
}

// ---
std::string& QGAMES::rtrim (std::string& s) 
{ 
	s.erase (std::find_if (s.rbegin (), s.rend (),
		std::not1 (std::ptr_fun <int, int> (std::isspace))).base (), s.end ());
	return (s); 
}

// ---
std::string& QGAMES::trim (std::string& s) 
{ 
	return (ltrim (rtrim (s))); 
}

// ---
std::string& QGAMES::replaceStr (std::string& s, const std::string& s1, const std::string& s2)
{
	size_t pos = 0;
	while ((pos = s.find (s1, pos)) != std::string::npos) 
	{
		 s.replace(pos, s1.length (), s2);
		 pos += s2.length ();
	}

	return (s);
}

// ---
std::string& QGAMES::toUpper (std::string& s)
{
	std::transform (s.begin (), s.end (), s.begin (),
		[] (unsigned char c) { return std::toupper (c); });
	return (s);
}

// ---
std::string& QGAMES::toLower (std::string& s)
{
	std::transform (s.begin (), s.end (), s.begin (),
		[] (unsigned char c) { return std::tolower (c); });
	return (s);
}

// ---
std::string QGAMES::generateId (const std::string& base, unsigned int lenId)
{
	std::string result = std::string (__NULL_STRING__);

	time_t t; time (&t); 
	std::stringstream tChrStr; tChrStr << t; 
	result = tChrStr.str (); 

	char pChr [6]; pChr [5] = 0;
	for (int i = 0; i < 5; i++)
		pChr [i] = 65 + (int) (((double) rand ()) / ((double) RAND_MAX) * ((double) 26));
	result += pChr;

	for (int j = 0; j < 5; j++)
	{
		unsigned int posA = 
			(unsigned int) (((double) rand ()) / ((double) RAND_MAX) * ((double) result.size ()));
		unsigned int posB = 
			(unsigned int) (((double) rand ()) / ((double) RAND_MAX) * ((double) result.size ()));
		if (posA != posB)
		{
			char tmpChr = result [posA];
			result [posA] = result [posB];
			result [posB] = tmpChr;
		}
	}

	return (result);
}

// ---
int QGAMES::secondsFromBeginning ()
{
	return ((int) time (NULL));
}

// ---
std::vector <std::string> QGAMES::getElementsFrom (const std::string& txt, char ch, int nE)
{
	std::vector <std::string> result;

	if (nE == -1)
		nE = 999999; // No limit...

	int i = 0;
	std::string cpTxt = txt;
	while (cpTxt != std::string (__NULL_STRING__) && i < nE)
	{
		std::string prt = cpTxt;
		int pC = (int) cpTxt.find_first_of (ch);
		if (pC == 0) { prt = std::string (__NULL_STRING__); cpTxt = cpTxt.substr (pC + 1); }
		else if (pC == ((int) cpTxt.size () - 1)) { prt = cpTxt.substr (0, pC); cpTxt = std::string (__NULL_STRING__); }
		else if (pC == -1) { cpTxt = std::string (__NULL_STRING__); }
		else { prt = cpTxt.substr (0, pC); cpTxt= cpTxt.substr (pC + 1); }
		result.push_back (prt);
	}

	// Adjust the size...
	if ((int) result.size () < nE && nE != 999999) 
		for (int i = (int) result.size (); i < nE; result.push_back (std::string (__NULL_STRING__)), i++);
	if ((int) result.size () > nE && nE != 999999)
		for (int i = (int) result.size (); i > nE; result.pop_back (), i++);

	return (result);
}

// ---
std::vector <int> QGAMES::getElementsFromAsInt (const std::string& txt, char ch, int n)
{
	std::vector <int> result;
	std::vector <std::string> txtResult = getElementsFrom (txt, ch, n);
	for (std::vector <std::string>::const_iterator i = txtResult.begin (); i != txtResult.end (); i++)
		result.push_back ((*i) == std::string (__NULL_STRING__) ? 0 : std::stoi ((*i)));
	return (result);
}

// ---
std::vector <QGAMES::bdata> QGAMES::getElementsFromAsBDATA (const std::string& txt, char ch, int n)
{
	std::vector <QGAMES::bdata> result;
	std::vector <std::string> txtResult = QGAMES::getElementsFrom (txt, ch, n);
	for (std::vector <std::string>::const_iterator i = txtResult.begin (); i != txtResult.end (); i++)
		result.push_back ((*i) == std::string (__NULL_STRING__) ? 0 : __BD std::stof ((*i)));
	return (result);
}

// ---
std::string QGAMES::onlyFileName (const std::string& fName)
{ 
	std::string result = (fName.find_last_of (__PATH_SEPARATOR__) != -1)
		? ((fName.find_last_of (__PATH_SEPARATOR__) != fName.length () - 1) 
			? fName.substr (fName.find_last_of (__PATH_SEPARATOR__) + 1) : std::string (__NULL_STRING__)) : fName;
	// This takes into account that there is no filename at all, then a null string is returned...
	
	// To avoid a parameter that is really a command to change the directory...
	if (result == std::string (".") ||
		result == std::string (".."))
		result = std::string (__NULL_STRING__);

	return (result);
}

// ---
std::string QGAMES::onlyPath (const std::string& fName)
{ 
	std::string result = (fName.find_last_of (__PATH_SEPARATOR__) != -1)
		? fName.substr (0, fName.find_last_of (__PATH_SEPARATOR__)) : std::string (__NULL_STRING__);

	// This is to cover situations like:
	// 'xxxxx/./xxxxx
	// 'xxxxx/../xxxxx
	// o any combination of those...
	std::string previosPath (__NULL_STRING__);
	if (result.find_last_of (__PATH_SEPARATOR__) != -1 &&
		result.find_last_of (__PATH_SEPARATOR__) != (result.length () - 1)) // To avoid 'xxxxx// (this is a mistake in the file name)
	{
		std::string lastPath (result.substr (result.find_last_of (__PATH_SEPARATOR__) + 1));
		if (lastPath == std::string (".."))
			result = QGAMES::onlyPath (result.substr (0, result.find_last_of (__PATH_SEPARATOR__)));
		else
		if (lastPath == std::string ("."))
			result = result.substr (0, result.find_last_of (__PATH_SEPARATOR__));
		// Any other value is a valid directory...
	}

	return (result);
}

// ---
std::string QGAMES::fileWithoutExtension (const std::string& fName)
{
	std::string result = fName;
	size_t pPos = fName.find_last_of ('.');
	if (pPos != std::string::npos)
	{
		if (pPos == 0) result = std::string (__NULL_STRING__);
		else result = fName.substr (0, pPos);
	}

	return (result);
}

// ---
std::string QGAMES::fileExtension (const std::string& fName)
{
	std::string result (__NULL_STRING__);
	size_t pPos = fName.find_last_of ('.');
	if (pPos != std::string::npos && pPos != (fName.length () - 1))
		result = fName.substr (pPos + 1);
	return (result);
}

// ---
std::string QGAMES::trimFullPath (const std::string& fName)
{
	std::string result = fName;
	result = QGAMES::trim (result);
	while (result.substr (0, 2) == std::string (".") + std::string (__PATH_SEPARATOR__))
		result = result.substr (2);
	return (result);
}

// ---
std::vector <std::string> QGAMES::listOfFiles (const std::string& dr)
{
	std::vector <std::string> result;

	DIR *dir;
	struct dirent *ent;

	/* Open directory stream */
	dir = opendir (dr.c_str ());
	if (dir != NULL) 
	{
		while ((ent = readdir (dir)) != NULL) 
			if (ent -> d_type == DT_REG)
				result.push_back (std::string (ent -> d_name));
		closedir (dir);
	}

	return (result);
}

// ---
std::vector <std::string> QGAMES::listOfDirs (const std::string& dr)
{
	std::vector <std::string> result;

	DIR *dir;
	struct dirent *ent;

	/* Open directory stream */
	dir = opendir (dr.c_str ());
	if (dir != NULL) 
	{
		while ((ent = readdir (dir)) != NULL) 
			if (ent -> d_type == DT_DIR)
				result.push_back (std::string (ent -> d_name));
		closedir (dir);
	}

	return (result);
}

// ---
QGAMES::bdata QGAMES::adjustDecimals (QGAMES::bdata a)
{
	return (__BD (((int) (pow (10,__QGAMES_ADJUSTDECIMALS) * a)) / 
		pow (10, __QGAMES_ADJUSTDECIMALS)));
}
