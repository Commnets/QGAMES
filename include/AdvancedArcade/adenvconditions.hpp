/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: adenvcondition.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 25/03/2018 \n
 *	Description: Defining complex environmental conditions. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADENVCONDITION__
#define __QGAMES_ADENVCONDITION__

#include <AdvancedArcade/addefinitions.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
	/** This class represents just a thunder. */
	class Thunder : public EnvironmentalCondition
	{
		public:
		/** 
		 *	Constructor:
		 *	@param	s	A reference to the scene where the rain is happening.
		 *	@param	sCh	The sound channel (for the noise).
		 *	@param	nS	Sound used for the thunder. NULL if no sound is needed.
		 */
		Thunder (Scene* s, 
			int sCh = __QGAMES_WHEATHERSOUNDCHANNEL__, Sound* nS = Game::game () -> sound (__QGAMES_THUNDERWAVSOUND__));

		/** From a set of parameters. */
		Thunder (Scene* s, const std::map <std::string, std::string>& prps);

		/** @see parent. 
			The method sends __QGAMES_THUNDERSTARTS__ when the thunder starts,
			and __QGAMES_THUNDERFINISHES__ when the thunder finishes (notification). */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		/** The number of the sound channel. */
		int _soundChannel;
		/** The sound. */
		Sound* _sound;

		// Counters...
		/** Counter to calculate the time thunder is on. */
		static const int _COUNTERTHUNDERON = 0;
		// Switches...
		/** Switch to know when the condition starts. */
		static const int _SWITCHTHUNDERSTARTS = 0;
		/** Switch to know when the thunder finishes. */
		static const int _SWITCHTHUNDERFINISHES = 1;

	};

	/** This class represents a set of thunders. */
	class Thunders  : public ComplexEnvironmentalCondition
	{
		public:
		/** Structure to hold the properties of the thunders. */
		struct Properties
		{
			Properties ()
				: _probability (50),
				  _frecuency ((bdata) 1),
				  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
				  _soundId (__QGAMES_THUNDERWAVSOUND__)
							{ }
			/** 
			 *	Constructor:
			 *	@param	p	The probability for the next thunder to appear (from 0 to 100).
			 *	@param	f	The frecuency of the thunders (positive).
			 *	@param	sCh	The sound channel (for the noise).
			 *	@param	sId	The sound id used for the thunder, if any. -1 if no sound is needed.
			 */
			Properties (int p, bdata f,	int sCh, int sId);

			/**
			  *	To build the thunders from a list of properties: \n
			  *	__QGAMES_THUNDERSWHEATHERATTRPROB__:		"ThundersProbability" \n
			  *	__QGAMES_THUNDERSWHEATHERATTRFREQ__:		"ThundersFrecuency" \n
			  *	__QGAMES_THUNDERSWHEATHERATTRSOUNDCHN__:	"ThundersSoundChannel" \n
			  *	__QGAMES_THUNDERSWHEATHERATTRSOUND__:		"ThundersSoundId"
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** The probability of the next thunder. */
			int _probability;
			/** The frecuency of the thunders. */
			bdata _frecuency;
			/** The channel for the sound of the thunder. */
			int _soundChannel;
			/** The id of the sound. */
			int _soundId;
		};

		/** 
		 *	Constructor:
		 *	@param	s		A reference to the scene where the storm is happening.
		 *	@param	prps	The properties to build the thunder.
		 */
		Thunders (Scene* s, const Properties& prps = Properties ());

		/** To get & set the properties.
			It is only possible to change the frecuency.  */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps);
	
		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		/** @see parent. */
		virtual void processEvent (const Event& e);

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		/** The properties. */
		Properties _properties;

		// Implementation
		/** A reference to the individual thunder. */
		Thunder* _thunder;

		// Counters...
		/** Counter to count how much time to the next thunder... */
		static const int _COUNTERTONEXTTHUNDER = 0;
		// Switches...
		/** Switch to control when to start the next lightning. */
		static const int _SWITCHCOUNTTONEXTTHUNDER = 0;
	};

	/** This class represents rain falling. \n
		It is possibile to define the density of the raindrops and
		the speed of the raindrops that fall down. \n
		The rain is a "permanent" condition. Once it starts can't stop unless the game orders it. \n
		But it can be moderated or changed on the fly. */
	class Rain : public EnvironmentalCondition
	{
		public:
		/** Struct to hold the properties of the rain. */
		struct Properties
		{
			Properties ()
				: _density ((bdata) 1),
				  _speed (2),
				  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
				  _soundId (__QGAMES_RAINWAVSOUND__),
				  _backgroundFormId (-1), // Means none
				  _dropFormId (__QGAMES_DROPINRAINFORM__)
							{ }

			/** 
			 *	Constructor:
			 *	@param	s	The scene reference where the rain is happening.
			 *	@param	d	The density of the rain. It can not be 0, but close to. 
			 *	@param	sp	The speed the rain moves.
			 *	@param	sCh	The sound channel (for the noise).
			 *	@param	sId	Number of the sound used for rain. -1 if no sound is needed.
			 *	@param	bF	Backgroung form id, normally used to dark a little bit the environment whilst rains. -1 if none.
			 *	@param	dF	Drop form id. It has to exist.
			 */
			Properties (bdata d, int sp, int sCh, int sId, int bF, int dF);

			/**
			  *	To build the rain from a list of properties: \n
			  *	__QGAMES_RAINWHEATHERATTRDENSITY__:		"RainDensity" \n
			  *	__QGAMES_RAINWHEATHERATTRSPEED__:		"RainSpeed" \n
			  *	__QGAMES_RAINWHEATHERATTRSOUNDCHN__:	"RainSoundChannel" \n
			  *	__QGAMES_RAINWHEATHERATTRSOUND__:		"RainSoundId" \n
			  *	__QGAMES_RAINWHEATHERATTRBK__:			"RainBackgroundId" \n
			  *	__QGAMES_RAINWHEATHERATTRDROP__:		"RainDropId"
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** The density of the drops. */
			bdata _density;
			/** Speed of the movement. */
			int _speed;
			/** The number of the sound channel. */
			int _soundChannel;
			/** The sound id. */
			int _soundId;
			/** The background id if any. -1 if there is none. */
			int _backgroundFormId; 
			/** Same but for drops. */
			int _dropFormId; 
		};

		/** 
		 *	Constructor:
		 *	@param	s		A reference to the scene where the rain is happening.
		 *	@param	prps	The properties to build the rain. 
		 */
		Rain (Scene* s, const Properties& prps = Properties ());

		/** To get & set the properties.
			The effects after it has started are unknown. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps);

		/** To get the current density. */
		bdata currentDensity () const
							{ return (_properties._density * _densityFactorManager.currentFactor ()); }

		// To change or adapt the environmental conditions
		/** Change the density factor manager. */
		void setDensityFactorManager (const bdata iF, bdata fF, bdata i = (bdata) 0.1, bool up = true);
		/** Reverse the density factor manager. */
		void reverseDensityFactorManager ()
								{ _densityFactorManager.reverse (); }
		/** Adapt the density factor manager. */
		void adaptDensityFactorManager (const bdata iF, bdata fF, bdata i = (bdata) 0.1, bool up = true)
								{ _densityFactorManager.adaptTo (iF, fF, i, up); }
		/** Same with the speed of the drops. */
		void setSpeedFactorManager (const bdata iF, bdata fF, bdata i = (bdata) 0.1, bool u = true);
		/** Reverse the speed factor manager. */
		void reverseSpeedFactorManager ()
								{ _speedFactorManager.reverse (); }
		/** Adapt the speed factor manager. */
		void adaptSpeedFactorManager (const bdata iF, bdata fF, bdata i = (bdata) 0.1, bool up = true)
								{ _speedFactorManager.adaptTo (iF, fF, i, up); }

		/** To kow the number of drops. */
		int numberDrops () const
								{ return ((int) _drops.size ()); }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		/** The properties of the rain. */
		Properties _properties;

		// Implementation
		/** To regulate a factor of the density to increment progresively the drops falling. */
		DensityFactorManager _densityFactorManager;
		/** To regulate the speed. */
		DensityFactorManager _speedFactorManager;
		/** The drops falling down. */
		Positions _drops;
		/* The width of the screen where drops have to be drawn. */
		int _width; 
		/** Same with height. */
		int _height; 

		// Counters...
		/** Counter to count phases to show full drops... */
		static const int _COUNTERTOMOVEFADE = 0;
		/** Counter to show the drops. */
		static const int _COUNTERTOSHOW = 1;
		/** Counter to increment the density and speed factor. */
		static const int _COUNTERTOINCREMENTDENSITYANDSPEED = 2;
		/** The intensity of the drops. */
		static const int _fades[6];
		// Switches...
		/** Switch to know when the condition starts. */
		static const int _SWITCHRAINSTARTS = 0;
	};

	/** This class represents an earthquake. 
		It is possibile to define the intensity of the earthquake. */
	class Earthquake : public EnvironmentalCondition
	{
		public:
		struct Properties
		{
			Properties ()
				: _intensity (1),
				  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
				  _soundId (__QGAMES_EARTHQUAKEWAVSOUND__)
							{ }

			/** 
			 *	Constructor:
			 *	@param	i	The intensity of the movement. From 0 to any number (number of pixels)
			 *	@param	sCh	The sound channel (for the noise).
			 *	@param	sId	Number of the sound. -1 means no sound.
			 */
			Properties (int i, int sCh, int sId);

			/**
			  * To create the Earthquake from a list of properties:
			  *	__QGAMES_EARTHQUAKEWHEATHERATTRINTEN__:	"EarthquakeIntensity" \n
			  *	__QGAMES_EARTHQUAKEWHEATHERATTRSDCHN__:	"EarthquakeSoundChannel" \n
			  *	__QGAMES_EARTHQUAKEWHEATHERATTRSOUND__:	"EarthquakeSoundId"
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** The intensity. */
			int _intensity;
			/** The number of the sound channel. */
			int _soundChannel;
			/** The sound id. */
			int _soundId;
		};

		/** 
		 *	Constructor:
		 *	@param	s		The scene reference where the earthquake is happening.
		 *	@param	prps	The properties of the earthquake.
		 */
		Earthquake (Scene* s, const Properties& prps = Properties ())
			: EnvironmentalCondition (__QGAMES_EARTHQUAKEWHEATHER__, s),
			  _properties (prps),
			  _lastVector (QGAMES::Vector::_cero) // Stablished at runtime...
							{ }

		/** To get & set the properties. 
			It is not possible to change the sound one it has started. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		private:
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		/** The properties. */
		Properties _properties;

		// Implementation
		Vector _lastVector;

		// Switches...
		/** Switch to know when the condition starts. */
		static const int _SWITCHEARTHQUAKESTARTS = 0;
	};

	/** This class represents wind. */
	class Wind : public EnvironmentalCondition
	{
		public:
		/** Struct to hold the properties of the wind. */
		struct Properties
		{
			Properties ()
				: _density ((bdata) 1),
				  _speed (2),
				  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
				  _soundId (__QGAMES_WINDWAVSOUND__),
				  _traceFormId (__QGAMES_WINDTRACEFORM__)
							{ }

			/** 
			 *	Constructor:
			 *	@param	d	The density of the wind. It can not be 0, but close to.
			 *	@param	sp	The speed the wind moves.
			 *	@param	sCh	The sound channel (for the noise).
			 *	@param	nS	Number of the sound used for wind. -1 means no sound.
			 *	@param	tF	Wind trace form id. It can't be null.
			 */
			Properties (bdata d, int sp, int sCh, int sId, int tId);

			/**
			  *	To create the wind from a list of properties: \n
			  *	__QGAMES_WINDWHEATHERATTRDENSITY__:		"WindDensity" \n
			  *	__QGAMES_WINDWHEATHERATTRSPEED__:		"WindSpeed" \n
			  *	__QGAMES_WINDWHEATHERATTRSOUNDCHN__:	"WindSoundChannel" \n
			  *	__QGAMES_WINDWHEATHERATTRSOUND__:		"WindSoundId" \n
			  *	__QGAMES_WINDWHEATHERATTRTRACE__:		"WindTraceId"
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** The density of the wind. */
			bdata _density;
			/** Speed of the movement. */
			int _speed;
			/** The number of the sound channel. */
			int _soundChannel;
			/** The sound id. */
			int _soundId;
			/** The id of the form representing the wind trace. */
			int _traceFormId; 
		};

		/** 
		 *	Constructor:
		 *	@param	s		A reference to the scene where where wind is happening.
		 *	@param	prps	The properties of the wind.
		 */
		Wind (Scene* s, const Properties& prps = Properties ());

		/** To get & set the properties. 
			The effects after it has started are unknown. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps);

		/** To know the current force. */
		bdata currentForce () const
							{ return (_properties._density * _densityFactorManager.currentFactor ()); }

		/** Change the density factor manager. */
		void setDensityFactorManager (const bdata iF, bdata fF, bdata i = (bdata) 0.1, bool up = true);
		/** Reverse the density factor manager. */
		void reverseDensityFactorManager ()
								{ _densityFactorManager.reverse (); }
		/** Adapt the density factor manager. */
		void adaptDensityFactorManager (const bdata iF, bdata fF, bdata i = (bdata) 0.1, bool up = true)
								{ _densityFactorManager.adaptTo (iF, fF, i, up); }
		/** Same with the speed of the wind. */
		void setSpeedFactorManager (const bdata iF, bdata fF, bdata i = (bdata) 0.1, bool u = true);
		/** Reverse the speed factor manager. */
		void reverseSpeedFactorManager ()
								{ _speedFactorManager.reverse (); }
		/** Adapt the speed factor manager. */
		void adaptSpeedFactorManager (const bdata iF, bdata fF, 
			bdata i = (bdata) 0.1, bool up = true)
								{ _speedFactorManager.adaptTo (iF, fF, i, up); }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		/** A refernce to the properties. */
		Properties _properties;

		// Implementation
		/** To regulate a factor of the density (from 0 to 1)
			to increment progresively the traces of wind moving. */
		DensityFactorManager _densityFactorManager;
		/** To regulate the speed. */
		DensityFactorManager _speedFactorManager;
		/** The traces of the wind. */
		Positions _traces;
		/** The width of the display where the effect is shown. */
		int _width;
		/** The height. */
		int _height;

		// Counters...
		/** Counter to count phases to show full wind... */
		static const int _COUNTERTOMOVEFADE = 0;
		/** Counter to show the wind moving.*/
		static const int _COUNTERTOSHOW = 1;
		/** Counter to increment the density and speed factor. */
		static const int _COUNTERTOINCREMENTDENSITYANDSPEED = 2;
		/** The intensity of the wind. */
		static const int _fades[6];
		// Switches...
		/** Switch to know when the condition starts. */
		static const int _SWITCHWINDSTARTS = 0;
	};

	/** This class represents a lightning. */
	class Lightning : public EnvironmentalCondition
	{
		public:
		/** 
		 *	Constructor:
		 *	@param	s	A reference to the scene where the lightning is happening.
		 *	@param	sCh	The sound channel (for the noise).
		 *	@param	nS	Number of the sound used for lightning.
		 */
		Lightning (Scene* s,
			int sCh = __QGAMES_WHEATHERSOUNDCHANNEL__, Sound* nS = Game::game () -> sound (__QGAMES_THUNDERWAVSOUND__));

		/** From a set of properties. */
		Lightning (Scene* s, const std::map <std::string, std::string>& prps);

		/** To verify whether the lightning is collisiong or not again an artist. */
		bool hasCollisionWith (Artist* a);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		typedef std::vector <Position> ThunderPoints;
		/** Recursive. To calculate the internal structure of the lightning. */
		void builtBolt (const Position& pS, const Position& pE,
			ThunderPoints& pts, bdata vr);

		private:
		/** The points to draw a lightning. Calculated inside builtBolt method. */
		ThunderPoints _thPoints;
		/** The number of the sound channel. */
		int _soundChannel;
		/** The sound. */
		Sound* _sound;

		// Counters...
		/** Counter to count when the lightning has to be shown. */
		static const int _COUNTERLIGHTNINGON = 0;
		/** Counter to count when the lightning has to start to fade off. */
		static const int _COUNTERTOFADEOFF = 1;
		/** Counting down fading off. */
		static const int _COUNTERFADEOFF = 2;
		/** The intensities of the lightning when dissapear. */
		static const int _fades[6];

		// Switches...
		/** Switch to know when the condition starts. */
		static const int _SWITCHLIGHTNINGSTARTS = 0;
		/** Switch to know when the lightning finishes. */
		static const int _SWITCHLIGHTNINGFINISHES = 1;
	};

	/** This class represents a set of lightings. */
	class Lightnings  : public ComplexEnvironmentalCondition
	{
		public:
		/** Structure to hold the properties of the lightnings. */
		struct Properties
		{
			Properties ()
				: _probability (50),
				  _frecuency ((bdata) 1),
				  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
				  _soundId (__QGAMES_THUNDERWAVSOUND__)
							{ }
			/** 
			 *	Constructor:
			 *	@param	p	The probability of the next lightnings (from 0 to 100).
			 *	@param	f	The frecuency of the lightnings (positive).
			 *	@param	sCh	The sound channel (for the noise).
			 *	@param	sId	The sound id used for the lightning. -1 if none.
			 */
			Properties (int p, bdata f,	int sCh, int sId);

			/**
			  *	To build the lightnings from a list of properties: \n
			  *	__QGAMES_LIGHTNINGSWHEATHERATTRPROB__:		"LightningsProbability" \n
			  *	__QGAMES_LIGHTNINGSWHEATHERATTRFREQ__:		"LightningsFrecuency" \n
			  *	__QGAMES_LIGHTNINGSWHEATHERATTRSOUNDCHN__:	"LightningsSoundChannel" \n
			  *	__QGAMES_LIGHTNINGSWHEATHERATTRSOUND__:		"LightningsSoundId"
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** The probability of the next lightning. */
			int _probability;
			/** The frecuency of the lightning. */
			bdata _frecuency;
			/** The channel for the sound of the lightning. */
			int _soundChannel;
			/** The id of the sound. */
			int _soundId;
		};

		/** 
		 *	Constructor:
		 *	@param	s		The scene reference where the storm is happening.
		 *	@param	prps	The properties of the lightnings.
		 */
		Lightnings (Scene* s, const Properties& prps);

		/** To get & set the properties.
			It is only possible to change the frecuency. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		/** @see parent. */
		virtual void processEvent (const Event& e);

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		/** A reference to the properties. */
		Properties _properties;

		// Implamentation
		/** A reference to the individual lightning. */
		Lightning* _lightning;

		// Counters...
		/** Counter to count how much time to the next lightning... */
		static const int _COUNTERTONEXTLIGHTNING = 0;

		// Switches...
		/** Switch to control when to start the next lightning. */
		static const int _SWITCHCOUNTTONEXTLIGHTNING = 0;
	};

	/** This class represents just the rainbow
		Sometimes, after raining, the rainbow appears!. */
	class Rainbow : public EnvironmentalCondition
	{
		public:
		/** 
		 *	Constructor:
		 *	@param	s	A reference to the scene where the rain is happening.
		 *  @param  sc	Reconds that the rainbow will be on the screen.
		 *	@param	rF	Rainbow trace form. It can't be null.
		 */
		Rainbow (Scene* s, bdata sc = (bdata) 3.0,
			Form* rF = Game::game () -> form (__QGAMES_RAINBOWFORM__));

		/** From a set of properties. */
		Rainbow (Scene* s, const std::map <std::string, std::string>& prps);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		/** Number of seconds that the rainbow will be on the screen. */
		bdata _secondsOnScreen;
		/** The rainbow. */
		Form* _rainbow;

		/** Counter to count when the rainbow has to be shown. */
		static const int _COUNTERRAINBOWON = 0;
		/** Counter to count when the rainbow has to start to fade off. */
		static const int _COUNTERTOFADEOFF = 1;
		/** Counting down fading off. */
		static const int _COUNTERFADEOFF = 2;
		/** The intensities of the rainbow when dissapear. */
		static const int _fades [6];
		// Switches...
		/** Switch to know when the condition starts. */
		static const int _SWITCHRAINBOWSTARTS = 0;
		/** Switch to know when the lightning finishes. */
		static const int _SWITCHRAINBOWFINISHES = 1;
	};

	/** This class represents an storm. */
	class Storm : public ComplexEnvironmentalCondition
	{
		public:
		/** Enum to represent the different phases of the storm.
			The algorithm inside will iterate over them randomly. */
		typedef enum
		{
			_NONE = 0,
			_SMOOTHRAIN,
			_RAIN,
			_THUNDERS,
			_RAINWITHTHUNDERS,
			_RAINWITHLIGHTNING,
			_RAINBOW
		} State;

		/** 
		 *	Constructor:
		 *	@param	s	The scene reference where the storm is happening.
		 *	@param	t	A reference to the thunder. Can't be null.
		 *	@param	l	A reference to the lightning. Can't be null.
		 *	@param	r	A reference to the rain. Can't be null.
		 *	@param	rb	A reference to the rainbow. Can't be null.
		 */
		Storm (Scene* s, Thunder* t, Lightning* l, Rain* r, Rainbow* rb);
	
		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		/** @see parent. */
		virtual void processEvent (const Event& e);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		// Implementation
		/** Internal method to decide which is the next storm state. 
			It can be changed inheriting from this class. */
		virtual State nextState () const;

		private:
		/** The current state of the storm. */
		State _state;
		// Sub environmental conditions...
		/** A reference to the thunder. */
		Thunder* _thunder;
		/** A reference to the lightning. */
		Lightning* _lightning;
		/** A reference to the rain. */
		Rain*  _rain;
		/** A reference to the rainbow. */
		Rainbow* _rainbow;

		// Counters...
		/** Counter to determinate whether the state has or not to be changed. */
		static const int _COUNTERTOCHANGESTATE = 0;
		/** Counter to count how much time to the next lightning... */
		static const int _COUNTERTONEXTLIGHTNING = 1;
		/** Counter to count how much time to the next thunder... */
		static const int _COUNTERTONEXTTHUNDER = 2;
		// Switches...
		/** Switch to control when to start the next lightning. */
		static const int _SWITCHCOUNTTONEXTLIGHTNING = 0;
		/** Switch to control when to start the next thunder. */
		static const int _SWITCHCOUNTTONEXTTHUNDER = 1;
	};

	/** This class represents clouds. */
	class Clouds : public EnvironmentalCondition
	{
		public:
		/** To hold the properties of the clouds. */
		struct Properties
		{
			Properties ()
				: _density ((bdata) 1),
				  _speed (2),
				  _soundChannel (__QGAMES_WHEATHERSOUNDCHANNEL__),
				  _soundId (__QGAMES_WINDWAVSOUND__),
				  _cloudFormId1 (__QGAMES_CLOUDS1FORM__), 
				  _cloudFormId2 (__QGAMES_CLOUDS2FORM__),
				  _cloudFormId3 (__QGAMES_CLOUDS3FORM__)
							{ }

			/** 
			 *	Constructor:
			 *	@param	d	The density of the clouds. It can not be 0, but close to.
			 *	@param	sp	The speed the clouds moves
			 *	@param	sCh	The sound channel (for the noise). -1 if none.
			 *	@param	nS	Number of the sound used for cloud moving.
			 *	@param	fC1	Form 1 of the cloud.
			 *	@param	fC2	Form 2 of the cloud.
			 *	@param	fC3	Form 3 of the cloud.
			 */
			Properties (bdata d, int sp, int sCh, int sId, int cF1, int cF2, int cF3);

			/** 
			  * To create a set of clouds moving from a set of properties: \n
			  *	__QGAMES_CLOUDSWHEATHERATTRDENSITY__:	"CloudsDensity" \n
			  *	__QGAMES_CLOUDSWHEATHERATTRSPEED__:		"CloudsSpeed" \n
			  *	__QGAMES_CLOUDSWHEATHERATTRSOUNDCHN__:	"CloudsSoundChannel" \n
			  *	__QGAMES_CLOUDSWHEATHERATTRSOUND__:		"CloudsSoundId" \n
			  *	__QGAMES_CLOUDSWHEATHERATTRCLOUDS__:	"CloudsId" \n
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** The density of the clouds. */
			bdata _density;
			/** Speed of the movement. */
			int _speed;
			/** The number of the sound channel. */
			int _soundChannel;
			/** The sound id. */
			int _soundId;
			/** Reference to the id of the clouds */
			int _cloudFormId1, _cloudFormId2, _cloudFormId3; 
		};

		/** 
		 *	Constructor:
		 *	@param	s		The scene reference where wind is happening.
		 *	@param	prps	The properties of the clouds. 
		 */
		Clouds (Scene* s, const Properties& prps = Properties ());

		/** To get & set the properties. 
			The consequences after started are unknown. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** Change the density factor manager. */
		void setDensityFactorManager (const bdata iF, bdata fF, 
			bdata i = __BD 0.1, bool up = true);
		/** Reverse the density factor manager. */
		void reverseDensityFactorManager ()
								{ _densityFactorManager.reverse (); }
		/** Adapt the density factor manager. */
		void adaptDensityFactorManager (const bdata iF, bdata fF, 
			bdata i = __BD 0.1, bool up = true)
								{ _densityFactorManager.adaptTo (iF, fF, i, up); }
		/** Same with the speed of the clouds. */
		void setSpeedFactorManager (const bdata iF, bdata fF, 
			bdata i = __BD 0.1, bool u = true);
		/** Reverse the speed factor manager. */
		void reverseSpeedFactorManager ()
								{ _speedFactorManager.reverse (); }
		/** Adapt the speed factor manager. */
		void adaptSpeedFactorManager (const bdata iF, bdata fF, 
			bdata i = __BD 0.1, bool up = true)
								{ _speedFactorManager.adaptTo (iF, fF, i, up); }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		/** A reference to the properties. */
		Properties _properties;

		// Implementation
		/** To regulate a factor of the density (from 0 to 1)
			to increment progresively the traces of clouds moving. */
		DensityFactorManager _densityFactorManager;
		/** To regulate the speed. */
		DensityFactorManager _speedFactorManager;
		/** The position of the clouds. */
		Positions _clouds;
		/** The wdth of the screen where clouds are drawn. */
		int _width;
		/** Sam ewith the height. */
		int _height;
		/** The same with the form. */
		int _formWidth;
		/** Idem. */
		int _formHeight;
		/** A reference to the form of the clouds. */
		Form* _cloudForms [3];
		/** The form to draw choosen randomicaly! */
		int _formToDraw; 

		// Counters...
		/** Counter to count phases to show full clouds... */
		static const int _COUNTERTOMOVEFADE = 0;
		/** Counter to show the clouds moving.*/
		static const int _COUNTERTOSHOW = 1;
		/** Counter to increment the density and speed factor. */
		static const int _COUNTERTOINCREMENTDENSITYANDSPEED = 2;
		/** The intensity of the clouds. */
		static const int _fades[6];

		// Switches...
		/** Switch to know when the condition starts. */
		static const int _SWITCHCLOUDSSTARTS = 0;
	};
}

#endif
  
// End of the file
/*@}*/
