#include "Artists.hpp"

// ---
BattleshipII::MothershipUFO::MothershipUFO (int cId, 
		BATTLESHIP::UFO::Behaviour* bhv, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: BATTLESHIP::BigUFO (cId, bhv, f, d),
	  _explosionFormId (50100), _explosionInitialFrame (0), _explosionFinalFrame (24),
	  _explosionStatus (NULL)
{
	counters () -> addCounter (new QGAMES::Counter (_COUNTEREXPLODING, game () -> framesPerSecond () * 6, 0, true, false));
	counters () -> addCounter (new QGAMES::Counter (_COUNTERTOCHANGEANIMATION, 6, 0, true, true));
}

// ---
QGAMES::Entity* BattleshipII::MothershipUFO::clone () const
{
	QGAMES::Entity* result = new BattleshipII::MothershipUFO 
		(_id, dynamic_cast <BATTLESHIP::Spaceship::Behaviour*> (_behaviour -> clone ()), _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
bool BattleshipII::MothershipUFO::toShoot (int f, const QGAMES::Vector& d, int nP)
{
	bool result = true;
	int j = 0;
	for (auto i : centralShootingPositionsForFormAndAspect (currentForm () -> id (), currentAspect ()))
	{
		if (__2IN3PROBABLE__)
			result &= BATTLESHIP::BigUFO::toShoot 
				(f, (i - QGAMES::Position::_cero).normalize (), j);
		j++;
	}
	
	return (result);
}

// ---
QGAMES::Position BattleshipII::MothershipUFO::shootingPosition (int nS, int tS, int nP) const
{
	assert (tS > nS && nS >= 0);

	QGAMES::bdata spc = __BD 20; // The total separation...
	QGAMES::Position iPos	= centerPosition () +
		centralShootingPositionsForFormAndAspect (currentForm () -> id (), currentAspect ())[nP];
	if (tS > 1)
		iPos += ((-spc / __BD 2) + (spc * __BD nS / __BD (tS - 1))) * 
			iPos.crossProduct (QGAMES::Vector::_zNormal).normalize ();
	
	return (iPos);
}

// ---
void BattleshipII::MothershipUFO::updatePositions ()
{
	BATTLESHIP::BigUFO::updatePositions ();

	if (isExploding ())
	{
		if (counter (_COUNTERTOCHANGEANIMATION) -> isEnd ())
		{
			for (auto i : _explosionStatus)
			{
				if (i -> _beingUsed)
				{
					if ((++i -> _numberAspect) > _explosionFinalFrame)
					{
						i -> _beingUsed = false;
						i -> _numberAspect = _explosionInitialFrame;
					}
				}
				else
				{
					if ((rand () % 100) > 90 && (i -> _numberTimesUsed < 3))
					{
						i -> _beingUsed = true;
						i -> _numberTimesUsed++;

						game () -> sound (__QGAMES_EXPLOSIONWAVSOUND__) -> play (-1); // To allow multiple...
					}
				}
			}
		}

		if (counter (_COUNTEREXPLODING) -> isEnd ())
		{
			setStateFor (_spaceElementState = _DESTROYED, orientation ());

			notify (QGAMES::Event (__BATTLESHIP_SPACEELMNTHASDESTROYED__, this));

			deleteExplosionStatus ();
		}
	}
}

// ---
void BattleshipII::MothershipUFO::drawOn (QGAMES::Screen* scr, const QGAMES::Position& pos)
{
	BATTLESHIP::BigUFO::drawOn (scr, pos);

	if (isExploding ())
	{
		for (auto i : _explosionStatus)
		{
			if (i -> _beingUsed)
			{
				QGAMES::Form* frm = game () -> form (_explosionFormId);
				QGAMES::Vector ctr =  frm -> centerPosition (i -> _numberAspect);
				frm -> drawOn (scr, i -> _numberAspect,
					((pos == QGAMES::Position::_noPoint) ? position () : pos) + (i -> _position - ctr) /** relative to the center */);
			}
		}
	}
}

// ---
void BattleshipII::MothershipUFO::setStateFor (int st, const QGAMES::Vector& o)
{
	BATTLESHIP::BigUFO::setStateFor (st, o);

	// A special state has been defined for big ufos when exploding
	// In that state (@see entities.xml) the animation is the same than when it stands still!
	// On top of it several explosions have to take place!
	// Now it is time to create the structure to control them...
	if (spaceElementState () == _EXPLODING)
	{
		int xDim = 5; int yDim = 5;
		_explosionStatus = std::vector <BattleshipII::MothershipUFO::ExplosionStatus*> (xDim * yDim);
		QGAMES::bdata w = __BD visualLength () / __BD xDim;
		QGAMES::bdata h = __BD visualHeight () / __BD yDim;

		for (int i = 0; i < yDim; i++)
		{
			for (int j = 0; j < xDim; j++)
				_explosionStatus [(i * xDim) + j] = new BattleshipII::MothershipUFO::ExplosionStatus 
					(false, 0, QGAMES::Position ((w * __BD j) + (w / __BD 2), (h * i) + (h / __BD 2), __BD 0), 
						// The centre of the square where to paint!
						_explosionInitialFrame);
		}
	}
}

// ---
QGAMES::Positions BattleshipII::MothershipUFO::calculateCentralShootingPositionsForFormAndAspect (int f, int a) const
{
	QGAMES::Positions result;

	if (type () == __BATTLESHIPII_ENEMTYTYPE1012__)
	{
		QGAMES::bdata ang = __BD (6 * a);
		QGAMES::bdata fct = __PI / __BD 180;
		result = QGAMES::Positions 
			{ __BD 180 * -QGAMES::Position::_yNormal, 
			  __BD 180 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 60.0 + ang) * fct),
			  __BD 180 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 120.0 + ang) * fct),
			  __BD 180 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 180.0 + ang) * fct),
			  __BD 180 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 240.0 + ang) * fct),
			  __BD 180 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 300.0 + ang) * fct)
			};
	}
	else
	{
		// The position of the shooting zones are the same for the aspect 
		// __BATTLESHIPII_ENEMTYTYPE1010__ and __BATTLESHIPII_ENEMTYTYPE1011__
		QGAMES::bdata ang = __BD (4 * a);
		QGAMES::bdata fct = __PI / __BD 180;
		result = QGAMES::Positions 
			{ __BD 280 * -QGAMES::Position::_yNormal, 
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 24.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 48.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 72.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 96.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 120.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 144.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 168.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 192.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 216.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 240.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 264.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 288.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 312.0 + ang) * fct),
			  __BD 280 * -QGAMES::Position::_yNormal.rotate 
				(QGAMES::Position::_cero, QGAMES::Position::_zNormal, (__BD 336.0 + ang) * fct),
			};
	}

	return (result);
}

// --
QGAMES::Rectangles BattleshipII::MothershipUFO::calculateWeakAreasForFormAndAspect (int f, int a) const
{
	QGAMES::Positions cPos = centralShootingPositionsForFormAndAspect (f, a);

	QGAMES::Rectangles result;
	QGAMES::Vector d (__BD 20, __BD 20, __BD 0);
	if (type () != __BATTLESHIPII_ENEMTYTYPE1012__)
		for (auto i : cPos)
			result.push_back (QGAMES::Rectangle (i - d, i + d));
	result.push_back (QGAMES::Rectangle (-d, d)); // Only the center in case of type == __BATTLESHIPII_ENEMTYTYPE1012__

	return (result);
}
