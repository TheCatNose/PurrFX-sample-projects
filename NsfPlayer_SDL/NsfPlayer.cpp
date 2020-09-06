#include <CAudioApp.h>
#include <PurrFX.h>

class CNsfPlayerApp: public CAudioApp
{
public:
	virtual bool init();
private:
	virtual void showUsage();
	virtual void onCommand(const std::string& i_sCommand, bool& o_bQuit);
	virtual void produceAudioData(char* i_pData, size_t i_nSize);

	void setTrack(int i_nTrackIndex);

	PurrFX::CNesPtr m_oNes;
	int m_nTrack = 0;
};

bool CNsfPlayerApp::init()
{
	std::cout << "Enter NSF file name: ";
	PurrFX::pathstring sFileName;
	PATH_CIN >> sFileName;

	if (!m_oNes->open(sFileName))
	{
		std::cout << "Can't open NSF file!";
		return false;
	}

	if (!m_oNes->setTrack(0))
	{
		std::cout << "Can't set track!";
		return false;
	}

	return true;
}

void CNsfPlayerApp::showUsage()
{
	std::cout <<
		"Enter 'p' to play previous track" << std::endl <<
		"Enter 'n' to play next track" << std::endl <<
		"Enter 'q' to quit" << std::endl <<
		"--------------------------------\n";
}

void CNsfPlayerApp::onCommand(const std::string& i_sCommand, bool& o_bQuit)
{
	if (i_sCommand == "q")
		o_bQuit = true;
	else if (i_sCommand == "p")
		setTrack(m_nTrack-1);
	else if (i_sCommand == "n")
		setTrack(m_nTrack+1);
}

void CNsfPlayerApp::produceAudioData(char* i_pData, size_t i_nSize)
{
	m_oNes->render(i_pData, i_nSize);
}

void CNsfPlayerApp::setTrack(int i_nTrackIndex)
{
	if (!m_oNes->setTrack(i_nTrackIndex))
		return;
		
	m_nTrack = i_nTrackIndex;
	std::cout << "Track " << (m_nTrack+1) << "/" << m_oNes->trackCount() << std::endl;
}



int main(int, char**)
{
	CNsfPlayerApp oApp;
	return oApp.start();
}
