modded class PlayerBase
{
	protected const ref array<string> DRINKS = {"Bottle_Base", "SodaCan_ColorBase"};
	protected const ref array<string> ALL_RADIATION_SOUNDS = {"Expansion_Geiger1_Soundset", "Expansion_Geiger2_Soundset", "Expansion_Geiger3_Soundset", "Expansion_Geiger4_Soundset", "Expansion_Geiger5_Soundset", "Expansion_Geiger6_Soundset", "Expansion_Geiger7_Soundset", "Expansion_Geiger8_Soundset"};

	protected ref array<ref EffectSound> m_RadiationSounds;
	protected int m_StatRadiation;
	
	override void Init()
	{	
		super.Init();
		
		m_RadiationSounds = new array<ref EffectSound>;

		RegisterNetSyncVariableInt("m_StatRadiation", 0, 100);
	}
	
	override void OnCommandVehicleStart()
	{
		super.OnCommandVehicleStart();

		GameInventory gameInv = GetInventory();
		if (gameInv)
			gameInv.UnlockInventory(LOCK_FROM_SCRIPT);
	}

	override void OnCommandVehicleFinish()
	{
		GameInventory gameInv = GetInventory();
		if (gameInv)
			gameInv.LockInventory(LOCK_FROM_SCRIPT);

		super.OnCommandVehicleFinish();
	}

	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		if (IsInVehicle() && CanPickupHeavyItem(item_to_hands))
			return true;

		return super.CanReceiveItemIntoHands(item_to_hands);
	}
	
	PlayerStat<float> GetStatRadiation()
	{
		if (GetPlayerStats()) 
		{
			PlayerStat<float> stat = PlayerStat<float>.Cast(GetPlayerStats().GetStatObject(EPlayerStatsNamalskAdventure.RADIATION));
			return stat;
		}
		return null;
	}
	
	void ForceUpdateRadiation()
	{
		m_StatRadiation = GetStatRadiation().Get();
	#ifdef NAMALSKADVENTURE_RADIATION_DEBUG
		Print(ToString() + "::ForceUpdateRadiation - Radiation: " + m_StatRadiation);
	#endif
		SetSynchDirty();
	}
	
	int GetClientRadiation()
	{
		return m_StatRadiation;
	}
	
	override bool Consume(ItemBase source, float amount, EConsumeType consume_type )
	{
		if (!super.Consume(source, amount, consume_type))
			return false;
		
		if (ExpansionStatic.IsAnyOf(source.GetType(), DRINKS))
		{
			float radiationCure = amount * 0.1;
			Print(ToString() + "::Consume - Drink amount: " + radiationCure);
			GetStatRadiation().Add(-radiationCure);
			ForceUpdateRadiation();
		}
		
		return true;
	}
	
	override void OnVariablesSynchronized()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		super.OnVariablesSynchronized();

		if (m_StatRadiation > 1)
		{
			UpdateRadiationSound();
		}
		else
		{
			RadiationSoundStop();
		}
	}
	
	protected void UpdateRadiationSound()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		int soundIndex = Math.Clamp(m_StatRadiation / 10, 0, 7);
		string sound = ALL_RADIATION_SOUNDS[soundIndex];

		if (m_RadiationSounds.Count() < soundIndex + 1)
			AddRadiationSound(sound);
		
		if (m_RadiationSounds.Count() < 1)
			AddRadiationSound(sound);
		
		if (m_StatRadiation > 10 && m_RadiationSounds.Count() < 2)
			AddRadiationSound(sound);
		
		if (m_StatRadiation > 30 && m_RadiationSounds.Count() < 3)
			AddRadiationSound(sound);
		
		if (m_StatRadiation > 60 && m_RadiationSounds.Count() < 4)
			AddRadiationSound(sound);
		
		if (m_StatRadiation > 90 && m_RadiationSounds.Count() < 5)
			AddRadiationSound(sound);
	}
	
	protected void AddRadiationSound(string sound)
	{
		EffectSound effectSound;
		PlaySoundSetLoop(effectSound, sound, 0, 0);
		
		if (effectSound)
			m_RadiationSounds.Insert(effectSound);
	}
	
	protected void RadiationSoundStop()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		
		foreach (EffectSound effectSound: m_RadiationSounds)
		{
			StopSoundSet(effectSound);
		}
		
		m_RadiationSounds.Clear();
	}
};