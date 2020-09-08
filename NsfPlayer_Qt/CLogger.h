#ifndef CLOGGER_H
#define CLOGGER_H

#include <purrfx/CLogDataConsumer.h>
#include <purrfx/CLogItemApuRegisterWrite.h>
#include <QObject>

class CLogger:  public QObject, public PurrFX::CLogDataConsumer
{
	Q_OBJECT
signals:
	void registerChanged(uint16_t i_nRegister, uint8_t i_nValue);
private:
	virtual void onNewItem(const PurrFX::CLogItem* i_pLogItem);
};

#endif // CLOGGER_H
