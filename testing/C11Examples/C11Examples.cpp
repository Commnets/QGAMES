#include <iostream>
#include <tuple>

class CLASSA final
{
	public:
	constexpr CLASSA (int x = 1)
		: _x (x)
				{  }

	constexpr CLASSA (const CLASSA&) = default;

	CLASSA& operator = (const CLASSA&) = default;

	constexpr int doble () const
				{ return (2 * _x); }

	private:
	const int _x;
};

class CLASSB final
{
	public:
	constexpr CLASSB ()
				{ }

	constexpr CLASSB (const CLASSB&) = default;

	CLASSB& operator = (const CLASSB&) = default;

	constexpr int value (int&& p) const
				{ return ((p >= 0 && p < 5) ? _elmts [p].doble () : _elmts [4].doble ()); }

	private:
	const CLASSA _elmts [5] = {1,2,3,4,5};
};

template <unsigned int NE>
class CLASSC 
{
	public:
	constexpr CLASSC (const CLASSC<NE - 1>& d)
		: _data (d)
				{ }

	constexpr int valueA () const
				{ return (_data.valueA ()); }

	private:
	const CLASSC <NE - 1> _data;
};

template <>
class CLASSC <0>
{  
	public:
	constexpr CLASSC (int&& a, int&& b)
		: _a (a), _b (b)
				{ }

	constexpr int valueA () const
				{ return (_a); }
	constexpr int valueB () const
				{ return (_b); }

	private :
	const int _a, _b;
};

int main (int argc, char* argv[])
{
	constexpr CLASSA Aa (5);
	constexpr int Ab = CLASSA (5).doble ();
	constexpr CLASSA Ac [5] = { 1,2,3,4,5 };
	static_assert (sizeof (Ac) / sizeof (int) == 5, "ok");

	constexpr CLASSB Ba;
	constexpr int Bb = CLASSB ().value (2);
	constexpr CLASSB Bc [5] = { };
	constexpr CLASSB Bd = Ba;
	constexpr CLASSB Be = CLASSB ();
	static_assert (sizeof (Ba) == 20, "ok"); // Every element is a point (word = 4 length)

	constexpr CLASSC<0> Ca1 (1,2);
	constexpr CLASSC<1> Ca2 (Ca1);
	constexpr CLASSC<2> Ca3 (Ca2);
	static_assert (Ca3.valueA () == 1,"ok");

	constexpr std::tuple <int, int, int> TUPLE (1,2,3);
	static_assert (std::get<0>(TUPLE) == 1, "ok");

	std::cout << Aa.doble () << std::endl;
	int p = 1; std::cout << Ba.value (p + 1) << std::endl;
	std::cout << CLASSC<3> (CLASSC<2> (CLASSC<1> (CLASSC<0> (4,5)))).valueA () << std::endl;
	std::cout << std::get<0>(TUPLE) << std::endl;
}
