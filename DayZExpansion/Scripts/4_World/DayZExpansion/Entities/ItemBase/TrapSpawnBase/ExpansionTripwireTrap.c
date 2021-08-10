class ExpansionTripwireTrap : TripwireTrap
{	
	void ExpansionTripwireTrap()
	{
		m_State = FOLDED;
		m_DamagePlayers = 0; 													//How much damage player gets when caught
		m_DamageOthers = 0; 													//How much damage player gets when caught
		m_InitWaitTime = 1; 														//After this time after deployment, the trap is activated
		m_DefectRate = 0; 													//Added damage after trap activation
		m_NeedActivation = false;
		m_AnimationPhaseGrounded = "inventory";
		m_AnimationPhaseSet = "placing";
		m_AnimationPhaseTriggered = "TripWire_triggered";
		m_InfoActivationTime = string.Format("#STR_TripwireTrap0%1#STR_TripwireTrap1", m_InitWaitTime.ToString());  // nefunguje dynamicke vyrazy mimo funkcii	
		m_AddActivationDefect = true;
	}
	
	override void OnVariablesSynchronized()
    {
        super.OnVariablesSynchronized();
		
		if ( IsDeploySound() )
		{
			PlayDeploySound();
		}
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())	
		{
			PlaySoundAttachGranade();
		}
		
		UpdateProxySelections();
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			PlaySoundAttachGranade();
		}
		
		UpdateProxySelections();
	}
	
	override void OnSteppedOn(EntityAI victim)
	{
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())	
		{
			TriggerTrap(victim);
		}
		else if (GetGame().IsMultiplayer() || GetGame().IsServer())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().ObjectDelete, 18000, false, this);
		}
	}
	
	void TriggerTrap(EntityAI victim)
	{
		if (GetInventory().AttachmentCount() > 0)
		{
			ItemBase attachment = ItemBase.Cast(GetInventory().GetAttachmentFromIndex(0));
			if (attachment)
			{
				if (attachment.IsInherited(Grenade_Base))
				{
					Grenade_Base.Cast(attachment).Unpin();
					GetInventory().DropEntity(InventoryMode.LOCAL, this, attachment);
				}
				else
				{
					attachment.OnActivatedByTripWire();
					GetInventory().DropEntity(InventoryMode.LOCAL, this, attachment);
				}
				
				PlayerBase victimPB = PlayerBase.Cast(victim);
				if (victimPB)
				{
					PlaySoundTriggered();
				}
				else if (victim.IsInherited(DayZCreatureAI))
				{
					PlaySoundTriggered();
				}
			}
		}
		
		SetState(TRIGGERED);
		UpdateProxySelections();
	}
	
	override void UpdateProxySelections()
	{
		if (GetInventory().AttachmentCount() > 0)
		{
			ItemBase attachment = ItemBase.Cast(GetInventory().GetAttachmentFromIndex(0));
			
			if (attachment)
			{
				string att_model = attachment.GetModelName();
				if (att_model != "")
				{
					// Hide all proxies
					for (int i = 1; i <= 3; i++)
					{
						HideSelection("s" + i + "_flashbang");
						HideSelection("s" + i + "_frag_RGD5");
						HideSelection("s" + i + "_grenade");
						HideSelection("s" + i + "_Road_flare");
						HideSelection("s" + i + "_smokegrenade");
						HideSelection("s" + i + "_smokegrenade_rdg2");
						HideSelection("s" + i + "_SodaCan_Used");
					}
					
					HideSelection("TripWire_set_grenade");
					HideSelection("TripWire_set_can");
					HideSelection("TripWire_triggered");
					HideSelection("user");
					HideSelection("inventory");
				}
			}
		}
	}
	
	void PlaySoundTriggered()
	{
		EffectSound sound = SEffectManager.PlaySound("Grenade_unpin_SoundSet", GetPosition(), 0, 0, false);
		sound.SetSoundAutodestroy( true );
	}
	
	void PlaySoundAttachGranade()
	{
		EffectSound sound = SEffectManager.PlaySound("Grenade_pin_SoundSet", GetPosition(), 0, 0, false);
		sound.SetSoundAutodestroy( true );
	}
		
	override void EEKilled( Object killer )
	{
		super.EEKilled( killer );
		
		OnSteppedOn( NULL );
	}
	
	override string GetDeploySoundset()
	{
		return "placeTripwireTrap_SoundSet";
	}
	
	override string GetPlaceSoundset()
	{
		return "putDown_FenceKit_SoundSet";
	}
	
	override string GetLoopDeploySoundset()
	{
		return "tripwire_deploy_SoundSet";
	}
	
	override string GetDeployFinishSoundset()
	{
		return "";
	}
}
