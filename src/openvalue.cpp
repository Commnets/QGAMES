#include <Common/openvalue.hpp>
#include <ostream>
#include <istream>
#include <algorithm>
#include <stdarg.h>

// ---
QGAMES::OpenValue::OpenValue (QGAMES::OpenValue::Type t, const std::string& v)
{
	_type = t;
	_boolValue = false; 
	_intValue = __MININT__;
	_strValue = std::string (__NULL_STRING__);
	_bdataValue = __MINBDATA__;

	switch (_type)
	{
		case QGAMES::OpenValue::Type::_BOOL:
			_boolValue = std::atoi (v.c_str ()) != 0;
			break;
		case QGAMES::OpenValue::Type::_INT:
			_intValue = std::atoi (v.c_str ());
			break;
		case QGAMES::OpenValue::Type::_STRING:
			_strValue = v;
			break;
		case QGAMES::OpenValue::Type::_BDATA:
			_bdataValue = __BD std::atof (v.c_str ());
			break;
		default:
			break;
	}
}

// ---
bool QGAMES::OpenValue::operator == (const QGAMES::OpenValue& v) const
{
	bool result = false;
	if (_type == v._type)
	{
		switch (_type)
		{
			case QGAMES::OpenValue::Type::_BOOL:
				result = boolValue () == v.boolValue ();
				break;
			case QGAMES::OpenValue::Type::_INT:
				result = intValue () == v.intValue ();
				break;
			case QGAMES::OpenValue::Type::_STRING:
				result = strValue () == v.strValue ();
				break;
			case QGAMES::OpenValue::Type::_BDATA:
				result = bdataValue () == v.bdataValue ();
				break;
			default:
				break;
		}
	}

	return (result);
}

// ---
std::ostream& QGAMES::operator << (std::ostream& s, const QGAMES::OpenValue& val)
{
	s << (int) val._type << std::endl;
	switch (val._type)
	{
		case QGAMES::OpenValue::Type::_BOOL:
			s << val._boolValue;
			break;
		case QGAMES::OpenValue::Type::_INT:
			s << val._intValue;
			break;
		case QGAMES::OpenValue::Type::_STRING:
			s << val._strValue;
			break;
		case QGAMES::OpenValue::Type::_BDATA:
			s << val._bdataValue;
			break;
		default:
			break;
	}

	return (s);
}

// ---
std::istream& QGAMES::operator >> (std::istream& s, QGAMES::OpenValue& val)
{
	val = QGAMES::OpenValue ();
	int tp; s >> tp; val._type = (QGAMES::OpenValue::Type) tp;

	switch (val._type)
	{
		case QGAMES::OpenValue::Type::_BOOL:
			s >> val._boolValue;
			break;
		case QGAMES::OpenValue::Type::_INT:
			s >> val._intValue;
			break;
		case QGAMES::OpenValue::Type::_STRING:
			s >> val._strValue;
			break;
		case QGAMES::OpenValue::Type::_BDATA:
			s >> val._bdataValue;
			break;
		default:
			break;
	}

	return (s);
}

// ---
QGAMES::SetOfOpenValues::SetOfOpenValues (const std::map <std::string, std::string>& prps, const std::string& n)
	: _name (n),
	  _openValues (),
	  _nestedOpenValues ()
{
	for (std::map <std::string, std::string>::const_iterator i = prps.begin (); i != prps.end (); i++)
		_openValues [std::atoi ((*i).first.c_str ())] = QGAMES::OpenValue ((*i).second);
}

// ---
int QGAMES::SetOfOpenValues::numberValues () const
{ 
	int result = (int) _openValues.size () + (int) _nestedOpenValues.size ();
	for (QGAMES::SetsOfOpenValues::const_iterator i = _nestedOpenValues.begin ();
			i != _nestedOpenValues.end (); result += (*i++).second.numberValues ());
	return (result);
}

// ---
bool QGAMES::SetOfOpenValues::operator == (const QGAMES::SetOfOpenValues& s) const
{
	// If the size of the internal structures to keep the open values are not equal
	// the content couldn't be equal either...
	if ((_openValues.size () != s._openValues.size ()) ||
		(_nestedOpenValues.size () != s._nestedOpenValues.size ()))
		return (false);

	// The nested values must be equal...
	// Take into account that the values might not be kept in the same order in each structure
	// So every element of the current nested open values structure should be compare with all
	// the rest to determine that it is not really comparable with that...
	bool result = true;
	for (QGAMES::SetsOfOpenValues::const_iterator i = _nestedOpenValues.begin ();
			i != _nestedOpenValues.end () && result; i++)
	{
		result = false;
		for (QGAMES::SetsOfOpenValues::const_iterator j = s._nestedOpenValues.begin ();
				j != s._nestedOpenValues.end () && !result; j++)
			result = ((*i).second == (*j).second);
	}

	// ...and finally...
	// the direct open values must be equal too...
	// Result is "true" at this point, remember!!
	for (QGAMES::OpenValues::const_iterator i = _openValues.begin (); 
			i != _openValues.end () && result; i++)
		if (!s.existOpenValue ((*i).first) ||
			(s.existOpenValue ((*i).first) && (*i).second != s.openValue ((*i).first)))
			result = false; 

	// Returns the result...
	return (result);
}

// ---
std::ostream& QGAMES::operator << (std::ostream& s, const QGAMES::SetOfOpenValues& vls)
{
	std::string nTS = 
		(vls._name == std::string (__NULL_STRING__)) ? std::string ("---") : vls._name;
	s << nTS << std::endl;

	// First the "direct" open values...
	s << (int) vls._openValues.size ();
	for (QGAMES::OpenValues::const_iterator i = vls._openValues.begin (); 
			i != vls._openValues.end (); i++)
		s << std::endl << (*i).first << std::endl << (*i).second; // Pair made up of key and value...

	// Then the nested ones...
	s << std::endl << (int) vls._nestedOpenValues.size ();
	for (QGAMES::SetsOfOpenValues::const_iterator i = vls._nestedOpenValues.begin ();
			i != vls._nestedOpenValues.end (); i++)
		s << std::endl << (*i).first << std::endl << (*i).second;

	return (s);
}

// ---
std::istream& QGAMES::operator >> (std::istream& s, QGAMES::SetOfOpenValues& vls)
{
	vls = QGAMES::SetOfOpenValues (); 

	s >> vls._name;
	if (vls._name == std::string ("---"))
		vls._name = std::string (__NULL_STRING__);

	// First the "direct" open values...
	int nE; s >> nE;
	for (; nE > 0; nE--)
	{
		int key;
		QGAMES::OpenValue value;
		s >> key >> value;
		vls._openValues [key] = value;
	}

	// ...and then the "nested" ones
	int nNE; s >> nNE;
	for (; nNE > 0; nNE--)
	{
		int key;
		QGAMES::SetOfOpenValues sValue;
		s >> key >> sValue;
		vls._nestedOpenValues [key] = sValue;
	}

	return (s);
}
