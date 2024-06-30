#include <CommonOGRE/ogreform.hpp>

// ---
OGREForm::OGREForm (int id, const std::string& n, const Ogre::MeshPtr& mesh)
	: QGAMES::Form (id, n, 1), //& The mesh has just one frame only...
	  _mesh (mesh)
{
	assert (mesh);
}

// ---
int OGREForm::width () const
{ 
	// The mesh is in a box, then their internal variables can be calculated 
	// taking into acount the different corners of the box...
	Ogre::AxisAlignedBox limits = _mesh -> getBounds ();
	return ((int) 
		(limits.getCorner (Ogre::AxisAlignedBox::FAR_LEFT_TOP) - 
		 limits.getCorner (Ogre::AxisAlignedBox::FAR_RIGHT_TOP)).length ()); 
}

// ---
int OGREForm::height () const
{
	Ogre::AxisAlignedBox limits = _mesh -> getBounds ();
	return ((int) 
		(limits.getCorner (Ogre::AxisAlignedBox::FAR_LEFT_TOP) - 
		 limits.getCorner (Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM)).length ()); 
}

// ---
int OGREForm::depth () const
{
	Ogre::AxisAlignedBox limits = _mesh -> getBounds ();
	return ((int) 
		(limits.getCorner (Ogre::AxisAlignedBox::FAR_LEFT_TOP) - 
		 limits.getCorner (Ogre::AxisAlignedBox::NEAR_LEFT_TOP)).length ()); 
}

// ---
QGAMES::Position OGREForm::centerPosition (int nF) const
{
	// No offset considered in the ogre forms...
	return (frameSizeVectorForFrame (nF) / __BD 2);
}

// ---
QGAMES::Position OGREForm::basePosition (int nF) const
{
	// No offset considered in the ogre forms...
	// It is supossed that usually the form is oriented in the plane XY...
	return (QGAMES::Vector (
		__BD (frameWidthForFrame (nF) >> 1), __BD frameHeightForFrame (nF), __BD (frameDepthForFrame (nF) >> 1)));
}

// ---
QGAMES::Rectangle OGREForm::roundingCentralZone (int nF) const
{
	// No offset considered in the ogree forms...
	QGAMES::Position p1 = QGAMES::Position (__BD 0, __BD 0, __BD (frameDepthForFrame (nF) >> 1));
	QGAMES::Position p2 = p1 + QGAMES::Vector (__BD frameWidthForFrame (nF), __BD frameHeightForFrame (nF), __BD 0);

	return (QGAMES::Rectangle (p1, p2, QGAMES::Vector::_zNormal));
}

// ---
QGAMES::Rectangle OGREForm::coverZone (int nF) const
{
	// No offset considered in the ogre forms...
	QGAMES::Position p1 = QGAMES::Position::_cero;
	QGAMES::Position p2 = p1 + QGAMES::Vector (__BD frameWidthForFrame (nF), __BD 0, __BD frameDepthForFrame (nF));

	return (QGAMES::Rectangle (p1, p2, QGAMES::Vector::_yNormal));
}

// ---
QGAMES::Rectangle OGREForm::baseZone (int nF) const
{
	// No offset considered on the ogree forms...
	return (coverZone (nF) + QGAMES::Vector (__BD 0, __BD frameHeightForFrame (nF), __BD 0));
}

// ---
OGRE2DForm::OGRE2DForm (int id, const std::string& n, int nf, const std::string& s, int w, int h, 
		QGAMES::Sprite2DFormData* fD, Ogre::Image* image)
	: QGAMES::Sprite2D (id, n, nf, s, w, h, fD),
	  _image (image)
{
	assert (_image);
}

// ---
OGRE2DForm::~OGRE2DForm ()
{
	delete (_image);
	_image = NULL;
}
