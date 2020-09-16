#ifndef CFRAMEDATAPROXY_H
#define CFRAMEDATAPROXY_H

#include <cassert>
#include <purrfx/CFrameDataProducer.h>
#include <purrfx/CFrameDataProducerRandomAccess.h>
#include <purrfx/CRegister.h>
#include "EDutyMode.h"

class CFrameDataProxy: public PurrFX::CFrameDataProducer
{
public:
	void start(PurrFX::CFrameDataProducerRandomAccess* i_pSrcProducer);

	void setPulse1DutyMode(EDutyMode i_eMode);
	void setPulse2DutyMode(EDutyMode i_eMode);
	void setPulse1DutyValue(uint8_t i_nValue);
	void setPulse2DutyValue(uint8_t i_nValue);

	// CFrameDataProducer implementation
	virtual bool               available() const;
	virtual PurrFX::CFrameData get() const;
	virtual bool               next();

private:
	PurrFX::CFrameDataProducerRandomAccess* m_pSrcProducer = nullptr;
	size_t m_nFrameIndex = 0;

	EDutyMode m_aMode[2]  = {EDutyMode::Sequence, EDutyMode::Sequence};
	uint8_t   m_aValue[2] = {0,0};
};

#endif // CFRAMEDATAPROXY_H
