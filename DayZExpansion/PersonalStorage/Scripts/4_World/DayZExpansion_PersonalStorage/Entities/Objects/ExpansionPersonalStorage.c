/**
 * ExpansionPersonalStorage.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionPersonalStorageBase
 * @brief		This class handles the static personal storage object base class.
 **/
class ExpansionPersonalStorageBase extends ExpansionStaticObjectBase
{
	protected int m_Expansion_StorageID = -1;

	// ------------------------------------------------------------
	// ExpansionPersonalStorageBase Constructor
	// ------------------------------------------------------------
	void ExpansionPersonalStorageBase()
	{
		if (IsMissionHost())
			SetAllowDamage(false);
		
		RegisterNetSyncVariableInt("m_Expansion_StorageID");
		
		if (!m_Expansion_NetsyncData)
			m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
	}

	// ------------------------------------------------------------
	// ExpansionPersonalStorageBase Constructor
	// ------------------------------------------------------------
	bool HasStorageID()
	{
		return (m_Expansion_StorageID > -1);
	}

	// ------------------------------------------------------------
	// ExpansionPersonalStorageBase Constructor
	// ------------------------------------------------------------
	int GetStorageID()
	{
		return m_Expansion_StorageID;
	}

	// ------------------------------------------------------------
	// ExpansionPersonalStorageBase Constructor
	// ------------------------------------------------------------
	void SetStorageID(int id)
	{
		m_Expansion_StorageID = id;
	}
};

class ExpansionPersonalStorageChest: ExpansionPersonalStorageBase {};
class ExpansionPersonalStorageOldChest: ExpansionPersonalStorageBase {};

class ExpansionPersonalProtectiveCaseBase: ExpansionOwnedContainer
{
	void ExpansionPersonalProtectiveCaseBase()
	{
		if (IsMissionHost())
			SetAllowDamage(false);
	}
};

class ExpansionPersonalProtectiveCaseAlpha: ExpansionPersonalProtectiveCaseBase {};
class ExpansionPersonalProtectiveCaseBeta: ExpansionPersonalProtectiveCaseBase {};
class ExpansionPersonalProtectiveCaseGamma: ExpansionPersonalProtectiveCaseBase {};
class ExpansionPersonalProtectiveCaseDelta: ExpansionPersonalProtectiveCaseBase {};
class ExpansionPersonalProtectiveCaseEpsilon: ExpansionPersonalProtectiveCaseBase {};
class ExpansionPersonalProtectiveCaseOmega: ExpansionPersonalProtectiveCaseBase {};

modded class PlayerBase
{
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (GetIdentity() && GetExpansionSettings().GetPersonalStorage().UsePersonalStorageCase)
				ExpansionPersonalStorageModule.GetModuleInstance().StorePersonalStorageCase(this);
		}
	}
};
