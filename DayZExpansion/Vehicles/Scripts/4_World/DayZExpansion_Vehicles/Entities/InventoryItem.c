/**
 * InventoryItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (newLevel == GameConstants.STATE_RUINED && IsInherited(CarWheel) && !IsInherited(CarWheel_Ruined))
			//! Do this in next frame so vanilla wheel replacement can run first
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_ReplaceWheelWithRuined);
	}

	string Expansion_GetRuinedWheelType()
	{
		string ruinedWheelType = GetType() + "_Ruined";

		if (GetGame().ConfigIsExisting("CfgVehicles " + ruinedWheelType))
			return ruinedWheelType;

		return string.Empty;
	}

	void Expansion_ReplaceWheelWithRuined()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);
#endif
		
		if (IsSetForDeletion())
		{
			//! Something else has likely already replaced the wheel and marked this one for deletion
			EXTrace.Print(EXTrace.VEHICLES, this, "Wheel is about to be deleted - ignoring");
			return;
		}

		string newWheel = Expansion_GetRuinedWheelType();
		
		if (newWheel != string.Empty)
		{
			//! Unlock to allow creating a new item
			if (IsLockedInSlot())
				UnlockFromParent();
			
			IEntity child = GetChildren();
			while (child)
			{
				RemoveChild(child, false);
				vector matrix[4];
				GetTransform(matrix);
				GetInventory().DropEntityWithTransform(InventoryMode.SERVER, this, EntityAI.Cast(child), matrix);
				child = GetSibling();
			}

			ReplaceWheelLambda lambda = new ReplaceWheelLambda(this, newWheel, null);
			lambda.SetTransferParams(true, true, true);
			GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, lambda);
		}
	}
}

class ExpansionUAZWheel: CarWheel {};
class ExpansionUAZWheel_Ruined: CarWheel_Ruined {};

class ExpansionBusWheel: CarWheel {};
class ExpansionBusWheelDouble: CarWheel {};
class ExpansionBusWheel_Ruined: CarWheel_Ruined {};
class ExpansionBusWheelDouble_Ruined: CarWheel_Ruined {};

class ExpansionTractorFrontWheel: CarWheel {};
class ExpansionTractorBackWheel: CarWheel {};
class ExpansionTractorFrontWheel_Ruined: CarWheel_Ruined {};
class ExpansionTractorBackWheel_Ruined: CarWheel_Ruined {};

class Expansion_Landrover_Wheel: CarWheel {};
class Expansion_Landrover_Wheel_Ruined: CarWheel_Ruined
{
	override void DeferredInit()
	{
		super.DeferredInit();

		//! TODO: Crutch, Expansion_Landrover_Wheel_Ruined doesn't have a proper ruined model + rvmats,
		//! so we force visual damaged state by applying damage equal to wheel health (setting health to zero alone does NOT work)
		if (GetGame().IsServer())
		{
			ProcessDirectDamage(DT_CUSTOM, this, "Tire", "TransportHit", "0 0 0", 800);
			SetHealth(0);
		}
	}
};

class ExpansionHydraulicHoses: ItemBase
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionDetach);
		AddAction(ActionAttach);
	}
};

class ExpansionIgniterPlug: ItemBase
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionDetach);
		AddAction(ActionAttach);
	}
};
