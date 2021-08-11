class ExpansionTripwireTrap extends TrapBase
{
	int m_State;
	static const int FOLDED = 3;
	static const int DEPLOYED = 2;
	static const int TRIGGERED = 1;
		
	private const int BLEED_SOURCE_PROB = 50;
	private const int MAX_BLEED_SOURCE = 1;
	
	private ref array<int> m_ClothingDmg;
	
	private string m_DefaultAttachment = "FlashGrenade";
	
	void ExpansionTripwireTrap()
	{
		//EXLogPrint("ExpansionTripwireTrap::ExpansionTripwireTrap - Start");
		
		m_State = FOLDED;
		m_DamagePlayers = 0; 													//How much damage player gets when caught
		m_DamageOthers = 0; 													//How much damage player gets when caught
		m_InitWaitTime = 1; 														//After this time after deployment, the trap is activated
		m_DefectRate = 0; 														//Added damage after trap activation
		m_NeedActivation = false;
		m_AnimationPhaseGrounded = "inventory";
		m_AnimationPhaseSet = "placing";
		m_AnimationPhaseTriggered = "TripWire_triggered";
		m_InfoActivationTime = string.Format("#STR_TripwireTrap0%1#STR_TripwireTrap1", m_InitWaitTime.ToString());  // Dynamic expressions out of function do not work
		m_AddActivationDefect = false;
		m_AddDeactivationDefect = true;
		
		//Order is important and must match clothing array in DamageClothing method
		m_ClothingDmg = new array<int>;
		m_ClothingDmg.Insert(10); 	//Trousers
		m_ClothingDmg.Insert(0);	//BackPack
		m_ClothingDmg.Insert(0);	//Vest
		m_ClothingDmg.Insert(0);	//HeadGear
		m_ClothingDmg.Insert(0);	//Mask
		m_ClothingDmg.Insert(0);	//Body
		m_ClothingDmg.Insert(10);	//Feet
		m_ClothingDmg.Insert(0);	//Gloves
		
		//EXLogPrint("ExpansionTripwireTrap::ExpansionTripwireTrap - End");
	}
		
	override void StartActivate(PlayerBase player)
	{
		//EXLogPrint("ExpansionTripwireTrap::StartActivate - Start");
		
		super.StartActivate( player );
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			EffectSound sound = SEffectManager.PlaySound("landmine_safetyPin_SoundSet", GetPosition(), 0, 0, false);
			sound.SetSoundAutodestroy( true );
		}
		
		//EXLogPrint("ExpansionTripwireTrap::StartActivate - End");
	}
	
	override void OnActivate()
	{
		//EXLogPrint("ExpansionTripwireTrap::OnActivate - Start");
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{			
			if (GetGame().GetPlayer())
			{
				PlaySoundActivate();
			}
		}
		
		//EXLogPrint("ExpansionTripwireTrap::OnActivate - End");
	}
	
	override void OnSteppedOn(EntityAI victim)
	{
		//EXLogPrint("ExpansionTripwireTrap::OnSteppedOn - Start");
		
		TriggerTrap(victim);
		
		//EXLogPrint("ExpansionTripwireTrap::OnSteppedOn - End");
	}
	
	void TriggerTrap(EntityAI victim)
	{
		//EXLogPrint("ExpansionTripwireTrap::TriggerTrap - Start");
		
		ItemBase attachment;
		
		if (GetGame().IsServer())
		{
			if (victim)
			{
				//! Check if we have a player
				PlayerBase victim_PB = PlayerBase.Cast(victim);
				if (victim_PB && victim_PB.IsAlive())
				{
					int randNum = Math.RandomInt(0, 100); //! Value used for probability evaluation
					if (randNum < BLEED_SOURCE_PROB)
					{
						for (int i = 0; i < MAX_BLEED_SOURCE; i++)
						{
							//! We add two bleeding sources max to lower half
							randNum = Math.RandomIntInclusive(0, PlayerBase.m_BleedingSourcesLow.Count() - 1);
		
							victim_PB.m_BleedingManagerServer.AttemptAddBleedingSourceBySelection(PlayerBase.m_BleedingSourcesLow[randNum]);
						}
					}
					
					DamageClothing(victim_PB);
				}
			}
			
			DecreaseHealth(10, false);
		}
		else if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			if (victim)
			{
				if (GetInventory().AttachmentCount() > 0)
				{
					attachment = ItemBase.Cast(GetInventory().GetAttachmentFromIndex(0));
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
			}
		}
		
		UpdateProxySelections();
		
		//EXLogPrint("ExpansionTripwireTrap::TriggerTrap - End");
	}
		
	void SetState(int state_ID)
	{
		m_State = state_ID;
	}
	
	int GetState()
	{
		return m_State;
	}
	
	void DeleteThis()
	{
		GetGame().ObjectDelete( this );
	}
	
	void DamageClothing(PlayerBase player)
	{
		//Get all currently equipped clothing
		// ---------------------------------------------

		ClothingBase trousers = 	ClothingBase.Cast(player.GetItemOnSlot("LEGS"));
		ClothingBase bag = ClothingBase.Cast(player.GetItemOnSlot("BACK"));
		ClothingBase vest = ClothingBase.Cast(player.GetItemOnSlot("VEST"));
		ClothingBase headGear = ClothingBase.Cast(player.GetItemOnSlot("HeadGear"));
		ClothingBase mask = ClothingBase.Cast(player.GetItemOnSlot("Mask"));
		ClothingBase shirt = ClothingBase.Cast(player.GetItemOnSlot("BODY"));
		ClothingBase shoes = ClothingBase.Cast(player.GetItemOnSlot("FEET"));
		ClothingBase gloves = ClothingBase.Cast(player.GetItemOnSlot("GLOVES"));

		//Array used to find all relevant information about currently equipped clothes
		array<ClothingBase> equippedClothes = new array<ClothingBase>;

		equippedClothes.Insert(trousers);
		equippedClothes.Insert(bag);
		equippedClothes.Insert(vest);
		equippedClothes.Insert(headGear);
		equippedClothes.Insert(mask);
		equippedClothes.Insert(shirt);
		equippedClothes.Insert(shoes);
		equippedClothes.Insert(gloves);

		// -----------------------------------------------
		
		int nbClothes = 0;

		//Damage all currently equipped clothes
		for ( int i = 0; i < equippedClothes.Count(); i++ )
		{
			//If no item is equipped on slot, slot is ignored
			if (equippedClothes[i] == null)
				continue;

			equippedClothes[i].DecreaseHealth(m_ClothingDmg[i], false);
			nbClothes++;
		}
	}
	
	void UpdateProxySelections()
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
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			EffectSound sound = SEffectManager.PlaySound("Grenade_unpin_SoundSet", GetPosition(), 0, 0, false);
			sound.SetSoundAutodestroy(true);
		}
	}
	
	void PlaySoundAttachGranade()
	{
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			EffectSound sound = SEffectManager.PlaySound("Grenade_pin_SoundSet", GetPosition(), 0, 0, false);
			sound.SetSoundAutodestroy(true);
		}
	}
	
	void PlaySoundActivate()
	{
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			EffectSound sound = SEffectManager.PlaySound("landmineActivate_SoundSet", GetPosition(), 0, 0, false);
			sound.SetSoundAutodestroy(true);
		}
	}
			
	override void EEKilled(Object killer)
	{
		//EXLogPrint("ExpansionTripwireTrap::EEKilled - Start");
		
		super.EEKilled(killer);
		
		OnSteppedOn(NULL);
		
		//EXLogPrint("ExpansionTripwireTrap::EEKilled - End");
	}
	
	override void OnVariablesSynchronized()
    {
		//EXLogPrint("ExpansionTripwireTrap::OnVariablesSynchronized - Start");
		
        super.OnVariablesSynchronized();
		
		//EXLogPrint("ExpansionTripwireTrap::OnVariablesSynchronized - End");
	}
	
	override void CreateTrigger()
	{
		//EXLogPrint("ExpansionTripwireTrap::CreateTrigger - Start");
		
		m_TrapTrigger = TrapTrigger.Cast(GetGame().CreateObject( "TrapTrigger", this.GetPosition(), false));
		vector mins = "-0.75 0.3 -0.01";
		vector maxs = "0.75 0.32 0.01";
		m_TrapTrigger.SetOrientation(this.GetOrientation());
		m_TrapTrigger.SetExtents(mins, maxs);	
		m_TrapTrigger.SetParentObject(this);
		
		//EXLogPrint("ExpansionTripwireTrap::CreateTrigger - End");
	}
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner) 
	{
		//EXLogPrint("ExpansionTripwireTrap::OnItemLocationChanged - Start");
		
		super.OnItemLocationChanged(old_owner, new_owner);
		
		FoldTripWire();
		
		UpdateProxySelections();
		
		//EXLogPrint("ExpansionTripwireTrap::OnItemLocationChanged - End");
	}
	
	override void SetupTrap()
	{ 
		if ( GetGame().IsServer() )
		{
			if ( this.GetHierarchyRootPlayer() && this.GetHierarchyRootPlayer().CanDropEntity( this) )  // kvoli desyncu
			{
				SetupTrapPlayer( PlayerBase.Cast( this.GetHierarchyRootPlayer() ) );
			}
		}
	}
	
	override void SetupTrapPlayer(PlayerBase player, bool set_position = true)
	{
		//EXLogPrint("ExpansionTripwireTrap::SetupTrapPlayer - Start");
		
		super.SetupTrapPlayer(player, set_position);
		
		SetState(DEPLOYED);
		
		//EXLogPrint("ExpansionTripwireTrap::SetupTrapPlayer - End");
	}
	
	override void StartDeactivate(PlayerBase player)
	{
		//EXLogPrint("ExpansionTripwireTrap::StartDeactivate - Start");
		
		super.StartDeactivate(player);
		
		SetState(FOLDED);
		
		//EXLogPrint("ExpansionTripwireTrap::StartDeactivate - End");
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		//EXLogPrint("ExpansionTripwireTrap::EEItemAttached - Start");
		
		super.EEItemAttached(item, slot_name);
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())	
		{
			PlaySoundAttachGranade();
		}
		
		UpdateProxySelections();
		
		//EXLogPrint("ExpansionTripwireTrap::EEItemAttached - End");
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		//EXLogPrint("ExpansionTripwireTrap::EEItemDetached - Start");
		
		super.EEItemDetached(item, slot_name);
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			PlaySoundAttachGranade();
		}
		
		UpdateProxySelections();
		
		//EXLogPrint("ExpansionTripwireTrap::EEItemDetached - End");
	}
	
	void FoldTripWire()
	{
		//EXLogPrint("ExpansionTripwireTrap::FoldTripWire - Start");
		
		if (m_AnimationPhaseGrounded != "")
		{
			SetAnimationPhase(m_AnimationPhaseSet, 1);
			
			if (m_AnimationPhaseTriggered != m_AnimationPhaseGrounded) 
			{
				SetAnimationPhase(m_AnimationPhaseTriggered, 1);
			}
			
			SetAnimationPhase(m_AnimationPhaseGrounded, 0);
		}
		
		//EXLogPrint("ExpansionTripwireTrap::FoldTripWire - End");
	}

	override string GetLoopDeploySoundset()
	{
		return "landmine_deploy_SoundSet";
	}
	
	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		//EXLogPrint("ExpansionTripwireTrap::OnPlacementComplete - Start");
		
		super.OnPlacementComplete(player, position, orientation);
			
		SetIsPlaceSound(true);
		
		if (GetGame().IsServer())
		{
			PlayerBase player_PB = PlayerBase.Cast(player);
			
			StartActivate(player_PB);
		}
		
		//EXLogPrint("ExpansionTripwireTrap::OnPlacementComplete - End");
	}
	
	override bool IsDeployable()
	{
		return true;
	}
	
	override bool CanBeClapped()
	{
		return true;
	}
	
	override bool CanBeDisarmed()
	{
		return true;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		//AddAction(ActionActivateTrap);
		AddAction(ActionDeployObject);
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		//EXLogPrint("ExpansionTripwireTrap::OnStoreSave - Start");
		
		super.OnStoreSave(ctx);
		
		//EXLogPrint("ExpansionTripwireTrap::OnStoreSave - End");
	}
	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{   
		//EXLogPrint("ExpansionTripwireTrap::OnStoreLoad - Start");
		
		if (!super.OnStoreLoad(ctx, version))
			return false;
		
		//EXLogPrint("ExpansionTripwireTrap::OnStoreLoad - End");
		
		return true;
	}
};