#include "CLogger.h"

void CLogger::onNewItem(const PurrFX::CLogItem* i_pLogItem)
{
	if (i_pLogItem->type() == PurrFX::ELogItemType::ApuRegisterWrite)
	{
		auto* pItem = static_cast<const PurrFX::CLogItemApuRegisterWrite*>(i_pLogItem);
		emit registerChanged(pItem->registerNumber(), pItem->registerValue());
	}
}
