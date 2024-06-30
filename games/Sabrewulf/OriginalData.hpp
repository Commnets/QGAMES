/** \addtogroup SabreWulf */
/*@{*/

#ifndef __ORIGINALDATA_HPP__
#define __ORIGINALDATA_HPP__

/** This class represents the original data.
	Original data has been extracted from web pages. */
class OriginalData
{
	public:
	static OriginalData _o;

	OriginalData ()
							{ }

	public:
	static int _amuletStartPositionAddress;
	static int _amuletStartPositionData [8][4];
	static int _mazeLayoutTypeAddress;
	static int _mazeLayoutTypeData [256];
	static int _mazeDefinitionAddress;
	static int _mazeDefinitionData [48];
	static int _mazeCompositionAddress;
	static int _mapCompositionData [1886];
	static int _grassBorderCompositionAddress;
	static int _grassBorderCompositionData [29];
	static int _objectPositionsAddress;
	static int _objectPositionsData [48][8];
	static int _orchildPositionAddress;
	static int _orchildPositionsData [48][2];
	static int _defaultInmortalsData [12][12];
	static int _orchildColours [16];
	static int _nastySpriteTypes [16];
	static int _objectColours [16];
};

#endif
  
// End of the file
/*@}*/