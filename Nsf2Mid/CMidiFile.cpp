#include "CMidiFile.h"

CMidiFile::CMidiFile(const PurrFX::pathstring& i_sFileName, uint16_t i_nNumberOfTracks, uint16_t i_nMusicSpeed):
	m_oFile(i_sFileName)
{
	assert(i_nMusicSpeed > 0);
	assert(i_nNumberOfTracks > 0);

	if (!isOpened())
		return;

	m_aData.resize(i_nNumberOfTracks);
	for (size_t i = 0; i < i_nNumberOfTracks; i++)
		m_aData[i].reserve(32*1024);
	m_aState.resize(i_nNumberOfTracks);

	writeHeader(i_nNumberOfTracks, i_nMusicSpeed);
}

CMidiFile::~CMidiFile()
{
	if (!isOpened())
		return;

	for (size_t i = 0; i < m_aData.size(); i++)
		writeTrack(i);
}

bool CMidiFile::isOpened() const
{
	return m_oFile.isOpened();
}

void CMidiFile::addKeyPressEvent(size_t i_nTrackIndex, int i_nOctave, int i_nNote, float i_nVolume)
{
	assert(isOpened());
	assert(i_nTrackIndex < m_aData .size());
	assert(i_nTrackIndex < m_aState.size());
	std::vector<uint8_t>& rData  = m_aData [i_nTrackIndex];
	CTrackState&          rState = m_aState[i_nTrackIndex];

	if (rState.bPressed)
		addKeyReleaseEvent(i_nTrackIndex);
	rState.bPressed = true;

	//////////////////
	// Prepare data //
	//////////////////

	uint32_t nTimeStamp = timestamp(rState.nTick);
	uint8_t  nEventType = 0x90; // 9 is "note on" event, 0 = channel #0
	uint8_t  nPitch     = pitch(i_nOctave, i_nNote);
	uint8_t  nVolume    = volume(i_nVolume);

	if (nPitch == rState.nPitch)
		return;
	rState.nPitch = nPitch;

	////////////////
	// Write data //
	////////////////

	while (true)
	{
		rData.push_back(uint8_t(nTimeStamp));
		if ( (nTimeStamp & 0x80) != 0 )
			nTimeStamp >>= 8;
		else
			break;
	}

	rData.push_back(nEventType);
	rData.push_back(nPitch);
	rData.push_back(nVolume);


	rState.nTick = 0;
}

void CMidiFile::addKeyReleaseEvent(size_t i_nTrackIndex)
{
	assert(isOpened());
	assert(i_nTrackIndex < m_aData .size());
	assert(i_nTrackIndex < m_aState.size());
	std::vector<uint8_t>& rData  = m_aData [i_nTrackIndex];
	CTrackState&          rState = m_aState[i_nTrackIndex];

	if (!rState.bPressed)
		return;
	rState.bPressed = false;

	//////////////////
	// Prepare data //
	//////////////////

	uint32_t nTimeStamp = timestamp(rState.nTick);
	uint8_t  nEventType = 0x80; // 8 is "note off" event, 0 = channel #0
	uint8_t  nPitch     = 0x00; // Not important
	uint8_t  nVolume    = 0x00; // Not important

	////////////////
	// Write data //
	////////////////

	while (true)
	{
		rData.push_back(uint8_t(nTimeStamp));
		if ( (nTimeStamp & 0x80) != 0 )
			nTimeStamp >>= 8;
		else
			break;
	}

	rData.push_back(nEventType);
	rData.push_back(nPitch);
	rData.push_back(nVolume);


	rState.nTick = 0;
}

void CMidiFile::tick()
{
	assert(isOpened());

	for (size_t i = 0; i < m_aState.size(); i++)
		m_aState[i].nTick++;
}

void CMidiFile::writeHeader(uint16_t i_nNumberOfTracks, uint16_t i_nMusicSpeed)
{
	const uint32_t nHeaderSize = 0x06;
	const uint16_t nSubformat  = 1; // Type-1

	m_oFile.write("MThd", 4);
	m_oFile.write(&nHeaderSize      , sizeof(nHeaderSize      ), PurrFX::EByteOrder::BigEndian);
	m_oFile.write(&nSubformat       , sizeof(nSubformat       ), PurrFX::EByteOrder::BigEndian);
	m_oFile.write(&i_nNumberOfTracks, sizeof(i_nNumberOfTracks), PurrFX::EByteOrder::BigEndian);
	m_oFile.write(&i_nMusicSpeed    , sizeof(i_nMusicSpeed    ), PurrFX::EByteOrder::BigEndian);
}

void CMidiFile::writeTrack(size_t i_nTrackIndex)
{
	const std::vector<uint8_t>& rData = m_aData[i_nTrackIndex];

	// Track header
	const uint8_t  aTrackEnding[] = { 0x00, 0xFF, 0x2F, 0x00 };
	const uint32_t nTrackDataSize = rData.size() + sizeof(aTrackEnding);
	m_oFile.write("MTrk", 4);
	m_oFile.write(&nTrackDataSize, sizeof(nTrackDataSize), PurrFX::EByteOrder::BigEndian);

	// Track data
	m_oFile.write(rData.data(), rData.size());

	// Track ending
	m_oFile.write(aTrackEnding, sizeof(aTrackEnding));
}

uint32_t CMidiFile::timestamp(uint32_t i_nTicks) const
{
	uint32_t nTimestamp;
	nTimestamp = i_nTicks & 0x7F;

	while ( (i_nTicks >>= 7) )
	{
		nTimestamp <<= 8;
		nTimestamp |= ((i_nTicks & 0x7F) | 0x80);
	}

	return nTimestamp;
}

uint8_t CMidiFile::pitch(int i_nOctave, int i_nNote) const
{
	if (i_nOctave < -1)
		i_nOctave = -1;
	if (i_nOctave >  9)
		i_nOctave =  9;

	if (i_nNote < 0)
		i_nNote = 0;
	if (i_nNote > 11)
		i_nNote = 11;

	if (i_nOctave == 9 &&
		i_nNote   >  7) // Last possible note is G9
		i_nNote   =  7;

	int nPitch = (i_nOctave+1)*12 + i_nNote;
	assert(nPitch >= 0 && nPitch <= 0x7F);
	return uint8_t(nPitch);
}

uint8_t CMidiFile::volume(float i_nVolume)
{
	if (i_nVolume < 0.0f)
		i_nVolume = 0.0f;
	if (i_nVolume > 1.0f)
		i_nVolume = 1.0f;

	return uint8_t(i_nVolume*0x7F);
}
