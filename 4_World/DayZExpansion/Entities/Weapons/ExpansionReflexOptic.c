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
	
	/*override void OnWorkStart()
	{
		if ( !GetGame().IsServer() || !GetGame().IsMultiplayer() ) // Client side
		{
			
		}
	}*/
	
	/*override void OnWork( float consumed_energy )
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{
			Battery9V battery = Battery9V.Cast( GetCompEM().GetEnergySource() );
			
			if (battery)
			{
				float efficiency = battery.GetEfficiency0To1();
				
				if ( efficiency < 1 )
				{
					//m_Light.SetIntensity( efficiency, GetCompEM().GetUpdateInterval() );
				}
				else
				{
					//m_Light.SetIntensity( 1, 0 );
				}
			}
		}
	}*/
	
	/*override void OnWorkStop()
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{

		}
	}*/
	
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
		Print("Reflex Optic UpdateVisualState m_CurrentMode: " + m_CurrentMode); 
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
		Print("Reflex Optic CycleOpticMode m_CurrentMode: " + m_CurrentMode); 
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
		super.OnStoreSave( ctx );
				
		ctx.Write( m_CurrentMode );
	}
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
		{
			return false;
		}
		
		if ( !ctx.Read( m_CurrentMode ) )
		{
			return false;
		}

		return true;
	}
}