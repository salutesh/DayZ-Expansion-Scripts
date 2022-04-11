/**
 * ExpansionVehicleBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODVEHICLE
modded class ExpansionVehicleBase
{
	protected string m_Expansion_LastDriverUID;
	protected bool m_Expansion_SynchLastDriverUID;
	protected bool m_Expansion_LastDriverUIDSynched;

	void ExpansionVehicleBase()
	{
		RegisterNetSyncVariableBool("m_Expansion_SynchLastDriverUID");
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_Expansion_SynchLastDriverUID != m_Expansion_LastDriverUIDSynched)
		{
			m_Expansion_LastDriverUIDSynched = m_Expansion_SynchLastDriverUID;

			if (!m_Expansion_SynchLastDriverUID)
				return;

			//! Reset m_Expansion_LastDriverUID client-side if vehicle has driver and it is not the player
			Human driver = CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER);
			Man player = GetGame().GetPlayer();
			if (driver && player && driver != player)
			{
				m_Expansion_LastDriverUID = "";
			}
		}
	}

	void ExpansionSetLastDriverUID(PlayerBase player)
	{
		m_Expansion_LastDriverUID = player.GetIdentityUID();

		if (!IsMissionHost())
			return;

		m_Expansion_SynchLastDriverUID = true;

		SetSynchDirty();
	}

	void ExpansionResetLastDriverUIDSynch()
	{
		m_Expansion_SynchLastDriverUID = false;

		SetSynchDirty();
	}

	string ExpansionGetLastDriverUID()
	{
		return m_Expansion_LastDriverUID;
	}
};
#endif
