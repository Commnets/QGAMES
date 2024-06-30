/** \ingroup Game */
/** \ingroup Screen */
/*@{*/

/**	
 *	@file	
 *	File: mask.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/09/2018 \n
 *	Description: Defines what a mask is. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_MASK__
#define __QGAMES_MASK__

#include <Common/screen.hpp>
#include <map>

namespace QGAMES
{
	/** The mask is a class used to hide or highlight (depends on the type) pixels in the screen. \n
		It is used to create effects over the screen. \n
		All masks use a buffer were the effect is created. */
	class Mask;
	typedef std::map <int, Mask*> Masks;

	class Mask
	{
		public:
		/** Define a mask. The paremeters are:
		  * @param id		The id to identify the mask. \n
		  *					Take into account masks will usually be created from a builder (@see MaskBuiler)
		  * @param btp		Type of blending. The type has to be interpreted by the screen implementation. \n
		  *					but usual types of blendings are: intead, add colours, less colours, etc...
		  * @param width	width in pixels of the mask. -1 means full visual width of the screen.
		  * @param height	height in pixels of the mask. -1 means full visual height of the screen.
		  */
		Mask (int id, int btp, int w = -1, int h = -1)
			: _id (id),
			  _blendingType (btp),
			  _width (w), _height (h),
			  _buffer (NULL)
							{ assert ((_width > 0 || _width == -1) && 
									  (_height > 0 || _height == -1)); }
		virtual ~Mask () 
						{ clear (); }

		/** To know the id of the mask. */
		int id () const
						{ return (_id); }

		/** To know or to set the type of blending. */
		int blendingType () const
						{ return (_blendingType); }
		void setBlendingType (int btp)
						{ _blendingType = btp; clear (); }
		/** To know or to set the size of the mask. */
		int width () const
						{ return (_width); }
		int height () const
						{ return (_height); }
		void setSize (int w = -1, int h = -1)
						{ _width = w; _height = h;
						  assert (_width > 0 && _height > 0);
						  clear (); }

		/** To create just its opposite. */
		virtual Mask* opposite () = 0;

		/** 
		  *	The content of the mask is got like a buffer. It is a temporary space in memory
		  *	that can be managed in many different ways. One of them (@see Buffer class in screen.hpp file)
		  *	can be to write it over the real screen e.g. \n
		  *	The buffer is created if it doesn't exist already. \n
		  *	Anytime a parameter of the mask changes 
		  *	(invoking the clear method in the default implementation for the size attributes only), 
		  *	the buffer will be created again when this method is invoked. 
		  */
		Screen::Buffer* bufferOver (Screen* scr);

		protected:
		/** To clear the temporal buffer. */
		void clear ()
							{ delete (_buffer); _buffer = NULL; }
		/** This method is just to draw what the mask is. */
		virtual void drawOver (Screen* scr) = 0;

		protected:
		/** The id. */
		int _id;
		/** Type of blending of the buffer.
			The value of this valriable is also into the bufer once it is created. */
		int _blendingType;
		/** The size of the mask. */
		int _width, _height;

		// Implementation
		/** The last buffer calculated. */
		Screen::Buffer* _buffer;
	};

	/** A circle in the middle of the zone defined.
		When writing this mask over a picture, what is IN the circle will remain.
		It is possible event to apply a potential filter with a color. */
	class INMiddleCircleMask : public Mask
	{
		public:
		struct Properties
		{
			Properties ()
				: _radix (0), _color (Color (255, 255, 255, 255))
						{ }

			Properties (const std::map <std::string, std::string>& prms);

			Properties (int r, const Color& c)
				: _radix (r), _color (c)
						{ assert (_radix >= 0); }

			int _radix;
			Color _color;
		};

		INMiddleCircleMask (int id, int btp, const Properties& prps = Properties (), int w = -1, int h = -1)
			: Mask (id, btp, w, h),
			  _properties (prps)
							{ }

		/** To know and change the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; clear (); }

		/** @see parent. */
		virtual Mask* opposite ();

		protected:
		/** @see parent. */
		virtual void drawOver (Screen* scr);

		protected:
		Properties _properties;
	};

	/** A circle in the middle of the zone defined. \n
		When writing this mask over a picture, what is OUT the circle will remain. \n
		It is possible to apply even a potential filter with a color. */
	class OUTMiddleCircleMask : public Mask
	{
		public:
		struct Properties
		{
			Properties ()
				: _radix (0), _color (Color (255, 255, 255, 255))
						{ }

			Properties (const std::map <std::string, std::string>& prms);

			Properties (int r, const Color& c)
				: _radix (r), _color (c)
						{ assert (_radix >= 0); }

			int _radix;
			Color _color;
		};

		OUTMiddleCircleMask (int id, int btp, const Properties& prps = Properties (), int w = -1, int h = -1)
			: Mask (id, btp, w, h),
			  _properties (prps)
							{ }

		/** To know and change the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; clear (); }

		/** @see parent. */
		virtual Mask* opposite ();

		protected:
		/** @see parent. */
		virtual void drawOver (Screen* scr);

		protected:
		Properties _properties;
	};

	/** A circle in a specific position of the defined zone. \n
		When writing this mask over a picture, what is IN the circle will remain. \n
		It is possible event to apply a potential filter with a color. \n
		The position of the centre of the circle is given in percentage 
		of the visible zone (between o and 1, if greater the center will be out). */
	class INCircleMask : public Mask
	{
		public:
		struct Properties
		{
			Properties ()
				: _radix (0), _position (), _color (Color (255, 255, 255, 255))
						{ }

			Properties (const std::map <std::string, std::string>& prms);

			Properties (int r, const Position& p, const Color& c)
				: _radix (r), _position (p), _color (c)
						{ assert (_radix >= 0); }

			int _radix;
			Position _position;
			Color _color;
		};

		INCircleMask (int id, int btp, const Properties& prps = Properties (), int w = -1, int h = -1)
			: Mask (id, btp, w, h),
			  _properties (prps)
							{ }

		/** To know and change the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; clear (); }

		/** @see parent. */
		virtual Mask* opposite ();

		protected:
		/** @see parent. */
		virtual void drawOver (Screen* scr);

		protected:
		Properties _properties;
	};

	/** A circle in a specific position of the defined zone. \n
		When writing this mask over a picture, what is OUT the circle will remain. \n
		It is possible to apply even a potential filter with a color. \n
		The position of the centre of the circle is given in percentage 
		of the visible zone (between o and 1, if greater the center will be out). */
	class OUTCircleMask : public Mask
	{
		public:
		struct Properties
		{
			Properties ()
				: _radix (0), _position (), _color (Color (255, 255, 255, 255))
						{ }

			Properties (const std::map <std::string, std::string>& prms);

			Properties (int r, const Position& p, const Color& c)
				: _radix (r), _position (p), _color (c)
						{ assert (_radix >= 0); }

			int _radix;
			Position _position;
			Color _color;
		};

		OUTCircleMask (int id, int btp, const Properties& prps = Properties (), int w = -1, int h = -1)
			: Mask (id, btp, w, h),
			  _properties (prps)
							{ }

		/** To know and change the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; clear (); }

		/** @see parent. */
		virtual Mask* opposite ();

		protected:
		/** @see parent. */
		virtual void drawOver (Screen* scr);

		protected:
		Properties _properties;
	};

	/** The zone defined is divided vertically into two. \n
		When the buffer is drawn over a picture what was drawn in the LEFT part will remain whilst the rest will be deleted. */
	class LeftSideMask : public Mask
	{
		public:
		struct Properties
		{
			Properties ()
				: _distance (0), _color (Color (255, 255, 255, 255))
						{ }

			Properties (const std::map <std::string, std::string>& prms);

			Properties (int d, const Color& c)
				: _distance (d), _color (c)
						{ assert (_distance >= 0); }

			int _distance;
			Color _color;
		};

		LeftSideMask (int id, int btp, const Properties& prps = Properties (), int w = -1, int h = -1)
			: Mask (id, btp, w, h),
			  _properties (prps)
							{ }

		/** To know and change the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; clear (); }

		/** @see parent. */
		virtual Mask* opposite ();

		protected:
		/** @see parent. */
		virtual void drawOver (Screen* scr);

		protected:
		Properties _properties;
	};

	/** The zone defined is divided vertically into two. \n
		What is written in the RIGHT part will remain, the other part will be deleted. */
	class RightSideMask : public Mask
	{
		public:
		struct Properties
		{
			Properties ()
				: _distance (0), _color (Color (255, 255, 255, 255))
						{ }

			Properties (const std::map <std::string, std::string>& prms);

			Properties (int d, const Color& c)
				: _distance (d), _color (c)
						{ assert (_distance >= 0); }

			int _distance;
			Color _color;
		};

		RightSideMask (int id, int btp, const Properties& prps = Properties (), int w = -1, int h = -1)
			: Mask (id, btp, w, h),
			  _properties (prps)
							{ }

		/** To know and change the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; clear (); }

		/** @see parent. */
		virtual Mask* opposite ();

		protected:
		/** @see parent. */
		virtual void drawOver (Screen* scr);

		protected:
		Properties _properties;
	};

	/** The zone defined is divided horizontally into two.
		When the buffer is drawn over a picture what was drawn in the UP part will remain whilst the rest will be deleted. */
	class UpSideMask : public Mask
	{
		public:
		struct Properties
		{
			Properties ()
				: _distance (0), _color (Color (255, 255, 255, 255))
						{ }

			Properties (const std::map <std::string, std::string>& prms);

			Properties (int d, const Color& c)
				: _distance (d), _color (c)
						{ assert (_distance >= 0); }

			int _distance;
			Color _color;
		};

		UpSideMask (int id, int btp, const Properties& prps = Properties (), int w = -1, int h = -1)
			: Mask (id, btp, w, h),
			  _properties (prps)
							{ }

		/** To know and change the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; clear (); }

		/** @see parent. */
		virtual Mask* opposite ();

		protected:
		/** @see parent. */
		virtual void drawOver (Screen* scr);

		protected:
		Properties _properties;
	};

	/** The zone defined is divided vertically into two.
		What is written in the DOWN part will remain, the other part will be deleted. */
	class DownSideMask : public Mask
	{
		public:
		struct Properties
		{
			Properties ()
				: _distance (0), _color (Color (255, 255, 255, 255))
						{ }

			Properties (const std::map <std::string, std::string>& prms);

			Properties (int d, const Color& c)
				: _distance (d), _color (c)
						{ assert (_distance >= 0); }

			int _distance;
			Color _color;
		};

		DownSideMask (int id, int btp, const Properties& prps = Properties (), int w = -1, int h = -1)
			: Mask (id, btp, w, h),
			  _properties (prps)
							{ }

		/** To know and change the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; clear (); }

		/** @see parent. */
		virtual Mask* opposite ();

		protected:
		/** @see parent. */
		virtual void drawOver (Screen* scr);

		protected:
		Properties _properties;
	};
}

#endif
  
// End of the file
/*@}*/