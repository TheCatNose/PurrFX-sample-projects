#include "cmainwidget.h"
#include "ui_cmainwidget.h"

CMainWidget::CMainWidget(QWidget* i_pParent)
	: QWidget(i_pParent)
	, m_pUI(new Ui::CMainWidget)
{
	m_pUI->setupUi(this);

	m_oNes->open();
	m_oNes->setFrameDataProducer(&m_oGen);

	connect(&m_oAudio, &CAudioStream::audioDataRequired, this, &CMainWidget::onAudioDataRequested);

	QComboBox* aCbxMode[] = {
		m_pUI->cbxRegister4000Mode,
		m_pUI->cbxRegister4001Mode,
		m_pUI->cbxRegister4002Mode,
		m_pUI->cbxRegister4003Mode,
		m_pUI->cbxRegister4004Mode,
		m_pUI->cbxRegister4005Mode,
		m_pUI->cbxRegister4006Mode,
		m_pUI->cbxRegister4007Mode,
		m_pUI->cbxRegister4008Mode,
		m_pUI->cbxRegister400AMode,
		m_pUI->cbxRegister400BMode,
		m_pUI->cbxRegister400CMode,
		m_pUI->cbxRegister400EMode,
		m_pUI->cbxRegister400FMode,
	};
	QSpinBox* aSpbValue[] = {
		m_pUI->spbRegister4000Value,
		m_pUI->spbRegister4001Value,
		m_pUI->spbRegister4002Value,
		m_pUI->spbRegister4003Value,
		m_pUI->spbRegister4004Value,
		m_pUI->spbRegister4005Value,
		m_pUI->spbRegister4006Value,
		m_pUI->spbRegister4007Value,
		m_pUI->spbRegister4008Value,
		m_pUI->spbRegister400AValue,
		m_pUI->spbRegister400BValue,
		m_pUI->spbRegister400CValue,
		m_pUI->spbRegister400EValue,
		m_pUI->spbRegister400FValue,
	};
	Q_ASSERT(sizeof(aCbxMode) == sizeof(aSpbValue));
	QStringList lTypes = {"None","Value","Random","Sin","Cos","Saw"};
	for (size_t i = 0; i < sizeof(aCbxMode)/sizeof(aCbxMode[0]); i++)
	{
		aCbxMode[i]->addItems(lTypes);
		aCbxMode[i]->setCurrentIndex(2);
		aCbxMode[i]->setProperty("index", i);
		connect(aCbxMode[i], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CMainWidget::onTypeChanged);

		aSpbValue[i]->setProperty("index", i);
		connect(aSpbValue[i], QOverload<int>::of(&QSpinBox::valueChanged), this, &CMainWidget::onValueChanged);
	}

	connect(m_pUI->btnNone, &QPushButton::clicked, this, &CMainWidget::onNone);

	connect(m_pUI->btnPlay , &QPushButton::clicked, this, &CMainWidget::onPlay);
	connect(m_pUI->btnPause, &QPushButton::clicked, this, &CMainWidget::onPause);
	connect(m_pUI->btnStop , &QPushButton::clicked, this, &CMainWidget::onStop);
}

CMainWidget::~CMainWidget()
{
	delete m_pUI;
}

void CMainWidget::onTypeChanged()
{
	QComboBox* pCbxType = static_cast<QComboBox*>(sender());
	PurrFX::ERegister      eRegister = PurrFX::ERegister( pCbxType->property("index").toInt() );
	CGenerator::EMode eMode     = CGenerator::EMode( pCbxType->currentIndex() );
	m_oGen.setMode(eRegister, eMode);
}

void CMainWidget::onValueChanged()
{
	QSpinBox* pSpbValue = static_cast<QSpinBox*>(sender());
	PurrFX::ERegister eRegister = PurrFX::ERegister( pSpbValue->property("index").toInt() );
	int               nValue    = pSpbValue->value();
	m_oGen.setValue(eRegister, nValue);
}

void CMainWidget::onNone()
{
	auto lChildren = findChildren<QComboBox*>();
	for (QComboBox* pCbxType: lChildren)
		pCbxType->setCurrentIndex(0);
}

void CMainWidget::onPlay()
{
	m_oNes->setTrack(0);
	m_oGen.start();
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
