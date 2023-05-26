/**
 * Land_Repair_Center.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PowerGenerator
{
	int GetFuelPercentage()
	{
		return m_FuelPercentage;
	}
};

class RepairBenchGenerator: PowerGenerator
{
	protected Land_Repair_Center m_RepairCenter;
	
	void RepairBenchGenerator()
	{
		
	}
	
	void ~RepairBenchGenerator()
	{
		
	}
	
	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEInit();
		
		InitRepairGenerator();
	}

	void InitRepairGenerator()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!GetGame().IsDedicatedServer())
		{
			InitRepairGeneratorClient();
		}

		if (GetGame().IsServer())
		{
			InitRepairGeneratorServer();
		}
	}
	
	void LinkRepairCenter(Land_Repair_Center repair_center)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, "Repair Center: " + repair_center.ToString());
		
		m_RepairCenter = repair_center;
	}

	protected void InitRepairGeneratorClient()
	{
		
	}

	protected void InitRepairGeneratorServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateDebugMarker, 500, false);
	#endif
	#endif
	}
	
	/*override void EEItemAttached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::EEItemAttached - Item: " + item.ToString() + " | Slot:" + slot_name);
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::EEItemDetached - Item: " + item.ToString() + " | Slot:" + slot_name);
	}*/
	
	override void SetActions()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.SetActions();
		
		AddAction(ActionTurnOnWorkbenchGenerator);
		AddAction(ActionTurnOffWorkbenchGenerator);
		
		RemoveAction(ActionTogglePlaceObject);
		RemoveAction(ActionPullOutPlug);
		RemoveAction(ActionTurnOnPowerGenerator);
		RemoveAction(ActionTurnOffPowerGenerator);
		RemoveAction(ActionPlaceObject);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI player)
	{
		return false;
	}

	// Generator is working
	override void OnWorkStart()
	{
		super.OnWorkStart();
		
		if (m_RepairCenter)
			m_RepairCenter.SetActiveState(true);
	}

	// Turn off when this runs out of fuel
	override void OnWorkStop()
	{
		super.OnWorkStop();
		
		if (m_RepairCenter)
			m_RepairCenter.SetActiveState(false);
	}

	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.AfterStoreLoad();

		GetGame().ObjectDelete(this);
	}
	
#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			markerModule.CreateServerMarker(GetType(), "Scrap Metal", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 44, 62, 80), true);
	}
#endif
#endif
	
	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};

/*class RepairBenchContainer: Container_Base
{
	void RepairBenchContainer()
	{
	}
	
	void ~RepairBenchContainer()
	{
	}
	
	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEInit();

		InitRepairContainer();
	}

	void InitRepairContainer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!GetGame().IsDedicatedServer())
		{
			InitRepairContainerClient();
		}

		if (GetGame().IsServer())
		{
			InitRepairContainerServer();
		}
	}

	protected void InitRepairContainerClient()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}

	protected void InitRepairContainerServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateDebugMarker, 500, false);
	#endif
	#endif
	}

	override bool CanCombineAttachment(notnull EntityAI e, int slot, bool stack_max_limit = false)
	{
		if (!super.CanCombineAttachment(e, slot, stack_max_limit))
			return false;

		return true;
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;

		return true;
	}

	//! NOTE: Despite being a vanilla method, nothing in vanilla checks CanDisplayCargo.
	//! We implement this ourself in 5_Mission/GUI/InventoryNew/Inherited/ContainerWithCargoAndAttachments.
	override bool CanDisplayCargo()
	{
		return true;
	}
	
	override bool CanLoadAttachment(EntityAI attachment)
	{
		return true;
	}
	
	override bool CanLoadItemIntoCargo(EntityAI item)
	{
		return true;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return true;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		return true;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return true;
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!super.CanReleaseAttachment(attachment))
			return false;

		return true;
	}

	override bool CanReleaseCargo(EntityAI cargo)
	{
		return true;
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		if (!super.CanSwapItemInCargo(child_entity, new_entity))
			return false;

		return true;
	}

	//! This hides the container from vicinity completely
	override bool IsInventoryVisible()
	{
		return true;
	}

	override bool IsHealthVisible()
	{
		return false;
	}
	
	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.AfterStoreLoad();

		GetGame().ObjectDelete(this);
	}

#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			markerModule.CreateServerMarker(GetType(), "Pipe Wrench", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 44, 62, 80), true);
	}
#endif
#endif
	
	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};*/

//! @note: Can only be used when the BuildingsModPack mod by Starlv is loaded: https://steamcommunity.com/sharedfiles/filedetails/?id=2270098553
class Land_Repair_Center: House
{
#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	protected ExpansionMarkerData m_ServerMarker;
#endif
#endif

	protected bool m_IsActive;

	protected RepairBenchGenerator m_RepairBenchGenerator;
	protected EntityAI m_Clutter;

	void Land_Repair_Center()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		SetEventMask(EntityEvent.INIT);
		
		RegisterNetSyncVariableBool("m_IsActive");
	}

	void ~Land_Repair_Center()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
		if (!m_ServerMarker)
			return;

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			markerModule.RemoveServerMarker(m_ServerMarker.GetUID());
	#endif
	#endif
		
		if (m_RepairBenchGenerator)
			GetGame().ObjectDelete(m_RepairBenchGenerator);
	}
	
	//! Called externaly from Namalsk module after object creation
	void InitRepairBuilding()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!GetGame().IsDedicatedServer())
		{
			InitRepairBuildingClient();
		}

		if (GetGame().IsServer())
		{
			InitRepairBuildingServer();
		}
	}

	protected void InitRepairBuildingClient()
	{
		
	}

	protected void InitRepairBuildingServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Object objGen = ExpansionWorldObjectsModule.SpawnObject("RepairBenchGenerator", GetPosition() + "-3.0 -2.5 -1.0", Vector(117.000038, 0.000000, 0.000000), false, false);
		if (!Class.CastTo(m_RepairBenchGenerator, objGen))
			GetGame().ObjectDelete(objGen);
		
		m_RepairBenchGenerator.LinkRepairCenter(this);

		Object objClutter = ExpansionWorldObjectsModule.SpawnObject("ClutterCutter6x6", GetPosition(), Vector(117.000038, 0.000000, 0.000000), false, false);
		if (!Class.CastTo(m_Clutter, objClutter))
			GetGame().ObjectDelete(objClutter);
		
	#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateDebugMarker, 500, false);
	#endif
	#endif
	}

	RepairBenchGenerator GetGenerator()
	{
		return m_RepairBenchGenerator;
	}
	
	bool GetActiveState()
	{
		return m_IsActive;
	}
	
	void SetActiveState(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		m_IsActive = state;
		SetSynchDirty();
	}

	override void SetActions()
	{
		super.SetActions();

        AddAction(ExpansionActionUseRepairCenter);
	}
	
	override void OnVariablesSynchronized()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnVariablesSynchronized();
	}

#ifdef DIAG
#ifdef EXPANSIONMODNAVIGATION
	void CreateDebugMarker()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionMarkerModule markerModule;
		CF_Modules<ExpansionMarkerModule>.Get(markerModule);
		if (markerModule)
			markerModule.CreateServerMarker(GetType(), "Tent", Vector(GetPosition()[0], GetPosition()[1] + 1.0, GetPosition()[2]), ARGB(255, 44, 62, 80), true);
	}
#endif
#endif
	
	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};
