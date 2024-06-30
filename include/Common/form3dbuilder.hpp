/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: form3dbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 08/02/201 \n
 *	Description: Defines a class to create forms 3D from a file. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_FORM3DBUILDER__
#define __QGAMES_FORM3DBUILDER__

#include <Common/formbuilder.hpp>

namespace QGAMES
{
	class Sprite2DBuilder;

	/** The builder for the 3D Forms. */
	class Form3DBuilder : public FormBuilder
	{
		public:
		/** 
		  *	A form is made up of many articulations having members linked each and associated to other articulations too.
		  *	A member is made up of faces, and the faces ocuppied a position in space (relative).
		  * A reference to an sprite 2B Buildeer is needed to create the graphics used e.g to draw faces.
		  *	The definition of the file should be as follows:
		  *	<?xml version="1.0"?> \n
		  *	<Forms> \n
		  *		<Include file="CCCCC"/> \n
		  *		<Form id="X" name="XXX" file=".\XXXX" nFrames="X"/> \n
		  *			<Articulation id="XX" position="XX,YY,ZZ" orientation="XX,YY,ZZ"> \n
		  *				<Articulation id="XX" position="XX,YY,ZZ" orientation="XX,YY,ZZ"> \n
		  *					<Member id="XX" position="XX,YY,ZZ"> \n
		  *						<Face id="XX" color="RR,GG,BB"> \n
		  *							<Position value="XX,YY,ZZ"/> \n
		  *							<Position value="XX,YY,ZZ"/> \n // Three minimum...
		  *							... \n
		  *						</Face> \n
		  *					</Member id="XX"> \n
		  *					... \n
		  *				</Articulation> \n
		  *				... \n
		  *				<Member id="XX"> \n
		  *					... \n
		  *				</Member>
		  *				... \n
		  *			</Articulation> \n
		  *			... \n
		  *			<Frame id="VV"> \n
		  *				<Turn id="XX,XX,..." angle="YY"/> \n
		  *				...
		  *			</Frame> \n
		  *			... \n
		  *		</Form> \n
		  *		...
		  *	</Forms> \n
		  */
		Form3DBuilder (const std::string& fDef, Sprite2DBuilder* gB); 

		protected:
		struct Form3DDefinition : public FormBuilder::FormDefinition
		{
			public:
			struct PositionDefinition
			{
			};

			struct FaceDefinition
			{
				FaceDefinition ()
					: _id (-1), _relativePositions (), _color (Color (255, 255, 255, 255))
							{ }

				FaceDefinition (int id, const Positions& pos, const Color& c)
					: _id (id), _relativePositions (pos), _color (c)
							{ }

				FaceDefinition (const FaceDefinition&) = default;

				FaceDefinition& operator = (const FaceDefinition&) = default;

				int _id;
				Positions _relativePositions;
				Color _color;
			};

			typedef std::vector <FaceDefinition> FaceDefinitions;

			struct MemberDefinition
			{
				MemberDefinition ()
					: _id (-1), _faces (), _relativePosition (Position::_cero)
							{ }

				MemberDefinition (int id, const FaceDefinitions& fD, const Position& pos)
					: _id (id), _faces (fD), _relativePosition (pos)
							{ }

				MemberDefinition (const MemberDefinition&) = default;

				MemberDefinition& operator = (const MemberDefinition&) = default;

				int _id;
				FaceDefinitions _faces;
				Position _relativePosition;
			};

			typedef std::vector <MemberDefinition> MemberDefinitions;

			struct ArticulationDefinition;
			typedef std::map <int, ArticulationDefinition> ArticulationDefinitions;

			struct ArticulationDefinition
			{
				ArticulationDefinition ()
					: _id (-1), _position (Position::_noPoint), _orientation (Vector::_noPoint), 
					  _members (), _articulations ()
							{ }

				ArticulationDefinition (int id, const Position& pos, const Vector& o, 
					const MemberDefinitions& mD, const ArticulationDefinitions& aD)
					: _id (id), _position (pos), _orientation (o), 
					  _members (mD), _articulations (aD)
							{ }

				ArticulationDefinition (const ArticulationDefinition&) = default;

				ArticulationDefinition& operator = (const ArticulationDefinition&) = default;
				
				int _id;
				Position _position;
				Vector _orientation;
				MemberDefinitions _members;
				ArticulationDefinitions _articulations;
			};

			struct ArticulationTurnDefinition
			{
				ArticulationTurnDefinition ()
					: _pathId (std::string (__NULL_STRING__)), _angle ((bdata) 0)
							{ }

				ArticulationTurnDefinition (const std::string& pId, bdata a)
					: _pathId (pId), _angle (a)
							{ }

				ArticulationTurnDefinition (const ArticulationTurnDefinition&) = default;

				ArticulationTurnDefinition& operator = (const ArticulationTurnDefinition&) = default;

				std::string _pathId;
				bdata _angle;
			};

			typedef std::vector <ArticulationTurnDefinition> ArticulationTurnDefinitions;
			typedef std::vector <ArticulationTurnDefinitions> FrameTurnsDefinition;

			struct FrameDefinition
			{
				FrameDefinition ()
					: _id (-1), _turns ()
							{ }

				FrameDefinition (int id, const ArticulationTurnDefinitions& t)
					: _id (id), _turns (t)
							{ }

				FrameDefinition (const FrameDefinition&) = default;

				FrameDefinition& operator = (const FrameDefinition&) = default;

				int _id;
				ArticulationTurnDefinitions _turns;
			};

			typedef std::vector <FrameDefinition> FrameDefinitions;

			Form3DDefinition ()
				: FormBuilder::FormDefinition (),
				  _articulations (), _frames ()
							{ }

			Form3DDefinition (int id, const std::string& n, const std::string& p, int nF,
				const ArticulationDefinitions& a, const FrameDefinitions& f)
				: FormBuilder::FormDefinition (id, n, p, nF),
				  _articulations (a), _frames (f)
							{ }

			ArticulationDefinitions _articulations;
			FrameDefinitions _frames;
		};

		/** @see parent. */
		virtual Form* createForm (FormDefinition* def) override;

		void readFile (const std::string& fDef);

		Form3DDefinition* readFormDefinition (TiXmlElement* e);

		Form3DDefinition::ArticulationDefinition readArticulationDefinition (TiXmlElement* e);
		Form3DDefinition::MemberDefinition readMemberDefinition (TiXmlElement* e);
		Form3DDefinition::FaceDefinition readFaceDefinition (TiXmlElement* e);
		Form3DDefinition::FrameDefinition readFrameDefinition (TiXmlElement* e);
		Form3DDefinition::ArticulationTurnDefinition readArticulationTurnDefinition (TiXmlElement* e);

		// Used later when the specific form has to be crated...
		Form3DData* createForm3DDdataFromForm3DDefinition (Form3DDefinition* d);

		protected:
		Sprite2DBuilder* _sprite2DBuilder;
	};
}

#endif
  
// End of the file
/*@}*/
