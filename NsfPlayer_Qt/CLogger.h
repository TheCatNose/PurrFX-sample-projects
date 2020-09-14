#ifndef CLOGGER_H
#define CLOGGER_H

#include <purrfx/CNesEventConsumer.h>
#include <purrfx/CNesEventApuRegisterWrite.h>
#include <QObject>

class CLogger:  public QObject, public PurrFX::CNesEventConsumer
{
	Q_OBJECT
signals:
	void registerChanged(uint16_t i_nRegister, uint8_t i_nValue);
private:
	virtual void onEvent(const PurrFX::CNesEvent* i_pEvent);
};

#endif // CLOGGER_H
