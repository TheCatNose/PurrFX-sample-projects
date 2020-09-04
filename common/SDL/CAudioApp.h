#pragma once

#include <iostream>
#include <cassert>
#include <string>
#include <SDL.h>

class CAudioApp
{
public:
	CAudioApp();
	int start();

protected:
	virtual void init() = 0;
	virtual void showUsage() = 0;
	virtual void onCommand(const std::string& i_sCommand, bool& o_bQuit) = 0;
	virtual void produceAudioData(char* i_pData, size_t i_nSize) = 0;
	
private:
	static CAudioApp* sm_pInstance;
	
	CAudioApp(const CAudioApp&) = delete;

	void eventLoop();
	static void audioCallback(void*, Uint8* i_pDdata, int i_nSize);
};