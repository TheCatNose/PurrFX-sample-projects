#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include "DString.h"
#include "CAudioStream.h"
#include "CLogger.h"
#include "purrfx/PurrFX.h"

namespace Ui { class CMainWidget; }

class CMainWidget : public QWidget
{
	Q_OBJECT

public:
	CMainWidget(QWidget* i_pParent = nullptr);
	~CMainWidget();

private slots:
	void onLoad();

	void onPrev();
	void onNext();

	void onPlay();
	void onPause();
	void onStop();

	void onAudioDataRequested(char* o_pBuffer, qint64 i_nBufferSize);
	void onRegisterValueChanged(uint16_t i_nRegister, uint8_t i_nValue);

private:
	void updateTrackText();
	void updatePrevNextButtons();
	void resetRegisterView();

	Ui::CMainWidget* m_pUI;
	CAudioStream m_oAudio;
	PurrFX::CNesPtr m_oNes;
	CLogger m_oLogger;
	int m_nTrack = 0;
};
#endif // CMAINWIDGET_H
