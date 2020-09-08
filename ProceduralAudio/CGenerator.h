#ifndef CAUDIOGENERATOR_H
#define CAUDIOGENERATOR_H

#include <random>
#include <CFrameDataProducer.h>

class CGenerator: public PurrFX::CFrameDataProducer
{
public:
	enum EMode
	{
		None,
		Value,
		Random,
		Sin,
		Cos,
		Saw,
	};

	CGenerator();
	void randomize();
	void setAllToNone();

	void start();
	void setMode (PurrFX::ERegister i_eRegister, EMode i_eMode);
	void setValue(PurrFX::ERegister i_eRegister, uint8_t i_nValue);

	// PurrFX::CFrameDataProducer implementation
	virtual bool               available() const;
	virtual PurrFX::CFrameData get() const;
	virtual bool               next();

private:
	int m_nFrame = 0;
	EMode   m_aMode [PurrFX::RegisterCount];
	uint8_t m_aValue[PurrFX::RegisterCount];
};

#endif // CAUDIOGENERATOR_H
