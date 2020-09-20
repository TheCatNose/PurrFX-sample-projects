#include "cmainwidget.h"
#include "ui_cmainwidget.h"

CMainWidget::CMainWidget(QWidget* i_pParent)
	: QWidget(i_pParent)
	, m_pUI(new Ui::CMainWidget)
{
	m_pUI->setupUi(this);

	m_oNes->open();
	m_oNes->setTrack(0);

	connect(m_pUI->btnLoad  , &QPushButton::clicked, this, &CMainWidget::onLoad);
	connect(m_pUI->btnExport, &QPushButton::clicked, this, &CMainWidget::onExport);
}

CMainWidget::~CMainWidget()
{
	delete m_pUI;
}

void CMainWidget::onLoad()
{
	QString sFileName = QFileDialog::getOpenFileName(nullptr, "Select NSF file to convert", "", "*.nsf");
	if (sFileName.size() == 0)
		return;

	if (!m_oNes->open(QPATHSTR(sFileName)))
		return;

	m_pUI->edtFile->setText( QFileInfo(sFileName).fileName() );

	m_pUI->spbTrack->setEnabled(true);
	m_pUI->spbTrack->setRange(1, m_oNes->trackCount());
	m_pUI->spbTrack->setValue(1);
	m_pUI->spbTime->setEnabled(true);

	m_pUI->btnExport->setEnabled(true);
}

void CMainWidget::onExport() const
{
	enum ETrack: size_t
	{
		Pulse1Track,
		Pulse2Track,
		TriangleTrack,
		TrackCount
	};

	QString sFileName = QString("%1 - track %2.mid")
		.arg( m_pUI->edtFile->text() )
		.arg( m_pUI->spbTrack->value() );
	sFileName = QFileDialog::getSaveFileName(nullptr, "Save as MIDI", sFileName, "*.mid");
	if (sFileName.size() == 0)
		return;

	CMidiFile oMidiFile( QPATHSTR(sFileName), TrackCount);
	if (!oMidiFile.isOpened())
		return;

	// Collect music data

	PurrFX::CFrameDataBuffer oFdBuffer( m_pUI->spbTime->value()*60 );
	m_oNes->setFrameDataConsumer(&oFdBuffer);
	m_oNes->setTrack( m_pUI->spbTrack->value()-1 );
	m_oNes->render();
	m_oNes->detachAll();

	// Export to midi

	PurrFX::CNesState oState;
	for (size_t i = 0; i < oFdBuffer.size(); i++)
	{
		PurrFX::CFrameData oFrameData = oFdBuffer.getAt(i);
		oState.update(oFrameData);

		bool bLastFrame = (i == oFdBuffer.size()-1);

		// Pulse1
		{
			auto oRegister4000 = oFrameData.getRegister(PurrFX::ERegister::Apu4000);
			uint8_t nVolume    = oRegister4000.get('V');
			uint8_t nVolumeMax = oRegister4000.max('V');
			bool bNoteStart =
				oFrameData.isSet(PurrFX::ERegister::Apu4002) ||
				oFrameData.isSet(PurrFX::ERegister::Apu4003) ;
			bool bNoteEnd   =
				oFrameData.isSet(PurrFX::ERegister::Apu4000) &&
				nVolume == 0x00;

			if (bNoteEnd || bLastFrame)
				oMidiFile.addKeyReleaseEvent(Pulse1Track);
			if (bNoteStart)
			{
				double nPitch = PurrFX::CNesCalculations::apuPulseFrequency( oState.apuPulse1Timer() );
				PurrFX::CNote note(nPitch);
				oMidiFile.addKeyPressEvent(Pulse1Track, note.octave(), note.note(), nVolume/float(nVolumeMax) );
			}
		}

		// Pulse2
		{
			auto oRegister4004 = oFrameData.getRegister(PurrFX::ERegister::Apu4004);
			uint8_t nVolume    = oRegister4004.get('V');
			uint8_t nVolumeMax = oRegister4004.max('V');
			bool bNoteStart =
				oFrameData.isSet(PurrFX::ERegister::Apu4006) ||
				oFrameData.isSet(PurrFX::ERegister::Apu4007) ;
			bool bNoteEnd   =
				oFrameData.isSet(PurrFX::ERegister::Apu4004) &&
				nVolume == 0x00;

			if (bNoteEnd || bLastFrame)
				oMidiFile.addKeyReleaseEvent(Pulse2Track);
			if (bNoteStart)
			{
				double nPitch = PurrFX::CNesCalculations::apuPulseFrequency( oState.apuPulse2Timer() );
				PurrFX::CNote note(nPitch);
				oMidiFile.addKeyPressEvent(Pulse2Track,  note.octave(), note.note(), nVolume/float(nVolumeMax) );
			}
		}

		// Triangle
		{
			bool bNoteStart =
				oFrameData.isSet(PurrFX::ERegister::Apu400A) ||
				oFrameData.isSet(PurrFX::ERegister::Apu400B) ;
			bool bNoteEnd   =
				oFrameData.isSet(PurrFX::ERegister::Apu4008) &&
				oFrameData.get  (PurrFX::ERegister::Apu4008) == 0x00;

			if (bNoteEnd || bLastFrame)
				oMidiFile.addKeyReleaseEvent(TriangleTrack);
			if (bNoteStart)
			{
				double nPitch = PurrFX::CNesCalculations::apuTriangleFrequency( oState.apuTriangleTimer() );
				PurrFX::CNote note(nPitch);
				oMidiFile.addKeyPressEvent(TriangleTrack, note.octave(), note.note(), 0.5 );
			}
		}

		oMidiFile.tick();
	}
}
