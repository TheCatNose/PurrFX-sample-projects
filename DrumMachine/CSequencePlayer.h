#ifndef CSEQUENCEPLAYER_H
#define CSEQUENCEPLAYER_H

#include <string>
#include "PurrFX.h"
#include "CDpcmDataFileReader.h"

class CSequencePlayer: public PurrFX::CFrameDataProducer
{
public:
	static const size_t DefaultFps = 10;

	CSequencePlayer(PurrFX::CDpcmDataFileReader& i_rDpcmProvider);

	void setSequence(const std::string& i_rsSequence);
	void setSpeed(size_t i_nFramesPerStep);
	void restart();

	// CFrameDataProducer implementation
	virtual bool               available() const;
	virtual PurrFX::CFrameData get() const;
	virtual bool               next();

private:
	size_t currentStep() const;

	PurrFX::CDpcmDataFileReader& m_rDpcmProvider;

	std::string m_sSequence;
	size_t      m_nFrame = 0;
	size_t      m_nFramesPerStep = DefaultFps;
};

#endif // CSEQUENCEPLAYER_H
