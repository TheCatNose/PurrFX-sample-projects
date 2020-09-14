#include <CAudioApp.h>
#include <purrfx/PurrFX.h>
#include "ReversePlayer/CFrameDataProxy.h"

class CReversePlayerApp: public CAudioApp
{
public:
	~CReversePlayerApp();
	virtual bool init();
private:
	virtual void showUsage();
	virtual void onCommand(const std::string& i_sCommand, bool& o_bQuit);
	virtual void produceAudioData(char* i_pData, size_t i_nSize);

	void setTrack(int i_nTrackIndex);
	void deleteDataBuffers();
	void prepareData();

	PurrFX::CNesPtr m_oNesSrc;
	PurrFX::CFrameDataBuffer* m_pFdBuffer   = nullptr;
	PurrFX::CDpcmDataBuffer*  m_pDpcmBuffer = nullptr;
	CFrameDataProxy m_oProxy;
	PurrFX::CNesPtr m_oNes;
	int m_nTrack = 0;
};

CReversePlayerApp::~CReversePlayerApp()
{
	deleteDataBuffers();
}

bool CReversePlayerApp::init()
{
	std::cout << "Enter NSF file name: ";
	PurrFX::pathstring sFileName;
	std::getline(PATH_CIN, sFileName);

	if (!m_oNesSrc->open(sFileName))
	{
		std::cout << "Can't open NSF file!";
		return false;
	}

	if (!m_oNesSrc->setTrack(0))
	{
		std::cout << "Can't set track!";
		return false;
	}
	prepareData();

	m_oNes->open();
	m_oNes->setTrack(0);

	return true;
}

void CReversePlayerApp::showUsage()
{
	std::cout <<
		"Enter 'p' to play previous track" << std::endl <<
		"Enter 'n' to play next track" << std::endl <<
		"Enter 'q' to quit" << std::endl <<
		"--------------------------------\n";
}

void CReversePlayerApp::onCommand(const std::string& i_sCommand, bool& o_bQuit)
{
	if (i_sCommand == "q")
		o_bQuit = true;
	else if (i_sCommand == "p")
		setTrack(m_nTrack-1);
	else if (i_sCommand == "n")
		setTrack(m_nTrack+1);
}

void CReversePlayerApp::produceAudioData(char* i_pData, size_t i_nSize)
{
	m_oNes->render(i_pData, i_nSize);
}

void CReversePlayerApp::setTrack(int i_nTrackIndex)
{
	m_nTrack = i_nTrackIndex;
	prepareData();

	std::cout << "Track " << (m_nTrack+1) << "/" << m_oNes->trackCount() << std::endl;
}

void CReversePlayerApp::deleteDataBuffers()
{
	if (m_pFdBuffer   != nullptr)
		delete m_pFdBuffer;
	if (m_pDpcmBuffer != nullptr)
		delete m_pDpcmBuffer;
	m_pFdBuffer   = nullptr;
	m_pDpcmBuffer = nullptr;
}

void CReversePlayerApp::prepareData()
{
	m_oNesSrc->detachAll();
	deleteDataBuffers();
	const int nTime = 60;
	m_pFdBuffer   = new PurrFX::CFrameDataBuffer( nTime*60 );
	m_pDpcmBuffer = new PurrFX::CDpcmDataBuffer(true);
	m_oNesSrc->setFrameDataConsumer(m_pFdBuffer);
	m_oNesSrc->setDpcmDataConsumer (m_pDpcmBuffer);
	m_oNesSrc->setTrack( m_nTrack );
	m_oNesSrc->render();

	m_oProxy.start(m_pFdBuffer);
	m_oNes->setFrameDataProducer(&m_oProxy);
	m_oNes->setDpcmDataProvider (m_pDpcmBuffer);
	m_oNes->setTrack(0);
}



int main(int, char**)
{
	CReversePlayerApp oApp;
	return oApp.start();
}
