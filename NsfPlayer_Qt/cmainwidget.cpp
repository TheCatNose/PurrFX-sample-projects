#include "cmainwidget.h"
#include "ui_cmainwidget.h"

CMainWidget::CMainWidget(QWidget* i_pParent)
	: QWidget(i_pParent)
	, m_pUI(new Ui::CMainWidget)
{
	m_pUI->setupUi(this);

	connect(&m_oAudio, &CAudioStream::audioDataRequired, this, &CMainWidget::onAudioDataRequested);

	connect(m_pUI->btnPrev , &QPushButton::clicked, this, &CMainWidget::onPrev);
	connect(m_pUI->btnNext , &QPushButton::clicked, this, &CMainWidget::onNext);

	connect(m_pUI->btnLoad , &QPushButton::clicked, this, &CMainWidget::onLoad);
	connect(m_pUI->btnPlay , &QPushButton::clicked, this, &CMainWidget::onPlay);
	connect(m_pUI->btnPause, &QPushButton::clicked, this, &CMainWidget::onPause);
	connect(m_pUI->btnStop , &QPushButton::clicked, this, &CMainWidget::onStop);
}

CMainWidget::~CMainWidget()
{
	delete m_pUI;
}

void CMainWidget::onLoad()
{
	QString sFileName = QFileDialog::getOpenFileName(nullptr, "Select NSF file to play", "", "*.NSF");
	if (sFileName.size() == 0)
		return;

	if (!m_oNes->open(QPATHSTR(sFileName)))
		return;

	m_pUI->edtFile->setText( QFileInfo(sFileName).baseName() );

	m_nTrack = 0;
	updateTrackText();
	updatePrevNextButtons();

	m_pUI->btnPlay->setEnabled(true);
	m_pUI->btnPause->setEnabled(false);
	m_pUI->btnStop->setEnabled(false);
}

void CMainWidget::onPrev()
{
	if (m_nTrack == 0)
		return;
	m_oNes->setTrack(--m_nTrack);
	updateTrackText();
	updatePrevNextButtons();
}

void CMainWidget::onNext()
{
	if (m_nTrack == m_oNes->trackCount()-1)
		return;
	m_oNes->setTrack(++m_nTrack);
	updateTrackText();
	updatePrevNextButtons();
}

void CMainWidget::onPlay()
{
	m_oNes->setTrack(m_nTrack);
	m_oAudio.start();
	m_pUI->btnPlay->setEnabled(false);
	m_pUI->btnPause->setEnabled(true);
	m_pUI->btnStop->setEnabled(true);
}

void CMainWidget::onPause()
{
	m_oAudio.pause();
}

void CMainWidget::onStop()
{
	m_oAudio.stop();
	m_pUI->btnPlay->setEnabled(true);
	m_pUI->btnPause->setEnabled(false);
	m_pUI->btnStop->setEnabled(false);
}

void CMainWidget::onAudioDataRequested(char* o_pBuffer, qint64 i_nBufferSize)
{
	m_oNes->render(o_pBuffer, size_t(i_nBufferSize));
}

void CMainWidget::updateTrackText()
{
	QString sTrack = QString("Track %1/%2")
		.arg( m_nTrack+1 )
		.arg( m_oNes->trackCount() );
	m_pUI->lblTrack->setText(sTrack);
}

void CMainWidget::updatePrevNextButtons()
{
	m_pUI->btnPrev->setEnabled( m_nTrack > 0);
	m_pUI->btnNext->setEnabled( m_nTrack < m_oNes->trackCount()-1 );
}
