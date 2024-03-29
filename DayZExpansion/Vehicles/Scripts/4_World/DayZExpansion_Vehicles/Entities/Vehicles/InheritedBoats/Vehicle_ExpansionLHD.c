/**
 * Vehicle_ExpansionLHD.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2019 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class Vehicle_ExpansionLHD : Vehicle_ExpansionZodiacBoat
{
	Object lhd1;
	Object lhd2;
	Object lhd3;
	Object lhd4;
	Object lhd5;
	Object lhd6;

	Object house1;
	Object house2;

	void Vehicle_ExpansionLHD()
	{
		//! Vanilla
		m_dmgContactCoef = 0.018;

		CreatePart("ExpansionLHD1", lhd1);
		CreatePart("ExpansionLHD2", lhd2);
		CreatePart("ExpansionLHD3", lhd3);
		CreatePart("ExpansionLHD4", lhd4);
		CreatePart("ExpansionLHD5", lhd5);
		CreatePart("ExpansionLHD6", lhd6);

		CreatePart("ExpansionLHDHouse1", house1);
		CreatePart("ExpansionLHDHouse2", house2);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateModels, 5000, true);

		SetAllowDamage(false);
	}

	void ~Vehicle_ExpansionLHD()
	{
		DeletePart(lhd1);
		DeletePart(lhd2);
		DeletePart(lhd3);
		DeletePart(lhd4);
		DeletePart(lhd5);
		DeletePart(lhd6);

		DeletePart(house1);
		DeletePart(house2);
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EXPANSION_LHD;
	}

	private void DeletePart(out Object obj)
	{
		if (obj)
		{
			GetGame().ObjectDelete(obj);
		}

		obj = NULL;
	}

	private void CreatePart(string part, out Object obj)
	{
		obj = GetGame().CreateObjectEx(part, "0 0 0", ECE_CREATEPHYSICS | ECE_LOCAL);

		obj.SetPosition("0 0 0");
		obj.SetOrientation("0 0 0");

		this.AddChild(obj, -1, false);

		obj.SetPosition("0 0 0");
		obj.SetOrientation("0 0 0");

		obj.Update();
	}

	override vector GetTransportCameraOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraOffset");
#endif

		return "0 40 0";
	}

	override float GetTransportCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraDistance");
#endif

		return 100;
	}

	override void UpdateLights(int new_gear = -1)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "UpdateLights").Add(new_gear);
#endif

		super.UpdateLights(new_gear);

		if (IsMissionClient())
		{
			ItemBase battery;

			if (IsVitalCarBattery())
				battery = ItemBase.Cast(FindAttachmentBySlotName("CarBattery"));
			if (IsVitalTruckBattery())
				battery = ItemBase.Cast(FindAttachmentBySlotName("TruckBattery"));
			if (IsVitalHelicopterBattery())
				battery = ItemBase.Cast(FindAttachmentBySlotName("ExpansionHelicopterBattery"));
			if (IsVitalAircraftBattery())
				battery = ItemBase.Cast(FindAttachmentBySlotName("ExpansionAircraftBattery"));

			if (battery)
			{
				int b;

				vector color;
				vector ambient;

				if (m_HeadlightsOn)
				{
					if (m_Lights.Count() == 0)
					{
						CreateLights(house1, "interiorlight", ExpansionPointLight, Vector(1, 0.788, 0.05), Vector(1, 0.788, 0.05), 25, 0.9, false, true);
						CreateLights(house1, "cerveny pozicni", ExpansionBlinkLight, Vector(0.12, 0.006, 0.006), Vector(0.1, 0.01, 0.01), 500, 0.1, true, true);

						// CreateParticle( lhd1, "bily pozicni", ParticleList.EXPANSION_LIGHT_WHITE );
						// CreateParticle( lhd2, "bily pozicni", ParticleList.EXPANSION_LIGHT_WHITE );
						// CreateParticle( lhd3, "bily pozicni", ParticleList.EXPANSION_LIGHT_WHITE );
						// CreateParticle( lhd4, "bily pozicni", ParticleList.EXPANSION_LIGHT_WHITE );

						// CreateParticle( lhd1, "modry pozicni", ParticleList.EXPANSION_LIGHT_BLUE );
						// CreateParticle( lhd2, "modry pozicni", ParticleList.EXPANSION_LIGHT_BLUE );
						// CreateParticle( lhd3, "modry pozicni", ParticleList.EXPANSION_LIGHT_BLUE );
						// CreateParticle( lhd4, "modry pozicni", ParticleList.EXPANSION_LIGHT_BLUE );
						// CreateParticle( lhd5, "modry pozicni", ParticleList.EXPANSION_LIGHT_BLUE );

						// CreateParticle( lhd1, "cerveny pozicni", ParticleList.EXPANSION_LIGHT_RED );
						// CreateParticle( lhd2, "cerveny pozicni", ParticleList.EXPANSION_LIGHT_RED );
						// CreateParticle( lhd3, "cerveny pozicni", ParticleList.EXPANSION_LIGHT_RED );
						// CreateParticle( lhd4, "cerveny pozicni", ParticleList.EXPANSION_LIGHT_RED );
						// CreateParticle( lhd5, "cerveny pozicni", ParticleList.EXPANSION_LIGHT_RED );
						// CreateParticle( lhd6, "cerveny pozicni", ParticleList.EXPANSION_LIGHT_RED );

						// CreateParticle( lhd6, "zluty pozicni", ParticleList.EXPANSION_LIGHT_YELLOW );
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

		SetSynchDirty();
	}

	override void EOnContact(IEntity other, Contact extra)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "EOnContact").Add(other).Add(extra);
#endif
	}

	override void OnContact(string zoneName, vector localPos, IEntity other, Contact data)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_4(ExpansionTracing.VEHICLES, this, "OnContact").Add(zoneName).Add(localPos).Add(other).Add(data);
#endif
	}

	void UpdateModels()
	{
		this.Update();
	}
};
