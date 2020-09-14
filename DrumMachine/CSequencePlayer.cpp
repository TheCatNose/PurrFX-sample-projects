#include "CSequencePlayer.h"

CSequencePlayer::CSequencePlayer(PurrFX::CDpcmDataFileReader& i_rDpcmProvider):
	m_rDpcmProvider(i_rDpcmProvider)
{
}

void CSequencePlayer::setSequence(const std::string& i_rsSequence)
{
	m_sSequence = i_rsSequence;
}

void CSequencePlayer::setSpeed(size_t i_nFramesPerStep)
{
	size_t nStep = currentStep();
	m_nFramesPerStep = i_nFramesPerStep;
	m_nFrame = nStep+i_nFramesPerStep;
}

void CSequencePlayer::restart()
{
	m_nFrame = 0;
}

bool CSequencePlayer::available() const
{
	return (m_nFrame % m_nFramesPerStep == 0);
}

PurrFX::CFrameData CSequencePlayer::get() const
{
	const size_t nStep = currentStep();
	char cSampleIndex = m_sSequence[nStep];
	if (cSampleIndex < '1' &&
		cSampleIndex > '9')
		return PurrFX::CFrameData();

	size_t nSampleIndex = size_t(cSampleIndex-'1');
	if (nSampleIndex >= m_rDpcmProvider.size())
		return PurrFX::CFrameData();

	m_rDpcmProvider.setCurrentIndex(nSampleIndex);
	auto* pSample = m_rDpcmProvider.getSample();

	PurrFX::CFrameData oFrameData;
	oFrameData.set(PurrFX::ERegister::Apu4010, 0x0F);
	oFrameData.set(PurrFX::ERegister::Apu4011, 0x00);
	oFrameData.set(PurrFX::ERegister::Apu4012, 0x00);
	oFrameData.set(PurrFX::ERegister::Apu4013, pSample->dpcmLength());
	return oFrameData;
}

bool CSequencePlayer::next()
{
	m_nFrame++;
	if (currentStep() >= m_sSequence.size())
		m_nFrame = 0;
	return true;
}

size_t CSequencePlayer::currentStep() const
{
	return m_nFrame/m_nFramesPerStep;
}
