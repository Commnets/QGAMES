#include <AdvancedArcade/adscenes.hpp>

// ---
void QGAMES::SceneWithBackground::initialize ()
{
	if (_backgroundMap != NULL) 
		_backgroundMap -> initialize ();

	QGAMES::Scene::initialize ();
}

// ---
void QGAMES::SceneWithBackground::inEveryLoop ()
{
	if (_backgroundMap != NULL)
		_backgroundMap -> inEveryLoop ();

	QGAMES::Scene::inEveryLoop ();
}

// ---
void QGAMES::SceneWithBackground::updatePositions ()
{
	if (_backgroundMap != NULL)
		_backgroundMap -> updatePositions ();

	QGAMES::Scene::updatePositions ();
}

// ---
void QGAMES::SceneWithBackground::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (_backgroundMap != NULL)
		_backgroundMap -> drawOn (s, p);

	QGAMES::Scene::drawOn (s, p);
}

// ---
void QGAMES::SceneWithBackground::finalize ()
{
	if (_backgroundMap != NULL)
		_backgroundMap -> finalize ();

	QGAMES::Scene::finalize ();
}
