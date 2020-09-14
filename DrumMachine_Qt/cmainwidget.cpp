#include "cmainwidget.h"
#include "ui_cmainwidget.h"

CMainWidget::CMainWidget(QWidget* i_pParent):
	QWidget(i_pParent),
	m_pUI(new Ui::CMainWidget),
	m_oDpcmProvider(false),
	m_oModel(m_oDpcmProvider),
	m_oSequencePlayer(m_oDpcmProvider)
{
	m_pUI->setupUi(this);

	m_oNes->setDpcmDataProvider(&m_oDpcmProvider);
	m_oNes->setFrameDataProducer(&m_oSequencePlayer);
	m_oNes->open();

	connect(&m_oAudio, &CAudioStream::audioDataRequired, this, &CMainWidget::onAudioDataRequested);

	connect(m_pUI->btnLoad , &QPushButton::clicked, this, &CMainWidget::onLoad);
	m_pUI->lstSamples->setModel(&m_oModel);

	connect(m_pUI->btnPlay , &QPushButton::clicked, this, &CMainWidget::onPlay);
	connect(m_pUI->btnPause, &QPushButton::clicked, this, &CMainWidget::onPause);
	connect(m_pUI->btnStop , &QPushButton::clicked, this, &CMainWidget::onStop);

	m_pUI->spbFPS->setValue(CSequencePlayer::DefaultFps);
	connect(m_pUI->spbFPS, QOverload<int>::of(&QSpinBox::valueChanged), this, &CMainWidget::onFpsChanged);

	connect(m_pUI->edtSequences, &QPlainTextEdit::cursorPositionChanged, this, &CMainWidget::onSequenceSelected);
}

CMainWidget::~CMainWidget()
{
	delete m_pUI;
}

void CMainWidget::onLoad()
{
	QString sFileName = QFileDialog::getOpenFileName(nullptr, "Select audio sample", "", "Audio samples (*.wav *.raw *.dmc);");
	if (sFileName.size() == 0)
		return;

	if (m_oDpcmProvider.load(QPATHSTR(sFileName)))
		m_oModel.onAppended();
}

void CMainWidget::onPlay()
{
	m_oSequencePlayer.restart();
	m_oNes->setTrack(0);
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

void CMainWidget::onFpsChanged()
{
	m_oSequencePlayer.setSpeed( size_t(m_pUI->spbFPS->value()) );
}

void CMainWidget::onSequenceSelected()
{
	QTextCursor oCursor = m_pUI->edtSequences->textCursor();
	auto sSequence = oCursor.block().text().toStdString();
	m_oSequencePlayer.setSequence(sSequence);
}

void CMainWidget::onAudioDataRequested(char* o_pBuffer, qint64 i_nBufferSize)
{
	m_oNes->render(o_pBuffer, size_t(i_nBufferSize));
}
