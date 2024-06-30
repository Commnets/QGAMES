/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: openvalue.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/03/2018 \n
 *	Description: Defines an open vale, and a set of those \n
 *				 This kind of constructions are used in many places. e.g in the cofigurationstate of the element. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OPENVALUE__
#define __QGAMES_OPENVALUE__

#include <Common/definitions.hpp>
#include <vector>
#include <map>

namespace QGAMES
{
	class OpenValue
	{
		public:
		typedef enum
		{
			_BOOL = 0,
			_INT = 1,
			_STRING = 2,
			_BDATA = 3
		} Type;

		OpenValue ()
			: _type (_BOOL), // Bool by default...
			  _boolValue (false), _intValue (__MININT__), _strValue (__NULL_STRING__), _bdataValue (__MINBDATA__)
					{ }
		OpenValue (Type t, const std::string& v);
		OpenValue (bool v)
			: _type (_BOOL), _boolValue (v),
			  _intValue (__MININT__), _strValue (__NULL_STRING__), _bdataValue (__MINBDATA__)
					{ }
		OpenValue (int v)
			: _type (_INT), _intValue (v),
			  _boolValue (false), _strValue (__NULL_STRING__), _bdataValue (__MINBDATA__)
					{ }
		OpenValue (const std::string& v)
			: _type (_STRING), _strValue (v),
			  _boolValue (false), _intValue (__MININT__), _bdataValue (__MINBDATA__)
					{ }
		OpenValue (bdata v)
			: _type (_BDATA), _bdataValue (v),
			  _boolValue (false), _intValue (__MININT__), _strValue (__NULL_STRING__)
					{ }

		// Comparation operators...
		/** Two OpenValues are equal if they have the same type and the same value for that type. */
		bool operator == (const OpenValue& v) const;
		bool operator != (const OpenValue& v) const
							{ return (!(*this == v)); }

		Type type () const
							{ return (_type); }
		bool boolValue () const
							{ return (_boolValue); }
		int intValue () const
							{ return (_intValue); }
		const std::string& strValue () const
							{ return (_strValue); }
		bdata bdataValue () const
							{ return (_bdataValue); }

		friend std::ostream& operator << (std::ostream& s, const OpenValue& val);
		friend std::istream& operator >> (std::istream& s, OpenValue& val);

		private:
		Type _type;
		bool _boolValue;
		int _intValue;
		std::string _strValue;
		bdata _bdataValue;
	};

	typedef std::map <int, OpenValue> OpenValues;

	class SetOfOpenValues;
	typedef std::map <int, SetOfOpenValues> SetsOfOpenValues;

	/** 
	  * A set of open values, or a set of of a set of open values, or both. \n
	  * This is a very complex structure to keep information open information used in many contexts,
	  * specially in those ones dedicated to preserve configuration information
	  * of the elements taking part of the game. \n
	  * They are also used to transmit (e.g) information through out the communication objects. \n
	  * There can be "direct" values, of nested ones. \n
	  * By default the nested elements are "blank". \n
	  * The structure can have a name. It could be usefull when checking whether the structure is valid.
	  */
	class SetOfOpenValues
	{
		public:
		SetOfOpenValues (const std::string& n = std::string (__NULL_STRING__))
			: _name (n),
			  _openValues (),
			  _nestedOpenValues () 
						{ }

		SetOfOpenValues (const OpenValues& val, const std::string& n = std::string (__NULL_STRING__))
			: _name (n),
			  _openValues (val),
			  _nestedOpenValues ()
						{ }

		SetOfOpenValues (const OpenValues& val, const SetsOfOpenValues& sVal, const std::string& n = std::string (__NULL_STRING__))
			: _name (n),
			  _openValues (val),
			  _nestedOpenValues (sVal)
							{ }

		SetOfOpenValues (const std::map <std::string, std::string>& prps,
			const std::string& n = std::string (__NULL_STRING__)); // From a list of properties...

		/** The name. */
		const std::string& name () const
							{ return (_name); }
		void setName (const std::string& n)
							{ _name = n; }

		/** The total. */
		int numberValues () const;
		/* The ids of the last elements inserted into the structure.
			It could be used to create more scalable layers of data. 
			If there is no data inside the values returned will be -1, meaning nothing. */
		int lastOpenValueId () const
							{ return (_openValues.empty () ? -1 : (*_openValues.rbegin ()).first); }
		int lastNestedOpenValueId () const
							{ return (_nestedOpenValues.empty () ? -1 : (*_nestedOpenValues.rbegin ()).first); }

		const OpenValues& openValues () const
							{ return (_openValues); }
		const SetsOfOpenValues& nestedOpenValues () const
							{ return (_nestedOpenValues); }

		bool existOpenValue (int nV) const
							{ return (_openValues.find (nV) != _openValues.end ()); }
		bool existSetOfOpenValues (int nS) const
							{ return (_nestedOpenValues.find (nS) != _nestedOpenValues.end ()); }
		// A list with the path to find the open value through out the nested set of open values...
		bool existOpenValue (const std::vector <int>& nV) const 
							{ return (((int) nV.size () == 1) 
								? existOpenValue (*nV.begin ())
								: existOpenValue (std::vector <int> (nV.begin ()++, nV.end ()))); }
		bool existSetOfOpenValues (const std::vector <int>& nV) const
							{ return (((int) nV.size () == 1) 
								? existSetOfOpenValues (*nV.begin ())
								: existSetOfOpenValues (std::vector <int> (nV.begin ()++, nV.end ()))); }

		const OpenValue& openValue (int nV) const
							{ return ((*_openValues.find (nV)).second); }
		const SetOfOpenValues& setOfOpenValues (int nS) const
							{ return ((*_nestedOpenValues.find (nS)).second); }
		const OpenValue& openValue (const std::vector <int>& nV) const
							{ return (((int) nV.size () == 1) 
								? openValue (*nV.begin ()) // To take into account the nested values...
								: openValue (std::vector <int> (nV.begin ()++, nV.end ()))); }
		const SetOfOpenValues& setOfOpenValues (const std::vector <int>& nV) const
							{ return (((int) nV.size () == 1) 
								? setOfOpenValues (*nV.begin ()) // To take into account the nested values...
								: setOfOpenValues (std::vector <int> (nV.begin ()++, nV.end ()))); }

		void addOpenValue (int nV, const OpenValue& v) // If it already exists, the content is replaced!
							{ _openValues [nV] = v; }
		void addSetOfOpenValues (int nS, const SetOfOpenValues& s)
							{ _nestedOpenValues [nS] = s; }
		void addOpenValue (const std::vector <int>& nV, const OpenValue& v)
							{ if ((int) nV.size () == 1) addOpenValue (*nV.begin (), v);
							  else addOpenValue (std::vector <int> (nV.begin ()++, nV.end ()), v); }
		void addSetOfOpenValues (const std::vector <int>& nV, const SetOfOpenValues& v)
							{ if ((int) nV.size () == 1) addSetOfOpenValues (*nV.begin (), v);
							  else addSetOfOpenValues (std::vector <int> (nV.begin ()++, nV.end ()), v); }

		void removeOpenValue (int nV)
							{ _openValues.erase (_openValues.find (nV)); }
		void removeSetOfOpenValues (int nS)
							{ _nestedOpenValues.erase (_nestedOpenValues.find (nS)); }
		void removeOpenValue (const std::vector <int>& nV)
							{ if ((int) nV.size () == 1) removeOpenValue (*nV.begin ());
							  else removeOpenValue (std::vector <int> (nV.begin ()++, nV.end ())); }
		void removeSetOfOpenValues (const std::vector <int>& nV)
							{ if ((int) nV.size () == 1) removeSetOfOpenValues (*nV.begin ());
							  else removeSetOfOpenValues (std::vector <int> (nV.begin ()++, nV.end ())); }

		bool operator == (const SetOfOpenValues& s) const;
		bool operator != (const SetOfOpenValues& s) const
							{ return (!(*this == s)); }

		friend std::ostream& operator << (std::ostream& s, const SetOfOpenValues& vls);
		friend std::istream& operator >> (std::istream& s, SetOfOpenValues& vls);

		private:
		std::string _name;
		OpenValues _openValues;
		SetsOfOpenValues _nestedOpenValues;
	};
}

#endif
  
// End of the file
/*@}*/