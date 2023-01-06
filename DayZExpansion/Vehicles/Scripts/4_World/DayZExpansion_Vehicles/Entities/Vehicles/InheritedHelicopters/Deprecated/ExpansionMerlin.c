/**
 * ExpansionMerlin.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionMerlinWreck : ExpansionHelicopterWreck
{
};

/**
 * @class		ExpansionMerlin
 * @brief
 */
class ExpansionMerlin : ExpansionHelicopterScript
{
	void ExpansionMerlin()
	{
		m_dmgContactCoef = 0.02;
		m_ExplosionSize = 50;

		m_MaxSpeed = 200.0;

		m_AltitudeFullForce = 1000.0;
		m_AltitudeNoForce = 2000.0;

		m_EngineStartDuration = 20.0;

		m_MinAutoRotateSpeed = 2.0;
		m_MaxAutoRotateSpeed = 10.0;

		m_BodyFrictionCoef = 1.6;
		m_LiftForceCoef = 1.0;

		m_CyclicForceCoef = 1.5;

		m_CyclicSideSpeed = 2.0;
		m_CyclicSideMax = 0.5;
		m_CyclicSideCoef = 0.6;

		m_CyclicForwardSpeed = 10.0;
		m_CyclicForwardMax = 1.0;
		m_CyclicForwardCoef = 0.4;

		m_AntiTorqueSpeed = 1.8;
		m_AntiTorqueMax = 0.04;

		m_BankForceCoef = 0.2;
		m_TailForceCoef = 6.0;
	}

	override void LoadConstantVariables()
	{
		super.LoadConstantVariables();
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return CarLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionHelicopterFrontLight));
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	override int GetSeatAnimationType(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatAnimationType").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		case 2:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		case 3:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		}

		return 0;
	}

	override int Get3rdPersonCameraType()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Get3rdPersonCameraType");
#endif

		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		return true;
	}

	override bool IsAreaAtDoorFree(int currentSeat, float maxAllowedObjHeight = 0.5, float horizontalExtents = 0.5, float playerHeight = 1.7)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_4(ExpansionTracing.VEHICLES, this, "IsAreaAtDoorFree").Add(currentSeat).Add(maxAllowedObjHeight).Add(horizontalExtents).Add(playerHeight);
#endif

		//! Getting out of seat should never be blocked because you stay inside the vehicle
		return true;
	}

	override bool IsVitalIgniterPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalIgniterPlug");
#endif

		return true;
	}

	override bool IsVitalHydraulicHoses()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHydraulicHoses");
#endif

		return true;
	}

	override bool LeavingSeatDoesAttachment(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "LeavingSeatDoesAttachment").Add(posIdx);
#endif

		return true;
	}

	override float GetActionDistanceFuel()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetActionDistanceFuel");
#endif

		return 2.5;
	}

	override vector GetTransportCameraOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraOffset");
#endif

		return "0 1 0";
	}

	override float GetTransportCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraDistance");
#endif

		return 25;
	}

	override float GetWreckAltitude()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetWreckAltitude");
#endif

		return 1.5;
	}

	override vector GetWreckOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetWreckOffset");
#endif

		return "0 0 -2.5";
	}

	override string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		switch (selection)
		{
		case "cargoramp":
			return "cargoramp";
		case "door_back_l":
			return "door_back_l";
		case "door_back_r":
			return "door_back_r";
		}

		return "";
	}

	override void UpdateLights(int new_gear = -1)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "UpdateLights").Add(new_gear);
#endif

		super.UpdateLights(new_gear);

		if (!GetGame().IsMultiplayer() || GetGame().IsClient())
		{
			ItemBase battery;

			if (IsVitalHelicopterBattery())
				battery = ItemBase.Cast(FindAttachmentBySlotName("ExpansionHelicopterBattery"));

			if (battery)
			{
				int b;

				vector color;
				vector ambient;

				if (m_HeadlightsOn)
				{
					if (m_Lights.Count() == 0)
					{
						CreateLights(this, "interiorlight1", ExpansionPointLight, Vector(1, 0.01, 0.01), Vector(1, 0.01, 0.01), 5, 0.85, false, true);
						CreateLights(this, "interiorlight2", ExpansionPointLight, Vector(1, 0.01, 0.01), Vector(1, 0.01, 0.01), 5, 0.85, false, true);
						CreateLights(this, "switch_lightsac", ExpansionPointLight, Vector(0, 0.1, 1), Vector(1, 1, 1), 5, 0.5, false, true);

						// CreateLights( this, "interiorlight1", ExpansionPointLight, Vector(0.1, 0.1, 0.1), Vector(0.1, 0.1, 0.1), 2, 0.1, false, true );
						// CreateLights( this, "interiorlight2", ExpansionPointLight, Vector(0.1, 0.1, 0.1), Vector(0.1, 0.1, 0.1), 2, 0.1, false, true );

						// CreateParticle( this, "bily pozicni", ParticleList.EXPANSION_LIGHT_WHITE );
						// CreateParticle( this, "zeleny pozicni", ParticleList.EXPANSION_LIGHT_YELLOW );
						// CreateParticle( this, "cerveny pozicni", ParticleList.EXPANSION_LIGHT_RED );
						// CreateParticle( this, "cerveny pozicni blik", ParticleList.EXPANSION_LIGHT_RED );
					}
				}
				else
				{
					for (b = 0; b < m_Particles.Count(); b++)
					{
						m_Particles[b].Stop();

						GetGame().ObjectDelete(m_Particles[b]);
					}

					for (b = -0; b < m_Lights.Count(); b++)
					{
						m_Lights[b].ExpansionSetEnabled(false);

						GetGame().ObjectDelete(m_Lights[b]);
					}

					m_Lights.Clear();
				}
			}
		}
	}

	override void OnDebugSpawn()
	{
		EntityAI entity;

		if (Class.CastTo(entity, this))
		{
			entity.GetInventory().CreateInInventory("ExpansionMerlinFrontWheel");
			entity.GetInventory().CreateInInventory("ExpansionMerlinFrontWheel");
			entity.GetInventory().CreateInInventory("ExpansionMerlinBackWheel");
			entity.GetInventory().CreateInInventory("ExpansionMerlinBackWheel");

			entity.GetInventory().CreateInInventory("ExpansionHydraulicHoses");
			entity.GetInventory().CreateInInventory("ExpansionIgniterPlug");
			entity.GetInventory().CreateInInventory("ExpansionHelicopterBattery");
			entity.GetInventory().CreateInInventory("HeadlightH7");
		}

		Fill(CarFluid.FUEL, 50);
		Fill(CarFluid.COOLANT, 6.0);
		Fill(CarFluid.OIL, 4.0);
	}
};
