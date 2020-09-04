#ifndef CAUDIOSTREAM_H
#define CAUDIOSTREAM_H

#include <QIODevice>
#include <QAudioOutput>

class CAudioStream: public QIODevice
{
	Q_OBJECT
public:
	CAudioStream();

	void start();
	void pause();
	void stop();

signals:
	void audioDataRequired(char* o_pBuffer, qint64 i_nBufferSize);

private:
	// QIODevice implementation
	virtual qint64 readData(char* o_pData, qint64 i_nSize);
	virtual qint64 writeData(const char*, qint64);

private:
	CAudioStream(const CAudioStream&) = delete;


	QAudioOutput* m_pAudioOutput = nullptr;
};

#endif
