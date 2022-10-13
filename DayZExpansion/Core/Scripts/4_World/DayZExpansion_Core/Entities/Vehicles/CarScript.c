/*
 * CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		CarScript
 * @brief		
 **/
modded class CarScript
{
	private static ref set< CarScript > m_allVehicles = new set< CarScript >;
	
	protected autoptr ExpansionZoneActor m_Expansion_SafeZoneInstance = new ExpansionZoneEntity<CarScript>(this);
	
	// Safezone
	protected bool m_SafeZone;
	protected string m_CurrentSkinName;
	
	bool m_Expansion_AcceptingAttachment;
	int m_Expansion_CargoCount;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void CarScript()
	{
		m_allVehicles.Insert( this );
		RegisterNetSyncVariableInt("m_Expansion_CargoCount");
	}
	
	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~CarScript()
	{
		if (!GetGame())
			return;

		int i;

		i = m_allVehicles.Find( this );
		if ( i >= 0 )
		{
			m_allVehicles.Remove( i );
		}
	}
	
	static set< CarScript > GetAll()
	{
		return m_allVehicles;
	}

	array< ExpansionSkin > ExpansionGetSkins()
	{
		return NULL;
	}

	bool ExpansionHasSkin( int skinIndex )
	{
		return false;
	}
	
	void ExpansionSetSkin( int skinIndex )
	{
	}
	
	// ------------------------------------------------------------
	bool IsInSafeZone()
	{
		return m_SafeZone;
	}

	// ------------------------------------------------------------
	bool CanBeDamaged()
	{
		if ( GetExpansionSettings().GetSafeZone().Enabled && IsInSafeZone() )
		{
			return !GetExpansionSettings().GetSafeZone().DisableVehicleDamageInSafeZone;
		}

		return true;
	}

	// ------------------------------------------------------------
	// Called only server side
	// ------------------------------------------------------------
	void OnEnterZone(ExpansionZoneType type)
	{
		if (type != ExpansionZoneType.SAFE) return;

		EXPrint(ToString() + "::OnEnterZone " + GetPosition());

		m_SafeZone = true;

		if ( GetExpansionSettings().GetSafeZone().DisableVehicleDamageInSafeZone )
			SetAllowDamage(false);
	}

	// ------------------------------------------------------------
	// Called only server side
	// ------------------------------------------------------------
	void OnExitZone(ExpansionZoneType type)
	{
		if (type != ExpansionZoneType.SAFE) return;

		EXPrint(ToString() + "::OnExitZone " + GetPosition());

		m_SafeZone = false;

		if ( CanBeDamaged() )
			SetAllowDamage(true);
	}

	bool Expansion_CanObjectAttach(Object obj)
	{
		return false;
	}

	override void EEInit()
	{
		super.EEInit();

		if (IsMissionHost() && GetExpansionSettings().GetSafeZone().Enabled)
			m_Expansion_SafeZoneInstance.Update();
	}

	override void DamageCrew(float dmg)
	{
		if (IsInSafeZone())
			return;

		super.DamageCrew(dmg);
	}

	override void SetActions()
	{
		super.SetActions();

#ifdef DIAG
		AddAction(ExpansionActionDebugStoreEntity);
#endif
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return;

		ctx.Write(m_CurrentSkinName);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return true;

		if (!ctx.Read(m_CurrentSkinName))
			return false;

		return true;
	}
	#endif
	
	set<Human> Expansion_GetVehicleCrew(bool playersOnly = true)
	{
		set<Human> players = new set<Human>;
		Human crew;
		
		//! Seated players
		for (int i = 0; i < CrewSize(); i++)
		{
			crew = CrewMember(i);
			if (!crew)
				continue;

			if (!playersOnly || crew.GetIdentity())
				players.Insert(crew);
		}

		//! Attached players
		IEntity child = GetChildren();
		while (child)
		{
			crew = Human.Cast(child);

			child = child.GetSibling();

			if (!crew)
				continue;

			if (!playersOnly || crew.GetIdentity())
				players.Insert(crew);
		}
		
		return players;
	}
};