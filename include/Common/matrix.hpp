/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: matrix.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a 3x3 matrix and its operations. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_MATRIX__
#define __QGAMES_MATRIX__

#include <Common/definitions.hpp>
#include <ostream>
#include <istream>
#include <array>

namespace QGAMES
{
	/** To manipulate a squared matrix.
		This is a temnplate. But what is really used in the QGAMES are usually 3x3 and 4x4 
		The rows are named from 0 to S - 1. The same for the columns.
		The definition is valid except a matrix with dimension 1. */
	template <typename T, unsigned S> class SMatrix
	{
		public:
		constexpr SMatrix ();
		constexpr SMatrix (T d)
			: _data ()
							{ SMatrix <T, S> (); for (int i = 0; i < S; i++) _data [i][i] = d; }
		constexpr SMatrix (T m [S][S]);
		constexpr SMatrix (const std::array <std::array <T, S>, S>& m);
		constexpr SMatrix (const SMatrix <T, S>& m);
		SMatrix <T, S>& operator = (const SMatrix <T, S>& m) = default;

		constexpr T element (unsigned y, unsigned x) const
							{ assert (x >= 0 && x < S && y >= 0 && y < S); return (_data [y][x]); }
		void setElement (unsigned y, unsigned x, const T& d) const
							{ assert (x >= 0 && x < S && y >= 0 && y < S); _data [y][x] = d; }
		constexpr T* operator [] (int n) const
							{ assert (n >= 0 && n < S); return (_data [n]); }
		T* operator [] (int n)
							{ assert (n >= 0 && n < S); return (_data [n]); }
		constexpr unsigned dimension () const
							{ return (S); }

		// Comparation...
		constexpr bool operator == (const SMatrix <T, S>& m) const;
		constexpr bool operator != (const SMatrix <T, S>& m) const
							{ return (!(*this == m)); }

		// Add...
		constexpr SMatrix <T, S> plus (const SMatrix <T, S>& m) const;
		constexpr SMatrix <T, S> operator + (const SMatrix <T, S>& m) const
							{ return (plus (m)); }
		constexpr SMatrix <T, S>& operator += (const SMatrix <T, S>& m)
							{ *this = *this + m; return (*this); }
		
		// Subtract...
		constexpr SMatrix <T, S> less (const SMatrix <T, S>& m) const;
		constexpr SMatrix <T, S> operator - (const SMatrix <T, S>& m) const
							{ return (less (m)); }
		constexpr SMatrix <T, S>& operator -= (const SMatrix <T, S>& m)
							{ *this = *this - m; return (*this); }
		
		// Multiply & Divide...
		constexpr SMatrix <T, S> multiply (const SMatrix <T, S>& m) const;
		constexpr SMatrix <T, S> operator * (const SMatrix <T, S>& m) const
							{ return (multiply (m)); }
		constexpr SMatrix <T, S>& operator *= (const SMatrix <T, S>& m)
							{ *this = *this * m; return (*this); }
		constexpr SMatrix <T, S> multiply (T n) const;
		constexpr SMatrix <T, S> operator * (T n) const
							{ return (multiply (n)); }
		constexpr SMatrix <T, S> operator / (T n) const
							{ assert (n != (T) 0); return (multiply (1 / n)); }
		constexpr SMatrix <T, S>& operator *= (T n)
							{ *this = *this * n; return (*this); }
		constexpr SMatrix <T, S>& operator /= (T n)
							{ *this = *this / n; return (*this); }
		constexpr friend SMatrix <T, S> operator * (T n, const SMatrix <T, S>& m)
							{ return (m * n); }

		// Output & input operators...
		friend std::ostream& operator << (std::ostream& o, const SMatrix <T, S>& m)
							{
								o << S;
								for (unsigned i = 0; i < S; i++)
									for (unsigned j = 0; j < S; j++)
										o << std::endl << m [i][j];

								return (o);
							}

		friend std::istream& operator >> (std::istream& i, const SMatrix <T, S>& m)
							{
								unsigned s = 0;

								i >> s;
								assert (S == s);

								for (unsigned i = 0; i < S; i++)
									for (unsigned j = 0; j < S; j++)
										i >> m [i][j];
								
								return (i);
							}

		// Other operations
		constexpr SMatrix <T, S - 1> without (unsigned i, unsigned j) const;
		constexpr SMatrix <T, S> adjoint () const;
		constexpr SMatrix <T, S> transposed () const;
		constexpr T determinant () const; // Recursive template...
		constexpr SMatrix <T, S> inverse () const;

		T _data [S][S];
	};

	/** When the matrix is 1 unit only 
		Not all funcitons are needed, just the ones used by the method determinant. */
	template <typename T> class SMatrix <T, 1>
	{
		public:
		constexpr SMatrix ()
			: _data ()
							{ _data [0][0] = (T) 0; }
		constexpr SMatrix (T m) 
			: _data () 
							{ _data [0][0] = m; }
		constexpr SMatrix (const SMatrix <T, 1>& m)
			: _data ()
							{ _data [0][0] = m._data [0][0]; }
		constexpr SMatrix <T, 1>& operator = (const SMatrix <T, 1>& m)
							{ _data [0][0] = m._data [0][0]; }

		constexpr T* operator [] (int n) const
							{ assert (n == 0); return (_data [0]); }
		T* operator [] (int n)
							{ assert (n == 0); return (_data [0]); }

		constexpr T determinant () const
							{ return (_data [0][0]); }

		T _data [1][1];
	};

	// The matrix needed for the games...
	using Matrix4 = SMatrix <bdata, 4>; 
	using Matrix3 = SMatrix <bdata, 3>;
	using Matrix2 = SMatrix <bdata, 2>;
	using Matrix1 = SMatrix <bdata, 1>;
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S>::SMatrix ()
	: _data ()
{ 
	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			_data [i][j] = (T) 0;
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S>::SMatrix (T m [S][S])
{
	SMatrix <T, S> ();

	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			_data [i][j] = m [i][j]; 
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S>::SMatrix (const std::array <std::array <T, S>, S>& m)
	: _data ()
{
	SMatrix <T, S> ();

	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			_data [i][j] = m [i][j]; 
}

//---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T,S>::SMatrix (const SMatrix <T, S>& m)
	: _data ()
{
	SMatrix <T, S> ();

	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			_data [i][j] = m._data [i][j]; 
}

// ---
template <typename T, unsigned S> 
constexpr bool QGAMES::SMatrix <T, S>::operator == (const QGAMES::SMatrix <T, S>& m) const
{
	bool result = true;

	for (int i = 0; i < S && result; i++)
		for (int j = 0; j < S && result; j++)
			result &= (_data [i][j] == m._data [i][j]); 

	return (result);
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S> QGAMES::SMatrix <T, S>::plus (const QGAMES::SMatrix <T, S>& m) const
{
	QGAMES::SMatrix<T, S> dt;

	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			dt._data [i][j] =  _data [i][j] + m._data [i][j];

	return (dt);
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S> QGAMES::SMatrix <T, S>::less (const QGAMES::SMatrix <T, S>& m) const
{
	QGAMES::SMatrix<T, S> dt;

	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			dt._data [i][j] =  _data [i][j] - m._data [i][j];

	return (dt);
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S> QGAMES::SMatrix <T, S>::multiply (const QGAMES::SMatrix <T, S>& m) const
{
	QGAMES::SMatrix <T, S> result;

	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			for (int k = 0; k < S; k++)
				result._data [i][j] += _data [i][k] * m._data [k][j];

	return (result);
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S> QGAMES::SMatrix <T, S>::multiply  (T n) const
{
	QGAMES::SMatrix <T, S> result;

	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			result._data [i][j] = _data [i][j] * n;

	return (result);
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S - 1> QGAMES::SMatrix <T, S>::without (unsigned i, unsigned j) const
{
	assert (i >= 0 && i < S && j >= 0 && j < S);

	QGAMES::SMatrix <T, S - 1> result;

	int iP = 0;
	int jP = 0; 
	for (int iC = 0; iC < S; iC++)
	{
		if (iC != i)
		{
			jP = 0;
			for (int jC = 0; jC < S; jC++)
			{
				if (jC != j)
				{
					result._data [iP][jP] = _data [iC][jC];
					jP++;
				}
			}

			iP++;
		}
	}

	return (result);
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S> QGAMES::SMatrix <T, S>::adjoint () const
{
	QGAMES::SMatrix <T, S> result;

	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			result._data [i][j] = ((((i + j) % 2) == 0) ? 1 : -1) * without (i, j).determinant ();

	return (result);
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S> QGAMES::SMatrix <T, S>::transposed () const
{
	QGAMES::SMatrix <T, S> result;

	for (int i = 0; i < S; i++)
		for (int j = 0; j < S; j++)
			result._data [i][j] = _data [j][i];

	return (result);
}

// ---
template <typename T, unsigned S> 
constexpr T QGAMES::SMatrix <T, S>::determinant () const
{
	T result = (T) 0;

	for (int j = 0; j < S; j++)
		result += ((((0 + j) % 2) == 0) ? 1 : -1) * _data [0][j] * without (0, j).determinant ();

	return (result);
}

// ---
template <typename T, unsigned S> 
constexpr QGAMES::SMatrix <T, S> QGAMES::SMatrix <T, S>::inverse () const
{
	T det = determinant ();
	assert (det != (T) 0);
	
	return ((1 / det) * adjoint ().transposed ()); 
}

#endif
  
// End of the file
/*@}*/
