#include "cmainwidget.h"
#include "ui_cmainwidget.h"

CMainWidget::CMainWidget(QWidget* i_pParent)
	: QWidget(i_pParent)
	, m_pUI(new Ui::CMainWidget)
{
	m_pUI->setupUi(this);

	m_oNes->open();
	m_oNes->setTrack(0);

	connect(&m_oAudio, &CAudioStream::audioDataRequired, this, &CMainWidget::onAudioDataRequested);

	connect(m_pUI->btnLoad , &QPushButton::clicked, this, &CMainWidget::onLoad);
	connect(m_pUI->btnPlay , &QPushButton::clicked, this, &CMainWidget::onPlay);
	connect(m_pUI->btnPause, &QPushButton::clicked, this, &CMainWidget::onPause);
	connect(m_pUI->btnStop , &QPushButton::clicked, this, &CMainWidget::onStop);
}

CMainWidget::~CMainWidget()
{
	deleteDataBuffers();
	delete m_pUI;
}

void CMainWidget::onLoad()
{
	QString sFileName = QFileDialog::getOpenFileName(nullptr, "Select NSF file to play", "", "*.NSF");
	if (sFileName.size() == 0)
		return;

	if (!m_oNesSrc->open(QPATHSTR(sFileName)))
		return;

	m_pUI->edtFile->setText( QFileInfo(sFileName).fileName() );

	m_pUI->spbTrack->setEnabled(true);
	m_pUI->spbTrack->setRange(1, m_oNesSrc->trackCount());
	m_pUI->spbTrack->setValue(1);
	m_pUI->spbTime->setEnabled(true);

	m_pUI->btnPlay->setEnabled(true);
	m_pUI->btnPause->setEnabled(false);
	m_pUI->btnStop->setEnabled(false);
}

void CMainWidget::onPlay()
{
	prepareData();
	m_oAudio.start();
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

void CMainWidget::deleteDataBuffers()
{
	if (m_pFdBuffer   != nullptr)
		delete m_pFdBuffer;
	if (m_pDpcmBuffer != nullptr)
		delete m_pDpcmBuffer;
	m_pFdBuffer   = nullptr;
	m_pDpcmBuffer = nullptr;
}

void CMainWidget::prepareData()
{
	m_oNesSrc->detachAll();
	deleteDataBuffers();
	m_pFdBuffer   = new PurrFX::CFrameDataBuffer( m_pUI->spbTime->value()*60 );
	m_pDpcmBuffer = new PurrFX::CDpcmDataBuffer(true);
	m_oNesSrc->setFrameDataConsumer(m_pFdBuffer);
	m_oNesSrc->setDpcmDataConsumer (m_pDpcmBuffer);
	m_oNesSrc->setTrack( m_pUI->spbTrack->value()-1 );
	m_oNesSrc->render();

	m_oProxy.start(m_pFdBuffer);
	m_oNes->setFrameDataProducer(&m_oProxy);
	m_oNes->setDpcmDataProvider (m_pDpcmBuffer);
	m_oNes->setTrack(0);
}
