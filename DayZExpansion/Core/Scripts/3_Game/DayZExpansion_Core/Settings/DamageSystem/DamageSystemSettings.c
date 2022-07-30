/**
 * ExpansionDamageSystemSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionDamageSystemSettings
 * @brief		Vehicle settings class
 **/
class ExpansionDamageSystemSettings: ExpansionSettingBase
{
	static const int VERSION = 1;
	
	bool Enabled;
	bool CheckForBlockingObjects;

	//! List of classnames that should use the Expansion damage system for explosion damage if enabled
	autoptr TStringArray ExplosionTargets = new TStringArray;

	//! Mapping bullet ammo type to explosion ammo type
	ref map<string, string> ExplosiveProjectiles = new map<string, string>;

	[NonSerialized()]
	private bool m_IsLoaded;

	override bool OnRecieve(ParamsReadContext ctx)
	{
		return true;
	}

	override void OnSend(ParamsWriteContext ctx)
	{
	}

	override int Send(PlayerIdentity identity)
	{
		return 0;
	}

	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionDamageSystemSettings s;
		if (!Class.CastTo(s, setting))
			return false;

		CopyInternal(s);

		return true;
	}

	private void CopyInternal(ExpansionDamageSystemSettings s)
	{
		Enabled = s.Enabled;
		CheckForBlockingObjects = s.CheckForBlockingObjects;
		ExplosionTargets = s.ExplosionTargets;
	}
	
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	override void Unload()
	{
		m_IsLoaded = false;
	}

	override bool OnLoad()
	{
		m_IsLoaded = true;

		bool save;

		bool settingsExist = FileExist(EXPANSION_DAMAGESYSTEM_SETTINGS);

		if (settingsExist)
		{
			EXPrint("[ExpansionDamageSystemSettings] Load existing setting file:" + EXPANSION_DAMAGESYSTEM_SETTINGS);

			JsonFileLoader<ExpansionDamageSystemSettings>.JsonLoadFile(EXPANSION_DAMAGESYSTEM_SETTINGS, this);
		}
		else
		{
			EXPrint("[ExpansionDamageSystemSettings] No existing setting file:" + EXPANSION_DAMAGESYSTEM_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}

		if (save)
			Save();

		return settingsExist;
	}

	override bool OnSave()
	{
		JsonFileLoader<ExpansionDamageSystemSettings>.JsonSaveFile(EXPANSION_DAMAGESYSTEM_SETTINGS, this);

		return true;
	}

	override void Update(ExpansionSettingBase setting)
	{
		super.Update(setting);

		ExpansionSettings.SI_DamageSystem.Invoke();
	}

	override void Defaults()
	{
		m_Version = VERSION;

#ifdef EXPANSIONMODBASEBUILDING
		Enabled = true;
		ExplosionTargets.Insert("ExpansionBaseBuilding");
		ExplosionTargets.Insert("ExpansionSafeBase");
#else
		Enabled = false;
#endif
		CheckForBlockingObjects = true;
		ExplosiveProjectiles.Insert("Bullet_40mm_Explosive", "Explosion_40mm_Ammo");
	}
	
	override string SettingName()
	{
		return "Damage System Settings";
	}
};
