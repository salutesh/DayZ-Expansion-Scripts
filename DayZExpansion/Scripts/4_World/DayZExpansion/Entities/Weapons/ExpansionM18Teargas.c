class Expansion_M18SmokeGrenade_Teargas: M18SmokeGrenade_White
{
	int CoughTimer = 1000;	
	bool ZoneActive = false;
	override void OnWorkStart()
	{
		super.OnWorkStart();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M18SmokeGrenade_Teargas::OnWorkStart Start");
		#endif

		ZoneActive = true;

		//! m_Delay = new Timer;
		//! m_Delay.Run(2, this, "MakePlayerCough", null, false);	

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(MakePlayerCough, 2000, false);

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M18SmokeGrenade_Teargas::OnWorkStart End");
		#endif
	}
	void MakePlayerCough()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M18SmokeGrenade_Teargas::MakePlayerCough Start");
		#endif
		
		CoughTimer = Math.RandomInt(1000, 5000);
		ref array<Object> nearest_objects = new array<Object>;
		ref array<CargoBase> proxy_cargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D( this.GetPosition(), 5, nearest_objects, proxy_cargos );
		
		for ( int i = 0; i < nearest_objects.Count(); i++ )
		{
			Object nearest_object = nearest_objects.Get(i);
			
			if ( nearest_object.IsInherited( PlayerBase ) )
			{

				PlayerBase player = PlayerBase.Cast( nearest_object );
				if ( player )
				{
					EntityAI foundMask = player.FindAttachmentBySlotName("Mask"); 
					if ( foundMask != NULL) 
					{
						if ( !foundMask.IsKindOf("GasMask") && !foundMask.IsKindOf("GP5GasMask") && !foundMask.IsKindOf("Expansion_Gasmask")  )
						{
							player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_COUGH);
							player.AddHealth("","Shock", -20);
						}
					}
					else 
					{
							player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_COUGH);
							player.AddHealth("","Shock", -20);
					}
				}
			}	
		}

		if ( ZoneActive )
		{

			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(MakePlayerCough, CoughTimer, false);
		}
		
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M18SmokeGrenade_Teargas::MakePlayerCough End");
		#endif

	}
	override void OnWorkStop()
	{
		super.OnWorkStop();
		ZoneActive = false;
	}	
};