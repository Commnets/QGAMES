/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: adletters.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 25/03/2018 \n
 *	Description: Defining types of letters. They are normally used during the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADLETTERS__
#define __QGAMES_ADLETTERS__

#include <AdvancedArcade/addefinitions.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
// Define a macro to simplify the definition of a new font.
// Use __DEFINEFONT__ (A,B) in the HPP file, and __IMPLEMENTFONT__ in the CPP correspondent file
// A is the name of the font
// B is the id of the font (it could be also a macro)
// The clone method is quite simple as there aren't either movements, nor animations nor states to copy in a text
// It is tru that the y could be added later, to emulate e.g complicate effects!!
#define __DEFINEFONT__(CLASSNAME, CLASSID) \
	class CLASSNAME : public QGAMES::ScoreObjectText \
	{ \
		public: \
		CLASSNAME (const std::string& t = std::string (__NULL_STRING__)); \
		virtual QGAMES::Entity* clone () const \
								{ return (new CLASSNAME (_text)); } \
		private: \
		virtual int formToDrawLetters () \
								{ return (CLASSID); } \
	};

#define __IMPLEMENTFONT__(CLASSNAME, CLASSID) \
	CLASSNAME::CLASSNAME (const std::string& t) \
			: QGAMES::ScoreObjectText (0, t, QGAMES::Forms (), 0) \
	{ \
		QGAMES::Forms f; \
		f [CLASSID] = QGAMES::Game::game () -> form (CLASSID); \
		setForms (f); \
		setText (t); \
	}

	// Ravie 12
	__DEFINEFONT__(Ravie12BlueFont,__QGAMES_RAVIE12BLUELETTERS__)
	__DEFINEFONT__(Ravie12GreenFont,__QGAMES_RAVIE12GREENLETTERS__)
	__DEFINEFONT__(Ravie12OrangeFont,__QGAMES_RAVIE12ORANGELETTERS__)

	// Ravie 24
	__DEFINEFONT__(Ravie24BlueFont,__QGAMES_RAVIE24BLUELETTERS__)
	__DEFINEFONT__(Ravie24GreenFont,__QGAMES_RAVIE24GREENLETTERS__)
	__DEFINEFONT__(Ravie24OrangeFont,__QGAMES_RAVIE24ORANGELETTERS__)

	// Ravie 36
	__DEFINEFONT__(Ravie36BlueFont,__QGAMES_RAVIE36BLUELETTERS__)
	__DEFINEFONT__(Ravie36GreenFont,__QGAMES_RAVIE36GREENLETTERS__)
	__DEFINEFONT__(Ravie36OrangeFont,__QGAMES_RAVIE36ORANGELETTERS__)

	// Ravie 72
	__DEFINEFONT__(Ravie72BlueFont,__QGAMES_RAVIE72BLUELETTERS__)
	__DEFINEFONT__(Ravie72GreenFont,__QGAMES_RAVIE72GREENLETTERS__)
	__DEFINEFONT__(Ravie72OrangeFont,__QGAMES_RAVIE72ORANGELETTERS__)

	// Courier 10
	__DEFINEFONT__(Courier10WhiteFont,__QGAMES_COURIER10WHITELETTERS__)
	__DEFINEFONT__(Courier10BlackFont,__QGAMES_COURIER10BLACKLETTERS__)

	// Showcard Gothic 24
	__DEFINEFONT__(ShowcardGothic24BlueFont,__QGAMES_SCARDGOTHIC24BLUELETTERS__)
	__DEFINEFONT__(ShowcardGothic24BrownFont,__QGAMES_SCARDGOTHIC24BROWNLETTERS__)
	__DEFINEFONT__(ShowcardGothic24GreenFont,__QGAMES_SCARDGOTHIC24GREENLETTERS__)
	__DEFINEFONT__(ShowcardGothic24RedFont,__QGAMES_SCARDGOTHIC24REDLETTERS__)
	__DEFINEFONT__(ShowcardGothic24YellowFont,__QGAMES_SCARDGOTHIC24YELLOWLETTERS__)

	// Showcard Gothic 32
	__DEFINEFONT__(ShowcardGothic32BlueFont,__QGAMES_SCARDGOTHIC32BLUELETTERS__)
	__DEFINEFONT__(ShowcardGothic32BrownFont,__QGAMES_SCARDGOTHIC32BROWNLETTERS__)
	__DEFINEFONT__(ShowcardGothic32GreenFont,__QGAMES_SCARDGOTHIC32GREENLETTERS__)
	__DEFINEFONT__(ShowcardGothic32RedFont,__QGAMES_SCARDGOTHIC32REDLETTERS__)
	__DEFINEFONT__(ShowcardGothic32YellowFont,__QGAMES_SCARDGOTHIC32YELLOWLETTERS__)

	// Showcard Gothic 36
	__DEFINEFONT__(ShowcardGothic36BlueFont,__QGAMES_SCARDGOTHIC36BLUELETTERS__)
	__DEFINEFONT__(ShowcardGothic36BrownFont,__QGAMES_SCARDGOTHIC36BROWNLETTERS__)
	__DEFINEFONT__(ShowcardGothic36GreenFont,__QGAMES_SCARDGOTHIC36GREENLETTERS__)
	__DEFINEFONT__(ShowcardGothic36RedFont,__QGAMES_SCARDGOTHIC36REDLETTERS__)
	__DEFINEFONT__(ShowcardGothic36YellowFont,__QGAMES_SCARDGOTHIC36YELLOWLETTERS__)

	// Showcard Gothic 64
	__DEFINEFONT__(ShowcardGothic64BlueFont,__QGAMES_SCARDGOTHIC64BLUELETTERS__)
	__DEFINEFONT__(ShowcardGothic64BrownFont,__QGAMES_SCARDGOTHIC64BROWNLETTERS__)
	__DEFINEFONT__(ShowcardGothic64GreenFont,__QGAMES_SCARDGOTHIC64GREENLETTERS__)
	__DEFINEFONT__(ShowcardGothic64RedFont,__QGAMES_SCARDGOTHIC64REDLETTERS__)
	__DEFINEFONT__(ShowcardGothic64YellowFont,__QGAMES_SCARDGOTHIC64YELLOWLETTERS__)

	// Showcard Gothic 72
	__DEFINEFONT__(ShowcardGothic72BlueFont,__QGAMES_SCARDGOTHIC72BLUELETTERS__)
	__DEFINEFONT__(ShowcardGothic72BrownFont,__QGAMES_SCARDGOTHIC72BROWNLETTERS__)
	__DEFINEFONT__(ShowcardGothic72GreenFont,__QGAMES_SCARDGOTHIC72GREENLETTERS__)
	__DEFINEFONT__(ShowcardGothic72RedFont,__QGAMES_SCARDGOTHIC72REDLETTERS__)
	__DEFINEFONT__(ShowcardGothic72YellowFont,__QGAMES_SCARDGOTHIC72YELLOWLETTERS__)
}

#endif
  
// End of the file
/*@}*/
