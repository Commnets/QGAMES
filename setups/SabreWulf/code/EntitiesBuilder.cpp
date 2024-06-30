#include "EntitiesBuilder.hpp"
#include "Sabreman.hpp"
#include "Orchild.hpp"
#include "Guardian.hpp"
#include "MtEntities.hpp"
#include "ObjEntities.hpp"
#include "Defs.hpp"

// ---
QGAMES::Entity* EntitiesBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{ 
	QGAMES::Entity* result = NULL;
	if (def._id == __SABREWULFSABREMAN__) 
		result = new Sabreman ();
	else if (def._id == __SABREWULFORCHILD__)
		result = new Orchild ();
	else if (def._id >= __SABREWULFOBJECTSARTIST__ && 
			 def._id < (__SABREWULFOBJECTSARTIST__ + __SABREWULFOBJECTSPERPLACE__)) 
		result = new SabreWulfObject (def._id);
	else if (def._id >= __SABREWULFNASTIESBASE__ &&
			 def._id < (__SABREWULFNASTIESBASE__ + __SABREWULFMAXNUMBERNASTIES__))
		result = new SabreWulfNastie (def._id);
	else if (def._id >= __SABREWULFINMORTALSBASE__ &&
			 def._id < (__SABREWULFINMORTALSBASE__ + __SABREWULFNUMBERINMORTALS__))
		result = new SabreWulfInmortal (def._id); 
	else if (def._id == __SABREWULFGUARDIAN__)
		result = new SabreWulfGuardian ();
	else if (def._id == __SABREWULFFIRE__)
		result = new SabreWulfNastie (def._id);
	else
		exit (1); // If the code is here...wrong!
	return (result);
}
