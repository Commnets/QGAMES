/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: characterstepsbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 30/03/2018 \n
 *	Description: Defines a class to build up character steps monitor. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_CHARACTERSTEPSBUILDER__
#define __QGAMES_CHARACTERSTEPSBUILDER__

#include <Common/charactersteps.hpp>
#include <map>
#include <string>

class TiXmlElement;

namespace QGAMES
{
	/** To create character steps monitors. */
	class CharacterControlStepsMonitorBuilder
	{
		public:
		/** 
		  *	The structure of a movements file definition is as follows:
		  *	Take care, it could contain include directive \n
		  *	<?xml version="1.0"?> \n
		  *	<Monitors> \n
		  *		<Include file="CCCC"/> \n
		  *		<Monitor id="X" cyclic="true/false"> \n
		  *			<Step type="Y"> \n
		  *				<Variable id="XX" value="XX"/> \n
		  *					... \n
		  *			</Step> \n
		  *			... \n
		  *		</Monitor> \n
		  *		... \n
		  *	</Monitors> 
		  */			
		CharacterControlStepsMonitorBuilder (const std::string& fDef);
		/** This builder doesn't own the monitors created. */

		/** To get a monitor for an entity. 
			Requieres the monitor id, and the character the monitor is creaed for. */
		CharacterControlStepsMonitor* monitorFor (int id, Character* chr);

		protected:
		struct StepDefinition;
		typedef std::vector <StepDefinition> StepDefinitions;

		struct StepDefinition
		{
			StepDefinition ()
				: _type (-1), // Meaning not defined...
				  _innerSteps (), // They coud exist or not...
				  _values () // There should not be values and inner steps at the same time...
							{ }
							
			int _type;
			std::map <std::string, std::string> _values;
			StepDefinitions _innerSteps;
		};
		
		struct MonitorDefinition
		{
			MonitorDefinition ()
				: _id (-1),
				  _cyclic (false),
				  _steps ()
							{ }

			int _id;
			bool _cyclic;
			StepDefinitions _steps;
		};

		/** To create the specific monitor. \n
			It can be overloaded, but a default implementation is given with the basic (and usually standard) monitor. */
		virtual CharacterControlStepsMonitor* createMonitor (const MonitorDefinition& def, 
			const CharacterControlSteps& stps, Character* chr);
		/** To create the specific and indivisual steps. \n
			It can be overloaded to add your own steps. \n
			The ones defined ar charactersteps.hpp are included in the this implmentation. */ 
		virtual CharacterControlStep* createStep (const StepDefinition& def);
		
		protected:
		/** To create a set of steps, usually from a inner definition. */
		CharacterControlSteps createSteps (const StepDefinitions& stps);

		private:
		void readFile (const std::string& fDef);
		MonitorDefinition readMonitorDefinition (TiXmlElement* m);
		StepDefinition readStepDefinition (TiXmlElement* m);
		StepDefinition readComplexStepDefinition (TiXmlElement* m);

		protected:
		typedef std::map <int, MonitorDefinition> Definitions;

		Definitions _definitions;

		std::string _definitionFile;
		std::string _basePath;
	};
}

#endif
  
// End of the file
/*@}*/
