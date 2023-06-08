// Copyright 2023 Michal Oginski.


#include "InventorySystem/SNItemBase.h"

USNItemBase::USNItemBase()
{
	Weight = 0.0f;
	ItemDisplayName = FText::FromString("Default Item Name");
	ItemDescription = FText::FromString("Default Item Description");
	UseActionText = FText::FromString("Default Use Action Text");
}
