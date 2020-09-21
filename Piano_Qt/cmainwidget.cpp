#include "cmainwidget.h"
#include "ui_cmainwidget.h"

CMainWidget::CMainWidget(QWidget* i_pParent)
	: QWidget(i_pParent)
	, m_pUI(new Ui::CMainWidget)
{
	m_pUI->setupUi(this);

	// For keyPressEvent()
	setFocusPolicy(Qt::StrongFocus);
	setFocus();

	m_oNes->open();
	m_oNes->setFrameDataProducer(&m_oGen);
	m_oNes->setTrack(0);
	m_oAudio.start();

	connect(&m_oAudio, &CAudioStream::audioDataRequired, this, &CMainWidget::onAudioDataRequested);
}

CMainWidget::~CMainWidget()
{
	delete m_pUI;
}

void CMainWidget::keyPressEvent(QKeyEvent* i_pEvent)
{
	QWidget::keyPressEvent(i_pEvent);
	if (i_pEvent->isAutoRepeat())
		return;

	if (m_nPressedKey != Qt::Key_unknown)
		return;

	const int LowOctave  = 3;
	const int HighOctave = LowOctave+1;

	int nOctave = -1, nNote = -1;
	switch(i_pEvent->key())
	{
	case Qt::Key_Z: nOctave = LowOctave;  nNote =  0; break;
	case Qt::Key_S: nOctave = LowOctave;  nNote =  1; break;
	case Qt::Key_X: nOctave = LowOctave;  nNote =  2; break;
	case Qt::Key_D: nOctave = LowOctave;  nNote =  3; break;
	case Qt::Key_C: nOctave = LowOctave;  nNote =  4; break;
	case Qt::Key_V: nOctave = LowOctave;  nNote =  5; break;
	case Qt::Key_G: nOctave = LowOctave;  nNote =  6; break;
	case Qt::Key_B: nOctave = LowOctave;  nNote =  7; break;
	case Qt::Key_H: nOctave = LowOctave;  nNote =  8; break;
	case Qt::Key_N: nOctave = LowOctave;  nNote =  9; break;
	case Qt::Key_J: nOctave = LowOctave;  nNote = 10; break;
	case Qt::Key_M: nOctave = LowOctave;  nNote = 11; break;

	case Qt::Key_Q: nOctave = HighOctave; nNote =  0; break;
	case Qt::Key_2: nOctave = HighOctave; nNote =  1; break;
	case Qt::Key_W: nOctave = HighOctave; nNote =  2; break;
	case Qt::Key_3: nOctave = HighOctave; nNote =  3; break;
	case Qt::Key_E: nOctave = HighOctave; nNote =  4; break;
	case Qt::Key_R: nOctave = HighOctave; nNote =  5; break;
	case Qt::Key_5: nOctave = HighOctave; nNote =  6; break;
	case Qt::Key_T: nOctave = HighOctave; nNote =  7; break;
	case Qt::Key_6: nOctave = HighOctave; nNote =  8; break;
	case Qt::Key_Y: nOctave = HighOctave; nNote =  9; break;
	case Qt::Key_7: nOctave = HighOctave; nNote = 10; break;
	case Qt::Key_U: nOctave = HighOctave; nNote = 11; break;

	case Qt::Key_PageUp:
		m_oGen.increaseDuty();
		break;
	case Qt::Key_PageDown:
		m_oGen.decreaseDuty();
		break;
	}

	if (nNote != -1)
	{
		m_nPressedKey = i_pEvent->key();
		m_oGen.playNote( PurrFX::CNote(nOctave,nNote) );
	}
}

void CMainWidget::keyReleaseEvent(QKeyEvent* i_pEvent)
{
	QWidget::keyReleaseEvent(i_pEvent);
	if (i_pEvent->isAutoRepeat())
		return;

	if (m_nPressedKey == i_pEvent->key())
		m_oGen.stopNote();

	m_nPressedKey = Qt::Key_unknown;
}

void CMainWidget::onAudioDataRequested(char* o_pBuffer, qint64 i_nBufferSize)
{
	m_oNes->render(o_pBuffer, size_t(i_nBufferSize));
}
