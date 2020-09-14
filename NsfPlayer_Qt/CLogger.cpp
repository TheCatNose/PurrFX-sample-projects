#include "CLogger.h"

void CLogger::onEvent(const PurrFX::CNesEvent* i_pEvent)
{
	if (i_pEvent->type() == PurrFX::ENesEventType::ApuRegisterWrite)
	{
		auto* pItem = static_cast<const PurrFX::CNesEventApuRegisterWrite*>(i_pEvent);
		emit registerChanged(pItem->registerNumber(), pItem->registerValue());
	}
}
