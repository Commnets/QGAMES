#include <Common/form3d.hpp>
#include <Common/screen.hpp>

// ---
QGAMES::Form3DData::Face QGAMES::Form3DData::Face::rotated 
	(const QGAMES::Position& p, const QGAMES::Vector& v, QGAMES::bdata ang) const
{ 
	QGAMES::Positions nP; 
	for (auto i : _positions) 
		nP.push_back (i.rotate (p, p + v, ang));

	return (QGAMES::Form3DData::Face (_id, nP, _color));
}

// ---
void QGAMES::Form3DData::Face::drawOn (QGAMES::Screen* scr, const QGAMES::Position& pos, int a)
{
	QGAMES::Color c = _color;
	c.setAlpha (a);

	for (int i = 0; i <= ((int) _positions.size () - 1); i += 2)
		scr -> drawTriangle (pos + _positions [i], pos + _positions [i + 1], 
			pos + (((i + 2) == ((int) _positions.size () + 2)) ? _positions [0] : _positions  [i + 2]), c, true);
}

// ---
bool QGAMES::Form3DData::Member::existsFace (int id) const
{
	bool result = false;
	for (int i = 0; i < (int) _faces.size () && !result; i++)
		if (_faces [i].id () == id) result = true;

	return (result);
}

// ---
const QGAMES::Form3DData::Face& QGAMES::Form3DData::Member::face (int id) const
{
	int result = -1;
	for (int i = 0; i < (int) _faces.size () && result == 1; i++)
		if (_faces [i].id () == id) result = i;

	assert (result != -1);
	return (_faces [result]);
}

// ---
QGAMES::Box QGAMES::Form3DData::Member::containingBox () const
{
	QGAMES::Boxes bxs;
	for (auto i : _faces)
		bxs.push_back (QGAMES::Box::boxToInclude (i.positions ()));
	return (QGAMES::Box::boxToInclude (bxs));
}

// ---
QGAMES::Form3DData::Member QGAMES::Form3DData::Member::rotate 
	(const QGAMES::Position& p, const QGAMES::Vector& v, QGAMES::bdata ang) const
{
	QGAMES::Form3DData::Faces nF = _faces;
	for (auto i : nF) 
		i.selfRotate (p, v, ang);

	return (QGAMES::Form3DData::Member (_id, nF, _referencePosition.rotate (p, p + v, ang)));
}

// ---
QGAMES::Box QGAMES::Form3DData::Articulation::containingBox () const
{
	QGAMES::Boxes bxs;
	for (auto i : _membersLinked)
		bxs.push_back (i.containingBox ()); // The box containing all members...
	for (auto i : _articulationsLinked)
		bxs.push_back (i.second.containingBox ()); // The box containing all articulation (recursive)

	return (QGAMES::Box::boxToInclude (bxs)); 
}

// ---
QGAMES::Form3DData::Articulation QGAMES::Form3DData::Articulation::rotate 
	(const QGAMES::Position& p, const QGAMES::Vector& v, QGAMES::bdata ang) const
{
	QGAMES::Form3DData::Articulations aLinked = _articulationsLinked;
	for (auto i : aLinked)
		i.second.selfRotate (p, v, ang);

	QGAMES::Form3DData::Members mLinked = _membersLinked;
	for (auto i : mLinked)
		i.selfRotate (p, v, ang);

	return (QGAMES::Form3DData::Articulation 
		(_id, _relativePosition.rotate (p, p + v, ang), _orientation, aLinked, mLinked));
}

// ---
void QGAMES::Form3DData::Articulation::selfRotate 
	(int id, const QGAMES::Position& p, const QGAMES::Vector& v, QGAMES::bdata ang)
{
	assert (existsSubArticulation (id));

	(*_articulationsLinked.find (id)).second.selfRotate (p, v, ang);
}

// ---
void QGAMES::Form3DData::Articulation::selfRotate 
	(const std::string& pId, const QGAMES::Position& p, const QGAMES::Vector& v, QGAMES::bdata ang)
{
	std::vector <int> aIds = QGAMES::getElementsFromAsInt (pId, ',');
	assert (!aIds.empty ());

	if ((int) aIds.size () == 1)
		selfRotate (aIds [0], p, v, ang);
	else
		selfRotate (pId.substr (pId.find_first_of (',')), p, v, ang);
}

// ---
void QGAMES::Form3DData::Articulation::selfRotate (int id, QGAMES::bdata ang)
{
	assert (existsSubArticulation (id));

	(*_articulationsLinked.find (id)).second.selfRotate (ang);
}

// ---
void QGAMES::Form3DData::Articulation::selfRotate (const std::string& pId, QGAMES::bdata ang)
{
	std::vector <int> aIds = QGAMES::getElementsFromAsInt (pId, ',');
	assert (!aIds.empty ());

	if ((int) aIds.size () == 1)
		selfRotate (aIds [0], ang);
	else
		selfRotate (pId.substr (pId.find_first_of (',')), ang);
}

// ---
void QGAMES::Form3DData::Articulation::drawOn (QGAMES::Screen* scr, const QGAMES::Position& pos, int a)
{
	for (auto i : _membersLinked)
		i.drawOn (scr, pos, a);

	for (auto i : _articulationsLinked)
		i.second.drawOn (scr, pos, a);
}

// ---
QGAMES::Form3DData::Form3DData 
		(const QGAMES::Form3DData::Articulations& sk, const QGAMES::Form3DData::FramesInfo& fI)
	: _skeleton (sk),
	  _framesInfo (fI)
{ 
#ifndef NDEBUG
	// To validate that the frames info has been well built!
	bool valid = true;
	for (int i = 0; i < (int) _framesInfo.size () && valid; i++) // Per frame...
	{
		for (QGAMES::Form3DData::ArticulationTurnInfoList::const_iterator j = _framesInfo [i]._turns.begin ();
				j != _framesInfo [i]._turns.end () && valid; j++)
		{
			std::vector <int> aIds = QGAMES::getElementsFromAsInt ((*j)._articulationIdPath, ',');
			assert (!aIds.empty ()); // The turn has to affect at the 1 articulation, so the list can't be empty
			
			QGAMES::Form3DData::Articulations::const_iterator aP = _skeleton.find (aIds [0]);
			if (valid = (aP == _skeleton.end ()) && (int) aIds.size () > 1)
			{
				QGAMES::Form3DData::Articulation art = (*aP).second;
				for (int k = 1; k < (int) aIds.size () && valid; k++)
					if (valid = art.existsSubArticulation (aIds [k]))
						art = art.subArticulation (aIds [k]); // Navigating in the articulations inside...
			}
		}
	}

	assert (valid);
#endif
}

// ---
QGAMES::Frame* QGAMES::Form3D::createFrame (int nf)
{
	assert (_data);
	assert (nf < numberFrames ());

	QGAMES::Form3DData::Articulations nSk = _data -> _skeleton; // A copy...

	for (int i = 0; i < (int) _data -> _framesInfo [nf]._turns.size (); i++)
	{
		std::vector <int> fId = QGAMES::getElementsFromAsInt (_data -> _framesInfo [nf]._turns[i]._articulationIdPath, ',');
		if ((int) fId.size () == 1)
			(*nSk.find (fId [0])).second.selfRotate (_data -> _framesInfo [nf]._turns[i]._angleToTurn);
		else
			(*nSk.find (fId [0])).second.selfRotate 
				(_data -> _framesInfo [nf]._turns[i]._articulationIdPath.substr 
					(_data -> _framesInfo [nf]._turns[i]._articulationIdPath.find_first_of (',')), 
						_data -> _framesInfo [nf]._turns[i]._angleToTurn);
	}

	return (new QGAMES::Form3DFrame (this, nf, nSk));
}

// ---
QGAMES::Box QGAMES::Form3D::maxContainingBox () const
{
	if (_maxContainingBox != QGAMES::Box::_noBox)
		return (_maxContainingBox);

	QGAMES::Boxes bxs;
	for (int i = 0; i < (int) _frames.size (); i++)
		bxs.push_back (frame3D (i) -> roundingBox ());

	return (_maxContainingBox = QGAMES::Box::boxToInclude (bxs));
}

// ---
QGAMES::Form3DFrame::Form3DFrame (QGAMES::Form* f, int nf, const QGAMES::Form3DData::Articulations& sk)
	: QGAMES::Frame (f, nf),
	  _skeleton (sk)
{ 
	assert (dynamic_cast <Form3D*> (_originalForm) != NULL); 
	assert (!_skeleton.empty ()); // Minimum one articulation...

	QGAMES::Boxes bxs;
	for (auto i : _skeleton)
		bxs.push_back (i.second.containingBox ());
	_frameLimits = QGAMES::Box::boxToInclude (bxs); // Calculated once...
}


// ---
void QGAMES::Form3DFrame::drawOn (QGAMES::Screen* s, const QGAMES::Position& p, int a)
{ 
	for (auto i : _skeleton) // For all articulations...
		i.second.drawOn (s, (p == QGAMES::Position::_noPoint) ? QGAMES::Position::_cero : p, a); 
}
