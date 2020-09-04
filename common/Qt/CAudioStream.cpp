#include "CAudioStream.h"

CAudioStream::CAudioStream()
{
	QAudioFormat oFormat;
	oFormat.setSampleRate(44100);
	oFormat.setChannelCount(2);
	oFormat.setSampleSize(16);
	oFormat.setCodec("audio/pcm");
	oFormat.setByteOrder(QAudioFormat::LittleEndian);
	oFormat.setSampleType(QAudioFormat::UnSignedInt);

	open(QIODevice::ReadOnly);

	m_pAudioOutput = new QAudioOutput(oFormat, this);
}

void CAudioStream::start()
{
	m_pAudioOutput->start(this);
}

void CAudioStream::pause()
{
	if (m_pAudioOutput->state() == QAudio::ActiveState)
		m_pAudioOutput->suspend();
	else
		m_pAudioOutput->resume();
}

void CAudioStream::stop()
{
	m_pAudioOutput->stop();
}

qint64 CAudioStream::readData(char* o_pData, qint64 i_nSize)
{
	if (i_nSize > 0 && o_pData != nullptr)
		emit audioDataRequired(o_pData, i_nSize);
	return i_nSize;
}

qint64 CAudioStream::writeData(const char*, qint64)
{
	return 0;
}
