// To test how rectangle class works...or not!!
#include "stdafx.h"

#include <Common/genalgorithms.hpp>
#include <Common/rectangle.hpp>
#include <iostream>

/** To test intersections. */
void testIntersection (const QGAMES::Rectangle& r1, const QGAMES::Rectangle& r2, bool eR)
{
	std::cout << std::string ("Testing intersection") << std::endl;
	std::cout << "Expected result: " << (eR ? std::string ("True") : std::string ("False")) << std::endl;
	std::cout << r1 << std::endl << r2 << std::endl;
	bool i = r1.intersectsWith (r2);
	std::cout << "Intersects?: " << (i ? std::string ("True") : std::string ("False")) << std::endl;
	if (i) std::cout << r1.intersectionWith (r2) << std::endl;
	std::cout << "----" << std::endl << std::endl;
}

/** To wait for a key. */
void waitForAKey ()
{
	char t;
	std::cout << std::string ("--->Key + enter to continue");
	std::cin >> t;
}

void test1 ()
{
	QGAMES::Rectangle r1 (QGAMES::Position (__BD 0, __BD 0, __BD 0), 
		QGAMES::Position (__BD 5, __BD 5, __BD 0), QGAMES::Vector::_zNormal, __BD 0);
	QGAMES::Rectangle r2 (QGAMES::Position (__BD 1, __BD 1, __BD 1),
		QGAMES::Position (__BD 6, __BD 6, __BD 1), QGAMES::Vector::_zNormal, __BD 0);

	// General intersection...
	{
		std::cout << "Test11:" << std::endl;
		testIntersection (r1, r2, false);
		testIntersection (r1 + QGAMES::Vector (__BD 0, __BD 0, __BD 1), r2, true);
	}

	// Rotating...
	// Rotate 90 degrees...
	// in this case, throwugh the Z axis..
	{
		std::cout << "Test12:" << std::endl;
		QGAMES::Position e1 = QGAMES::Position::_cero;
		QGAMES::Position e2 = e1 + QGAMES::Vector (__BD 0, __BD 0, __BD 1);
		testIntersection (r1.rotate (r1.center (), r1.center () + QGAMES::Vector::_zNormal, __BD (__PI / 2)) + 
					QGAMES::Vector (__BD 0, __BD 0, __BD 1),
			   r2.rotate (r2.center (), r2.center () + QGAMES::Vector::_zNormal, __BD (__PI / 2)), true);
	}

	// Rotating 90 degrees in another plane...
	// In this case, through the Y axis
	{
		std::cout << "Test13:" << std::endl;
		QGAMES::Position e1 = QGAMES::Position::_cero;
		QGAMES::Position e2 = e1 + QGAMES::Vector (__BD 0, __BD 1, __BD 0);
		testIntersection (r1.rotate (e1, e2, __BD (__PI / 2)), r2.rotate (e1, e2, __BD (__PI / 2)), false);
		testIntersection (r1.rotate (e1, e2, __BD (__PI / 2)) + QGAMES::Vector (__BD 1, __BD 0, __BD 0), 
			r2.rotate (e1, e2, __BD (__PI / 2)), true);
	}

	// Rotating 90 degrees in another plane...
	// In this case, through the X axis
	{
		std::cout << "Test14:" << std::endl;
		QGAMES::Position e1 = QGAMES::Position::_cero;
		QGAMES::Position e2 = e1 + QGAMES::Vector (__BD 1, __BD 0, __BD 0);
		testIntersection (r1.rotate (e1, e2, __BD (__PI / 2)), r2.rotate (e1, e2, __BD (__PI / 2)), false);
		testIntersection (r1.rotate (e1, e2, __BD (__PI / 2)) + QGAMES::Vector (__BD 0, __BD -1, __BD 0), 
			r2.rotate (e1, e2, __BD (__PI / 2)), true);
	}

	// Rotating...
	// Rotate 30 degrees...
	// in this case, throwugh the Z axis..
	{
		std::cout << "Test15:" << std::endl;
		QGAMES::Position e1 = QGAMES::Position::_cero;
		QGAMES::Position e2 = e1 + QGAMES::Vector (__BD 0, __BD 0, __BD 1);
		testIntersection (r1.rotate (r1.center (), r1.center () + QGAMES::Vector::_zNormal, __BD (__PI / 6)) + 
					QGAMES::Vector (__BD 0, __BD 0, __BD 1),
			   r2.rotate (r2.center (), r2.center () + QGAMES::Vector::_zNormal, __BD (__PI / 6)), true);
	}

	// Rotating 30 degrees in another plane...
	// In this case, through the Y axis
	{
		std::cout << "Test16:" << std::endl;
		QGAMES::Position e1 = QGAMES::Position::_cero;
		QGAMES::Position e2 = e1 + QGAMES::Vector (__BD 0, __BD 1, __BD 0);
		testIntersection (r1.rotate (e1, e2, __BD (__PI / 6)), r2.rotate (e1, e2, __BD (__PI / 6)), false);
		testIntersection (r1.rotate (e1, e2, __BD (__PI / 6)), 
			(r2 - QGAMES::Vector (__BD 0, __BD 0, __BD 1)).rotate (e1, e2, __BD (__PI / 6)), true);
	}

	// Now it is time to test the intersection in different planes
	// It is possible to calculate whether there is intersection but not the intersection itself
	// becasue it wouldn't be a rectrangle any case but a simple line!
	{
		std::cout << "Test17:" << std::endl;
		QGAMES::Position e1 = QGAMES::Position::_cero;
		QGAMES::Position e2 = e1 + QGAMES::Vector (__BD 0, __BD 1, __BD 0);
		testIntersection (r1.rotate (e1, e2, __BD (__PI / 6)), 
			(r2 - QGAMES::Vector (__BD 0, __BD 0, __BD 1)).rotate (e1, e2, __BD (__PI / 3)), true);
	}
}

void test2 ()
{
	std::cout << "Test21:" << std::endl;
	QGAMES::Rectangle r1 (QGAMES::Position (__BD 0, __BD 0, __BD 0), 
		QGAMES::Position (__BD 5, __BD 5, __BD 0), QGAMES::Vector::_zNormal, __BD 0);
	for (int i = 0; i <= 360; i += 30)
		std::cout << r1.rotate (r1.center (), r1.center () + QGAMES::Vector::_zNormal, 
			(__BD i * __PI / __BD 180)) << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	test1 ();
	waitForAKey ();
	test2 ();
	waitForAKey ();
}
