#include "CAudioApp.h"

CAudioApp* CAudioApp::sm_pInstance = nullptr;

CAudioApp::CAudioApp()
{
	assert(sm_pInstance == nullptr);
	sm_pInstance = this;
}

int CAudioApp::start()
{
	bool bInitSuccess = (SDL_Init(SDL_INIT_AUDIO) == 0);
	bool bSuccess = bInitSuccess;

	if (bSuccess)
	{
		m_pMutex = SDL_CreateMutex();
		bSuccess = m_pMutex != nullptr;
	}

	bool bAudioInitSuccess = false;
	if (bSuccess)
	{
		SDL_AudioSpec oAudioSpec;

		oAudioSpec.freq     = 44100;
		oAudioSpec.format   = AUDIO_S16LSB;
		oAudioSpec.channels = 2;
		oAudioSpec.samples  = 1024;
		oAudioSpec.callback = audioCallback;

		bAudioInitSuccess = (SDL_OpenAudio(&oAudioSpec, nullptr) == 0);
		bSuccess = bAudioInitSuccess;
	}

	if (bSuccess)
	{
		SDL_PauseAudio(0);

		if (init())
		{
			showUsage();
			eventLoop();
		}
	}

	// Clean up and end

	if (bAudioInitSuccess)
		SDL_CloseAudio();

	if (m_pMutex != nullptr)
		SDL_DestroyMutex(m_pMutex);

	if (bInitSuccess)
		SDL_Quit();

	return bSuccess?0:1;
}

void CAudioApp::audioCallback(void*, Uint8* i_pDdata, int i_nSize)
{
	assert(sm_pInstance != nullptr);
	
	SDL_LockMutex(sm_pInstance->m_pMutex);
	
	sm_pInstance->produceAudioData(
		reinterpret_cast<char*>(i_pDdata),
		size_t(i_nSize)
	);

	SDL_UnlockMutex(sm_pInstance->m_pMutex);
};

void CAudioApp::eventLoop()
{
	while(true)
	{
		std::string sCommand;
		std::cin >> sCommand;
		bool bQuit = false;

		SDL_LockMutex(m_pMutex);
		onCommand(sCommand, bQuit);
		SDL_UnlockMutex(m_pMutex);

		if (bQuit)
			break;
	}
}
