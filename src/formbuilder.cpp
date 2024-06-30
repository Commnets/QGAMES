#include <Common/formbuilder.hpp>
#include <Common/definitions.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>

// ---
QGAMES::FormBuilder::FormBuilder (const std::string& fDef)
	: _definitions (),
	  _forms (),
	  _definitionFile (fDef),
	  _basePath (QGAMES::onlyPath (fDef))
{
	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");
}

// ---
QGAMES::FormBuilder::~FormBuilder ()
{
	for (QGAMES::Forms::const_iterator i = _forms.begin (); i != _forms.end (); i++)
		delete ((*i).second);
	_forms.clear ();

	for (QGAMES::FormBuilder::Definitions::const_iterator j = _definitions.begin ();
			j != _definitions.end (); j++)
		delete ((*j).second);
	_definitions.clear ();
}

// ---
std::vector <int> QGAMES::FormBuilder::resourcesId () const
{
	std::vector <int> result;
	for (QGAMES::FormBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		result.push_back ((*i).first);
	return (result);
}

// ---
void QGAMES::FormBuilder::preLoad ()
{
	for (QGAMES::FormBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		if (_forms.find ((*i).second -> _id) == _forms.end ())
			form ((*i).second -> _id) -> preLoad ();
}

// ---
QGAMES::Form* QGAMES::FormBuilder::form (int fId)
{
	QGAMES::Forms::const_iterator i = _forms.find (fId);
	if (i != _forms.end ())
		return ((*i).second);
	
	__WLOGPLUS (std::string ("Loading form: ") + std::to_string (fId));

	QGAMES::FormBuilder::Definitions::const_iterator j = _definitions.find (fId);
	assert (j != _definitions.end ()); // It doesn't exist...
	
	QGAMES::Form* form = createForm ((*j).second);
	assert (form); // createForm is an user's method...just in case!
	_forms.insert (QGAMES::Forms::value_type (fId, form));
	return (form);
}

// ---
bool QGAMES::FormBuilder::existsFormForFileDefinition (const std::string& f) const
{
	bool result = false;
	for (QGAMES::FormBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end () && !result; i++)
		result = ((*i).second -> _fileDefinition == f);
	return (result);
}

// ---
int QGAMES::FormBuilder::formIdForFileDefinition (const std::string& f) const
{
	int result = __MININT__;
	for (QGAMES::FormBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end () && result == __MININT__; i++)
		if ((*i).second -> _fileDefinition == f)
			result = (*i).second -> _id;

	assert (result != __MININT__);

	return (result);
}

// ---
bool QGAMES::FormBuilder::existsFormForName (const std::string& n) const
{
	bool result = false;
	for (QGAMES::FormBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end () && !result; i++)
		result = ((*i).second -> _name == n);
	return (result);
}

// ---
int QGAMES::FormBuilder::formIdForName (const std::string& n) const
{
	int result = __MININT__;
	for (QGAMES::FormBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end () && result == __MININT__; i++)
		if ((*i).second -> _name == n)
			result = (*i).second -> _id;

	assert (result != __MININT__);

	return (result);
}
