/**
 * BarrelHoles_ColorBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Inherits from FireplaceBase
modded class BarrelHoles_ColorBase
{
	void BarrelHoles_ColorBase()
	{
		m_Expansion_GlobalID = new ExpansionGlobalID();
		RegisterNetSyncVariableBool("m_Expansion_HasEntityStorage");
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		#ifdef SERVER
		if (!m_Expansion_GlobalID.m_IsSet)
			m_Expansion_GlobalID.Acquire();
		#endif

		m_Expansion_GlobalID.OnStoreSave(ctx);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (ctx.GetVersion() < 45)
			return true;

		if (!m_Expansion_GlobalID.OnStoreLoad(ctx))
			return false;

		return true;
	}
	#endif

	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionStoreContents);
		AddAction(ExpansionActionRestoreContents);
	}

	override bool Expansion_CanUseVirtualStorage(bool restoreOverride = false)
	{
		if (IsBurning())
			return false;

		return super.Expansion_CanUseVirtualStorage(restoreOverride);
	}

	override void Open()
	{
		super.Open();

		if (GetGame().IsServer() && Expansion_CanUseVirtualStorage(true))
			Expansion_RestoreContents();
	}

	override void Close()
	{
		if (GetGame().IsServer() && !IsBurning() && Expansion_CanUseVirtualStorage())
			Expansion_StoreContents();

		super.Close();
	}

	override bool IsNonExpansionOpenable()
	{
		return true;
	}
}
