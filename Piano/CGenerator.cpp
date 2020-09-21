#include "CGenerator.h"

CGenerator::CGenerator()
{
}

void CGenerator::playNote(const PurrFX::CNote& i_rNote)
{
	m_nTimer = PurrFX::CNesCalculations::apuPulseTimer(i_rNote.frequency());
	m_eState = EState::Attack;
	m_nFrame = 0;
	update();
}

void CGenerator::stopNote()
{
	m_eState = EState::Release;
	m_nFrame = 0;
	update();
}

void CGenerator::increaseDuty()
{
	if (m_nDuty < 3)
		m_nDuty++;
}

void CGenerator::decreaseDuty()
{
	if (m_nDuty > 0)
		m_nDuty--;
}

bool CGenerator::available() const
{
	return true;
}

PurrFX::CFrameData CGenerator::get() const
{
	return m_oFrameData;
}

bool CGenerator::next()
{
	switch(m_eState)
	{
	case EState::None:
		return false;
	case EState::Attack:
		if (++m_nFrame == 3)
		{
			m_eState = EState::Sustain;
			m_nFrame = 0;
		}
		break;
	case EState::Sustain:
		m_nFrame++;
		break;
	case EState::Release:
		if (++m_nFrame == 4)
		{
			m_eState = EState::None;
			m_nFrame = 0;
			return false;
		}
	}
	update();
	return true;
}

void CGenerator::update()
{
	m_oFrameData.reset();

	switch(m_eState)
	{
	case EState::None:
		break;
	case EState::Attack:
		{
			assert(m_nFrame < 3);
			uint8_t aVolume[3] = {4,8,12};
			PurrFX::CRegister oReg4000(PurrFX::ERegister::Apu4000);
			oReg4000.set('D', m_nDuty);
			oReg4000.set('L', 1);
			oReg4000.set('C', 1);
			oReg4000.set('V', aVolume[m_nFrame]);
			m_oFrameData.set(oReg4000);

			if (m_nFrame == 0)
			{
				uint8_t nReg4002, nReg4003;
				PurrFX::CNesCalculations::decomposeApuPulseTimer(nReg4002, nReg4003, m_nTimer);
				m_oFrameData.set(PurrFX::ERegister::Apu4002, nReg4002);
				m_oFrameData.set(PurrFX::ERegister::Apu4003, nReg4003);
			}
		}
		break;
	case EState::Sustain:
		{
			// Vibrato on each 5th frame
			const size_t nVibratoStartFrame = 12;
			if (m_nFrame >= nVibratoStartFrame &&
				m_nFrame % 5 == 0)
			{
				bool bAlterTimer = m_nFrame % 10 == 0;
				uint16_t nTimer = m_nTimer;
				if (bAlterTimer)
					nTimer += 2;

				uint8_t nReg4002, nReg4003;
				PurrFX::CNesCalculations::decomposeApuPulseTimer(nReg4002, nReg4003, nTimer);
				m_oFrameData.set(PurrFX::ERegister::Apu4002, nReg4002);
				if (m_oNesState.get(PurrFX::ERegister::Apu4003) != nReg4003)
					m_oFrameData.set(PurrFX::ERegister::Apu4003, nReg4003);
			}
		}
		break;
	case EState::Release:
		{
			assert(m_nFrame < 4);
			uint8_t aVolume[4] = {9,6,3,0};
			PurrFX::CRegister oReg4000(PurrFX::ERegister::Apu4000);
			oReg4000.set('D', m_nDuty);
			oReg4000.set('L', 1);
			oReg4000.set('C', 1);
			oReg4000.set('V', aVolume[m_nFrame]);
			m_oFrameData.set(oReg4000);

			if (m_nFrame == 0)
			{
				uint8_t nReg4002, nReg4003;
				PurrFX::CNesCalculations::decomposeApuPulseTimer(nReg4002, nReg4003, m_nTimer);
				m_oFrameData.set(PurrFX::ERegister::Apu4002, nReg4002);
				m_oFrameData.set(PurrFX::ERegister::Apu4003, nReg4003);
			}
		}
		break;
	}

	m_oNesState.update(m_oFrameData);
}
