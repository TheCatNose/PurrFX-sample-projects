#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include "DString.h"
#include "CAudioStream.h"
#include "ProceduralAudio/CGenerator.h"
#include "purrfx/PurrFX.h"

namespace Ui { class CMainWidget; }

class CMainWidget : public QWidget
{
	Q_OBJECT

public:
	CMainWidget(QWidget* i_pParent = nullptr);
	~CMainWidget();

private slots:

	void onTypeChanged();
	void onValueChanged();

	void onNone();

	void onPlay();
	void onPause();
	void onStop();

	void onAudioDataRequested(char* o_pBuffer, qint64 i_nBufferSize);

private:

	Ui::CMainWidget* m_pUI;

	CAudioStream m_oAudio;
	PurrFX::CNesPtr m_oNes;
	CGenerator      m_oGen;
};
#endif // CMAINWIDGET_H
