#ifndef CAUDIOGENERATOR_H
#define CAUDIOGENERATOR_H

#include <purrfx/PurrFX.h>

class CGenerator: public PurrFX::CFrameDataProducer
{
public:
	CGenerator();

	void playNote(const PurrFX::CNote& i_rNote);
	void stopNote();

	void increaseDuty();
	void decreaseDuty();

	// PurrFX::CFrameDataProducer implementation
	virtual bool               available() const;
	virtual PurrFX::CFrameData get() const;
	virtual bool               next();

private:
	void update();

	enum class EState { None, Attack, Sustain, Release };

	uint8_t  m_nDuty  = 0;
	uint16_t m_nTimer = 0;

	EState             m_eState = EState::None;
	size_t             m_nFrame = 0;
	PurrFX::CFrameData m_oFrameData;
	PurrFX::CNesState  m_oNesState;
};

#endif // CAUDIOGENERATOR_H
