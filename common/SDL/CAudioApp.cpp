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
		SDL_AudioSpec oAudioSpec;

		oAudioSpec.freq     = 44100;
		oAudioSpec.format   = AUDIO_S16LSB;
		oAudioSpec.channels = 2;
		oAudioSpec.samples  = 1024;
		oAudioSpec.callback = audioCallback;

		bSuccess = (SDL_OpenAudio(&oAudioSpec, nullptr) == 0);
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

void CAudioApp::audioCallback(void*, Uint8* i_pDdata, int i_nSize)
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
