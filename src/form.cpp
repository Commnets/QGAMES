#include <Common/form.hpp>
#include <Common/screen.hpp>
#include <Common/game.hpp>

// ---
QGAMES::Form::Form (int id, const std::string& n, int nf)
	: _id (id),
	  _name (n),
	  _numberFrames (nf),
	  _frames ()
{
	_frames.resize (_numberFrames);
	for (int i = 0; i < _numberFrames; i++)
		_frames [i] = NULL;
}

// ---
QGAMES::Form::~Form ()
{
	for (int i = 0; i < _numberFrames; i++)
		delete (_frames [i]);
}

// ---
void QGAMES::Form::preLoad ()
{
	for (int i = 0; i < _numberFrames; i++)
		if (_frames [i] == NULL)
			_frames [i] = frame (i); 
}

// ---
QGAMES::Frame* QGAMES::Form::frame (int nf)
{
	return ((_frames [nf] == NULL) ? (_frames [nf] = createFrame (nf)) : _frames [nf]);
}

// ---
void QGAMES::Form::drawOn (QGAMES::Screen* s, int nf, const QGAMES::Position& p, int a)
{
	assert (s);

	QGAMES::Frame* f = frame (nf);
	f -> drawOn (s, p, a);
}
