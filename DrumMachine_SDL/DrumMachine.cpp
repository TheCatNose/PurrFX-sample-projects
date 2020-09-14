#include <CAudioApp.h>
#include <purrfx/PurrFX.h>
#include "DrumMachine/CSequencePlayer.h"

class CReversePlayerApp: public CAudioApp
{
public:
	CReversePlayerApp();
	virtual bool init();
private:
	virtual void showUsage();
	virtual void onCommand(const std::string& i_sCommand, bool& o_bQuit);
	virtual void produceAudioData(char* i_pData, size_t i_nSize);

	PurrFX::CNesPtr             m_oNes;
	PurrFX::CDpcmDataFileReader m_oDpcmProvider;
	CSequencePlayer             m_oSequencePlayer;
	size_t m_nFramesPerStep = CSequencePlayer::DefaultFps;
};

CReversePlayerApp::CReversePlayerApp():
	m_oDpcmProvider(false),
	m_oSequencePlayer(m_oDpcmProvider)
{}

bool CReversePlayerApp::init()
{
	m_oNes->setDpcmDataProvider(&m_oDpcmProvider);
	m_oNes->setFrameDataProducer(&m_oSequencePlayer);
	m_oNes->open();
	m_oNes->setTrack(0);
	return true;
}

void CReversePlayerApp::showUsage()
{
	std::cout <<
		"Enter 'l' to load audio sample" << std::endl <<
		"Enter 's' to enter drum sequence" << std::endl <<
		"Enter '+' to speed up" << std::endl <<
		"Enter '-' to slow down" << std::endl <<
		"Enter 'q' to quit" << std::endl <<
		"--------------------------------\n";
}

void CReversePlayerApp::onCommand(const std::string& i_sCommand, bool& o_bQuit)
{
	if (i_sCommand == "q")
		o_bQuit = true;
	else if (i_sCommand == "l")
	{
		std::cout << "Enter file name> ";
		PurrFX::pathstring sFileName;
		PATH_CIN.ignore();
		std::getline(PATH_CIN, sFileName);
		size_t nSize = m_oDpcmProvider.size();
		m_oDpcmProvider.load(sFileName);
		if (nSize == m_oDpcmProvider.size())
			std::cout << "Not loaded" << std::endl;
		else
			std::cout << "Sample id is " << m_oDpcmProvider.size() << std::endl;
	}
	else if (i_sCommand == "s")
	{
		std::cout << "Enter drum sequence> ";
		std::string sSequence;
		std::cin.ignore();
		std::getline(std::cin, sSequence);
		m_oSequencePlayer.setSequence(sSequence);
	}
	else if (i_sCommand == "+")
	{
		if (m_nFramesPerStep > 1)
		{
			m_nFramesPerStep--;
			m_oSequencePlayer.setSpeed(m_nFramesPerStep);
		}
	}
	else if (i_sCommand == "-")
	{
		m_nFramesPerStep++;
		m_oSequencePlayer.setSpeed(m_nFramesPerStep);
	}
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
