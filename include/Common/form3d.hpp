/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: form3D.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 07/02/2021 \n
 *	Description: Defines a class to represent a QGAMES 3D form.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_FORM3D__
#define __QGAMES_FORM3D__

#include <Common/form.hpp>
#include <Common/color.hpp>

namespace QGAMES
{
	/** The information used by the form. */
	class Form3DData
	{
		public:
		/** The faces used in the definition of a member.
			The face is always drawn using triangles of a color. */
		class Face
		{
			public:
			Face () = delete;

			Face (int id , const Positions& pos, const Color& c)
				: _id (id), _positions (pos), _color (c)
								{ assert ((int) pos.size () >= 3); }

			Face (const Face& f) = default;

			Face& operator = (const Face& f) = default;

			int id () const
								{ return (_id); }
			const Positions& positions () const
								{ return (_positions); }
			const Color& color () const
								{ return (_color); }
			void setColor (const Color& c)
								{ _color = c; }

			/** To get the box containing the face. */
			Box containingBox () const
								{ return (Box::boxToInclude (_positions)); }

			Face rotated (const Position& p, const Vector& v, bdata ang) const;
			void selfRotate (const Position& p, const Vector& v, bdata ang)
								{ *this = rotated (p, v, ang); }

			void drawOn (Screen* scr, const Position& pos, int a = 255);

			protected:
			int _id;
			Positions _positions; // Always relative positions from Position::_cero
			Color _color; 
		};

		/** It has to be a vector to maintain the relative order between (related) them if needed. */
		using Faces = std::vector <Face>;

		/** A member is made up of a set of faces. 
			The member's position is always relative to form's position.
			Members' faces' positions are relative to the reference position of the member. */
		class Member
		{
			public:
			Member () = delete;

			Member (int id, Faces& fs, const Position& rP = Position::_cero)
				: _id (id), _faces (fs), _referencePosition (rP)
								{ assert ((int) _faces.size () >= 1); }

			Member (const Member& sF) = default;

			Member& operator = (const Member& fS) = default;

			int id () const
								{ return (_id); }
			const Faces& faces () const
								{ return (_faces); }
			const Position& referencePosition () const
								{ return (_referencePosition); }

			bool existsFace (int id) const;
			const Face& face (int id) const;

			/** To get the box containing the member. */
			Box containingBox () const;

			Member rotate (const Position& p, const Vector& v, bdata ang) const;
			Member rotate (const Vector& v, bdata ang) const
								{ return (rotate (_referencePosition, v, ang)); }
			void selfRotate (const Position& p, const Vector& v, bdata ang)
								{ *this = rotate (p, v, ang); }
			void selfRotate (const Vector& v, bdata ang)
								{ selfRotate (_referencePosition, v, ang); }

			void drawOn (Screen* scr, const Position& pos, int a = 255)
								{ for (auto i : _faces) i.drawOn (scr, _referencePosition + pos, a); }

			protected:
			int _id;
			Faces _faces;
			Position _referencePosition; // Always relative position from Positions::_cero
		};

		using Members = std::vector <Member>;

		class Articulation;
		using Articulations = std::map <int, Articulation>;

		/** Used as an anchor for members.
			Members' positions have nothing to do with Articulations' ones. 
			An articulation could have others pending from it, meaning that when one it is moved the rest go with it. */
		class Articulation
		{
			public:
			Articulation () = delete;

			Articulation (int id, const Position& pr, const Vector&n, const Articulations& aL, const Members& mL)
				: _id (id), _relativePosition (pr), _orientation (n), 
				  _articulationsLinked (aL), 
				  _membersLinked (mL)
							{ }

			Articulation (const Articulation& a) = default;

			Articulation& operator = (const Articulation& a) = default;

			int id () const
							{ return (_id); }
			const Position& relativePosition () const
							{ return (_relativePosition); }
			const Vector& orientation () const
							{ return (_orientation); }
			const Articulations& articulationsLinked () const
							{ return (_articulationsLinked); }
			const Members& membersLinked () const
							{ return (_membersLinked); }

			bool existsSubArticulation (int id) const
							{ return (_articulationsLinked.find (id) != _articulationsLinked.end ()); }
			const Articulation& subArticulation (int id) const
							{ return ((*_articulationsLinked.find (id)).second); }

			/** To get the box containing the articulation. */
			Box containingBox () const;

			// The rotation is what an articulation dows actually!
			Articulation rotate (const Position& p, const Vector& v, bdata ang) const;
			void selfRotate (const Position& p, const Vector& v, bdata ang)
							{ *this = rotate (p, v, ang); }
			void selfRotate (int id, const Position& p, const Vector& v, bdata ang);
			void selfRotate (const std::string& pId, const Position& p, const Vector& v, bdata ang); // Recursive...
			Articulation rotate (bdata ang) const
							{ return (rotate (_relativePosition, _orientation, ang)); }
			void selfRotate (bdata ang)
							{ selfRotate (_relativePosition, _orientation, ang); }
			void selfRotate (int id, bdata ang);
			void selfRotate (const std::string& pId, bdata ang); // Recursive...

			void drawOn (Screen* scr, const Position& pos, int a = 255);

			protected:
			int _id;
			Position _relativePosition;
			Vector _orientation;
			Articulations _articulationsLinked;
			Members _membersLinked;
		};

		/** Info about somtehing to turn in a specific articulation. 
			This info will be later used to determine the aspect of a frame within the form. */
		struct ArticulationTurnInfo
		{
			ArticulationTurnInfo () = delete;

			ArticulationTurnInfo (const std::string& pId, bdata a)
				: _articulationIdPath (pId), 
				  _angleToTurn (a)
							{ }

			ArticulationTurnInfo (const ArticulationTurnInfo&) = default;

			ArticulationTurnInfo& operator = (const ArticulationTurnInfo&) = default;

			std::string _articulationIdPath; // List of articulations' id separated by comma.
			bdata _angleToTurn; // Angle to turn. The basic situation is 0.
		};

		using ArticulationTurnInfoList = std::vector <ArticulationTurnInfo>;

		/** Info about a frame. */
		struct FrameInfo
		{
			FrameInfo () = delete;

			FrameInfo (int id, const ArticulationTurnInfoList& tL)
				: _id (id), _turns (tL)
							{ }

			FrameInfo (const FrameInfo&) = default;

			FrameInfo& operator = (const FrameInfo&) = default;

			int _id;
			ArticulationTurnInfoList _turns;
		};

		using FramesInfo = std::vector <FrameInfo>; // In order as frames comes...

		Form3DData (const Articulations& sk, const FramesInfo& fI);

		Articulations _skeleton; // Original one...
		FramesInfo _framesInfo; // How the diferent articulations should turn to get every frame
	};

	class Form3D;

	/** The 3D frame, consist on taking the original skeleton y moving a little bit the articulations requested. */
	class Form3DFrame : public Frame
	{
		public:
		Form3DFrame (Form* f, int nf, const Form3DData::Articulations& sk);

		/** Info about the frame. Used from the form actually. */
		int width () const
							{ return ((int) _frameLimits.width ()); }
		int height () const
							{ return ((int) _frameLimits.height ()); }
		int depth () const
							{ return ((int) _frameLimits.depth ()); }
		Position centerPosition () const
							{ return (_frameLimits.center ()); }
		Position basePosition () const
							{ return (_frameLimits.bottomRectangle ().center ()); }
		Rectangle roundingCentralZone () const
							{ return (_frameLimits.betweenFrontAndBackRectangle ()); }
		Rectangle coverZone () const
							{ return (_frameLimits.topRectangle ()); }
		Rectangle baseZone () const
							{ return (_frameLimits.bottomRectangle ()); }
		Box roundingBox () const
							{ return (_frameLimits); }

		/** @see parent. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint, int a = 255) override;

		protected:
		Form3DData::Articulations _skeleton;

		// Implementation
		Box _frameLimits;
	};

	/** The 3D Form. 
		The 3D Form is made up of members moved by articulations. */
	class Form3D : public Form
	{
		public:
		Form3D (int id, const std::string& n, int nf, Form3DData* d)
			: Form (id, n, nf),
			  _data (d),
			  _maxContainingBox ()
							{ assert (_data); }

		/** @see parent. 
			Rally these expressions make no sense in this type of forms, 
			But the calculate the limits of the box containing any type of frame. */
		virtual int width () const override
							{ return ((int) maxContainingBox ().width ()); }
		virtual int height () const override
							{ return ((int) maxContainingBox ().height ()); }
		virtual int depth () const override
							{ return ((int) maxContainingBox ().depth ()); }

		/** @se parent. */
		virtual int frameWidth () const override
							{ return (width ()); }
		virtual int frameHeight () const override
							{ return (height ()); }
		virtual int frameDepth () const override
							{ return (depth ()); }
		virtual int frameWidthForFrame (int nF) const override
							{ return (frame3D (nF) -> width ()); }
		virtual int frameHeightForFrame (int nF) const override
							{ return (frame3D (nF) -> height ()); }
		virtual int frameDepthForFrame (int nF) const override
							{ return (frame3D (nF) -> depth ()); }
		virtual int frameXOffset () const override
							{ return (0); }
		virtual int frameYOffset () const override
							{ return (0); }
		virtual int frameZOffset () const override
							{ return (0); }

		/** @see parent. */
		virtual Position centerPosition (int nF) const override
							{ return ( frame3D (nF) -> centerPosition ()); }
		virtual Position basePosition (int nF) const override
							{ return ( frame3D (nF) -> basePosition ()); }
		virtual Rectangle roundingCentralZone (int nF) const override
							{ return ( frame3D (nF) -> roundingCentralZone ()); }
		virtual Rectangle coverZone (int nF) const override
							{ return ( frame3D (nF) -> coverZone ()); }
		virtual Rectangle baseZone (int nF) const override
							{ return ( frame3D (nF) -> baseZone ()); }
		virtual Box roundingBox (int nF) const override
							{ return ( frame3D (nF) -> roundingBox ()); }

		protected:
		/** @see parent. */
		virtual Frame* createFrame (int nf) override;

		// Implementation
		const Form3DFrame* frame3D (int nF) const
							{ return (dynamic_cast <const Form3DFrame*> (_frames [nF]));  }
		Form3DFrame* frame3D (int nF)
							{ return (dynamic_cast <Form3DFrame*> (_frames [nF]));  }
		Box maxContainingBox () const;

		protected:
		Form3DData* _data;

		// Implementation
		mutable Box _maxContainingBox;
	};
}

#endif
  
// End of the file
/*@}*/
