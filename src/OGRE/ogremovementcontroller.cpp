#include <CommonOGRE/ogremovementcontroller.hpp>
#include <CommonOGRE/ogreinputhandler.hpp>

// ---
void OGREFreeMovementCamera::updatePositions ()
{
	Ogre::FrameEvent* lEvnt = ((OGREInputHandler*) inputHandler ()) -> 
		handler () -> lastFrameEvent ();
	if (lEvnt == NULL)
		return; // Nothing to do with no event
	_timeBetweenTwoFrames = lEvnt -> timeSinceLastFrame; // Important for the next method...
	QGAMES::FreeMovementCamera::updatePositions ();
}
