/**
 * ExpansionReflexMRSOptic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionReflexMRSOptic::ExpansionReflexMRSOptic Start");
		#endif
		
		RegisterNetSyncVariableInt( "m_CurrentMode");
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionReflexMRSOptic::ExpansionReflexMRSOptic End");
		#endif
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
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionReflexMRSOptic::UpdateVisualState - Start");
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Reflex Optic UpdateVisualState m_CurrentMode: " + m_CurrentMode); 
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
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionReflexMRSOptic::UpdateVisualState - End");
		#endif			
	}

	void CycleOpticMode()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionReflexMRSOptic::CycleOpticMode - Start");
		#endif
		
		m_CurrentMode++;
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Reflex Optic CycleOpticMode m_CurrentMode: " + m_CurrentMode); 
		#endif
		if ( m_CurrentMode > 3 )
			m_CurrentMode = 0;

		SetSynchDirty();
		
		UpdateVisualState();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionReflexMRSOptic::CycleOpticMode - End");
		#endif		
	}
	
	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionReflexMRSOptic::OnVariablesSynchronized - Start");
		#endif
		
		super.OnVariablesSynchronized();
		
		UpdateVisualState();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionReflexMRSOptic::OnVariablesSynchronized - End");
		#endif		
	}
	
	override void SetActions()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionReflexMRSOptic::SetActions - Start");
		#endif
		
		super.SetActions();		
		AddAction( ExpansionActionCycleOpticsModeInHands );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionReflexMRSOptic::SetActions - End");
		#endif
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif
		
		super.OnStoreSave( ctx );
				
		ctx.Write( m_CurrentMode );
	}

	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef CF_MODULE_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif
		
		if ( Expansion_Assert_False( ctx.Read( m_CurrentMode ), "[" + this + "] Failed reading m_CurrentMode" ) )
			return false;
			
		return true;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave( CF_ModStorage storage, string modName )
	{
		super.CF_OnStoreSave( storage, modName );

		if ( modName != "DZ_Expansion" )
			return;

		storage.Write( m_CurrentMode );
	}
	
	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion" )
			return true;

		if ( Expansion_Assert_False( storage.Read( m_CurrentMode ), "[" + this + "] Failed reading m_CurrentMode" ) )
			return false;

		return true;
	}
	#endif
}