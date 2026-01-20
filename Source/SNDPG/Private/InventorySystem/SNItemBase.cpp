// Copyright 2026 out of sCope team - intrxx


#include "InventorySystem/SNItemBase.h"

USNItemBase::USNItemBase()
	: Weight(0)
	, ItemCategory(EItemCategory::None)
	, ItemDisplayName(FText::FromString("Default Item Name"))
	, ItemDescription(FText::FromString("Default Item Description"))
	, UseActionText(FText::FromString("Default Use Action Text"))
{
}
