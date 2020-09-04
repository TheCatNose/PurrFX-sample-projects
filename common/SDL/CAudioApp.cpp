#include "CAudioApp.h"

CAudioApp* CAudioApp::sm_pInstance = nullptr;

CAudioApp::CAudioApp()
{
	assert(sm_pInstance == nullptr);
	sm_pInstance = this;
}

int CAudioApp::start()
{
	bool bSuccess = (SDL_Init(SDL_INIT_AUDIO) == 0);
	if (bSuccess)
	{
		SDL_AudioSpec desiredSpec;

		desiredSpec.freq     = 44100;
		desiredSpec.format   = AUDIO_S16LSB;
		desiredSpec.channels = 2;
		desiredSpec.samples  = 1024;
		desiredSpec.callback = audioCallback;

		bSuccess = (SDL_OpenAudio(&desiredSpec, nullptr) == 0);
		if (bSuccess)
		{
			SDL_PauseAudio(0);

			init();
			showUsage();
			eventLoop();
		}

		SDL_Quit();
	}

	return bSuccess?0:1;
}

void CAudioApp::audioCallback(void* pUserdata, Uint8* i_pDdata, int i_nSize)
{
	assert(sm_pInstance != nullptr);
	sm_pInstance->produceAudioData(
		reinterpret_cast<char*>(i_pDdata),
		size_t(i_nSize)
	);
};

void CAudioApp::eventLoop()
{
	while(true)
	{
		std::string sCommand;
		std::cin >> sCommand;
		bool bQuit = false;
		onCommand(sCommand, bQuit);
		if (bQuit)
			break;
	}
}
