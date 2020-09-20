#ifndef CMIDIFILE_H
#define CMIDIFILE_H

#include <vector>
#include <cassert>
#include <cstdint>
#include "purrfx/CBufferedFileWriter.h"
#include "purrfx/DClass.h"

class CMidiFile final
{
	CLASS_DISABLE_DEFAULT_CONSTRUCTOR(CMidiFile)
	CLASS_MAKE_NON_COPYABLE(CMidiFile)
public:
	CMidiFile(const PurrFX::pathstring& i_sFileName, uint16_t i_nNumberOfTracks, uint16_t i_nMusicSpeed = 0x18);
	~CMidiFile();

	bool isOpened() const;

	void addKeyPressEvent(size_t i_nTrackIndex, int i_nOctave, int i_nNote, float i_nVolume);
	void addKeyReleaseEvent(size_t i_nTrackIndex);
	void tick();

private:
	void writeHeader(uint16_t i_nNumberOfTracks, uint16_t i_nMusicSpeed);
	void writeTrack(size_t i_nTrackIndex);

	uint32_t timestamp(uint32_t i_nTicks) const;      // Converting to variable length form
	uint8_t  pitch(int i_nOctave, int i_nNote) const; // Converting to 00 to 7F range (C-1 to G9)
	uint8_t  volume(float i_nVolume);                 // Converting to 00 to 7F range

	PurrFX::CBufferedFileWriter m_oFile;
	
	struct CTrackState
	{
		uint32_t nTick;
		bool     bPressed = false;
		uint8_t  nPitch   = 0xFF;
	};
	std::vector<CTrackState>          m_aState;
	std::vector<std::vector<uint8_t>> m_aData;
};

#endif // CMIDIFILE_H
