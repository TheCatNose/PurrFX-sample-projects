#include "cmainwidget.h"
#include "ui_cmainwidget.h"

CMainWidget::CMainWidget(QWidget* i_pParent)
	: QWidget(i_pParent)
	, m_pUI(new Ui::CMainWidget)
{
	m_pUI->setupUi(this);

	m_oNes->logItemTypeDisable(PurrFX::ELogItemType::CodeLabel);
	m_oNes->logItemTypeDisable(PurrFX::ELogItemType::CpuInstruction);
	m_oNes->logItemTypeDisable(PurrFX::ELogItemType::FrameEnd);
	m_oNes->logItemTypeDisable(PurrFX::ELogItemType::FrameStart);
	m_oNes->setLogDataConsumer(&m_oLogger);
	connect(&m_oLogger, &CLogger::registerChanged, this, &CMainWidget::onRegisterValueChanged);

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

	m_pUI->edtFile->setText( QFileInfo(sFileName).fileName() );

	m_nTrack = 0;
	updateTrackText();
	updatePrevNextButtons();
	resetRegisterView();

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

void CMainWidget::onRegisterValueChanged(uint16_t i_nRegister, uint8_t i_nValue)
{
	QString sValue = QString("%1").arg(i_nValue, 2, 16, QChar('0'));
	switch(i_nRegister)
	{
	case 0x4000: m_pUI->lblRegister4000->setText(sValue); return;
	case 0x4001: m_pUI->lblRegister4001->setText(sValue); return;
	case 0x4002: m_pUI->lblRegister4002->setText(sValue); return;
	case 0x4003: m_pUI->lblRegister4003->setText(sValue); return;
	case 0x4004: m_pUI->lblRegister4004->setText(sValue); return;
	case 0x4005: m_pUI->lblRegister4005->setText(sValue); return;
	case 0x4006: m_pUI->lblRegister4006->setText(sValue); return;
	case 0x4007: m_pUI->lblRegister4007->setText(sValue); return;
	case 0x4008: m_pUI->lblRegister4008->setText(sValue); return;
	case 0x400A: m_pUI->lblRegister400A->setText(sValue); return;
	case 0x400B: m_pUI->lblRegister400B->setText(sValue); return;
	case 0x400C: m_pUI->lblRegister400C->setText(sValue); return;
	case 0x400E: m_pUI->lblRegister400E->setText(sValue); return;
	case 0x400F: m_pUI->lblRegister400F->setText(sValue); return;
	case 0x4010: m_pUI->lblRegister4010->setText(sValue); return;
	case 0x4011: m_pUI->lblRegister4011->setText(sValue); return;
	case 0x4012: m_pUI->lblRegister4012->setText(sValue); return;
	case 0x4013: m_pUI->lblRegister4013->setText(sValue); return;
	default:
		break;
	}
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

void CMainWidget::resetRegisterView()
{
	QList<QLabel*> lLabels = {
		m_pUI->lblRegister4000, m_pUI->lblRegister4001, m_pUI->lblRegister4002, m_pUI->lblRegister4003,
		m_pUI->lblRegister4004, m_pUI->lblRegister4005, m_pUI->lblRegister4006, m_pUI->lblRegister4007,
		m_pUI->lblRegister4008, m_pUI->lblRegister400A, m_pUI->lblRegister400B,
		m_pUI->lblRegister400C, m_pUI->lblRegister400E, m_pUI->lblRegister400F,
		m_pUI->lblRegister4010, m_pUI->lblRegister4011, m_pUI->lblRegister4012, m_pUI->lblRegister4013,
	};
	for (auto* pLabel: lLabels)
		pLabel->setText("00");
}
