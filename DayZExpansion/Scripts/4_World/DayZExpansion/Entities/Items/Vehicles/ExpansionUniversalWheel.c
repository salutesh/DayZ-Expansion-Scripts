#ifndef EXPANSION_UNIVERSAL_WHEEL_DISABLE
class ExpansionUniversalWheel: CarWheel
{
	override void OnWasAttached( EntityAI parent, int slot_id )
	{ 
		super.OnWasAttached(parent, slot_id);	
		if (parent.IsKindOf("CarScript"))
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id); //this is needed because if you attach with user action for some reason the item doesnt change.
		
	}	
	void ConvertWheel(EntityAI parent, int slot_id)
	{
		CarScript car = CarScript.Cast(parent);
		string wheelType = car.ExpansionGetWheelType(slot_id);
				
		if (GetGame().IsMultiplayer())
			parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, new CarWheelChangeLambda(this, wheelType));
	//	else
		//	parent.GetInventory().ReplaceItemWithNew(InventoryMode.LOCAL, new CarWheelChangeLambda(this, wheelType));
		

	};
}
modded class HatchbackWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		CarWheelChangeLambda lambda = new CarWheelChangeLambda(this, "ExpansionUniversalWheel");
		if (GetGame().IsMultiplayer())
			parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
		//else
			//parent.GetInventory().ReplaceItemWithNew(InventoryMode.LOCAL, lambda );
	}
};
modded class Sedan_02_Wheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		CarWheelChangeLambda lambda = new CarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
modded class CivSedanWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		CarWheelChangeLambda lambda = new CarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
modded class Hatchback_02_Wheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		CarWheelChangeLambda lambda = new CarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionUAZWheel extends CarWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		CarWheelChangeLambda lambda = new CarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionBusWheel extends CarWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		CarWheelChangeLambda lambda = new CarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionBusWheelDouble extends CarWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		CarWheelChangeLambda lambda = new CarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionTractorFrontWheel extends CarWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		CarWheelChangeLambda lambda = new CarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionTractorBackWheel extends CarWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		CarWheelChangeLambda lambda = new CarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
#endif