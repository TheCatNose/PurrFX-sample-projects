#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include "DString.h"
#include "CAudioStream.h"
#include "purrfx/PurrFX.h"
#include "ReversePlayer/CFrameDataProxy.h"

namespace Ui { class CMainWidget; }

class CMainWidget : public QWidget
{
	Q_OBJECT

public:
	CMainWidget(QWidget* i_pParent = nullptr);
	~CMainWidget();

private slots:
	void onLoad();

	void onPlay();
	void onPause();
	void onStop();

	void onAudioDataRequested(char* o_pBuffer, qint64 i_nBufferSize);

private:
	void deleteDataBuffers();
	void prepareData();

	Ui::CMainWidget* m_pUI;
	CAudioStream m_oAudio;
	PurrFX::CNesPtr m_oNesSrc;
	PurrFX::CFrameDataBuffer* m_pFdBuffer   = nullptr;
	PurrFX::CDpcmDataBuffer*  m_pDpcmBuffer = nullptr;
	CFrameDataProxy m_oProxy;
	PurrFX::CNesPtr m_oNes;
};
#endif // CMAINWIDGET_H
