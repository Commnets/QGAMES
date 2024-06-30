#include <Common/resourcesreader.hpp>
#include <Common/definitions.hpp>
#include <Common/genalgorithms.hpp>
#include <cassert>
#include <zpp.h>

QGAMES::ResourceReader* QGAMES::ResourceReader::_theResourceReader = NULL;

// ---
QGAMES::ResourceReader::ResourceReader ()
{
	assert (!_theResourceReader); // The only one...

	_theResourceReader = this;
}

// ---
QGAMES::ResourceReader* QGAMES::ResourceReader::resourceReader ()
{
	if (!_theResourceReader) 
		new StandardResourceReader (); // If there is none, creates a default one...
	return (_theResourceReader); 
}

// ---
QGAMES::InZipResourceReader::InZipResourceReader (const std::string& tL)
	: QGAMES::ResourceReader (),
	  _resources (),
	  _tempLocation (tL)
{
	// Opens all zip archives in the root directory...
	// This is to simplify how they are managed!
	zppZipArchive::openAll ("*.icf");
#ifndef NDEBUG
	zppZipArchive::dumpGlobalMap ();
#endif
}

// ---
QGAMES::InZipResourceReader::~InZipResourceReader ()
{
	// All files temporaly opened have to be deleted!
	for (QGAMES::InZipResourceReader::MapOfResources::const_iterator i = _resources.begin ();
			i != _resources.end (); i++)
		std::remove ((*i).second.c_str ());
	_resources.clear ();
}

// ---
void QGAMES::InZipResourceReader::setTempLocation (const std::string& tL)
{
	_tempLocation = tL;
	QGAMES::replaceStr (_tempLocation, 
		std::string (__OLDPATHWINDOWS_SEPARATOR__), std::string (__PATH_SEPARATOR__)); // Just in case...
}

// ---
std::string QGAMES::InZipResourceReader::implementationNameFor (const std::string& rN)
{
	QGAMES::InZipResourceReader::MapOfResources::const_iterator i = _resources.find (rN);
	if (i != _resources.end ())
		return ((*i).second); // If the file has already been got, then it is reused...

	// In the zlib library all path separators are UNIX style!
	// That's the reason to change many things and align all to that!
	std::string rNT = rN;
#ifdef _WIN32
	// Just in case
	QGAMES::replaceStr (rNT, std::string (__OLDPATHWINDOWS_SEPARATOR__), std::string (__PATH_SEPARATOR__));
#endif
	rNT = QGAMES::trimFullPath (rN);

	std::string dFN = QGAMES::trimFullPath (_tempLocation + std::string (__PATH_SEPARATOR__) + rNT);
	izppstream s (rNT.c_str ());
	assert (!s.fail ()); // Just in case...
	std::ofstream d (dFN.c_str (), std::ios::binary);
	assert (!d.fail ()); // Just in case...
	d << s.rdbuf ();
	s.close ();
	d.close ();

	// Adds the filename just to be reused next time...
	_resources.insert (QGAMES::InZipResourceReader::MapOfResources::value_type (rN, dFN));
	
	return (dFN);
}
