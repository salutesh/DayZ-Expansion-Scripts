/**
 * ExpansionVehicleActionStartEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleActionStartEngine : ActionContinuousBase
{
	private	const float ROUGH_SPECIALTY_WEIGHT = 0.5;
	static const float MINIMUM_BATTERY_ENERGY = 5.0;

	bool m_BatteryCon = false;
	bool m_SparkCon = false;
	bool m_BeltCon = false;
	bool m_FuelCon = false;

	private	ExpansionVehicleBase m_Vehicle;

	void ExpansionVehicleActionStartEngine()
	{
		m_CallbackClass = ActionStartCarCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
		m_LockTargetOnUse = false;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "Start " + m_Vehicle.EngineGetName() + " Engine";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionHumanCommandVehicle vehCmd = player.GetCommand_ExpansionVehicle();

		if (vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
		{
			m_Vehicle = vehCmd.GetExpansionVehicle();
			if (m_Vehicle && !m_Vehicle.EngineIsOn())
			{
				m_CommandUID = m_Vehicle.EngineStartAnimation();

				if (m_Vehicle.GetHealthLevel("Engine") >= GameConstants.STATE_RUINED)
					return false;

				if (m_Vehicle.CrewMemberIndex(player) == DayZPlayerConstants.VEHICLESEAT_DRIVER)
				{
					if (m_Vehicle.GetLockedState() == ExpansionVehicleLockState.FORCEDUNLOCKED)
						return true;

					if (!m_Vehicle.HasKey())
						return true;

					ExpansionCarKey key;
					if (GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 1)
					{
						return player.HasKeyForCar(m_Vehicle);
					}
					else if (GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 2)
					{
						if (!Class.CastTo(key, player.GetItemInHands()))
							return false;

						if (!m_Vehicle.IsCarKeys(key))
							return false;

						return true;
					}

					//! GetExpansionSettings().GetVehicle().VehicleRequireKeyToStart == 0, or invalid number but this still saves checking
					return true;
				}
			}
		}

		return false;
	}

	override void OnStartServer(ActionData action_data)
	{
		ExpansionHumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_ExpansionVehicle();

		if (vehCommand)
		{
			ExpansionVehicleBase car = vehCommand.GetExpansionVehicle();
			if (car)
			{
				m_BatteryCon = false;
				m_SparkCon = false;
				m_BeltCon = false;
				m_FuelCon = false;

				EntityAI item = null;
				if (car.IsVitalCarBattery() || car.IsVitalTruckBattery() || car.IsVitalHelicopterBattery() || car.IsVitalAircraftBattery())
				{
					if (car.IsVitalCarBattery())
						item = car.FindAttachmentBySlotName("CarBattery");
					if (car.IsVitalTruckBattery())
						item = car.FindAttachmentBySlotName("TruckBattery");
					if (car.IsVitalHelicopterBattery())
						item = car.FindAttachmentBySlotName("ExpansionHelicopterBattery");
					if (car.IsVitalAircraftBattery())
						item = car.FindAttachmentBySlotName("ExpansionAircraftBattery");
					if (item && !item.IsRuined() && item.GetCompEM().GetEnergy() > MINIMUM_BATTERY_ENERGY)
						m_BatteryCon = true;
				}
				else
				{
					m_BatteryCon = true;
				}

				item = null;
				if (car.IsVitalSparkPlug() || car.IsVitalGlowPlug() || car.IsVitalIgniterPlug())
				{
					if (car.IsVitalIgniterPlug())
						item = car.FindAttachmentBySlotName("ExpansionIgniterPlug");
					if (car.IsVitalSparkPlug())
						item = car.FindAttachmentBySlotName("SparkPlug");
					if (car.IsVitalGlowPlug())
						item = car.FindAttachmentBySlotName("GlowPlug");
					if (item && !item.IsRuined())
						m_SparkCon = true;
				}
				else
				{
					m_SparkCon = true;
				}

				//TODO:: make it proper when the belt will be ready
				item = null;
				m_BeltCon = true;
				if (car.IsVitalEngineBelt() || car.IsVitalHydraulicHoses())
				{
					if (car.IsVitalGlowPlug())
						item = car.FindAttachmentBySlotName("EngineBelt");
					if (car.IsVitalHydraulicHoses())
						item = car.FindAttachmentBySlotName("ExpansionHydraulicHoses");
					if (item && !item.IsRuined())
						m_BeltCon = true;
				}
				else
				{
					m_BeltCon = true;
				}

				float fuel = car.GetFluidFraction(CarFluid.FUEL);
				if (fuel > 0)
					m_FuelCon = true;

				if (!GetGame().IsDedicatedServer())
				{
					//Now we can play the proper sound
					EffectSound effectSound = NULL;

					if (m_FuelCon && m_BeltCon && m_SparkCon && m_BatteryCon)
					{
						effectSound = SEffectManager.CreateSound(car.m_EngineStartOK, car.GetPosition());
					}
					else
					{
						if (!m_BatteryCon)
							effectSound = SEffectManager.CreateSound(car.m_EngineStartBattery, car.GetPosition());
						else if (!m_SparkCon)
							effectSound = SEffectManager.CreateSound(car.m_EngineStartPlug, car.GetPosition());
						else if (!m_FuelCon)
							effectSound = SEffectManager.CreateSound(car.m_EngineStartFuel, car.GetPosition());
					}

					WaveKind waveKind = WaveKind.WAVEEFFECT;

					if (action_data.m_Player.GetParent() == NULL || !action_data.m_Player.IsCameraInsideVehicle())
					{
						waveKind = WaveKind.WAVEEFFECTEX;
					}

					if (effectSound != NULL)
					{
						effectSound.SetSoundWaveKind(waveKind);
						effectSound.SoundPlay();
					}
				}
			}
		}
	}

	override void OnStartClient(ActionData action_data)
	{
		OnStartServer(action_data);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		ExpansionVehicleBase vehicle = ExpansionVehicleBase.Cast(action_data.m_Player.GetParent());
		if (vehicle && m_FuelCon && m_BeltCon && m_SparkCon && m_BatteryCon)
			vehicle.EngineStart();
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}
};
