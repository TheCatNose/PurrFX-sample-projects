#include <CAudioApp.h>
#include <PurrFX.h>
#include "../ProceduralAudio/CGenerator.h"

class CReversePlayerApp: public CAudioApp
{
public:
	virtual bool init();
private:
	virtual void showUsage();
	virtual void onCommand(const std::string& i_sCommand, bool& o_bQuit);
	virtual void produceAudioData(char* i_pData, size_t i_nSize);

	PurrFX::CNesPtr m_oNes;
	CGenerator m_oGen;
};

bool CReversePlayerApp::init()
{
	m_oNes->setFrameDataProducer(&m_oGen);
	m_oNes->open();
	m_oNes->setTrack(0);
	return true;
}

void CReversePlayerApp::showUsage()
{
	std::cout <<
		"Enter 'r' to set random mode for all registers" << std::endl <<
		"Enter 'n' to set 'none' mode for all registers" << std::endl <<
		"Enter 'q' to quit" << std::endl <<
		"--------------------------------\n";
}

void CReversePlayerApp::onCommand(const std::string& i_sCommand, bool& o_bQuit)
{
	if (i_sCommand == "q")
		o_bQuit = true;
	else if (i_sCommand == "n")
		m_oGen.setAllToNone();
	else if (i_sCommand == "r")
		m_oGen.randomize();
}

void CReversePlayerApp::produceAudioData(char* i_pData, size_t i_nSize)
{
	m_oNes->render(i_pData, i_nSize);
}



int main(int, char**)
{
	CReversePlayerApp oApp;
	return oApp.start();
}
