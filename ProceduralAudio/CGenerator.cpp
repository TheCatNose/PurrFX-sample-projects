#include "CGenerator.h"

CGenerator::CGenerator()
{
	for (size_t i = 0; i < PurrFX::RegisterCount; i++)
	{
		m_aMode[i] = Random;
		m_aValue[i] = 0xFF;
	}
}

void CGenerator::randomize()
{
	static std::default_random_engine           oRandomEngine;
	static std::uniform_int_distribution<short> oModeDistribution(0,Saw);
	static std::uniform_int_distribution<short> oValueDistribution(0x00,0xFF);

	for (size_t i = 0; i < PurrFX::RegisterCount; i++)
	{
		m_aMode[i]  = EMode  (oModeDistribution (oRandomEngine));
		m_aValue[i] = uint8_t(oValueDistribution(oRandomEngine));
	}
}

void CGenerator::setAllToNone()
{
	for (size_t i = 0; i < PurrFX::RegisterCount; i++)
		m_aMode[i] = None;
}

void CGenerator::start()
{
	m_nFrame = 0;
}

void CGenerator::setMode(PurrFX::ERegister i_eRegister, CGenerator::EMode i_eMode)
{
	m_aMode[size_t(i_eRegister)] = i_eMode;
}

void CGenerator::setValue(PurrFX::ERegister i_eRegister, uint8_t i_nValue)
{
	m_aValue[size_t(i_eRegister)] = i_nValue;
}

bool CGenerator::available() const
{
	return true;
}

PurrFX::CFrameData CGenerator::get() const
{
	auto fRandom = []()->uint8_t {
		static std::default_random_engine           oRandomEngine;
		static std::uniform_int_distribution<short> oRandomDistribution(0x00,0xFF);
		return uint8_t( oRandomDistribution(oRandomEngine) );
	};

	PurrFX::CFrameData oData;
	for (size_t i = 0; i <= 13; i++)
	{
		PurrFX::ERegister eRegister = PurrFX::ERegister(i);
		switch(m_aMode[i])
		{
		case None:
			break;
		case Value:
			oData.set(eRegister, m_aValue[i]);
			break;
		case Random:
			oData.set(eRegister, fRandom());
			break;
		case Sin:
			oData.set(eRegister, uint8_t(sin(m_nFrame*0.01)*0xFF));
			break;
		case Cos:
			oData.set(eRegister, uint8_t(cos(m_nFrame*0.01)*0xFF));
			break;
		case Saw:
			oData.set(eRegister, uint8_t(m_nFrame%0xFF));
			break;
		}
	}

	return oData;
}

bool CGenerator::next()
{
	m_nFrame++;
	return true;
}
