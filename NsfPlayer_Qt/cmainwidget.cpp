#include "cmainwidget.h"
#include "ui_cmainwidget.h"

CMainWidget::CMainWidget(QWidget* i_pParent)
	: QWidget(i_pParent)
	, ui(new Ui::CMainWidget)
{
	ui->setupUi(this);

	connect(&m_oAudio, &CAudioStream::audioDataRequired, this, &CMainWidget::onAudioDataRequested);

	connect(ui->btnPrev , &QPushButton::clicked, this, &CMainWidget::onPrev);
	connect(ui->btnNext , &QPushButton::clicked, this, &CMainWidget::onNext);

	connect(ui->btnLoad , &QPushButton::clicked, this, &CMainWidget::onLoad);
	connect(ui->btnPlay , &QPushButton::clicked, this, &CMainWidget::onPlay);
	connect(ui->btnPause, &QPushButton::clicked, this, &CMainWidget::onPause);
	connect(ui->btnStop , &QPushButton::clicked, this, &CMainWidget::onStop);
}

CMainWidget::~CMainWidget()
{
	delete ui;
}

void CMainWidget::onLoad()
{
	QString sFileName = QFileDialog::getOpenFileName(nullptr, "Select NSF file to play", "", "*.NSF");
	if (sFileName.size() == 0)
		return;

	if (!m_oNes->open(QPATHSTR(sFileName)))
		return;

	ui->edtFile->setText( QFileInfo(sFileName).baseName() );

	m_nTrack = 0;
	updateTrackText();
	updatePrevNextButtons();

	ui->btnPlay->setEnabled(true);
	ui->btnPause->setEnabled(false);
	ui->btnStop->setEnabled(false);
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
	ui->btnPlay->setEnabled(false);
	ui->btnPause->setEnabled(true);
	ui->btnStop->setEnabled(true);
}

void CMainWidget::onPause()
{
	m_oAudio.pause();
}

void CMainWidget::onStop()
{
	m_oAudio.stop();
	ui->btnPlay->setEnabled(true);
	ui->btnPause->setEnabled(false);
	ui->btnStop->setEnabled(false);
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
	ui->lblTrack->setText(sTrack);
}

void CMainWidget::updatePrevNextButtons()
{
	ui->btnPrev->setEnabled( m_nTrack > 0);
	ui->btnNext->setEnabled( m_nTrack < m_oNes->trackCount()-1 );
}
