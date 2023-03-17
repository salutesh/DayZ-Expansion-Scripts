#ifndef EXPANSION_UNIVERSAL_WHEEL_DISABLE
//TODO: Banana must add support for ExpansionVehicleBase as well
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
			parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, new ExpansionCarWheelChangeLambda(this, wheelType));
	//	else
		//	parent.GetInventory().ReplaceItemWithNew(InventoryMode.LOCAL, new ExpansionCarWheelChangeLambda(this, wheelType));
		

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
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
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
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
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
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
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
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
modded class Truck_01_Wheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
modded class Truck_01_WheelDouble
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionUAZWheel: CarWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionBusWheel: CarWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionBusWheelDouble: CarWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionTractorFrontWheel: CarWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionTractorBackWheel: CarWheel
{
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		super.OnWasDetached(parent, slot_id);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ConvertWheel, 500, false, parent, slot_id);
	}
	void ConvertWheel(EntityAI parent, int slot_id)
	{		
		ExpansionCarWheelChangeLambda lambda = new ExpansionCarWheelChangeLambda(this, "ExpansionUniversalWheel");
		parent.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda );
	}
};
class ExpansionBusWheel_Ruined: CarWheel_Ruined {};
class ExpansionBusWheelDouble_Ruinedel: CarWheel_Ruined {};
#endif