#ifndef CFRAMEDATAPROXY_H
#define CFRAMEDATAPROXY_H

#include <cassert>
#include <CFrameDataProducer.h>
#include <CFrameDataProducerRandomAccess.h>

class CFrameDataProxy: public PurrFX::CFrameDataProducer
{
public:
	void start(PurrFX::CFrameDataProducerRandomAccess* i_pSrcProducer);

	// CFrameDataProducer implementation
	virtual bool               available() const;
	virtual PurrFX::CFrameData get() const;
	virtual bool               next();

private:
	PurrFX::CFrameDataProducerRandomAccess* m_pSrcProducer = nullptr;
	PurrFX::CFrameData m_oInitFd;
	int m_nFrameIndex = 0;
};

#endif // CFRAMEDATAPROXY_H
