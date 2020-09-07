#include "CFrameDataProxy.h"

void CFrameDataProxy::start(PurrFX::CFrameDataProducerRandomAccess* i_pSrcProducer)
{
	assert(i_pSrcProducer != nullptr);
	assert(i_pSrcProducer->size() > 0);
	m_pSrcProducer = i_pSrcProducer;
	m_nFrameIndex = 0; // Most likely there is init code
}

bool CFrameDataProxy::available() const
{
	assert(m_pSrcProducer != nullptr);
	return true;
}

PurrFX::CFrameData CFrameDataProxy::get() const
{
	assert(m_pSrcProducer != nullptr);
	return m_pSrcProducer->getAt(m_nFrameIndex);
}

bool CFrameDataProxy::next()
{
	assert(m_pSrcProducer != nullptr);
	m_nFrameIndex--;
	if (m_nFrameIndex == -1)
		m_nFrameIndex = m_pSrcProducer->size()-1;
	return true;
}
