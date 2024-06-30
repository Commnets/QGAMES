#include <AdvancedArcade/adbuilders.hpp>
#include <AdvancedArcade/admaps.hpp>
#include <AdvancedArcade/adgamestate.hpp>
#include <AdvancedArcade/adletters.hpp>

// ---
QGAMES::AdvancedTextBuilder::AdvancedTextBuilder ()
	: QGAMES::TextBuilder (),
	  _fontData ()
{
	// Ravie
	QGAMES::AdvancedTextBuilder::FontDataPerSize fR;
	// Ravie 12...
	QGAMES::AdvancedTextBuilder::FontDataList fR12;
	fR12.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE12BLUELETTERS__, __QGAMES_RAVIELETTERS__, 12, __QGAMES_BLUECOLOR__));
	fR12.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE12GREENLETTERS__, __QGAMES_RAVIELETTERS__, 12, __QGAMES_GREENCOLOR__));
	fR12.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE12ORANGELETTERS__, __QGAMES_RAVIELETTERS__, 12, __QGAMES_ORANGECOLOR__));
	fR [12] = fR12;
	// Ravie 24
	QGAMES::AdvancedTextBuilder::FontDataList fR24;
	fR24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE24BLUELETTERS__, __QGAMES_RAVIELETTERS__, 24, __QGAMES_BLUECOLOR__));
	fR24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE24GREENLETTERS__, __QGAMES_RAVIELETTERS__, 24, __QGAMES_GREENCOLOR__));
	fR24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE24ORANGELETTERS__, __QGAMES_RAVIELETTERS__, 24, __QGAMES_ORANGECOLOR__));
	fR [24] = fR24;
	// Ravie 36
	QGAMES::AdvancedTextBuilder::FontDataList fR36;
	fR36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE36BLUELETTERS__, __QGAMES_RAVIELETTERS__, 36, __QGAMES_BLUECOLOR__));
	fR36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE36GREENLETTERS__, __QGAMES_RAVIELETTERS__, 36, __QGAMES_GREENCOLOR__));
	fR36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE36ORANGELETTERS__, __QGAMES_RAVIELETTERS__, 36, __QGAMES_ORANGECOLOR__));
	fR [36] = fR36;
	// Ravie 72
	QGAMES::AdvancedTextBuilder::FontDataList fR72;
	fR72.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE72BLUELETTERS__, __QGAMES_RAVIELETTERS__, 72, __QGAMES_BLUECOLOR__));
	fR72.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE72GREENLETTERS__, __QGAMES_RAVIELETTERS__, 72, __QGAMES_GREENCOLOR__));
	fR72.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_RAVIE72ORANGELETTERS__, __QGAMES_RAVIELETTERS__, 72, __QGAMES_ORANGECOLOR__));
	fR [72] = fR72;
	_fontData [__QGAMES_RAVIELETTERS__] = fR;

	// Showcard
	QGAMES::AdvancedTextBuilder::FontDataPerSize fSC;
	// Showcard 24
	QGAMES::AdvancedTextBuilder::FontDataList fSC24;
	fSC24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC24BLUELETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 24, __QGAMES_BLUECOLOR__));
	fSC24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC24BROWNLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 24, __QGAMES_DARKYELLOWCOLOR__));
	fSC24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC24GREENLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 24, __QGAMES_GREENCOLOR__));
	fSC24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC24REDLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 24, __QGAMES_REDCOLOR__));
	fSC24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC24YELLOWLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 24, __QGAMES_YELLOWCOLOR__));
	fSC [24] = fSC24;
	// Showcard 32
	QGAMES::AdvancedTextBuilder::FontDataList fSC32;
	fSC32.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC32BLUELETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 32, __QGAMES_BLUECOLOR__));
	fSC32.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC32BROWNLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 32, __QGAMES_DARKYELLOWCOLOR__));
	fSC32.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC32GREENLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 32, __QGAMES_GREENCOLOR__));
	fSC32.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC32REDLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 32, __QGAMES_REDCOLOR__));
	fSC32.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC32YELLOWLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 32, __QGAMES_YELLOWCOLOR__));
	fSC [32] = fSC32;
	// Showcard 36
	QGAMES::AdvancedTextBuilder::FontDataList fSC36;
	fSC36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC36BLUELETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 36, __QGAMES_BLUECOLOR__));
	fSC36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC36BROWNLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 36, __QGAMES_DARKYELLOWCOLOR__));
	fSC36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC36GREENLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 36, __QGAMES_GREENCOLOR__));
	fSC36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC36REDLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 36, __QGAMES_REDCOLOR__));
	fSC36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC36YELLOWLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 36, __QGAMES_YELLOWCOLOR__));
	fSC [36] = fSC36;
	// Showcard 64
	QGAMES::AdvancedTextBuilder::FontDataList fSC64;
	fSC64.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC64BLUELETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 64, __QGAMES_BLUECOLOR__));
	fSC64.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC64BROWNLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 64, __QGAMES_DARKYELLOWCOLOR__));
	fSC64.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC64GREENLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 64, __QGAMES_GREENCOLOR__));
	fSC64.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC64REDLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 64, __QGAMES_REDCOLOR__));
	fSC64.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC64YELLOWLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 64, __QGAMES_YELLOWCOLOR__));
	fSC [64] = fSC64;
	// Showcard 72
	QGAMES::AdvancedTextBuilder::FontDataList fSC72;
	fSC72.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC72BLUELETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 72, __QGAMES_BLUECOLOR__));
	fSC72.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC72BROWNLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 72, __QGAMES_DARKYELLOWCOLOR__));
	fSC72.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC72GREENLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 72, __QGAMES_GREENCOLOR__));
	fSC72.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC72REDLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 72, __QGAMES_REDCOLOR__));
	fSC72.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_SCARDGOTHIC72YELLOWLETTERS__, __QGAMES_SCARDGOTHICLETTERS__, 72, __QGAMES_YELLOWCOLOR__));
	fSC [72] = fSC72;
	_fontData [__QGAMES_SCARDGOTHICLETTERS__] = fSC;

	// Courier
	QGAMES::AdvancedTextBuilder::FontDataPerSize fC;
	// Courier 10
	QGAMES::AdvancedTextBuilder::FontDataList fC10;
	fC10.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_COURIER10BLACKLETTERS__, __QGAMES_COURIERLETTERS__, 10, __QGAMES_BLACKCOLOR__));
	fC10.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__QGAMES_COURIER10WHITELETTERS__, __QGAMES_COURIERLETTERS__, 10, __QGAMES_WHITECOLOR__));
	fC [10] = fC10;
	_fontData [__QGAMES_COURIERLETTERS__] = fC;
}

// ---
int QGAMES::AdvancedTextBuilder::nextSizeTo (int fnt)
{
	int result = fnt; // By default the next size doesn't exist...

	QGAMES::AdvancedTextBuilder::FontData fntDt = fontFor (fnt);
	if (fntDt._id != -1)
	{
		// All the sizes for that type...
		QGAMES::AdvancedTextBuilder::FontDataPerSize pS = _fontData.find (fntDt._type) -> second;
		// Look for the size, and take the next if any...
		QGAMES::AdvancedTextBuilder::FontDataPerSize::const_iterator i = pS.find (fntDt._size); i++; // Next
		// If any, iterates over all individual elements, looking for the closest one in terms of color...
		if (i != pS.end ())
		{
			QGAMES::bdata a; // Temporal variable used in the loop...
			QGAMES::bdata d = __MAXBDATA__;
			QGAMES::AdvancedTextBuilder::FontData nFntDt;
			for (QGAMES::AdvancedTextBuilder::FontDataList::const_iterator j = (*i).second.begin ();
					j != (*i).second.end (); j++)
			{
				if ((a = distanceBetweenColors ((*j)._color, fntDt._color)) < d)
				{
					nFntDt = (*j);
					result = nFntDt._id;
					d = a;
				}
			}
		}
	}
	
	return (result);
}

// ---
int QGAMES::AdvancedTextBuilder::previousSizeTo (int fnt)
{
	int result = fnt; // By default the next size doesn't exist...

	QGAMES::AdvancedTextBuilder::FontData fntDt = fontFor (fnt);
	if (fntDt._id != -1)
	{
		// All the sizes for that type...
		QGAMES::AdvancedTextBuilder::FontDataPerSize pS = _fontData.find (fntDt._type) -> second;
		// Look for the size, and take the next if any...
		QGAMES::AdvancedTextBuilder::FontDataPerSize::const_iterator i = pS.find (fntDt._size); i--; // Previous
		// If any, iterates over all individual elements, looking for the closest one in terms of color...
		if (i != pS.end ())
		{
			QGAMES::bdata a; // Temporal variable used in the loop...
			QGAMES::bdata d = __MAXBDATA__;
			QGAMES::AdvancedTextBuilder::FontData nFntDt;
			for (QGAMES::AdvancedTextBuilder::FontDataList::const_iterator j = (*i).second.begin ();
					j != (*i).second.end (); j++)
			{
				if ((a = distanceBetweenColors ((*j)._color, fntDt._color)) < d)
				{
					nFntDt = (*j);
					result = nFntDt._id;
					d = a;
				}
			}
		}
	}
	
	return (result);
}

// ---
int QGAMES::AdvancedTextBuilder::inColor (int fnt, const QGAMES::Color& cl)
{
	int result = fnt; // By default the next size doesn't exist...

	QGAMES::AdvancedTextBuilder::FontData fntDt = fontFor (fnt);
	if (fntDt._id != -1)
	{
		// All the sizes for that type...
		QGAMES::AdvancedTextBuilder::FontDataPerSize pS = _fontData.find (fntDt._type) -> second;
		// Look for the size...
		QGAMES::AdvancedTextBuilder::FontDataPerSize::const_iterator i = pS.find (fntDt._size);
		// If any, iterates over all individual elements, looking for the closest one in terms of color...
		if (i != pS.end ())
		{
			QGAMES::bdata a; // Temporal variable used in the loop...
			QGAMES::bdata d = __MAXBDATA__;
			QGAMES::AdvancedTextBuilder::FontData nFntDt;
			for (QGAMES::AdvancedTextBuilder::FontDataList::const_iterator j = (*i).second.begin ();
					j != (*i).second.end (); j++)
			{
				if ((a = distanceBetweenColors ((*j)._color, cl)) < d)
				{
					nFntDt = (*j);
					result = nFntDt._id;
					d = a;
				}
			}
		}
	}
	
	return (result);
}

// ---
QGAMES::ScoreObjectText* QGAMES::AdvancedTextBuilder::createText (int fnt, const std::string& txt)
{
	QGAMES::ScoreObjectText* result = NULL;
	
	switch (fnt)
	{
		case __QGAMES_RAVIE12BLUELETTERS__:
			result = new QGAMES::Ravie12BlueFont (txt);
			break;
		case __QGAMES_RAVIE12GREENLETTERS__:
			result = new QGAMES::Ravie12GreenFont (txt);
			break;
		case __QGAMES_RAVIE12ORANGELETTERS__:
			result = new QGAMES::Ravie12OrangeFont (txt);
			break;
		case __QGAMES_RAVIE24BLUELETTERS__:
			result = new QGAMES::Ravie24BlueFont (txt);
			break;
		case __QGAMES_RAVIE24GREENLETTERS__:
			result = new QGAMES::Ravie24GreenFont (txt);
			break;
		case __QGAMES_RAVIE24ORANGELETTERS__:
			result = new QGAMES::Ravie24OrangeFont (txt);
			break;
		case __QGAMES_RAVIE36BLUELETTERS__:
			result = new QGAMES::Ravie36BlueFont (txt);
			break;
		case __QGAMES_RAVIE36GREENLETTERS__:
			result = new QGAMES::Ravie36GreenFont (txt);
			break;
		case __QGAMES_RAVIE36ORANGELETTERS__:
			result = new QGAMES::Ravie36OrangeFont (txt);
			break;
		case __QGAMES_RAVIE72BLUELETTERS__:
			result = new QGAMES::Ravie72BlueFont (txt);
			break;
		case __QGAMES_RAVIE72GREENLETTERS__:
			result = new QGAMES::Ravie72GreenFont (txt);
			break;
		case __QGAMES_RAVIE72ORANGELETTERS__:
			result = new QGAMES::Ravie72OrangeFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC24BLUELETTERS__:
			result = new QGAMES::ShowcardGothic24BlueFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC24BROWNLETTERS__:
			result = new QGAMES::ShowcardGothic24BrownFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC24GREENLETTERS__:
			result = new QGAMES::ShowcardGothic24GreenFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC24REDLETTERS__:
			result = new QGAMES::ShowcardGothic24RedFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC24YELLOWLETTERS__:
			result = new QGAMES::ShowcardGothic24YellowFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC32BLUELETTERS__:
			result = new QGAMES::ShowcardGothic32BlueFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC32BROWNLETTERS__:
			result = new QGAMES::ShowcardGothic32BrownFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC32GREENLETTERS__:
			result = new QGAMES::ShowcardGothic32GreenFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC32REDLETTERS__:
			result = new QGAMES::ShowcardGothic32RedFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC32YELLOWLETTERS__:
			result = new QGAMES::ShowcardGothic32YellowFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC36BLUELETTERS__:
			result = new QGAMES::ShowcardGothic36BlueFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC36BROWNLETTERS__:
			result = new QGAMES::ShowcardGothic36BrownFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC36GREENLETTERS__:
			result = new QGAMES::ShowcardGothic36GreenFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC36REDLETTERS__:
			result = new QGAMES::ShowcardGothic36RedFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC36YELLOWLETTERS__:
			result = new QGAMES::ShowcardGothic36YellowFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC64BLUELETTERS__:
			result = new QGAMES::ShowcardGothic64BlueFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC64BROWNLETTERS__:
			result = new QGAMES::ShowcardGothic64BrownFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC64GREENLETTERS__:
			result = new QGAMES::ShowcardGothic64GreenFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC64REDLETTERS__:
			result = new QGAMES::ShowcardGothic64RedFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC64YELLOWLETTERS__:
			result = new QGAMES::ShowcardGothic64YellowFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC72BLUELETTERS__:
			result = new QGAMES::ShowcardGothic72BlueFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC72BROWNLETTERS__:
			result = new QGAMES::ShowcardGothic72BrownFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC72GREENLETTERS__:
			result = new QGAMES::ShowcardGothic72GreenFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC72REDLETTERS__:
			result = new QGAMES::ShowcardGothic72RedFont (txt);
			break;
		case __QGAMES_SCARDGOTHIC72YELLOWLETTERS__:
			result = new QGAMES::ShowcardGothic72YellowFont (txt);
			break;
		case __QGAMES_COURIER10BLACKLETTERS__:
			result = new QGAMES::Courier10BlackFont (txt);
			break;
		case __QGAMES_COURIER10WHITELETTERS__:
			result = new QGAMES::Courier10WhiteFont (txt);
			break;
		
		default:
			assert (0);
	}

	return (result);
}

// ---
QGAMES::Entity* QGAMES::AdvancedEntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = NULL;
	if (def._id == __QGAMES_COMMTYNETSPAUSEENTITYID__)
		result = new QGAMES::BasicCharacter (def._id);

	assert (result); // Just to show a warning pop up window
	return (result);
}

// ---
QGAMES::FollowingACurveMovement::CurveTemplate* QGAMES::CurveTemplateBuilder::createBasicDefaultCurve (int t)
{
	QGAMES::FollowingACurveMovement::CurveTemplate* result = NULL;

	switch (t)
	{
		case __QGAMES_NOMOVEMENTTRACE__:
			result = new QGAMES::FollowingACurveMovement::NoCurveTemplate ();
			break;

		case __QGAMES_LINEARCURVEMOVEMENTTRACE___:
			result = new QGAMES::FollowingACurveMovement::LineCurveTemplate 
				(1, QGAMES::Position (__BD 1, __BD 0, __BD 0));
			break;

		case __QGAMES_CIRCULARCURVEMOVEMENTTRACE___:
			result = new QGAMES::FollowingACurveMovement::CircleCurveTemplate 
				(1, QGAMES::Vector::_zNormal, __BD 1);
			break;

		case __QGAMES_SINUSOIDALCURVEMOVEMENTTRACE__:
			result = new QGAMES::FollowingACurveMovement::SinosoidalCurveTemplate
				(1, QGAMES::Vector::_zNormal, 100, 100);
			break;

		case __QGAMES_COMPLEXCURVEMOVEMENTTRACE__:
			result = new QGAMES::FollowingACurveMovement::ComplexCurveTemplate 
				(QGAMES::FollowingACurveMovement::CurveTemplates ()); // With no curves inside...
			break;

		default:
			assert (0); // It should be here...
	}

	return (result);
}

// ---
QGAMES::FollowingACurveMovement::CurveTemplate* QGAMES::CurveTemplateBuilder::createCurve 
	(int t, const std::map <int, double>& prms)
{
	QGAMES::FollowingACurveMovement::CurveTemplate* result = NULL;

	switch (t)
	{
		// No movement
		// Valid even for 3D
		case __QGAMES_NOMOVEMENTTRACE__:
		{
			result = new QGAMES::FollowingACurveMovement::NoCurveTemplate ();
		}

		break;

		// A linear trace...
		// Valid even for 3D
		case __QGAMES_LINEARCURVEMOVEMENTTRACE___:
		{
			assert (prms.find (0) != prms.end () && // The length of the trace...
					prms.find (1) != prms.end () && // The speed factor. 
													// The bigger the number is the lesser the final speed will be...
					prms.find (2) != prms.end () && // x coordinate to the orientation
					prms.find (3) != prms.end () && // y
					prms.find (4) != prms.end ());  // z
			assert ((*prms.find (1)).second != __BD 0); // Not possible speed reduction factor be 0...
			
			result = new QGAMES::FollowingACurveMovement::LineCurveTemplate 
				((int) ((*prms.find (0)).second / (*prms.find (1)).second), // steps...
				 QGAMES::Vector (__BD (*prms.find (2)).second, 
								 __BD (*prms.find (3)).second, 
								 __BD (*prms.find (4)).second).normalize () * __BD (*prms.find (0)).second); // Towards?

			// The linear speed will be length / steps = speed factor
		}
			
		break;

		// A circular trace... 
		// Valid even for 3D
		case __QGAMES_CIRCULARCURVEMOVEMENTTRACE___:
		{
			assert (prms.find (0) != prms.end () && // The radius of the trace...
					prms.find (1) != prms.end () && // The speed factor. 
													// The bigger the number is the lesser the final speed will be...
					prms.find (2) != prms.end () && // x coordinate to the orientation
					prms.find (3) != prms.end () && // y
					prms.find (4) != prms.end () && // z
					prms.find (5) != prms.end () && // initial angle (in radians)
					prms.find (6) != prms.end ());  // final
			assert ((*prms.find (1)).second != __BD 0); // Not possible speed reduction factor be 0...

			result = new QGAMES::FollowingACurveMovement::CircleCurveTemplate
				((int) (std::abs ((*prms.find (5)).second - (*prms.find (6)).second) / (*prms.find (1)).second * 100),  // steps
				 QGAMES::Vector (__BD (*prms.find (2)).second, 
								 __BD (*prms.find (3)).second, 
								 __BD (*prms.find (4)).second).normalize (), // normalized
				 __BD (*prms.find (0)).second,  
				 __BD (*prms.find (5)).second,
				 __BD (*prms.find (6)).second);

			// The angular speed will be anggle moved / steps = speed factor
			// The greater the radius is the greater the equivalent linear speed will be for the same angular one...
		}

		break;

		// A sinusoidal movement...
		// Valid even in 3D
		case __QGAMES_SINUSOIDALCURVEMOVEMENTTRACE__:
		{
			assert (prms.find (0) != prms.end () && // The range of the trace...
					prms.find (1) != prms.end () && // The period of the trace...
					prms.find (2) != prms.end () && // The speed factor. 
													// The bigger the number is the lesser the final speed will be...
					prms.find (3) != prms.end () && // x coordinate to the orientation
					prms.find (4) != prms.end () && // y
					prms.find (5) != prms.end () && // z
					prms.find (6) != prms.end () && // initial angle (in radians)
					prms.find (7) != prms.end () );  // final
			assert ((*prms.find (2)).second != __BD 0); // Not possible speed reduction factor be 0...
			
			result = new QGAMES::FollowingACurveMovement::SinosoidalCurveTemplate
				((int) (std::abs ((*prms.find (6)).second - (*prms.find (7)).second) / (*prms.find (2)).second * 100),  // steps
				 QGAMES::Vector (__BD (*prms.find (3)).second, 
								 __BD (*prms.find (4)).second, 
								 __BD (*prms.find (5)).second).normalize (), // normalized
				 __BD (*prms.find (0)).second,  
				 __BD (*prms.find (1)).second,
				 __BD (*prms.find (6)).second,
				 __BD (*prms.find (7)).second);
		}

		break;

		// A linear dancing movement...
		// Valid even for 3D
		case __QGAMES_LINEARDANCINGCURVEMOVTRACE___:
		{
			assert (prms.find (0) != prms.end () && // The length of the trace...
					prms.find (1) != prms.end () && // The speed factor. 
													// The bigger the number is the lesser the final speed will be...
					prms.find (2) != prms.end () && // x coordinate to the orientation
					prms.find (3) != prms.end () && // y
					prms.find (4) != prms.end ());  // z
			assert ((*prms.find (1)).second != __BD 0); // Not possible speed reduction factor be 0...
			
			QGAMES::FollowingACurveMovement::CurveTemplate* crv1 = createCurve (__QGAMES_LINEARCURVEMOVEMENTTRACE___, prms);

			std::map <int, double> oPrms = prms;
			oPrms [2] = -oPrms [2]; oPrms [3] = -oPrms [3]; oPrms [4] = -oPrms [4]; // Changel the movement...
			QGAMES::FollowingACurveMovement::CurveTemplate* crv2 = createCurve (__QGAMES_LINEARCURVEMOVEMENTTRACE___, oPrms);
			
			QGAMES::FollowingACurveMovement::CurveTemplates crvs; 
			crvs.push_back (crv1); crvs.push_back (crv2);

			result = new QGAMES::FollowingACurveMovement::ComplexCurveTemplate (crvs);

			// The linear speed of each section will be the same...
		}

		break;

		// A circular dancing movement...
		// Valid even for 3D
		case __QGAMES_CIRCULARDANCINGCURVEMOVTRACE___:
		{
			assert (prms.find (0) != prms.end () && // The radius of the trace...
					prms.find (1) != prms.end () && // The speed factor. 
													// The bigger the number is the lesser the final speed will be...
					prms.find (2) != prms.end () && // x coordinate to the orientation
					prms.find (3) != prms.end () && // y
					prms.find (4) != prms.end () && // z
					prms.find (5) != prms.end () && // initial angle (in radians)
					prms.find (6) != prms.end ());  // final
			assert ((*prms.find (1)).second != __BD 0); // Not possible speed reduction factor be 0...

			QGAMES::FollowingACurveMovement::CurveTemplate* crv1 = createCurve (__QGAMES_CIRCULARCURVEMOVEMENTTRACE___, prms);
			
			std::map <int, double> oPrms = prms;
			double t = oPrms [5]; oPrms [5] = oPrms [6]; oPrms [6] = t; 
			QGAMES::FollowingACurveMovement::CurveTemplate* crv2 = createCurve (__QGAMES_CIRCULARCURVEMOVEMENTTRACE___, oPrms);
	
			QGAMES::FollowingACurveMovement::CurveTemplates crvs; 
			crvs.push_back (crv1); crvs.push_back (crv2);

			result = new QGAMES::FollowingACurveMovement::ComplexCurveTemplate (crvs);

			// The angular speed of each section will be the same...
		}

		break;

		// A random circular movement to the right and then linear movement to the down part of the screen
		// Only valid for the XY plane
		case __QGAMES_XYCIRCLEANDLINERIGHTMOVTRACE__:
		{
			assert (prms.find (0) != prms.end () && // The radius of the trace...
					prms.find (1) != prms.end () && // The speed factor. 
													// The bigger the number is the lesser the final speed will be...
					prms.find (2) != prms.end () && // The exit angle
					prms.find (3) != prms.end ());  // the length of the line...
			assert ((*prms.find (1)).second != __BD 0); // Not possible speed reduction factor be 0...
			
			// Angular speed in the circular section and linear speed in the linear one must be equivalent.
			// The speed (linear) in the linear section is equivalent to the speed factor (sf in pixels / loop)
			// The speed (angular) in the circular section should be then sf / radious * conversion factor (pixel / meters)

			QGAMES::bdata eA = __BD (*prms.find (2)).second;
			while (eA > __PI) eA -= __BD 2 * __PI;
			double sfc = (*prms.find (1)).second / (*prms.find (0)).second * 100.0; 

			QGAMES::FollowingACurveMovement::CurveTemplate* crv1 = 
				createCurve (__QGAMES_CIRCULARCURVEMOVEMENTTRACE___, 
					std::map <int, double> { 
											{ 0, (*prms.find (0)).second },
											{ 1, sfc },
											{ 2, 0.0 }, { 3, 0.0 }, { 4, 1.0 }, // in the x y plane always...
											{ 5, (double) __PI }, // Initial angle
											{ 6, (double) eA } // To the right (clockwise). Lesser that PI.
										});
			
			QGAMES::FollowingACurveMovement::CurveTemplate* crv2 = 
				createCurve (__QGAMES_LINEARCURVEMOVEMENTTRACE___, 
					std::map <int, double> { 
											{ 0, (*prms.find (3)).second }, // length of the movement...
											{ 1, (*prms.find (1)).second }, // The same step...
											{ 2, std::sin ((double) eA) }, { 3, std::cos ((double) eA) }, { 4, 0.0 }
										   });
			
			QGAMES::FollowingACurveMovement::CurveTemplates crvs; 
			crvs.push_back (crv1); crvs.push_back (crv2);

			result = new QGAMES::FollowingACurveMovement::ComplexCurveTemplate (crvs);
		}

		break;

		// Same but to the other side...
		// Again, only valid for XY plane
		case __QGAMES_XYCIRCLEANDLINELEFTMOVTRACE__:
		{
			assert (prms.find (0) != prms.end () && // The radius of the trace...
					prms.find (1) != prms.end () && // The speed factor. 
													// The bigger the number is the lesser the final speed will be...
					prms.find (2) != prms.end () && // The exit angle
					prms.find (3) != prms.end ());  // the length of the line...
			assert ((*prms.find (1)).second != __BD 0); // Not possible speed reduction factor be 0...
			
			QGAMES::bdata eA = __BD (*prms.find (2)).second;
			while (eA < 0) eA += __BD 2 * __PI;
			double sfc = (*prms.find (1)).second / (*prms.find (0)).second * 100.0; 
			
			QGAMES::FollowingACurveMovement::CurveTemplate* crv1 = 
				createCurve (__QGAMES_CIRCULARCURVEMOVEMENTTRACE___, 
					std::map <int, double> { 
											{ 0, (*prms.find (0)).second },
											{ 1, sfc },
											{ 2, 0.0 }, { 3, 0.0 }, { 4, 1.0 }, // in the x y plane always...
											{ 5, 0.0 }, // Initial angle
											{ 6, (double) eA } // To the left (anti clockwise). Greater than 0
										   });

			QGAMES::FollowingACurveMovement::CurveTemplate* crv2 = 
				createCurve (__QGAMES_LINEARCURVEMOVEMENTTRACE___, 
					std::map <int, double> { 
											{ 0, (*prms.find (3)).second }, // length of the movement...
											{ 1, (*prms.find (1)).second }, // The same step...
											{ 2, -std::sin ((double) eA) }, { 3, -std::cos ((double) eA) }, { 4, 0.0 }
										   });
			QGAMES::FollowingACurveMovement::CurveTemplates crvs; 
			crvs.push_back (crv1); crvs.push_back (crv2);

			result = new QGAMES::FollowingACurveMovement::ComplexCurveTemplate (crvs);
		}

		break;

		// A line, then a circle, then another line...
		// Always in the XY plane
		case __QGAMES_XYLINECIRCLELINEMOVTRACE__:
		{
			assert (prms.find (0) != prms.end () && // The length of the linear trace...
					prms.find (1) != prms.end () && // The radious f the curve...
					prms.find (2) != prms.end () && // The initial angle
					prms.find (3) != prms.end () && // the angle to turn. Positive will mean clowise and negatibe anto clockwise...
					prms.find (4) != prms.end ());  // The speed factor. 
													// The bigger the number is the lesser the final speed will be...
			assert ((*prms.find (4)).second != __BD 0); // Not possible speed reduction factor be 0...

			double cWF = (*prms.find (3)).second > 0.0 ? -1.0 : 1.0;
			double iAC = cWF * (QGAMES::bdata) (__PI / 2) + (*prms.find (2)).second;
			double fAC = iAC + (*prms.find (3)).second;
			double iAR = fAC - cWF * (double) (__PI / __BD 2);
			double sfc = (*prms.find (4)).second / (*prms.find (1)).second * 100.0; 

			QGAMES::FollowingACurveMovement::CurveTemplate* crv1 = 
				createCurve (__QGAMES_LINEARCURVEMOVEMENTTRACE___, 
					std::map <int, double> { 
											{ 0, (*prms.find (0)).second }, 
											{ 1, (*prms.find (4)).second }, 
											{ 2,  std::cos ((*prms.find (2)).second) }, 
											{ 3, -std::sin ((*prms.find (2)).second) }, 
											{ 4, 0.0 }, // The plane XY...
										   });

			QGAMES::FollowingACurveMovement::CurveTemplate* crv2 = 
				createCurve (__QGAMES_CIRCULARCURVEMOVEMENTTRACE___, 
					std::map <int, double> { 
											{ 0, (*prms.find (1)).second },
											{ 1, sfc },
											{ 2, 0.0 }, { 3, 0.0 }, { 4, 1.0 }, // in the XY plane always...
											{ 5, iAC }, 
											{ 6, fAC } 
										   });

			QGAMES::FollowingACurveMovement::CurveTemplate* crv3 = 
				createCurve (__QGAMES_LINEARCURVEMOVEMENTTRACE___, 
					std::map <int, double> { 
											{ 0, 2.0 * (*prms.find (0)).second }, // double distance with the line after the circle...
											{ 1, (*prms.find (4)).second }, 
											{ 2, std::cos (iAR) }, { 3, -std::sin (iAR) }, { 4, 0.0 }, // The plane XY...
										   });

			QGAMES::FollowingACurveMovement::CurveTemplates crvs; 
			crvs.push_back (crv1); crvs.push_back (crv2); crvs.push_back (crv3);

			result = new QGAMES::FollowingACurveMovement::ComplexCurveTemplate (crvs);
		}

		break;

		default:
			assert (0); // It shouln't be here...
	}

	assert (result); // It shoukdn't be null ever...
	return (result);
}

// ---
QGAMES::AdvancedMovementBuilder::AdvancedMovementBuilder (const std::string& fDef, CurveTemplateBuilder* cB)
	: QGAMES::MovementBuilder (fDef),
	  _curveBuilder (cB)
{ 
	assert (_curveBuilder); // Can't be null ever...
}

// ---
QGAMES::Movement* QGAMES::AdvancedMovementBuilder::createMovement (const QGAMES::MovementBuilder::MovementDefinition& def)
{
	QGAMES::Movement* result = NULL;
	if (def._id == __QGAMES_MOVNOMOVEMENTID__)
		result = new QGAMES::NoMovement (def._id, def._variables);

	assert (result); // Just to show a warning pop up window
	return (result);
}

// ---
QGAMES::Map* QGAMES::BackgroundObjectMapBuilder::createMap (int id, int w, int h, int d, const QGAMES::Layers& l, 
	const QGAMES::MapProperties& pties)
{
	return (new QGAMES::BackgroundMap (id, l, w, h, pties));
}

// ---
QGAMES::Layer* QGAMES::BackgroundObjectMapBuilder::createLayer (int id, const std::string& name, 
	QGAMES::Objects& objs, QGAMES::Map* m, const QGAMES::LayerProperties& lP)
{
	return (new QGAMES::BackgroundLayer (id, name, objs, m, lP));
}

// ---
QGAMES::Widget* QGAMES::AdvancedGUISystemBuilder::createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def)
{
	QGAMES::Widget* result = NULL;
	if (def._id == __QGAMES_STDGUISYSTEMMAINWIDGET__ ||
		def._id == __QGAMES_STDGUISYSTEMMAINCONFWIDGET__) 
		result = new QGAMES::SelectionOptionsGameState::MenuControlWidget (def._id);

	assert (result); // Just to warning a pop up window
	return (result);
}

// ---
bool QGAMES::ForAdvancedGameStatesAddon::canCreateGameState (int t) const
{
	return (t == __QGAMES_GAMESTATENULLTYPE__ ||
			t == __QGAMES_GAMESTATEWAITINGTIMETYPE__ ||
			t == __QGAMES_GAMESTATEUNTILENTERTYPE__ ||
			t == __QGAMES_GAMESTATEWAITORUNTILKEYTYPE__ ||
			t == __QGAMES_GAMESTATESPENDUNLESSKEYTYPE__ ||
			t == __QGAMES_GAMESTATESHOWSTDTEXTTYPE__  ||
			t == __QGAMES_GAMESTATESHOWPLAYERNUMTYPE__ ||
			t == __QGAMES_GAMESTATEFPSTYPE__ ||
			t == __QGAMES_GAMESTATEPLAYSOUNDTYPE__ ||
			t == __QGAMES_GAMESTATESHOWFIXFORMTYPE__ ||
			t == __QGAMES_GAMESTATESHOWFORMTYPE__ ||
			t == __QGAMES_GAMESTATESHOWENTITYTYPE__ ||
			t == __QGAMES_GAMESTATELOADINGTYPE__  ||
			t == __QGAMES_GAMESTATECOUNTINGDOWNTYPE__ ||
			t == __QGAMES_GAMESTATEHALLOFFAMETYPE__  ||
			t == __QGAMES_GAMESTATEROLLINGFRMSTYPE__ ||
			t == __QGAMES_GAMESTATESELECTOPTIONSTYPE__ ||
			t == __QGAMES_GAMESTATEBLINKINGTEXTTYPE__);
}

// ---
QGAMES::GameState* QGAMES::ForAdvancedGameStatesAddon::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	QGAMES::GameState* result = NULL;
	switch (def -> _type)
	{
		case __QGAMES_GAMESTATENULLTYPE__:
			result = new QGAMES::NULLGameState (
				def -> _id,
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATEWAITINGTIMETYPE__:
			result = new QGAMES::SpendingTimeGameState (
				def -> _id,
				__BD (std::atof (def -> _attributes.find (__QGAMES_GAMESTATEWAITINGTIMEATTRTIME__) -> second.c_str ())),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATEUNTILENTERTYPE__:
			result = new QGAMES::ForEverUntilIntroKeyOrLeftButtonMousePressedGameState (
				def -> _id,
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATEWAITORUNTILKEYTYPE__:
			result = new QGAMES::SpendingTimeOrWaitingUntilAnyKeyPressedGameState (
				def -> _id, 
				__BD (std::atof (def -> _attributes.find (__QGAMES_GAMESTATEWAITINGTIMEATTRTIME__) -> second.c_str ())),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATESPENDUNLESSKEYTYPE__:
			result = new QGAMES::SpedingTimeUnlessKeyIsPressedGameState (
				def -> _id,
				__BD (std::atof (def -> _attributes.find (__QGAMES_GAMESTATEWAITINGTIMEATTRTIME__) -> second.c_str ())),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATESHOWSTDTEXTTYPE__:
			result = new QGAMES::ShowingStandardTextGameState (
				def -> _id,
				def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
				std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
				QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATESHOWPLAYERNUMTYPE__:
			result = new QGAMES::ShowingCurrentPlayerNumberGameState (
				def -> _id,
				def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
				std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
				QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATEFPSTYPE__:
			result = new QGAMES::ShowingFrameRateGameState (
				def -> _id, 
				def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
				std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
				QGAMES::ShowingTextGameState::Properties (def -> _attributes), 
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATEPLAYSOUNDTYPE__:
			result = new QGAMES::PlayingASoundGameState (
				def -> _id,
				QGAMES::PlayingASoundGameState::Properties (def -> _attributes),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATESHOWFIXFORMTYPE__:
			result = new QGAMES::ShowingFixFormGameState (
				def -> _id,
				QGAMES::ShowingFixFormGameState::Properties (def -> _attributes),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATESHOWFORMTYPE__:
			result = new QGAMES::ShowingAppearingFormGameState (
				def -> _id,
				QGAMES::ShowingAppearingFormGameState::Properties (def -> _attributes),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATESHOWENTITYTYPE__:
			result = new QGAMES::ShowingEntityGameState (
				def -> _id,
				QGAMES::ShowingEntityGameState::Properties (def -> _attributes),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATELOADINGTYPE__:
			result = new QGAMES::LoadingGameState (
				def -> _id,
				QGAMES::LoadingGameState::Properties (def -> _attributes),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATECOUNTINGDOWNTYPE__:
			result = new QGAMES::CountingDownGameState (
				def -> _id,
				QGAMES::CountingDownGameState::Properties (def -> _attributes),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATEHALLOFFAMETYPE__:
			result = new QGAMES::ShowingHallOfFameGameState (
				def -> _id,
				QGAMES::ShowingHallOfFameGameState::Properties (def -> _attributes),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATEROLLINGFRMSTYPE__:
			result = new QGAMES::RollingFormsGameState (
				def -> _id,
				QGAMES::RollingFormsGameState::Properties (def -> _attributes),
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATESELECTOPTIONSTYPE__:
			result = new QGAMES::StandardInitialSelectionOptionsGameState (
				def -> _id,
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		case __QGAMES_GAMESTATEBLINKINGTEXTTYPE__:
			result = new QGAMES::ShowingBlinkingStandardTextGameState (
				def -> _id,
				def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
				std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
				QGAMES::ShowingBlinkingStandardTextGameState::Properties (def -> _attributes), // Other attributes...
				(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL);
			break;

		default:
			assert (0); // It should be here, just in case...
	}

	return (result);
}

// ---
QGAMES::GameState* QGAMES::ForCreditTitlesGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	QGAMES::ForCreditTitlesGameStateAddOn::GameStateCreditTitlesDefinition* defC =
		(QGAMES::ForCreditTitlesGameStateAddOn::GameStateCreditTitlesDefinition*) def;
	return (new QGAMES::CreditTitlesGameState 
		(def -> _id, QGAMES::CreditTitlesGameState::Properties (defC -> _titles, def -> _attributes),
		(def->_nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
void QGAMES::ForCreditTitlesGameStateAddOn::readSpecificBlockOver (const std::string& blkName,
	GameStateBuilder::GameStateDefinition* def, TiXmlElement* e)
{
	if (blkName == std::string (__QGAMES_GAMESTATECREDITTITLESTBLK__))
	{
		for (TiXmlElement* defElement = e -> FirstChildElement ();
			 defElement != NULL; defElement = defElement -> NextSiblingElement ())
			if (strcmp (defElement -> Value (), __QGAMES_GAMESTATECREDITTITLEPARAM__) == 0)
				((QGAMES::ForCreditTitlesGameStateAddOn::GameStateCreditTitlesDefinition*) def) -> 
					_titles.push_back (defElement -> Attribute (__QGAMES_GAMESTATECREDITTITLEVALUEPRM__));
	}
	else
		assert (0); // No other type of block is allowed under this add on...

	// Other blocks are not treated here...
}

// ---
QGAMES::GameState* QGAMES::ForShowingInstructionsGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	QGAMES::ForShowingInstructionsGameStateAddOn::GameStateInstructionsDefinition* defI =
		(QGAMES::ForShowingInstructionsGameStateAddOn::GameStateInstructionsDefinition*) def;

	// First of all...creates the lines...
	QGAMES::ShowingInstructionsGameState::Properties::TextLineList tL;
	for (int i = 0; i < (int) defI -> _instructions.size (); i++)
	{
		QGAMES::ForShowingInstructionsGameStateAddOn::GameStateInstructionDefinition iD = defI  -> _instructions [i];
		tL.push_back (QGAMES::ShowingInstructionsGameState::Properties::TextLine 
			(iD._line, iD._type, iD._font, iD._position, (QGAMES::ShowingInstructionsGameState::Properties::TextLine::PosReference) iD._reference));
	}

	return (new QGAMES::ShowingInstructionsGameState (
		def -> _id, QGAMES::ShowingInstructionsGameState::Properties (tL, def -> _attributes),
		(def->_nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
void QGAMES::ForShowingInstructionsGameStateAddOn::readSpecificBlockOver (const std::string& blkName, 
	QGAMES::GameStateBuilder::GameStateDefinition* def, TiXmlElement* e)
{
	if (blkName == std::string (__QGAMES_GAMESTATESHOWINSTBLK__))
	{
		for (TiXmlElement* defElement = e -> FirstChildElement ();
			 defElement != NULL; defElement = defElement -> NextSiblingElement ())
		{
			if (strcmp (defElement -> Value (), __QGAMES_GAMESTATESHOWINSTPARAM__) == 0)
			{
				QGAMES::ForShowingInstructionsGameStateAddOn::GameStateInstructionDefinition sDef;
				sDef._line = defElement -> Attribute (__QGAMES_GAMESTATESHOWINSTLINEPRM__);
				defElement -> Attribute (__QGAMES_GAMESTATESHOWINSTTYPEPRM__, &sDef._type);
				defElement -> Attribute (__QGAMES_GAMESTATESHOWINSTFONTPRM__, &sDef._font);
				double pos;	defElement -> Attribute (__QGAMES_GAMESTATESHOWINSTPOSPRM__, &pos); sDef._position = __BD pos;
				defElement -> Attribute (__QGAMES_GAMESTATESHOWINSTREFPRM__, &sDef._reference);
				assert (sDef._reference == 0 || sDef._reference == 1 || sDef._reference == 2);

				((QGAMES::ForShowingInstructionsGameStateAddOn::GameStateInstructionsDefinition*) def) -> 
					_instructions.push_back (sDef);
			}
		}
	}
	else
		assert (0);

	// Other type of blocks are not treaten here...
}

// ---
QGAMES::GameState* QGAMES::ForRollingStatesGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	QGAMES::ForRollingStatesGameStateAddOn::GameStateRollingStatesDefinition* defR =
		(QGAMES::ForRollingStatesGameStateAddOn::GameStateRollingStatesDefinition*) def;
	QGAMES::RollingStatesGameState::SeparationStates sts;
	for (std::vector <QGAMES::ForRollingStatesGameStateAddOn::
			GameStateRollingStatesDefinition::SeparationAttrs>::const_iterator i = defR -> _separations.begin ();
			i != defR -> _separations.end (); i++)
		sts.push_back (QGAMES::RollingStatesGameState::SeparationState ((*i)));
	return (new QGAMES::RollingStatesGameState
		(def -> _id,  sts,
		(def->_nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
void QGAMES::ForRollingStatesGameStateAddOn::readSpecificBlockOver (const std::string& blkName, 
			QGAMES::GameStateBuilder::GameStateDefinition* def, TiXmlElement* e)
{
	if (blkName == std::string (__QGAMES_GAMESTATEROLLSEPSBLOCK__))
	{
		std::vector <QGAMES::ForRollingStatesGameStateAddOn::			
				GameStateRollingStatesDefinition::SeparationAttrs> stors;
		for (TiXmlElement* defElement = e -> FirstChildElement ();
			 defElement != NULL; defElement = defElement -> NextSiblingElement ())
		{
			if (strcmp (defElement -> Value (), __QGAMES_GAMESTATEROLLSEPBLOCK__) == 0)
			{
				QGAMES::ForRollingStatesGameStateAddOn::					
						GameStateRollingStatesDefinition::SeparationAttrs storsAttrs;
				for (TiXmlElement* attr = defElement -> FirstChildElement ();
						attr != NULL; attr = attr -> NextSiblingElement ())
				{
					std::string id (__NULL_STRING__);
					std::string value (__NULL_STRING__);
					id = attr -> Attribute (__QGAMES_GAMESTATEATTRATTRID__);
					value = attr -> Attribute (__QGAMES_GAMESTATEATTRATTRVALUE__);
					assert (id != std::string (__NULL_STRING__) && value != std::string (__NULL_STRING__));
					storsAttrs [id] = value;
				}

				stors.push_back (storsAttrs);
			}
			else
				assert (0); // Other type of block is not allowed...
		}

		((QGAMES::ForRollingStatesGameStateAddOn::GameStateRollingStatesDefinition*) def) -> _separations = stors;
	}
	else
		assert (0); // No other type of block is allowed under this add on...

	// Other blocks are not treated here...
}

// ---
QGAMES::GameState* QGAMES::ForAdvancedGameStateControlAddon::createControlGameState 
	(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
	 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl)
{
	return (new QGAMES::AdvancedGameStateControl (def -> _id, 
			builder () -> gameState (std::atoi (def -> _attributes.find (__QGAMES_GAMESTATEADVANCEDCTRLPAUSE__) -> second.c_str ())), 
			sts, fl));
}

// ---
QGAMES::bdata QGAMES::AdvancedTextBuilder::distanceBetweenColors (const QGAMES::Color& c1, const QGAMES::Color& c2)
{
	return (QGAMES::Position (__BD c1.red (), __BD c1.green (), __BD c1.blue ()).
		distanceTo (QGAMES::Position (__BD c2.red (), __BD c2.green (), __BD c2.blue ())));
}

// ---
QGAMES::AdvancedTextBuilder::FontData QGAMES::AdvancedTextBuilder::fontFor (int id)
{
	FontData result;
	for (QGAMES::AdvancedTextBuilder::FontDataMap::const_iterator i = _fontData.begin ();
			i != _fontData.end () && result._id == -1; i++)
		for (QGAMES::AdvancedTextBuilder::FontDataPerSize::const_iterator j = (*i).second.begin ();
				j != (*i).second.end () && result._id == -1; j++)
			for (QGAMES::AdvancedTextBuilder::FontDataList::const_iterator k = (*j).second.begin ();
					k != (*j).second.end () && result._id == -1; k++)
				if ((*k)._id == id) result = (*k);
	return (result); // It could be -1, if not found...
}
