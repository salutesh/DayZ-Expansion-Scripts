/**
 * ExpansionActionStartMotor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionStartMotor : ActionContinuousBase
{
	private	const float ROUGH_SPECIALTY_WEIGHT = 0.5;
	static const float MINIMUM_BATTERY_ENERGY = 5.0;

	bool m_BatteryCon = false;
	bool m_SparkCon = false;
	bool m_BeltCon = false;
	bool m_FuelCon = false;

	void ExpansionActionStartMotor()
	{
		m_CallbackClass = ActionStartCarCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
		m_LockTargetOnUse = false;
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTNone;
		m_ConditionItem = new CCINone;
	}

	override string GetText()
	{
		return "#STR_EXPANSION_UA_START_BOAT";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		if (vehCommand)
		{
			Transport trans = vehCommand.GetTransport();

			if (trans)
			{
				ExpansionBoatScript m_Boat;
				if (Class.CastTo(m_Boat, trans) && !m_Boat.MotorIsOn())
				{
					if (m_Boat.GetHealthLevel("Engine") >= GameConstants.STATE_RUINED)
						return false;

					if (m_Boat.CrewMemberIndex(player) == DayZPlayerConstants.VEHICLESEAT_DRIVER)
						return true;
				}
			}
		}

		return false;
	}

	override void OnStartServer(ActionData action_data)
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();

		if (vehCommand)
		{
			Transport trans = vehCommand.GetTransport();
			if (trans)
			{
				ExpansionBoatScript m_Boat;
				if (Class.CastTo(m_Boat, trans))
				{
					m_BatteryCon = false;
					m_SparkCon = false;
					m_BeltCon = false;
					m_FuelCon = false;

					EntityAI item = null;
					if (m_Boat.IsVitalCarBattery() || m_Boat.IsVitalTruckBattery())
					{

						if (m_Boat.IsVitalCarBattery())
							item = m_Boat.FindAttachmentBySlotName("CarBattery");
						if (m_Boat.IsVitalTruckBattery())
							item = m_Boat.FindAttachmentBySlotName("TruckBattery");
						if (item && !item.IsRuined() && item.GetCompEM().GetEnergy() > MINIMUM_BATTERY_ENERGY)
							m_BatteryCon = true;
					}
					else
					{
						m_BatteryCon = true;
					}

					item = null;
					if (m_Boat.IsVitalSparkPlug() || m_Boat.IsVitalGlowPlug())
					{

						if (m_Boat.IsVitalSparkPlug())
							item = m_Boat.FindAttachmentBySlotName("SparkPlug");
						if (m_Boat.IsVitalGlowPlug())
							item = m_Boat.FindAttachmentBySlotName("GlowPlug");
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
					if (m_Boat.IsVitalEngineBelt())
					{
						if (m_Boat.IsVitalGlowPlug())
							item = m_Boat.FindAttachmentBySlotName("EngineBelt");
						if (item && !item.IsRuined())
							m_BeltCon = true;
					}
					else
					{
						m_BeltCon = true;
					}

					float fuel = m_Boat.GetFluidFraction(CarFluid.FUEL);
					if (fuel > 0)
						m_FuelCon = true;

					if (!GetGame().IsMultiplayer() || GetGame().IsClient())
					{
						//Now we can play the proper sound
						EffectSound effectSound = NULL;

						if (m_FuelCon && m_BeltCon && m_SparkCon && m_BatteryCon)
						{
							effectSound = SEffectManager.CreateSound(m_Boat.m_EngineStartOK, m_Boat.GetPosition());
						}
						else
						{
							if (!m_BatteryCon)
								effectSound = SEffectManager.CreateSound(m_Boat.m_EngineStartBattery, m_Boat.GetPosition());
							else if (!m_SparkCon)
								effectSound = SEffectManager.CreateSound(m_Boat.m_EngineStartPlug, m_Boat.GetPosition());
							else if (!m_FuelCon)
								effectSound = SEffectManager.CreateSound(m_Boat.m_EngineStartFuel, m_Boat.GetPosition());
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
	}

	override void OnStartClient(ActionData action_data)
	{
		OnStartServer(action_data);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if (vehCommand)
		{
			ExpansionBoatScript m_Boat;
			if (Class.CastTo(m_Boat, vehCommand.GetTransport()))
			{
				if (m_FuelCon && m_BeltCon && m_SparkCon && m_BatteryCon)
					m_Boat.MotorStart();
			}
		}
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}
};