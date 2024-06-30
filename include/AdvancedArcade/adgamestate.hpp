/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: adgamestate.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/04/2018 \n
 *	Description: Defines advanced game state ready to be used in the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADGAMESTATE__
#define __QGAMES_ADGAMESTATE__

#include <AdvancedArcade/addefinitions.hpp>
#include <Arcade/arcadeinclude.hpp>
#include <AdvancedArcade/adgame.hpp>

namespace QGAMES
{
	class ScoreObjectText;

	/** A NULL game state. JUst do nothing before finishing. 
		It shouldn't make sense to have a nested game state too, but it is considered just in case. */
	class NULLGameState : public GameState
	{
		public: 
		NULLGameState (int id, GameState* nS = NULL)
			: GameState (id, Game::game (), nS)
							{ }

		/** @see parent.
			Just finishes. */
		virtual void updatePositions ()
							{ notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); }

	};

	/** A game state just to spend the time. */
	class SpendingTimeGameState : public GameState
	{
		public:
		SpendingTimeGameState (int id, bdata s, GameState* nS = NULL)
			: GameState (id, Game::game (), nS),
			  _seconds (s)
							{ }

		/** To change the time. */
		bdata seconds () const
							{ return (_seconds); }
		void setSeconds (bdata s)
							{ _seconds = s; }

		/** To restart to count. */
		void reStart ();

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
							{ return (new Counters ()); }

		protected:
		/** The seconds to wait. */
		bdata _seconds;

		/** To control time. */
		static const int _COUNTERTIME = 0;
	};

	/** A game state that only finished when a event happens in the system.
		The class has to be extended to filter the event exactly. 
		By default it reacts to any event coming from there. */
	class ForEverUntilInputHandlerEventGameState : public GameState
	{
		public:
		/** The handler is used to determinate whether the 
			input event has or not to stop the state. */
		class Handler 
		{
			public:
			friend ForEverUntilInputHandlerEventGameState;
			
			Handler ()
				: _owner (NULL)
							{ }
			virtual ~Handler () 
							{ }

			ForEverUntilInputHandlerEventGameState* owner ()
							{ return (_owner); }

			/** Returns true when because the event, the parent state should finish, and false 
				in any other circumstance. To overload in classes inheriting this one. */
			virtual bool stopWhen (const Event& evnt) = 0;

			private:
			// Implementation. Set by the container class using the friend capability
			ForEverUntilInputHandlerEventGameState* _owner;
		};

		ForEverUntilInputHandlerEventGameState (int id, Handler* hdl, GameState* nS = NULL);
		~ForEverUntilInputHandlerEventGameState ();

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void onExit ();

		/** @see parent. */
		virtual void processEvent (const Event& evnt);

		protected:
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		protected:
		/** A reference to the handler managing the events. */
		Handler* _handler;

		/** Switch to indicate the game state has finished... */
		static const int _SWITCHFINISH = 0;
	};

	/** This state finishes when the enter key or the left button of the mouse is pressed. 
		The button 0 of the joystick is equivalent to enter and left button. */
	class ForEverUntilIntroKeyOrLeftButtonMousePressedGameState : 
			public ForEverUntilInputHandlerEventGameState
	{
		public:
		/** The handler just to detect this type of event. */
		class InputHandler : public Handler
		{
			public:
			InputHandler ()
				: Handler ()
							{ }

			/** @see parent. */
			virtual bool stopWhen (const Event& evnt);
		};

		ForEverUntilIntroKeyOrLeftButtonMousePressedGameState (int id, GameState* nS = NULL)
			: ForEverUntilInputHandlerEventGameState (id, new InputHandler (), nS)
							{ }
	};

	/** A game state that is a mixture of the previous two. */
	class SpendingTimeOrWaitingForInputHandlerGameState : public ForEverUntilInputHandlerEventGameState
	{
		public:
		SpendingTimeOrWaitingForInputHandlerGameState (int id, bdata s, Handler* hdl, GameState* nS = NULL)
			: ForEverUntilInputHandlerEventGameState (id, hdl, new SpendingTimeGameState (id * 10, s, nS)),
			  _timeSpend (NULL)
							{ _timeSpend = (SpendingTimeGameState*) nestedState (); }

		~SpendingTimeOrWaitingForInputHandlerGameState ()
							{ delete (_timeSpend); _timeSpend = NULL; }

		private:
		SpendingTimeGameState* _timeSpend;
	};

	/** A game state similar to previous but finishes when intro key or left mouse button is pressed */
	class SpendingTimeOrWaitingUntilAnyKeyPressedGameState : public SpendingTimeOrWaitingForInputHandlerGameState
	{
		public:
		/** The handler just to detect any key is pressed. */
		class InputHandler : public Handler
		{
			public:
			InputHandler ()
				: Handler ()
							{ }

			/** @see parent. */
			virtual bool stopWhen (const Event& evnt);
		};

		SpendingTimeOrWaitingUntilAnyKeyPressedGameState 
				(int id, bdata s, GameState* nS = NULL)
			: SpendingTimeOrWaitingForInputHandlerGameState (id, s, new InputHandler (), nS)
							{ }
	};

	/** A game state that spends a time and finishes unless input handler indicates the opposite in the meantime. */
	class SpendingTimeUnlessInputHandlerEventGameState : public ForEverUntilInputHandlerEventGameState
	{
		public:
		SpendingTimeUnlessInputHandlerEventGameState (int id, bdata s, Handler* hdl, GameState* nS = NULL)
			: ForEverUntilInputHandlerEventGameState (id, hdl, new SpendingTimeGameState (id * 10, s, nS))
							{ }

		/** @see parent. */
		virtual void processEvent (const Event& evnt);
	};

	/** This is the previous game state but when the intro key or the left mouse button is pressed. */
	class SpedingTimeUnlessKeyIsPressedGameState : public SpendingTimeUnlessInputHandlerEventGameState
	{
		public:
		/** The handler just to detect any key is pressed. */
		class InputHandler : public Handler
		{
			public:
			InputHandler ()
				: Handler ()
							{ }

			/** @see parent. */
			virtual bool stopWhen (const Event& evnt);
		};

		SpedingTimeUnlessKeyIsPressedGameState (int id, bdata s, GameState* nS = NULL)
			: SpendingTimeUnlessInputHandlerEventGameState (id, s, new InputHandler (), nS)
							{ }
	};

	/** A game state to draw a text in the screen. */
	class ShowingTextGameState : public GameState
	{
		public:
		/** This structure is used to determinate how to draw the text. */
		struct Properties
		{
			typedef enum { _CENTER = 0, _TOPLEFT, _BOTTONRIGHT } PosReference;

			Properties ()
				: _fade (255),
				  _reference (_TOPLEFT),
				  _position (Position ((bdata) 0.5, (bdata) 0.5, (bdata) 0)), // Center position...
				  _orientation (Vector ((bdata) 1, (bdata) 0, (bdata) 0)) // The orientation of the letters...
							{ }

			/**
			  *	@param f	Alpha grade to show the text. From 0 to 255..
			  *	@param p	Relative position in the screen where to show the left up corner of the text. \n
			  *				Coordinates from 0 to 1 (percentage) both included. 
			  */
			Properties (int f, const Position& p, PosReference pR, const Vector& o)
				: _fade (f),
				  _position (p),
				  _reference (pR),
				  _orientation (o)
				  			{ assert ((_fade >= 0 && _fade <= 255) &&
									  (_position.posX () >= (bdata) 0 && _position.posX () <= (bdata) 1) &&
									  (_position.posY () >= (bdata) 0 && _position.posY () <= (bdata) 1) &&
									  (_position.posZ () >= (bdata) 0 && _position.posZ () <= (bdata) 1)); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file:
				...
				<Attributes>
					<Attribute id = "FADE" value = "{0..255}"/>
					<Attribute id = "POSITION" value = "{0..1},{0..1},{0..1}"/>
					<Attribute id = "POSREFERENCE" value = "0|1|2"/> // Meaning _CENTER, _TOPLEFT, _BOTTOMRIGHT
					<Attribute id = "ORIENTATION" value = "X,Y,Z"/>
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough

			/** Between 0 and 255. */
			int _fade;
			/** This a relative position. So 0,0,0 means left up front corner and 1,1,1 right, bottom back one.
				0.5 indicates the middle of the screen. */
			Position _position;
			/** The previous point refers to a specific location of the text. */
			PosReference _reference;
			/** This is the orientation of the letters of the text. */
			Vector _orientation;
		};

		ShowingTextGameState (int id, const Properties& prps = Properties (), GameState* nS = NULL);
		~ShowingTextGameState ()
							{ delete (_text); }

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s);

		protected:
		/** To create or actualize the text that is going to be shown in the screen.
			It has to be overloaded by classes inheriting this one. */
		virtual ScoreObjectText* createText () = 0;

		/** To actualize the text (if necessary) being created with the previous method.
			This method is invoked in updatePositions method.
			Noting is done by default. */
		virtual void actualizeText ()
							{ }

		protected:
		/** The properties. */
		Properties _properties;

		// Implementation
		ScoreObjectText* _text;
	};

	/** 
		A game state to draw a standard text.
		The builder receives also the text to draw and the font used to do so.
		When the state is created from a builder, those two parameters have also to be defined
		in the list of attributes defining it:
		<Attributes>
			...
			<Attribute id = "TEXT" value="???"/>
			<Attribute id = "FONT" value="{0..}"/> // The internal number representing the font...
			...
		</Attributes>
		This is valid for any other class inheriting from this one.
	  */
	class ShowingStandardTextGameState : public ShowingTextGameState
	{
		public:
		ShowingStandardTextGameState (int id, const std::string& txt, int fTp,
				const Properties& prps = Properties (), GameState* nS = NULL)
			: ShowingTextGameState (id, prps, nS),
			  _textString (txt),
			  _fontType (fTp)
							{ }

		protected:
		/** @see parent. 
			Here it is like a factory method. */
		virtual ScoreObjectText* createText ();

		protected:
		std::string _textString;
		int _fontType;
		/** Potential values the value of the font. */
	};

	/** It is an special type of showing a standard text, consisting on blinking a little bit */
	class ShowingBlinkingStandardTextGameState : public ShowingStandardTextGameState
	{
		public:
		/** This structure is used to determinate how to draw the text. */
		struct Properties : public ShowingStandardTextGameState::Properties
		{
			Properties ()
				: ShowingStandardTextGameState::Properties (),
				  _elapsedCycles (2),
				  _numberFadeChanges (5),
				  _fadeStep (5)
							{ }

			/**
			  * The parameters of the parent plus:
			  * @param eC	Number of cycles before changing.
			  * @param nC	Number of changes in the fade to happen. 
			  * @param sC	Size of the step in any fade change. 
			  */
			Properties (int f, const Position& p, PosReference pR, const Vector& o, int eC, int nC, int sC)
				: ShowingStandardTextGameState::Properties (f, p, pR, o),
				  _elapsedCycles (2),
				  _numberFadeChanges (5),
				  _fadeStep (5)
							{ assert (_elapsedCycles > 0 && _numberFadeChanges > 0 && _fadeStep > 0); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file:
				...
				<Attributes>
					<Attribute id = "FADE" value = "{0..255}"/>
					<Attribute id = "ELAPSEDCYCLES" value = "{0..}"/>
					<Attribute id = "NUMBERFADECHANGES" value = "{0..}"/>
					<Attribute id = "FADESTEP" value = "{0..}"/>
					<Attribute id = "POSITION" value = "{0..1},{0..1},{0..1}"/>
					<Attribute id = "POSREFERENCE" value = "0|1|2"/> // Meaning _CENTER, _TOPLEFT, _BOTTOMRIGHT
					<Attribute id = "ORIENTATION" value = "X,Y,Z"/>
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough

			/** The number of game cycles to spend before chaing the fade level. */
			int _elapsedCycles;
			/** The number of changes to happen (going up and the same going down). */
			int _numberFadeChanges;
			/** The value the fade to be increment when it has to change. */
			int _fadeStep;
		};

		ShowingBlinkingStandardTextGameState (int id, const std::string& txt, int fTp,
				const Properties& prps = Properties (), GameState* nS = NULL)
			: ShowingStandardTextGameState (id, txt, fTp, prps, nS),
			  _properties (prps)
							{ }

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual void onEnter () override;

		protected:
		virtual void actualizeText ();

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		protected:
		Properties _properties;

		static const int _ELAPSEDCYCLEDCOUNTER = 0;
		static const int _NUMBERFADECHANGESCOUNTER = 1;
		static const int _COUNTINGUPSWITCH = 0;
	};

	/** A game state to draw the number of the current player. 
		The constructor receives a text to be drawn behind the number of the current player. */
	class ShowingCurrentPlayerNumberGameState : public ShowingStandardTextGameState
	{
		public:
		ShowingCurrentPlayerNumberGameState (int id, const std::string& txt, int fTp,
				const Properties& prps = Properties (), GameState* nS = NULL)
			: ShowingStandardTextGameState (id, txt, fTp, prps, nS)
							{ }

		protected:
		/** @see parent. */
		void actualizeText ();
	};

	/** A game state to draw the framerate.
		It could be nested in another one or used directly. */
	class ShowingFrameRateGameState : public ShowingStandardTextGameState
	{
		public:
		ShowingFrameRateGameState (int id = __QGAMES_GAMESTATEFPS__, // Can be created by default...
				const std::string& txt = std::string ("fps"), int fTp = __QGAMES_RAVIE24BLUELETTERS__,
				const Properties& prps = Properties (150, Position ((bdata) 0.98, (bdata) 0.98, (bdata) 0), 
					Properties::_BOTTONRIGHT, Vector ((bdata) 1, (bdata) 0, (bdata) 0)), GameState* nS = NULL)
			: ShowingStandardTextGameState (id, txt, fTp, prps, nS)
							{ }

		protected:
		/** @see parent. */
		void actualizeText ();
	};

	/** A state to play a sound. */
	class PlayingASoundGameState : public GameState
	{
		public:
		struct Properties
		{
			Properties ()
				: _soundId (0),
				  _channelId (0)
							{ }

			Properties (int sId, int cId)
				: _soundId (sId),
				  _channelId (cId)
							{ assert (_soundId >= 0 && _channelId >= 0); }

			/** A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id = "SOUNDID" value = "{0..}"/>
					<Attribute id = "CHANNELID" value = "{0..}"/>
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// The standard Default constructor is enough
			
			/** The id of the sound from a sound builder. */
			int _soundId;
			/** The channel to play the sound. */
			int _channelId;
		};

		PlayingASoundGameState (int id = __QGAMES_GAMESTATEPLAYSOUND__,
			const Properties& prps = Properties (), GameState* nS = NULL)
			: GameState (id, Game::game (), nS),
			  _properties (prps),
			  _soundPlaying (false)
							{ }

		void stopSound ()
							{ if (_soundPlaying) 
								{ game () -> soundBuilder () -> sound (_properties._soundId) -> pause ();  
								  _soundPlaying = false; } }
		void continueSound ()
							{ if (!_soundPlaying) 
								{ game () -> soundBuilder () -> sound (_properties._soundId) -> resume ();  
								  _soundPlaying = true; } }

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual void onEnter ();
		virtual void onExit ();

		protected:
		Properties _properties;

		// Implementation
		bool _soundPlaying;
	};

	/** A state just to show a form. */
	class ShowingFixFormGameState : public GameState
	{
		public:
		struct Properties
		{
			typedef enum { _CENTER = 0, _TOPLEFT, _BOTTONRIGHT } PosReference;

			Properties ()
				: _formId (__QGAMES_COMMTYNETSBIGLOGO__),
				  _fade (255),
				  _position (Position ((bdata) 0.5, (bdata) 0.5, (bdata) 0)), // Center in a 2D screen...
				  _reference (_CENTER)
							{ }
			/*
			  *	@param fId	Form id (to load from external resources).
			  * @param fd	The fade grade used to show the form. 
			  *	@param p	Relative position in the screen where to show the left up corner of the form. \n
			  *				Coordinates from 0 to 1 (percentage) both included.
			  * @param pR	The previous position refers to a specific location of the form.
			  */
			Properties (int fId, int fd, const Position& p, PosReference pR)
				: _formId (fId),
				  _fade (fd),
				  _position (p),
				  _reference (pR)
							{ assert (_fade >= 0 && _fade <= 255 && 
									  (_position.posX () >= (bdata) 0 && _position.posX () <= (bdata) 1) &&
									  (_position.posY () >= (bdata) 0 && _position.posY () <= (bdata) 1) &&
									  (_position.posZ () >= (bdata) 0 && _position.posZ () <= (bdata) 1)); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id = "FORMID" value = "{0..}"/>
					<Attribute id = "FADE" value = "{0..255}"/>
					<Attribute id = "POSITION" value = "{0..1},{0..1},{0..1}"/>
					<Attribute id = "POSREFERENCE" value = "0|1|2"/> // Meaning _CENTER, _TOPLEFT, _BOTTOMRIGHT
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough

			int _formId;
			int _fade;
			Position _position;
			/** The previous position can refer to many different parts of the form being shown. */
			PosReference _reference;
		};

		ShowingFixFormGameState (int id = __QGAMES_GAMESTATESHOWFIXFORM__,
				const Properties& prps = Properties (), GameState* nS = NULL)
				: QGAMES::GameState (id, Game::game (), nS),
				  _properties (prps)
							{ }

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual void drawOn (Screen* s);

		protected:
		/** The properties. */
		Properties _properties;
	};

	/** A state just to show a form for a while.
		It can be possible to create as appearing or disappearing the form. */
	class ShowingAppearingFormGameState : public GameState
	{
		public:
		struct Properties
		{
			typedef enum { _CENTER = 0, _TOPLEFT, _BOTTONRIGHT } PosReference;

			Properties ()
				: _formId (__QGAMES_COMMTYNETSBIGLOGO__),
				  _initialFade (255), _finalFade (255), _fadeIncrement (10),
				  _loopsPerFadeChange (6), 
				  _position (Position ((bdata) 0.5, (bdata) 0.5, (bdata) 0)), // Center in a 2D screen...
				  _reference (_CENTER)
							{ }
			/*
			  *	@param fId	Form id (to load from external resources).
			  *	@param iF	Initial fade of the form. From 0 to 255
			  *	@param fF	Final fade of the the form. From 0 to 255.
			  * @param fI	Fade increment when it takes place (look at next parameters). Could be negative.
			  * @param lPFC	Loops spent before increment the fade value.
			  *	@param p	Relative position in the screen where to show the left up corner of the form. \n
			  *				Coordinates from 0 to 1 (percentage) both included.
			  * @param pR	The previous position refers to a specific location of the form.
			  */
			Properties (int fId, int iF, int fF, int fI, int lPFC, const Position& p, PosReference pR)
				: _formId (fId),
				  _initialFade (iF), _finalFade (fF), _fadeIncrement (fI),
				  _loopsPerFadeChange (lPFC),
				  _position (p),
				  _reference (pR)
							{ assert (_initialFade >= 0 && _finalFade >= 0 && 
									  _initialFade <= 255 && _finalFade <= 255 &&
									  _loopsPerFadeChange >= 1 && 
									  ((_finalFade - _initialFade) / _fadeIncrement) > 1 && // Just to avoid bad working...
									  (_position.posX () >= (bdata) 0 && _position.posX () <= (bdata) 1) &&
									  (_position.posY () >= (bdata) 0 && _position.posY () <= (bdata) 1) &&
									  (_position.posZ () >= (bdata) 0 && _position.posZ () <= (bdata) 1)); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id = "FORMID" value = "{0..}"/>
					<Attribute id = "INITIALFADE" value = "{0..255}"/>
					<Attribute id = "FINALFADE" value = "{0..255}"/>
					<Attribute id = "FADEINCREMENT" value = "{-255..255}"/>
					<Attribute id = "LOOPSPERFADECHANGE" value = "{0..}"/>
					<Attribute id = "POSITION" value = "{0..1},{0..1},{0..1}"/>
					<Attribute id = "POSREFERENCE" value = "[0,1,2]"/> // Meaning _CENTER, _TOPLEFT, _BOTTOMRIGHT
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough

			int _formId;
			int _initialFade;
			int _finalFade;
			int _fadeIncrement;
			/** The rhythm the fade changes.
				The bigger the parameter is the longer it takes to change. */
			int _loopsPerFadeChange;
			Position _position;
			/** The previous position can refer to many different parts of the form being shown. */
			PosReference _reference;
		};

		ShowingAppearingFormGameState (int id = __QGAMES_GAMESTATESHOWFORM__,
				const Properties& prps = Properties (), GameState* nS = NULL);

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		bool hasFinished () // When the for has reached the final state...
							{ return (_currentFade == _properties._finalFade); }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
							{ return (new Counters ()); }

		protected:
		/** The properties. */
		Properties _properties;

		// Implementation
		int _currentFade;

		/** To control the time between two fade transitions. */
		static const int _COUNTERWAITFADE = 0;
	};

	/** A game state to use just to show an entity.  */
	class ShowingEntityGameState : public GameState
	{
		public:
		/** This structure is used to determinate where the entity showing the game paused is drawn. */
		struct Properties
		{
			typedef enum { _CENTER = 0, _TOPLEFT, _BOTTONRIGHT } PosReference;

			Properties ()
				: _entityId (__QGAMES_COMMTYNETSPAUSEENTITYID__),
				  _fade (255),
				  _controlPosition (true),
				  _position (Position ((bdata) 0.5, (bdata) 0.5, (bdata) 0)), // Center position in a 2D screen...
				  _reference (_CENTER)
							{ }

			/**
			  *	@param eId	Entity id from the entity builder.
			  *	@param f	Alpha grade to show the entity. From 0 to 255.
			  * @param cP	whether to control or not the position of the entity.
			  *				It is not, the position is controlled by the own entity.
			  *	@param p	Relative position in the screen where to show the left up corner of the text. \n
			  *				Coordinates from 0 to 1 (percentage) both included. 
			  *				This parameter is only valid when entity's position is controlled by the game state.
			  * @param pR	The previous position refers to a specific location of the form.
			  */
			Properties (int eId, int f, bool cP, const Position& p, PosReference pR)
				: _entityId (eId),
				  _fade (f),
				  _controlPosition (cP),
				  _position (p),
				  _reference (pR)
				  			{ assert ((_fade >= 0 && _fade <= 255) &&
									  (_position.posX () >= (bdata) 0 && _position.posX () <= (bdata) 1) &&
									  (_position.posY () >= (bdata) 0 && _position.posY () <= (bdata) 1) &&
									  (_position.posZ () >= (bdata) 0 && _position.posZ () <= (bdata) 1)); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id = "ENTITYID" value = "{0..}"/>
					<Attribute id = "FADE" value = "{0..255}"/>
					<Attribute id = "CONTROLPOSITION" value = "YES|NO"/>
					<Attribute id = "POSITION" value = "{0..1},{0..1},{0..1}"/>
					<Attribute id = "POSREFERENCE" value = "0|1|2"/> // Meaning _CENTER, _TOPLEFT, _BOTTOMRIGHT
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough

			int _entityId;
			int _fade;
			bool _controlPosition;
			/** Indicates a relative position.
				So 0,0,0 means left up front corner, and 1,1,1 right bottom back one.
				0.5 indicates the center the of visualization area. */
			Position _position;
			/** The previous position can refer to many different parts of the fomr being shown. */
			PosReference _reference;
		};

		ShowingEntityGameState (int id = __QGAMES_GAMESTATESHOWENTITY__, 
				const Properties& prps = Properties (), GameState* nS = NULL);

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s);
		virtual void onExit ();

		protected:
		/** The properties. */
		Properties _properties;
	};

	/** A game state showing a form being fulfilled. */
	class ShowingFormBeingFulfilledGameState : public GameState
	{
		public:
		struct Properties
		{
			typedef enum { _CENTER = 0, _TOPLEFT, _BOTTONRIGHT } PosReference;

			Properties ()
				: _formId (__QGAMES_COMMTYNETSBIGLOGO__),
				  _fade (255),
				  _position (Position ((bdata) 0.5, (bdata) 0.5, (bdata) 0)), // Center position in a 2D screen...
				  _reference (_CENTER),
				  _reverse (false), // Means from the left (or top) to the end...
				  _horizontal (false) // Means vertical...
							{ }

			/**
			  *	@param fId	Form id from the form builder.
			  *	@param f	Alpha grade to show the form. From 0 to 255.
			  *	@param p	Relative position in the screen where to show the left up corner of the form. \n
			  *				Coordinates from 0 to 1 (percentage) both included. 
			  * @param pR	The previous position refers to a specific location of the frame being fulfilled.
			  *	@param r	From down to up? (true means from down to up, false the opposite).
			  *	@param h	From right to left? (true means from left to right).
			  */
			Properties (int fId, int f, const Position& p, PosReference pR, bool r, bool h)
				: _formId (fId),
				  _fade (f),
				  _position (p),
				  _reference (pR),
				  _reverse (r),
				  _horizontal (h)
				  			{ assert ((_fade >= 0 && _fade <= 255) &&
									  (_position.posX () >= (bdata) 0 && _position.posX () <= (bdata) 1) &&
									  (_position.posY () >= (bdata) 0 && _position.posY () <= (bdata) 1) &&
									  (_position.posZ () >= (bdata) 0 && _position.posZ () <= (bdata) 1)); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id = "FORMID" value = "{0..}"/>
					<Attribute id = "FADE" value = "{0..255}"/>
					<Attribute id = "POSITION" value = "{0..1},{0..1},{0..1}"/>
					<Attribute id = "POSREFERENCE" value = "0|1|2"/> // Meaning _CENTER, _TOPLEFT, _BOTTOMRIGHT
					<Attribute id = "REVERSE" value = "YES|NO"/>
					<Attribute id = "HORIZONTAL" value = "YES|NO"/>
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough

			int _formId;
			int _fade;
			/** Indicates a relative position within the visualization area.
				So 0,0,0 would mean left up from corner, and 1,1,1 the opposite (right down and back one).
				0.5 points to the middle. */
			Position _position;
			/** The previous position refers to a specific location of the entity being fulfilled. */
			PosReference _reference;
			/** Whether the form fills starting from left up corner or from the bottom right one. 
				true means bottom right. false means top left one. */
			bool _reverse;
			/** Whether the form fills in the horizontal direction or in the vertical one. */
			bool _horizontal;
		};

		ShowingFormBeingFulfilledGameState (int id, 
				const Properties& prps = Properties (), GameState* nS = NULL);

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		bool hasFinished ()
							{ return (_currentPercentage >= 100); }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s);

		protected:
		/** To execute what is needed during the "loading" effect.
			The method returns the new percentage. But default it is incremented in one until reaching the limit. */
		virtual int doAction ()
							{ return (++_currentPercentage); }

		protected:
		/** The properties. */
		Properties _properties;

		// Implementation
		int _currentPercentage;
	};

	/** A class to load the resources of the game with a form being fulfill behind. */
	class LoadingGameState : public ShowingFormBeingFulfilledGameState
	{
		public:
		LoadingGameState (int id = __QGAMES_GAMESTATELOADING__, 
				const Properties& prps = Properties (), GameState* nS = NULL);

		/** To know how many items have to be loaded per doAction execution. */
		int toLoadPerCycle () const
							{ return (_toLoadPerCycle); }
		void setToLoadPerCycle (int lC)
							{ assert (lC > 0); _toLoadPerCycle = lC; }

		/** @see parent. */
		virtual void onEnter ();

		protected:
		/** @see parent. */
		virtual int doAction ();

		protected:
		/** How many items have to be loaded per doAction execution? */
		int _toLoadPerCycle;

		// Implementation
		/** The max number of elements to load per type of resources. */
		int _maxFormsToLoad, 
			_maxSoundsToLoad,
			_maxMovementsToLoad, 
			_maxEntitiesToLoad,
			// Objects are not considered because they are quit simple...
			_maxMapsToLoad,
			_maxWorldsToLoad;
		/** The current number of elements per type already loaded. */
		int _currentFormsLoaded,
			_currentSoundsLoaded,
			_currentMovementsLoaded,
			_currentEntitiesLoaded,
			_currentMapsLoaded,
			_currentWorldsLoaded;
		/** The list of the id of the elements per type to load. */
		std::vector <int> _refForms,
						  _refSounds,
						  _refMovements,
						  _refEntities,
						  _refMaps,
						  _refWorlds;
	};

	/** A game state to show the instructions. */
	class ShowingInstructionsGameState : public GameState
	{
		public:
		/** This structure is used to determinate how to draw the different lines of the instructions. 
			Each text of the instructions can have a different aspect that other. */
		struct Properties
		{
			struct TextLine
			{
				typedef enum { _CENTER = 0, _LEFT, _RIGHT } PosReference;

				TextLine ()
					: _text (std::string (__NULL_STRING__)),
					  _type (0),
					  _font (__QGAMES_RAVIE24ORANGELETTERS__),
					  _position (0.5), // In the middle of the visible screen...
					  _reference (_CENTER)
							{ }

				TextLine (const std::string& txt, int tp, int fnt, bdata pos, PosReference ref)
					: _text (txt),
					  _type (tp),
					  _font (fnt),
					  _position (pos),
					  _reference (ref)
							{ assert (_font >= 0 && 
									  (_position >= 0 && _position <= 1)); }

				// Default copy constructor is enough
				
				/** The text to draw in the line */
				std::string _text;
				/** The type of the text: The title, one line, the bottom,... The meaning
					will depend on the class that read it. */
				int _type;
				/** The font used to draw this line. */
				int _font;
				/** Where is the position in the screen. */
				bdata _position;
				/** Where the text is located in the line. */
				PosReference _reference;
			};

			typedef std::vector <TextLine> TextLineList;

			Properties ()
				: _texts (TextLineList ()),
				  _textSeparation (0),
				  _visibleZone (Rectangle (Position ((bdata) 0, (bdata) 0, (bdata) 0),
										   Position ((bdata) 1, (bdata) 1, (bdata) 1))), // The full screen...
				  _fade (255),
				  _movement (-1), // ...from bottom to top of the visible zone...
				  _toMove (0)
							{ }

			/**
			  *	@param txt	A vector with the list of the text to draw.
			  * @param tS	The number of pixels among the lines of the text. It could be negative.
			  * @param rect	The rectrangle where the 
			  * @param f	Alpha grade to show the texts. From 0 to 255..
			  * @param m	Number of pixels to move up the text.
			  * @param s	Number of seconds before moving the texts again as a whole.
			  */
			Properties (const TextLineList& txts, int tS, const Rectangle& rect, 
						int f, int m, int sm)
				: _texts (txts),
				  _textSeparation (tS),
				  _visibleZone (rect),
				  _fade (f),
				  _movement (m),
				  _toMove (sm)
				  			{ assert (!_texts.empty () &&
								      (_fade >= 0 && _fade <= 255) &&
									  _movement > 0); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				This an standard definition. 
				...
			 	<Instructions>
			 		<Instruction line = "XXXX" type = "X" font = "{0..}" position {0..1} reference = {0,1,2} />
			 		...
			 	</Instructions>
				<Attributes>
					<Attribute id = "SEPARATION" value = "X"/>
					<Attribute id = "VISIBLEZONE" value = "X1,Y1,Z1,X2,Y2,Z2"/>
					<Attribute id = "FADE" value = "{0..255}"/>
					<Attribute id = "MOVEMENT" value = "{1.."/>
					<Attribute id = "TOMOVE" value = "{0..}"/>
				</Attributes>
				...
			*/
			Properties (const TextLineList& txt, const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough

			/** The list of the texts to show in the instructions. */
			TextLineList _texts;
			/** The separation among the different lines of the text in pixels. 
				It could be negative event, meaning the lines overlap each other. */
			int _textSeparation;
			/** The rectangle where the instructions are visible. */
			Rectangle _visibleZone;
			/** Between 0 and 255. */
			int _fade;
			/** This is the number of pixels to move up. */
			int _movement;
			/** The number of seconds to wait before moving again the whole texts. */
			int _toMove;
		};

		ShowingInstructionsGameState (int id, const Properties& prps = Properties (), GameState* nS = NULL)
			: GameState (id, Game::game (), nS),
			  _properties (prps),
			  _currentPosition (0), // which is the position inside of the visible zone in pixels... (0 bottom)
			  _lastPositionDrawn (0)
							{ }

		~ShowingInstructionsGameState ()
							{ deleteTexts (); }

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		/** To create one of the lines of the instructions. */
		virtual QGAMES::ScoreObjectText* createTextForLine (const Properties::TextLine& tL);
		/** To delete all text created. */
		void deleteTexts ();

		protected:
		/** The properties. */
		Properties _properties;

		static const int _COUNTERTOMOVE = 0;
		static const int _SWITCHTOMOVE = 0;

		// Implementation
		std::vector <QGAMES::ScoreObjectText*> _texts;
		int _currentPosition; // Relative In the y axis...
		bool _lastPositionDrawn; // The last position drawn...

	};

	/** A state to count down. */
	class CountingDownGameState : public GameState
	{
		public:
		struct Properties
		{
			Properties ()
				: _formId (__QGAMES_GAMESTATECOUNTDOWNSTDBSCFORM__),
				  _beepSoundId (__QGAMES_BEEPWAVSOUND__),
				  _framesPerNumber (__QGAMES_GAMESTATECOUNTDOWNSTDFRAMES__),
				  _framesOverlapped (2),
				  _numbers (__QGAMES_GAMESTATECOUNTDOWNSTDNUMBERS__), // A number can be any thing remember!!
				  _position (Position ((bdata) 0.5, (bdata) 0.5, (bdata) 0)) // The center of the counting...
							{ }

			/**
			  * @param fId	The form id (from the form builder) with the basic size.
			  * @param sId	The sound id (from the sound builder).
			  * @param fPN	The number of different frames representing each number. \n
			  *				The more number of frames the more quality in the game state.
			  *	@param fO	How many frames before a number reaches it biggest size, the next number has to start to appear?.
			  * @param n	How many number have to be counting?
			  *	@param p	Relative position in the screen where to show the center of each number. \n
			  *				Coordinates from 0 to 1 (percentage) both included.
			  */
			Properties (int fId, int sId, int fPN, int fO, int n, const Position& p)
				: _formId (fId),
				  _beepSoundId (sId),
				  _framesPerNumber (fPN),
				  _framesOverlapped (fO),
				  _numbers (n),
				  _position (p)
				  			{ assert ((_framesOverlapped < _framesPerNumber && _numbers > 0) &&
									  (_position.posX () >= (bdata) 0 && _position.posX () <= (bdata) 1) &&
									  (_position.posY () >= (bdata) 0 && _position.posY () <= (bdata) 1) &&
									  (_position.posZ () >= (bdata) 0 && _position.posZ () <= (bdata) 1)); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id = "FORMID" value = "{0..}"/>
					<Attribute id = "BEEPSOUNDID" value = "{0..}"/>
					<Attribute id = "FRAMESPERNUMBER" value = "{0..MAX}"/>
					<Attribute id = "FRAMESOVERLAPPED" value = "{0..MAX - 1}"/>
					<Attribute id = "NUMBERS" value = "{0..}"/>
					<Attribute id = "POSITION" value = "{0..1},{0..1},{0..1}"/>
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough
			/** The form having the numbers with the small size possible.
				Take into account that a number of framePerNumber must follow in id to that. */
			int _formId;
			/** Id of the sound to play when a new number appears. 
				It has to exist, but it is not checked when a properties object is created. */
			int _beepSoundId;
			int _framesPerNumber;
			/** How many frames in advance the new number overlaps the previous one?
				Obviously it couldn't be greater than the number of total frames per number. */
			int _framesOverlapped;
			int _numbers;
			/** This a reference position where to how the numbers. 
				0,0,0 would mean the left top from par of the screen, and 1,1,1 the right bottom back one.
				So 0.5 is just the middle. */
			Position _position;
		};

		CountingDownGameState (int id = __QGAMES_GAMESTATECOUNTINGDOWN__, 
				const Properties& prps = Properties (), GameState* nS = NULL);

		const Properties& properties () const
							{ return (_properties); }
		/** In this case, when properties change there is no change but start back again. */
		void setProperties (const Properties& prps);

		bool hasFinished ()
							{ return (onOffSwitch (_SWITCHSTATEHASFINISHED) -> isOn ()); }
		void finish () // To force the finalization of the counting down...
							{ onOffSwitch (_SWITCHSTATEHASFINISHED) -> set (true); }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s);
		virtual void onExit ();

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		protected:
		/** The properties. */
		Properties _properties;

		// Implementation
		struct State 
		{
			State ()
				: _number (0), _size (0), _fade (255), _used (false)
								{ }
			State (int n, int s, int f, bool u)
				: _number (n), _size (s), _fade (f), _used (false)
								{ }

			int _number;
			int _size;
			int _fade;
			bool _used;
		};

		typedef std::vector <State> States;
		States _states;

		/** To control the time a number is on the screen... */
		static const int _COUNTERONSCREEN = 0;
		/** To indicate that the state has finished. */
		static const int _SWITCHSTATEHASFINISHED = 0;
	};

	/** A state to show the hall of the fame. 
		This state doesn't allow to introduce a new one. It just to show the current one simply. */
	class ShowingHallOfFameGameState : public GameState
	{
		public:
		/** This structure is used to determinate how to draw the hall of fame. */
		struct Properties
		{
			Properties ()
				: _fade (255),
				  _position (Position ((bdata) 10, (bdata) 10, (bdata) 0)), // Center position in a 2D screen...
				  _separation (5), // Separation in pixels among each entry
				  _font (__QGAMES_RAVIE24ORANGELETTERS__)
							{ }
			/**
			  * @param f	Alpha grade for the letters shown in the hall of fame. From 0 to 255.
			  *	@param p	Relative position in the screen where to show the center of each number. \n
			  *				Coordinates from 0 to 1 (percentage) both included.
			  *	@param s	The separation in pixels among the lines of text. Can be negative \n
			  *				meaning the lines link a little bit.
			  *	@param fnt	The id of the font used.
			  */
			Properties (int f, const Position& p, int s, int fnt = __QGAMES_RAVIE24ORANGELETTERS__)
				: _fade (f),
				  _position (p),
				  _separation (s), // It could be negative (to link a little bit the lines if wanted)
				  _font (fnt)
				  			{ assert ((_fade >= 0 && _fade <= 255) &&
									  (_position.posX () >= (bdata) 0 && _position.posX () <= (bdata) 1) &&
									  (_position.posY () >= (bdata) 0 && _position.posY () <= (bdata) 1) &&
									  (_position.posZ () >= (bdata) 0 && _position.posZ () <= (bdata) 1)); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id = "FONT" value ="{0..}"/>
					<Attribute id = "FADE" value = "{0..255}"/>
					<Attribute id = "POSITION" value = "{0..1},{0..1},{0..1}"/>
					<Attribute id = "SEPARATION" value = "X"/>
					<Attribute id = "FONT" value ="{0..}"/> // The internal code of the font...
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough

			int _fade;
			/** This is relative position in the visualization area
				So 0,0,0 would mean the top,left and front part. 1,1,1 would mean the opposite, it is the right, bottom back one.
				0.5 indicates the middle of the screen. */
			Position _position;
			/** Which is the separation in pixels among the different entries of the hall of fame.
				It could be negative, indicating the texts overlap a little. */
			int _separation;
			/** The id of the font used. */
			int _font;
		};

		ShowingHallOfFameGameState (int id = __QGAMES_GAMESTATEHALLOFFAME__, 
				const Properties& prps = Properties (), GameState* nS = NULL);
		~ShowingHallOfFameGameState ()
							{ deleteTexts (); }

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual void onEnter ();
		virtual void drawOn (Screen* s);

		protected:
		/** Method to order the hall of fame before being printed out. 
			By default it does nothing. */
		virtual std::multimap <std::string, SetOfOpenValues> orderHallOfFame 
			(const std::multimap <std::string, SetOfOpenValues>& hF)
							{ return (hF); }
		/** Method to determinate how to create the text representing any entry of the hall of fame. 
			This is invoked when the game state enters. 
			By default Ravie24Orange is used as font, and only key and first value are used to draw. */ 
		virtual ScoreObjectText* createTextFor (const std::string& eK, const SetOfOpenValues& sV);

		private:
		// Implementation
		void deleteTexts ();

		protected:
		/** The properties. */
		Properties _properties;

		// Implementation
		std::vector <ScoreObjectText*> _texts;
	};

	/** This state consists on several forms (pictures) appearing, standing for a while 
		and disappearing whilst the next one starts to appear.
		The state is often used to create rolling backgrounds at presentation time. */
	class RollingFormsGameState : public GameState 
	{
		public:
		struct Properties
		{
			Properties ()
				: _forms (),
				  _delay (6),
				  _secondsOnScreen ((bdata) 1),
				  _direction (Vector ((bdata) 1, (bdata) 0, (bdata) 0)),
				  _instances (1), // Just 1 instance so far by default...
				  _sequential (false)
							{ }

			/**
			  * @param frms	list of forms rolling.
			  * @param d	loops a form spend in the same position. \n
			  *				The bigger the number is the longer it takes to move forward.
			  *	@param sS	Seconds, or fraction of seconds, a form is moving along in the screen
			  *				before the next one start to blend with the current one. 
			  *	@param dir	The direction of the movement (can be what ever). \n
			  *				The value of the coordinates affects to the speed of the movement as well.
			  *	@param i	Number of instances the initial form is replicated.
			  * @param s	Indicates whether the forms are sequential or not.
			  */
			Properties (std::vector <int> frms, int d, bdata sS, const Vector& dir, int i, bool s)
				: _forms (frms),
				  _delay (d),
				  _secondsOnScreen (sS),
				  _direction (dir),
				  _instances (i),
				  _sequential (s)
							{ assert (!_forms.empty () && _delay > 0 && 
									  _secondsOnScreen > (bdata) 0 && _instances > 0); } 

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id = "FORMS" value = "{0..},{0..},.."/>
					<Attribute id = "DELAY" value = "{0..}"/>
					<Attribute id = "SECONDSONSCREEN" value = "{0..}"/>
					<Attribute id = "DIRECTION" value = "{0..1},{0..1},{0..1}"/>
					<Attribute id = "INSTANCES" value = "{1..}"/>
					<Attribute id = "SEQUENTIAL" value = "YES|NO"/>
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough

			/** To check whether two set of properties sill generate or not similar results
				in the rolling background effect later. 
				It is used to move the status of a rolling background to another when it is possible. */
			bool similarTo (const Properties& prps) const
							{ return (_forms.size () == prps._forms.size () &&
									  _direction == prps._direction &&
									  _instances == prps._instances &&
									  _sequential == prps._sequential); }

			std::vector <int> _forms;
			int _delay;
			bdata _secondsOnScreen;
			/** The movement of the direction.
				It indicates also the speed. So 2,0,0 is double than 1,0,0 in terms of pixels. */
			Vector _direction;
			int _instances;
			bool _sequential;
		};

		RollingFormsGameState (int id = __QGAMES_GAMESTATEROLLINGFRMS__, 
				const Properties& prps = Properties (), GameState* nS = NULL);
		// No destructor is needed as status is made up object not pointers to them.

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps);

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s);

		/** Move current movement status from a rolling state to another when
			both are similar. This method is really dangerous because is changing a very internal
			attribute related with the mechanism of the state. */
		void addoptStatusFrom (RollingFormsGameState* st);

		protected:
		/** To create the forms status first time they are used. */
		void createForms ();

		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		protected:
		/** The properties. */
		Properties _properties;

		/** A class to define the status (position among many other things) of the different
			forms shown during the presentation. */
		struct PresentationStatus 
		{
			PresentationStatus ()
				: _form (NULL),
				  _fadeGrade (0),
				  _status (_INVISIBLE)
							{ }

			Form* _form;
			std::vector <Vector> _positions;
			int _fadeGrade;
			enum { _VISIBLE = 0, _FADEIN = 1, _FADEOUT = 2, _INVISIBLE = 3 } _status;
		};

		/** The different backgrounds used during the presentation. */
		std::vector <PresentationStatus> _presentationStatus;

		// Implementation...
		/** What are the forms to paint at any moment. 
			Maximum there are two: the one appearing and other one disappearing. */
		int _frmToPaint1, _frmToPaint2;

		// Counters & Switches
		static const int _COUNTERTOMOVE = 0;
		static const int _COUNTERTOSTARTNEXT = 1;
		static const int _SWITCHDOINGFRMFADE = 0;
	};

	/** This state is to show several texts for a while.
		They appear, stand for a while and disappear little by little. 
		This state is often used to create the credits in a game at presentation time. */
	class CreditTitlesGameState : public GameState
	{
		public:
		struct Properties
		{
			typedef enum { _CENTER = 0, _TOPLEFT, _BOTTONRIGHT } PosReference;

			Properties ()
				: _titles (),
				  _maxLettersPerTitle (-1),
				  _position (Vector::_cero),
				  _reference (_CENTER),
				  _secondsOnScreen ((bdata) 1),
				  _font (__QGAMES_RAVIE24ORANGELETTERS__)
							{ }

			/**
			  *	@param tts	The texts to show.
			  * @param mL	The max number of letters to show per line. -1 means not to split the line.
			  * @param rP	Where to locate the text. \n
			  *				This is a relative value. Coordinate from 0 to 1 both included.
			  * @param pR	Type of adjustment. That parameter gives more value to the previous one. \n
			  *				_CENTER means that the other point would refer to the center of the text (the very center). \n
			  *				_TOPLEFT will refer to the left up corner of the text. \n
			  *				_BOTTOMRIGHT same but to the right down corner.
			  *	@param sS	Number of seconds (or fraction) a text is in the screen.
			  * @param fnt	The font used to draw the titles.
			  */
			Properties (const std::vector <std::string>& tts, int mL, const Vector& rP, 
					PosReference pR, bdata sS, int fnt = __QGAMES_RAVIE24ORANGELETTERS__)
				: _titles (tts),
				  _maxLettersPerTitle (mL),
				  _position (rP),
				  _reference (pR),
				  _secondsOnScreen (sS),
				  _font (fnt)
							{ assert ((_maxLettersPerTitle == -1 || _maxLettersPerTitle > 1) &&
									  _secondsOnScreen > (bdata) 0); }

			/** 
			 	A constructor based on parameters.
		 	 	It is used in the game state constructor.
			 	The structure of the game state control definition file :
			 	...
			 	<Titles>
			 		<Title value="XXXX"/>
			 		...
			 	</Titles>
			 	<Attributes>
			 		<Attribute id = "MAXLETTERSPERTITLE" value = "{1..}"/>
			 		<Attribute id = "POSITION" value = "{0..1},{0..1},{0..1}"/>
			 		<Attribute id = "REFERENCE" value = "0|1|2"/> // Meaning _CENTER, _TOPLEFT, or BOTTOMRIGHT
			 		<Attribute id = "SECONDSONSCREEN" value = "{0..}"/>
					<Attribute id = "FONT" value = "{0..}/> // The internal id of the font...
			 	</Attributes>
			 	...
			 */
			Properties (const std::vector <std::string>& tts, 
				const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough

			std::vector <std::string> _titles;
			/** -1 will mean there is no limit, so titles will occupy whatever is needed. */
			int _maxLettersPerTitle;
			/** The reference position indicates where to locate the titles within the screen.
				It refers to the central position of the text. 
				Coordinates must be among 0 and 1,meaning 0 top left front position and 1 right, down and back position.
				So, 0.5 will indicate the middle point of the visualization area. */
			Position _position;
			/** The adjustment.*/
			PosReference _reference;
			/* Time in seconds every title is fixed on the screen. 
				The value could be a fraction of a second and obviously always positive. */
			bdata _secondsOnScreen;
			/** The font used to paint. */
			int _font;
		};

		CreditTitlesGameState (int id = __QGAMES_GAMESTATECREDITTITLES__, 
			const Properties& prps = Properties (), GameState* nS = NULL);
		~CreditTitlesGameState ()
							{ destroyTitles ();}

		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps);

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s);

		protected:
		/** To create the text with the right font.
			This is something that can be overloaded in every particular implementation. 
			By default, Ravie 24 Orange is used. */
		virtual ScoreObjectText* createText (const std::string& txt);
		/** To create the titles first time they are used. */
		void createTitles ();
		/** Same to destroy them. */
		void destroyTitles ();

		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		protected:
		/** The properties. */
		Properties _properties;

		protected:
		/** A class to define the status of every title (position among many other things)
			within the application. */
		struct TitleStatus
		{
			typedef enum 
				{ _VISIBLE = 0, _FADEOUT = 1, _INVISIBLE = 3 } Status;

			TitleStatus (ScoreObjectText* txt, int mLL, Properties::PosReference adj, int fG, Status st);
			~TitleStatus ();

			/** The different lines per title. */
			std::vector <ScoreObjectText*> _texts;
			/** The starting point of every title line. */
			std::vector <Position> _positions;

			// Implementation
			// Used during the flow...
			int _fadeGrade; 
			Status _status;
		};

		/** A vector with the titles to present at any moment. */
		std::vector <TitleStatus*> _titleStatus;

		// Implementation...
		/** Which one is the title being painted at a moment. */
		int _titleToPaint;

		static const int _COUNTERTEXTATSCREEN = 0;
		static const int _SWITCHDOINGTEXTFADE = 0;
	};

	/** An state to manage a selection process.
		A selection process goes through different 
		The GUI system manages a menu, but it necessary to program what to do 
		when a widget is selected. 
		In many occasions after selecting one option in one menu, another menu has to appear.
		This class manages all these iterations. */
	class SelectionOptionsGameState : public GameState
	{
		public:
		/** A menu is a set with the GUI and where to go (if any) when a widget is selected. */
		struct Menu
		{
			/** A connection indicates a menu to go to and a widget 
				to be focused first (if any. */
			struct Connection
			{
				/**
					* @param mId	The menu to go. 
					* @param fwId	The widget to be focused first. -1 means the default one.
					*/
				Connection (int mId, int fwId = -1)
					: _menuId (mId),
					  _firstFocusedWidgetId (fwId)
						{ }

				int _menuId;
				int _firstFocusedWidgetId;
			};

			/** The second int is the menu connected,
				and the first int is the widget connecting to that new menu. */
			typedef std::map <int, Connection> Connections;

			/**
				* @param id		The id of the gui system used to represent the menu.
				* @param pos	The position in the screen of the menu (first letter). 
				* @param c		A set of connections. It can be empty, meaning there are no connections.
				*/
			Menu (int id, const Position& pos = Position::_cero, const Connections& c = Connections ())
				: _guiSystemId (id),
				  _position (pos),
				  _connections (c)
						{ }

			int _guiSystemId;
			Position _position;
			Connections _connections; // If any...
		};

		typedef std::map <int, Menu> Menus;

		/** The main widget has to inherit from this class. 
			This class only ensures the events coming from the widgets are sent to the game state. 
			The logic to control how the options affects the game is then in the game state. */
		class MenuControlWidget : public CompositeWidget
		{
			public:
			MenuControlWidget (int id, Decorator* d = NULL, const ListOfWidgets& wdts = ListOfWidgets (), int wF = -1)
				: CompositeWidget (id, d, wdts, wF)
							{ }

			/** @see parent. */
			virtual void processEvent (const Event& evnt)
							{ notify (evnt); } // Just notifies the event in order the game state (observing it) shall takes note
		};

		/** When an widget is selected, apart of looking for a potential other gui system connected
			the handler is invoked just in case something should be done. */
		class Handler
		{
			public:
			friend SelectionOptionsGameState;

			Handler ()
				: _owner (NULL)
							{ }
			virtual ~Handler ()
							{ }

			SelectionOptionsGameState* owner ()
							{ return (_owner); }

			/** What to do specifically when a widget is selected.
				Return a code for further actions. The basic process event will undertand the next:
				-1 Nothing to do nothing, -2 To rebuilt the menu, any other number to exit the menu. */
			virtual int whenWidgetSelected (Widget* wdt) = 0;

			protected:
			SelectionOptionsGameState* _owner;
		};

		/** A handler doing anything. */
		class NoHandler : public Handler
		{
			public:
			NoHandler ()
				: Handler ()
							{ }

			/** @see parent. */
			virtual int whenWidgetSelected(Widget* wdt)
							{ return (-1); /** It doesn't make state to finish either. */ }
		};

		SelectionOptionsGameState (int id, const Menus& ms, Handler* = new NoHandler, GameState* ns = NULL);
		~SelectionOptionsGameState ();

		int lastOptionSelected () const
							{ return (_lastOptionSelected); }

		const Menus& menus () const
							{ return (_menus); }
		void setMenus (const Menus& ms);

		std::string informationText () const
							{ return ((_informationText != NULL) ? 
								_informationText -> text () : std::string (__NULL_STRING__)); }
		void showInformationText (const std::string& eTxt);

		Handler* handler ()
							{ return (_handler); }
		const Handler* handler () const
							{ return (_handler); }
		void setHandler (Handler* hd);

		GUISystem* currentGUISystem ()
							{ return (_currentGUISystem); }
		const GUISystem* currentGUISystem () const
							{ return (_currentGUISystem); }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* scr);
		virtual void onExit ();

		/** @see parent. */
		virtual void processEvent (const Event& evnt);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters () 
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		protected:
		/** When a new GUI system is loaded, then sometimes it has to be initialized.
			By default it does nothing. */
		virtual void initializeGUISystem ()
							{ }
		virtual void finalizeGUISystem ()
							{ }
		virtual ScoreObjectText* createEmptyInformationText ()
							{ return (game () -> textBuilder () -> 
								createText (__QGAMES_RAVIE12BLUELETTERS__, std::string (__NULL_STRING__))); }

		private:
		void deleteCurrentGUISystem ();

		protected:
		/** A reference to the handler managing what to do. */
		Menus _menus;
		Handler* _handler;
		/** When the state finishes, this variable keeps the las options selected. 
			-1 means no option to leave the state has been selected yet. */
		int _lastOptionSelected;
		/** Somethimes something managing the menus could happen, and a message needs to be shown. 
			This is the right element to be used. */
		ScoreObjectText* _informationText;


		// Implementation
		Menu _currentMenu;
		GUISystem* _currentGUISystem;
		Menu::Connection _connectionSelected;

		/** Counters to blink (if any) information text. */
		static const int _COUNTERTOBLINKTEXT = 0;
		static const int _COUNTERBLINKINGTEXT = 1;
		static const int _FADEBLINKING [20];
		/** Switch to indicate the game state has finished... */
		static const int _SWITCHFINISH = 0;
		static const int _SWITCHREBUILTMENU = 1;
	};

	/** The standard initial menu. 
		Could be fit in most of games. */
	class StandardInitialSelectionOptionsGameState : public SelectionOptionsGameState
	{
		protected:
		class SelectionHandler;

		public:
		static std::string _CONFFILENAME;

		StandardInitialSelectionOptionsGameState (
			int id = __QGAMES_GAMESTATESELECTOPTIONS__,
			GameState* ns = NULL,
			const Menus& m = createStandardMenus (), 
			SelectionOptionsGameState::Handler* h = new SelectionHandler);

		/** @see parent. */
		virtual void onEnter ();
		virtual void onExit ();

		protected:
		/** @see parent. */
		virtual void initializeGUISystem ();
		virtual void finalizeGUISystem ();

		protected:
		static SelectionOptionsGameState::Menus createStandardMenus ();

		protected:
		class SelectionHandler : public SelectionOptionsGameState::Handler
		{
			public:
			SelectionHandler ()
				: SelectionOptionsGameState::Handler ()
							{ }

			/** @see parent. */
			virtual int whenWidgetSelected (Widget* wdt);
		};

		// Implementation
		AdvancedArcadeGame::Conf* _configuration;
		std::map <int, QGAMES::KeyCode> _keycodesConverter;
		std::map <QGAMES::KeyCode, int> _keyCodesConverterInv;
	};

	/** To manage two states simultaneously.
		Each state will be drawn using a mask. 
		Defining masks complementary one each other, very beautiful effects can be achieved. */
	class ShowingTwoStatesGameState : public GameState
	{
		public:
		/** Describes a separation. 
			A separation is defined by the two GameStates being separated, and
			the mask used to show each. Really just only one is needed because the other one
			will be the opposite of the one received as parameter. 
			The separation can change along the time, so it has methods invoked from the GameState's life cycle. 
			Take into account that the separation can either initialized the game states or not.
			This could be useful in transitions between two states. */
		class Separation : public Notifier
		{
			public:
			struct Properties
			{
				Properties ()
					: _ticks (0),
					  _increment (0)
							{ }
				Properties (int t, int i)
					: _ticks (t),
					  _increment (i)
							{ }

				int _ticks;
				int _increment;
			};

			Separation (Mask* msk, GameState* stA, GameState* stB, const Properties& prps = Properties ());
			virtual ~Separation ();

			/** To control whether the game states are or not initialized. */
			bool initializeStateA () const
							{ return (_initializeStateA); }
			bool initializeStateB () const
							{ return (_initializeStateB); }
			bool finalizeSateA () const
							{ return (_finalizeStateA); }
			bool finalizeStateB () const
							{ return (_finalizeStateB); }
			void setInitializeStates (bool iSA, bool iSB)
							{ _initializeStateA = iSA; _initializeStateB = iSB; }
			void setFinalizeStates (bool fSA, bool fSB)
							{ _finalizeStateA = fSA; _finalizeStateB = fSB; }

			/** To stop or to move the state of the separation. */
			void stop ()
							{ _moving = false; }
			void start ()
							{ _moving = true; }
			bool moving () const
							{ return (_moving); }
			int percentage () const
							{ return (_percentage); }
			void setPercentage (int p);

			Mask* mask ()
							{ return (_mask); }
			const Mask* mask () const
							{ return (_mask); }
			GameState* gameStateA ()
							{ return (_stateA); }
			const GameState* gameStateA () const
							{ return (_stateA); }
			GameState* gameStateB ()
							{ return (_stateB); }
			const GameState* gameStateB () const
							{ return (_stateB); }
			const Properties& properties () const
							{ return (_properties); }
			void setProperties (const Properties& prps) 
							{ _properties = prps; }

			/** Must be overloaded later. */
			virtual void initialize ();
			virtual void update ();
			virtual void finalize ();
			virtual void drawOn (Screen* scr);

			protected:
			/** To actualize the mask to an specific percentage.
				The way the percentage is actualized will depend on the type of mask, so in the type of separation used. 
				It has to be defined in every mask. */
			virtual void actualizeMaskWith (int p) = 0;

			protected:
			/** The mask used to show the stateA.
				The opposite mask will be used to show the gameB. */
			Mask* _mask;
			/** None can't be null. */
			GameState* _stateA;
			GameState* _stateB;
			/** The properties. */
			Properties _properties;
			/** Does the separation move?. */
			bool _moving;
			/** Percentage the separation is.
				It goes from 0 to 100. When any of both is reached, and event is thrown.
				This event can be caught by the state above to execute an action. */
			int _percentage;

			/** Whether to initialize and finalize the different game states. */
			bool _initializeStateA, _initializeStateB;
			bool _finalizeStateA, _finalizeStateB;

			// Implementation
			/** It has to be destroyed, because it's been created in this class. */
			Mask* _oppositeMask;
			/** To count when to move the separation. */
			int _counter;
		};

		// A set of standard separations
		/** A separation made of a circle growing up from the center. */
		class CircleGrowingFromCenterSeparation : public Separation
		{
			public:
			CircleGrowingFromCenterSeparation (GameState* stA, GameState* stB, 
						const Properties& prps = Properties ())
				: Separation (new OUTMiddleCircleMask (0, Screen::Buffer::_MODBLENDMODE, 
					OUTMiddleCircleMask::Properties (0, __QGAMES_WHITECOLOR__)), stA, stB, prps)
							{ }

			protected:
			/** @see parent. */
			virtual void actualizeMaskWith (int p);
		};

		/** A separation made of a circle growing down to the center. */
		class CircleReducingToCenterSeparation : public Separation
		{
			public:
			CircleReducingToCenterSeparation (GameState* stA, GameState* stB, 
						const Properties& prps = Properties ())
				: Separation (new INMiddleCircleMask (0, Screen::Buffer::_MODBLENDMODE, 
					INMiddleCircleMask::Properties (0, __QGAMES_WHITECOLOR__)), stA, stB, prps)
							{ }

			protected:
			/** @see parent. */
			virtual void actualizeMaskWith (int p);
		};

		/** A separation made of vertical line moving from the left to the right. */
		class VerticalLineMovingFromLeftSeparation : public Separation
		{
			public:
			VerticalLineMovingFromLeftSeparation (GameState* stA, GameState* stB, 
						const Properties& prps = Properties ())
				: Separation (new RightSideMask (0, Screen::Buffer::_MODBLENDMODE, 
					RightSideMask::Properties (0, __QGAMES_WHITECOLOR__)), stA, stB, prps)
							{ }

			protected:
			/** @see parent. */
			virtual void actualizeMaskWith (int p);
		};

		/** A separation made of vertical line moving from the right to the left. */
		class VerticalLineMovingFromRightSeparation : public Separation
		{
			public:
			VerticalLineMovingFromRightSeparation (GameState* stA, GameState* stB, 
						const Properties& prps = Properties ())
				: Separation (new LeftSideMask (0, Screen::Buffer::_MODBLENDMODE, 
					LeftSideMask::Properties (0, __QGAMES_WHITECOLOR__)), stA, stB, prps)
							{ }

			protected:
			/** @see parent. */
			virtual void actualizeMaskWith (int p);
		};

		/** A separation made of horizontal line moving from the up to the down. */
		class HorizontalLineMovingFromTopSeparation : public Separation
		{
			public:
			HorizontalLineMovingFromTopSeparation (GameState* stA, GameState* stB, 
						const Properties& prps = Properties ())
				: Separation (new DownSideMask (0, Screen::Buffer::_MODBLENDMODE, 
					DownSideMask::Properties (0, __QGAMES_WHITECOLOR__)), stA, stB, prps)
							{ }

			protected:
			/** @see parent. */
			virtual void actualizeMaskWith (int p);
		};

		/** A separation made of horizontal line moving from the down to the top. */
		class HorizontalLineMovingFromBottomSeparation : public Separation
		{
			public:
			HorizontalLineMovingFromBottomSeparation (GameState* stA, GameState* stB, 
						const Properties& prps = Properties ())
				: Separation (new UpSideMask (0, Screen::Buffer::_MODBLENDMODE, 
					UpSideMask::Properties (0, __QGAMES_WHITECOLOR__)), stA, stB, prps)
							{ }

			protected:
			/** @see parent. */
			virtual void actualizeMaskWith (int p);
		};

		/** The state is owner of the masks. 
			It is not the owner of the states under management. The game is. 
			But it is the owner of the separation, and it can't be null. */
		ShowingTwoStatesGameState (int id, Separation* sp = NULL, GameState* nS = NULL)
			: GameState (id, Game::game (), nS),
			  _separation (sp)
							{ }
		~ShowingTwoStatesGameState ()
							{ delete (_separation); }

		/** To change the separation. */
		Separation* separation () 
							{ return (_separation); }
		const Separation* separation () const
							{ return (_separation); }
		void setSeparation (Separation* s);

		/** And to control it. */
		void setMoveSeparation ()
							{ if (_separation) _separation -> start (); }
		void setStopSeparation ()
							{ if (_separation) _separation -> stop (); }
		bool movingSeparation ()
							{ return (_separation ? _separation -> moving () : false); }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* scr);
		virtual void onExit ();

		protected:
		Separation* _separation;
	};

	/** An state to iterate among different game states. */
	class RollingStatesGameState : public ShowingTwoStatesGameState
	{
		public:
		struct SeparationState
		{
			SeparationState ()
				: _typeRolling (0), 
				  _stateName (__NULL_STRING__),
				  _secondsOnScreen (__BD 0),
				  _ticks (0),
				  _increment (0)
							{ }

			SeparationState (int t, const std::string& sN, bdata sS, int tc, int i)
				: _typeRolling (t), 
				  _stateName (sN), 
				  _secondsOnScreen (sS),
				  _ticks (tc),
				  _increment (i)
							{ assert (_secondsOnScreen >= 0 && _ticks >= 0 && _increment >= 0); }

			/** 
			 	A constructor based on parameters.
		 	 	It is used in the game state constructor.
			 	The structure of the game state control definition file :
			 	...
			 	<Separations>
			 		<Separation>
			 			<Attribute id="TYPEROLLING" value="0|1|..."/>
			 			<Attribute id="STATENAME" value="XXXX"/>
			 			<Attribute id="SECONDSONSCREEN" value="{0..}"/>
			 			<Attribute id="TICKS" value="{1..}"/>
			 			<Attribute id="INCREMENT" value="{1..}"/>
			 		</Separaion>
			 		...
			 	</Separations>
			 	...
			 */
			SeparationState (const std::map <std::string, std::string>& prps);

			int _typeRolling;
			std::string _stateName;
			bdata _secondsOnScreen;
			int _ticks;
			int _increment;
		};

		// To simplify the definition of a list of states...
		typedef std::vector <SeparationState> SeparationStates;

		// The types of separation
		// Any other can be added later...
		static const int _CIRCLEGROWINGFROMCENTER = 0;
		static const int _CIRCLEDECRESINGTOCENTER = 1;
		static const int _VERTICALLINEFROMLEFTTORIGHT = 2;
		static const int _VERTICALLINEFROMRIGHTTOLEFT = 3;
		static const int _HORIZONTALLINEFROMTOPTODOWN = 4;
		static const int _HORIZONTALLINEFROMDOWNTOTOP = 5;

		RollingStatesGameState (int id, const SeparationStates& sS, GameState* nS = NULL);

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();

		/** @see parent. */
		virtual void processEvent (const Event& evnt);

		protected:
		void activeSeparation (int nS);
		/** Factory method. To create the type of separation. */
		virtual Separation* createSeparation (int t, GameState* sA, GameState* sB, const Separation::Properties& prps);

		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		protected:
		SeparationStates _states;

		// The counter used to determinate whether it is necessary to change the separation state.
		static const int _SEPARATIONONSCREENCOUNTER = 0;
		// The switch used to indicate that rolling stops...
		static const int _SWITCHROLLING = 0;

		// Implementation
		/** The current state treated. */
		int _currentState;
		/** To control when to change the state being used. */
		bool _toChangeState;
	};

	/** A state to control the rest where the paused mode is considered. */
	class AdvancedGameStateControl : public GameStateControl
	{
		public:
		AdvancedGameStateControl (int id, GameState* psState, const GameStates& sts, const FlowMap& fM)
			: GameStateControl (id, Game::game (), sts, fM),
			  _pauseState (psState),
			  _overlayState (NULL),
			  _stopped (false)
							{ assert (_pauseState); }

		/** To pause and continue the game. */
		void stop ();
		void resume ();
		bool isStopped () const
							{ return (_stopped); }

		/** Set an overlay. It could be used e.g. to show the frames per second. 
			If NULL, then if had an overlay, it would be removed. */
		void setOverlay (GameState* oState);

		/** @see parent */
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* scr);

		protected:
		GameState* _pauseState;
		GameState* _overlayState;
		bool _stopped;
	};
}

#endif
  
// End of the file
/*@}*/