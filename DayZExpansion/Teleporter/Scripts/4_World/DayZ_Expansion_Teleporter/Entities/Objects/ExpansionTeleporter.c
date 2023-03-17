/**
 * ExpansionTeleporter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionQuestSExpansionTeleporterBase
 * @brief		This class handles teleporter objects used to head-out or take-in quests
 **/

class Expansion_Teleporter_Base: BuildingSuper
{
	protected ExpansionTeleporterTrigger m_Trigger;
	
	void Expansion_Teleporter_Base()
	{
		if (IsMissionHost())
			SetAllowDamage(false);
	}
	
	void ~Expansion_Teleporter_Base()
	{
		if (m_Trigger)
			GetGame().ObjectDelete(m_Trigger);
	}
	
	void SetTeleportData(ExpansionTeleportData teleportData)
	{
		vector pos = teleportData.GetObjectPosition();
		vector ori = teleportData.GetObjectOrientation();
		
		if (!m_Trigger)
		{
			Object obj = GetGame().CreateObjectEx("ExpansionTeleporterTrigger", pos, ECE_NONE);
			m_Trigger = ExpansionTeleporterTrigger.Cast(obj);
		}
		
		if (!m_Trigger)
		{
			GetGame().ObjectDelete(obj);
			return;
		}

		m_Trigger.SetPosition(pos);
		m_Trigger.SetOrientation(ori);
		m_Trigger.SetTeleportData(teleportData, this);
	}
	
	ExpansionTeleporterTrigger GetTrigger()
	{
		return m_Trigger;
	}
	
	bool IsEVRStormActive()
	{
	#ifdef NAMALSK_SURVIVAL
	    NamEventManager event_manager;
	    g_Script.CallFunction(GetGame().GetMission(), "GetNamEventManager", event_manager, null);
	
	    if (!event_manager)
	        return false;
	
	    return EVRStorm.Cast(event_manager.GetEvent(EVRStorm)) || EVRStorm.Cast(event_manager.GetEvent(EVRStormDeadly));
	#endif
	
		return false;
	}
};
class Expansion_Teleporter_Big: Expansion_Teleporter_Base {};