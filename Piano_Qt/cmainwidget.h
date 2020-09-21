#ifndef CMAINWIDGET_H
#define CMAINWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QKeyEvent>
#include "CAudioStream.h"
#include "Piano/CGenerator.h"
#include "purrfx/PurrFX.h"

namespace Ui { class CMainWidget; }

class CMainWidget : public QWidget
{
	Q_OBJECT

public:
	CMainWidget(QWidget* i_pParent = nullptr);
	~CMainWidget();

protected:
	virtual void keyPressEvent  (QKeyEvent* i_pEvent);
	virtual void keyReleaseEvent(QKeyEvent* i_pEvent);

private slots:
	void onAudioDataRequested(char* o_pBuffer, qint64 i_nBufferSize);

private:
	Ui::CMainWidget* m_pUI;

	int m_nPressedKey = Qt::Key_unknown;

	CAudioStream    m_oAudio;
	PurrFX::CNesPtr m_oNes;
	CGenerator      m_oGen;
};
#endif // CMAINWIDGET_H
