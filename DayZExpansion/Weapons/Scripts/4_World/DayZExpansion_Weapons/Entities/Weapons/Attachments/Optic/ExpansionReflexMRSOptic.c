/**
 * ExpansionReflexMRSOptic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionReflexMRSOptic: ItemOptics
{
	protected int m_CurrentMode = 3;

	void ExpansionReflexMRSOptic()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "ExpansionReflexMRSOptic");
#endif

		RegisterNetSyncVariableInt( "m_CurrentMode");
	}
	
	// Inventory manipulation
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		if ( GetCompEM().IsWorking() )
		{
			if (player)
			{
				vector ori_rotate = player.GetOrientation();
				ori_rotate = ori_rotate + Vector(270,0,0);
				SetOrientation(ori_rotate);
			}
		}
	}
	
	protected void UpdateVisualState()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "UpdateVisualState");
#endif

		if ( m_CurrentMode == 0)
		{
			SetAnimationPhase("Mode0",0);
			SetAnimationPhase("Mode1",1);
			SetAnimationPhase("Mode2",1);
			SetAnimationPhase("Mode3",1);
			SetAnimationPhase("SwitchMode1",0);
			SetAnimationPhase("SwitchMode2",0);
			SetAnimationPhase("SwitchMode3",0);
			
		}
		if ( m_CurrentMode == 1)
		{
			SetAnimationPhase("Mode0",1);
			SetAnimationPhase("Mode1",0);
			SetAnimationPhase("Mode2",1);
			SetAnimationPhase("Mode3",1);
			SetAnimationPhase("SwitchMode1",1);
			SetAnimationPhase("SwitchMode2",0);
			SetAnimationPhase("SwitchMode3",0);
		}
		if ( m_CurrentMode == 2)
		{
			SetAnimationPhase("Mode0",1);
			SetAnimationPhase("Mode1",1);
			SetAnimationPhase("Mode2",0);
			SetAnimationPhase("Mode3",1);
			SetAnimationPhase("SwitchMode1",0);
			SetAnimationPhase("SwitchMode2",1);
			SetAnimationPhase("SwitchMode3",0);			
		}
		if ( m_CurrentMode == 3)
		{
			SetAnimationPhase("Mode0",1);
			SetAnimationPhase("Mode1",1);
			SetAnimationPhase("Mode2",1);
			SetAnimationPhase("Mode3",0);
			SetAnimationPhase("SwitchMode1",0);
			SetAnimationPhase("SwitchMode2",0);
			SetAnimationPhase("SwitchMode3",1);
		}		
	}

	void CycleOpticMode()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "CycleOpticMode");
#endif
		
		m_CurrentMode++;

		if ( m_CurrentMode > 3 )
			m_CurrentMode = 0;

		SetSynchDirty();
		
		UpdateVisualState();
	}
	
	override void OnVariablesSynchronized()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "OnVariablesSynchronized");
#endif
		
		super.OnVariablesSynchronized();
		
		UpdateVisualState();
	}
	
	override void SetActions()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "SetActions");
#endif
		
		super.SetActions();		

		AddAction( ExpansionActionCycleOpticsModeInHands );
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Weapons];
		if (!ctx) return;

		ctx.Write(m_CurrentMode);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Weapons];
		if (!ctx) return true;

		if (!ctx.Read(m_CurrentMode))
			return false;

		return true;
	}
	#endif
};
