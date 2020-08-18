/**
 * ExpansionGenerator.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionGenerator
 * @brief		
 **/
class ExpansionGenerator: ItemBase
{
	protected ref Timer 		m_SoundLoopStartTimer;

	protected EffectSound 		m_EngineLoop;
	protected EffectSound 		m_EngineStart;
	protected EffectSound 		m_EngineStop;

	static const string			START_SOUND = "powerGeneratorTurnOn_SoundSet";
	static const string			STOP_SOUND = "powerGeneratorTurnOff_SoundSet";
	static const string			LOOP_SOUND = "powerGeneratorLoop_SoundSet";
	static const string 		SPARKPLUG_ATTACH_SOUND = "sparkplug_attach_SoundSet";
	static const string 		SPARKPLUG_DETACH_SOUND = "sparkplug_detach_SoundSet";

	private static float 		m_FuelTankCapacity; 
	private static float 		m_FuelToEnergyRatio; 
	private float 				m_Fuel;
	private int					m_FuelPercentage;

	ItemBase 					m_SparkPlug;

	protected ref Effect 		m_Smoke;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionGenerator()
	{
		SetEventMask(EntityEvent.INIT);

		m_FuelPercentage = 50;

		RegisterNetSyncVariableInt("m_FuelPercentage", 0, 100);
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
		RegisterNetSyncVariableBool("m_IsPlaceSound");
		
		m_SoundLoopStartTimer = new Timer();
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionGenerator()
	{
		m_SoundLoopStartTimer = NULL;
	}

	// ------------------------------------------------------------
	// SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();

		AddAction( ExpansionActionFillGeneratorTankOnGround );
		AddAction( ExpansionActionTurnOnGeneratorOnGround );
		AddAction( ExpansionActionTurnOffGeneratorOnGround );
	}

	// ------------------------------------------------------------
	// Init
	// ------------------------------------------------------------
	override void EOnInit( IEntity other, int extra)
	{
		super.EOnInit( other, extra );
		
		if ( IsMissionHost() )
		{
			m_FuelPercentage = GetCompEM().GetEnergy0To100();
			SetSynchDirty();
		}
		
		UpdateFuelMeter();
	}

	// ------------------------------------------------------------
	// OnWork
	// ------------------------------------------------------------
	override void OnWork( float consumed_energy )
	{
		if ( IsMissionHost() )
		{
			m_FuelPercentage = GetCompEM().GetEnergy0To100();
			SetSynchDirty();
		}
		
		UpdateFuelMeter();
	}

	// ------------------------------------------------------------
	// OnWorkStart
	//! NEED FIX
	// ------------------------------------------------------------
	override void OnWorkStart()
	{
		//super.OnWorkStart();

		if ( !IsMissionHost() )
			return;

		ExpansionWorldMappingModule module;
		if ( Class.CastTo( module, GetModuleManager().GetModule( ExpansionWorldMappingModule ) ) )
		{
			module.TurnOnGenerator( GetPosition() );
		}
	}
	
	// ------------------------------------------------------------
	// OnWorkStop
	//! NEED FIX
	// ------------------------------------------------------------
	override void OnWorkStop()
	{
		//super.OnWorkStop();

		if ( !IsMissionHost() )
			return;

		ExpansionWorldMappingModule module;
		if ( Class.CastTo( module, GetModuleManager().GetModule( ExpansionWorldMappingModule ) ) )
		{
			module.TurnOffGenerator( GetPosition() );
		}
	}

	// ------------------------------------------------------------
	// EEItemAttached
	// ------------------------------------------------------------
	override void EEItemAttached( EntityAI item, string slot_name )
	{
		super.EEItemAttached( item, slot_name );
		
		ItemBase item_IB = ItemBase.Cast( item );
		
		if ( item_IB.IsKindOf("Sparkplug") )
		{
			m_SparkPlug = item_IB;
			
			if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
			{
				SEffectManager.PlaySound(SPARKPLUG_ATTACH_SOUND, GetPosition() );
			}
		}
	}

	// ------------------------------------------------------------
	// EEItemDetached
	// ------------------------------------------------------------
	override void EEItemDetached( EntityAI item, string slot_name )
	{
		super.EEItemDetached( item, slot_name );
		
		ItemBase item_IB = ItemBase.Cast( item );
		
		if ( item_IB.IsKindOf("Sparkplug") )
		{
			m_SparkPlug = NULL;
			GetCompEM().SwitchOff();
			
			if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() )
			{
				SEffectManager.PlaySound(SPARKPLUG_DETACH_SOUND, GetPosition() );
			}
		}

	}

	// ------------------------------------------------------------
	// IsInventoryVisible
	// ------------------------------------------------------------
	override bool IsInventoryVisible()
	{
		return true;
	}

	// ------------------------------------------------------------
	// CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	// ------------------------------------------------------------
	// OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
				
		UpdateFuelMeter();
		
		if ( IsPlaceSound() )
		{
			PlayPlaceSound();
		}
	}

	// ------------------------------------------------------------
	// Expansion UpdateFuelMeter
	// ------------------------------------------------------------
	void UpdateFuelMeter()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			SetAnimationPhase("dial_fuel", m_FuelPercentage * 0.01);
		}
	}

	// ------------------------------------------------------------
	// Expansion SetFuel
	// ------------------------------------------------------------
	void SetFuel(float fuel_amount)
	{
		if (m_FuelTankCapacity > 0)
		{
			m_FuelToEnergyRatio = GetCompEM().GetEnergyMax() / m_FuelTankCapacity;
			GetCompEM().SetEnergy(fuel_amount * m_FuelToEnergyRatio);
			m_FuelPercentage = GetCompEM().GetEnergy0To100();
			SetSynchDirty();
			UpdateFuelMeter();
		}
	}

	// ------------------------------------------------------------
	// Expansion AddFuel
	// ------------------------------------------------------------
	float AddFuel(float available_fuel)
	{
		if ( available_fuel == 0 )
		{
			return 0;
		}
		
		float needed_fuel = GetMaxFuel() - GetFuel();
		
		if ( needed_fuel > available_fuel )
		{
			SetFuel(GetFuel() + available_fuel);
			return available_fuel;
		}
		else
		{
			SetFuel(GetMaxFuel());
			return needed_fuel;
		}
	}

	// ------------------------------------------------------------
	// Expansion CanAddFuel
	// ------------------------------------------------------------
	bool CanAddFuel( ItemBase container )
	{
		if (container)
		{
			int liquid_type	= container.GetLiquidType();
			
			if ( container.GetQuantity() > 0  &&  GetCompEM().GetEnergy() < GetCompEM().GetEnergyMax()  &&  (liquid_type & LIQUID_GASOLINE) )
			{
				return true;
			}
		}
		
		return false;
	}

	// ------------------------------------------------------------
	// Expansion GetFuel
	// ------------------------------------------------------------
	float GetFuel()
	{
		return GetCompEM().GetEnergy() / m_FuelToEnergyRatio;
	}

	// ------------------------------------------------------------
	// Expansion GetMaxFuel
	// ------------------------------------------------------------
	float GetMaxFuel()
	{
		return m_FuelTankCapacity;
	}

	// ------------------------------------------------------------
	// Expansion StartLoopSound
	// ------------------------------------------------------------
	void StartLoopSound()
	{
		if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			if ( GetCompEM().IsWorking() )
			{
				PlaySoundSetLoop( m_EngineLoop, LOOP_SOUND, 0, 0 );
				
				vector local_pos = "0.3 0.21 0.4";
				vector local_ori = "270 0 0";
				m_Smoke = new EffGeneratorSmoke();
				SEffectManager.PlayOnObject(m_Smoke, this, local_pos, local_ori);
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion HasSparkplug
	// ------------------------------------------------------------
	bool HasSparkplug()
	{
		if ( m_SparkPlug )
		{
			if ( !m_SparkPlug.IsRuined() )
			{
				return true;
			}
		}
		
		return false;
	}
}