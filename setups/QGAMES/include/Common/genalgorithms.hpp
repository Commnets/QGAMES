/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: genalgorithms.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/01/2017 \n
 *	Description: Defines common routines for the game library. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_GENALGORITHMS__
#define __QGAMES_GENALGORITHMS__

#include <Common/definitions.hpp>
#include <vector>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace QGAMES
{
	// Managing strings...
	std::string& ltrim (std::string& s); 
	std::string& rtrim (std::string& s);
	std::string& trim (std::string& s);
	std::string& replaceStr (std::string& s, const std::string& s1, const std::string& s2);
	std::string& toUpper (std::string& s);
	std::string& toLower (std::string& s);

	// Generate a random id...
	std::string generateId (const std::string& base = std::string (__NULL_STRING__),
		unsigned int lenId = 10);

	// Seconds from the beginning...
	int secondsFromBeginning ();

	// Managing list of things...
	/** To extract the elements from a "txt", separated by "ch" up to a maximum of "n" */
	std::vector <std::string> getElementsFrom (const std::string& txt, char ch, int n = -1);
	/** Same, but converting them to int */
	std::vector <int> getElementsFromAsInt (const std::string& txt, char ch, int n = -1);
	/** Same but converting them into a QGAMES::bdata */
	std::vector <QGAMES::bdata> getElementsFromAsBDATA (const std::string& txt, char ch, int n = -1);

	// Managing file system...
	std::string onlyFileName (const std::string& fName);
	std::string onlyPath (const std::string& fName);
	std::string fileWithoutExtension (const std::string& fName);
	std::string fileExtension (const std::string& fName);
	std::string trimFullPath (const std::string& fName);
	std::vector <std::string> listOfFiles (const std::string& dr); // To get the list of file in a directory..
	std::vector <std::string> listOfDirs (const std::string& dr); // To get the list of directories in a dir

	// Managing numbers
	/** To get the sgn of a number */
	template <typename T> int sgn (T val)
							{ return (0 < val) - (val < 0); }
	/** To determinate whether a number if or not between two. Strictly between! */
	template <typename T> bool valueBetween (T val, T min, T max)
							{ return (val > min && val < max); }
	/** To determinate whether a pair of values are in a square. 
		The limits have to be ordered otherwise the method won't work properly. Strictly in! */
	template <typename T> bool parOfValuesInSquare (T valX, T valY, T minX, T maxX, T minY, T maxY)
							{ return (valueBetween (valX, minX, maxX) && valueBetween (valY, minY, maxY)); }
	/** To determinate whether a square of values intersects other. Strictly intersects! */
	template <typename T> bool squareIntersectSquare 
		(T min1X, T max1X, T min1Y, T max1Y, T min2X, T max2X, T min2Y, T max2Y)
							{ return (parOfValuesInSquare (min1X, min1Y, min2X, max2X, min2Y, max2Y) ||
									  parOfValuesInSquare (max1X, min1Y, min2X, max2X, min2Y, max2Y) ||
									  parOfValuesInSquare (min1X, max1Y, min2X, max2X, min2Y, max2Y) ||
									  parOfValuesInSquare (max1X, max1Y, min2X, max2X, min2Y, max2Y) ||
									  parOfValuesInSquare (min2X, min2Y, min1X, max1X, min1Y, max1Y) ||
									  parOfValuesInSquare (max2X, min2Y, min1X, max1X, min1Y, max1Y) ||
									  parOfValuesInSquare (min2X, max2Y, min1X, max1X, min1Y, max1Y) ||
									  parOfValuesInSquare (max2X, max2Y, min1X, max1X, min1Y, max1Y)); }

	// Data used by the point, polygon, rectangle, etc... to round errors and calculations
	bdata adjustDecimals (bdata a);
	static int __QGAMES_ADJUSTDECIMALS = 6; // It can be changed at runtime
	static bdata __QGAMES_ERROR = __BD 0.001; // It can be changed at runtime
}

#endif
  
// End of the file
/*@}*/
