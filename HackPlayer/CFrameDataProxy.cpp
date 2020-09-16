#include "CFrameDataProxy.h"

void CFrameDataProxy::start(PurrFX::CFrameDataProducerRandomAccess* i_pSrcProducer)
{
	assert(i_pSrcProducer != nullptr);
	assert(i_pSrcProducer->size() > 0);
	m_pSrcProducer = i_pSrcProducer;
	m_nFrameIndex = 0;
}

void CFrameDataProxy::setPulse1DutyMode(EDutyMode i_eMode) { m_aMode[0] = i_eMode; }
void CFrameDataProxy::setPulse2DutyMode(EDutyMode i_eMode) { m_aMode[1] = i_eMode; }
void CFrameDataProxy::setPulse1DutyValue(uint8_t i_nValue) { m_aValue[0] = i_nValue; }
void CFrameDataProxy::setPulse2DutyValue(uint8_t i_nValue) { m_aValue[1] = i_nValue; }

bool CFrameDataProxy::available() const
{
	assert(m_pSrcProducer != nullptr);
	return true;
}

PurrFX::CFrameData CFrameDataProxy::get() const
{
	assert(m_pSrcProducer != nullptr);
	PurrFX::CFrameData oData = m_pSrcProducer->getAt(m_nFrameIndex);

	if (m_aMode[0] != EDutyMode::Untouched)
	{
		if (oData.isSet(PurrFX::ERegister::Apu4000))
		{
			auto oRegister = oData.getRegister(PurrFX::ERegister::Apu4000);
			if (m_aMode[0] == EDutyMode::Sequence)
				oRegister.set('D', uint8_t((m_nFrameIndex/2)%4));
			else
				oRegister.set('D', m_aValue[0]);
			oData.set(oRegister);
		}
	}
	if (m_aMode[1] != EDutyMode::Untouched)
	{
		if (oData.isSet(PurrFX::ERegister::Apu4004))
		{
			auto oRegister = oData.getRegister(PurrFX::ERegister::Apu4004);
			if (m_aMode[1] == EDutyMode::Sequence)
				oRegister.set('D', uint8_t((m_nFrameIndex/2)%4));
			else
				oRegister.set('D', m_aValue[1]);
			oData.set(oRegister);
		}
	}

	return oData;
}

bool CFrameDataProxy::next()
{
	assert(m_pSrcProducer != nullptr);
	m_nFrameIndex++;
	if (m_nFrameIndex == m_pSrcProducer->size())
		m_nFrameIndex  = 0;
	return true;
}
