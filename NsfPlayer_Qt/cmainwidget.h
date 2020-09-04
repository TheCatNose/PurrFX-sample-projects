#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include "DString.h"
#include "CAudioStream.h"
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

private:
	void updateTrackText();
	void updatePrevNextButtons();

	Ui::CMainWidget* m_pUI;
	CAudioStream m_oAudio;
	PurrFX::CNesPtr m_oNes;
	int m_nTrack = 0;
};
#endif // CMAINWIDGET_H
