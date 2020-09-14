#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QTextBlock>

#include "DString.h"
#include "CAudioStream.h"
#include "CSamplesListModel.h"
#include "DrumMachine/CSequencePlayer.h"

#include "purrfx/PurrFX.h"
#include "purrfx/CDpcmDataFileReader.h"

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

	void onFpsChanged();
	void onSequenceSelected();

	void onAudioDataRequested(char* o_pBuffer, qint64 i_nBufferSize);

private:
	Ui::CMainWidget* m_pUI;

	CAudioStream m_oAudio;
	PurrFX::CNesPtr             m_oNes;
	PurrFX::CDpcmDataFileReader m_oDpcmProvider;
	CSamplesListModel           m_oModel;
	CSequencePlayer             m_oSequencePlayer;
};
#endif // CMAINWIDGET_H
