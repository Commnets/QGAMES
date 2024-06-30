#include <CommonOGRE/ogreengine.hpp>
#include <CommonOGRE/ogregame.hpp>
#include <assert.h>

// ---
OGREElementAdditionalData::OGREElementAdditionalData ()
	: QGAMES::Element::ExternalData (),
	  _ogreSceneManager (NULL), 
	  _ogreSceneNode (NULL), // For all...
	  _ogreEntity (NULL), 
	  _ogreAnimationState (NULL), // For entities...
	  _lastForm (NULL), 
	  _lastAnimation (NULL),
	  _ambientLight (QGAMES::Color (0, 0, 0)), // For scenes...(black by default)
	  _lightPosition (QGAMES::Position::_cero)
{
	// Do nothing
}

// ---
OGREExternalEngine::OGREExternalEngine (OGREGame* gI)
	: _ogreGame (gI)
{ 
	assert (_ogreGame); // It can't be null...
}

// ---
void OGREExternalEngine::whenCreateGUISystem (QGAMES::GUISystem* sys)
{
	_ogreGame -> whenCreateGUISystem (sys);
}

// ---
void OGREExternalEngine::whenDeleteGUISystem (QGAMES::GUISystem* sys)
{
	_ogreGame -> whenDeleteGUISystem (sys);
}

// ---
void OGREExternalEngine::whenInitialize (QGAMES::GUISystem* sys)
{
	_ogreGame -> whenInitialize (sys);
}

// ---
void OGREExternalEngine::whenInEveryLoop (QGAMES::GUISystem* sys)
{
	_ogreGame -> whenInEveryLoop (sys);
}

// ---
void OGREExternalEngine::whenDrawOn (QGAMES::GUISystem* sys, QGAMES::Screen* s, const QGAMES::Position& p)
{
	_ogreGame -> whenDrawOn (sys, s, p);
}

// ---
void OGREExternalEngine::whenCreateElement (QGAMES::Element* elmnt)
{ 
	_ogreGame -> whenCreateElement (elmnt); 
}

// ---
void OGREExternalEngine::whenDeleteElement (QGAMES::Element* elmnt)
{
	_ogreGame -> whenDeleteElement (elmnt);
}

// ---
void OGREExternalEngine::whenInitialize (QGAMES::Element* elmnt)
{ 
	_ogreGame -> whenInitialize (elmnt); 
}

// ---
void OGREExternalEngine::whenInEveryLoop (QGAMES::Element* elmnt)
{ 
	_ogreGame -> whenInEveryLoop (elmnt); 
}

// ---
void OGREExternalEngine::whenUpdatePositions (QGAMES::Element* elmnt)
{ 
	_ogreGame -> whenUpdatePositions (elmnt);
}

// ---
void OGREExternalEngine::whenDrawOn (QGAMES::Element* elmnt, QGAMES::Screen* s, const QGAMES::Position& p)
{ 
	_ogreGame -> whenDrawOn (elmnt, s, p); 
}

// ---
void OGREExternalEngine::whenFinalize (QGAMES::Element* elmnt)
{ 
	_ogreGame -> whenFinalize (elmnt); 
}
