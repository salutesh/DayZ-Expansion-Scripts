/**
 * Weapon_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionWeaponType
{
	Handgun,
	Shotgun,
	Submachinegun,
	Assault,
	Marksman,
	GenericRifle,
	Launcher,
	Archery
};

class ExpansionWeaponInfo
{
	static ref TStringArray OPTICS = {"weaponOptics", "weaponOpticsAK", "weaponOpticsAug", "weaponOpticsHunting", "weaponOpticsMosin", "ExpansionSniperOptics"};

	ref map<ExpansionFireMode, int> m_FireModes = new map<ExpansionFireMode, int>();
	ref TStringArray m_ChamberableFrom = {};
	float m_AvgDmg;
	float m_ReloadTimeMin = float.MAX;
	float m_DPS;
	int m_FireRate;  //! rounds per second
	bool m_AutoReload;
	ExpansionWeaponType m_WeaponType;

	void ExpansionWeaponInfo(Weapon_Base weapon)
	{
		string type = weapon.GetType();

		//! Fire modes
		TStringArray modes = {};
		weapon.ConfigGetTextArray("modes", modes);
		foreach (int i, string mode: modes)
		{
			ExpansionFireMode fireMode = typename.StringToEnum(ExpansionFireMode, mode);
			if (fireMode != -1)
			{
				float reloadTime = g_Game.ConfigGetFloat(CFG_WEAPONSPATH + " " + type + " " + mode + " reloadTime");
				if (reloadTime > 0.0 && reloadTime < m_ReloadTimeMin)
					m_ReloadTimeMin = reloadTime;
			#ifdef DIAG_DEVELOPER
				EXTrace.Print(EXTrace.WEAPONS, null, type + " mode " + mode + " reload time " + reloadTime);
			#endif
				m_FireModes[fireMode] = i;

				switch (fireMode)
				{
					case ExpansionFireMode.Burst:
					case ExpansionFireMode.FullAuto:
					case ExpansionFireMode.SemiAuto:
						m_AutoReload = true;
						break;
				}
			}
		}

		//! Compatible ammo
		weapon.ConfigGetTextArray("chamberableFrom", m_ChamberableFrom);

		//! Avg dmg
		string bulletType;
		float dmg;
		int count;
		foreach (string ammoType: m_ChamberableFrom)
		{
			if (g_Game.ConfigGetText(CFG_MAGAZINESPATH + " " + ammoType + " ammo", bulletType))
			{
				dmg = g_Game.ConfigGetFloat(CFG_AMMO + " " + bulletType + " DamageApplied Health damage");
				if (dmg > 0.0)
				{
					m_AvgDmg += dmg;
					count++;
				}
			}
		}
		if (count > 0)
			m_AvgDmg /= count;

		//! DPS
		if (m_AvgDmg > 0 && m_ReloadTimeMin < float.MAX && m_AutoReload)
		{
			m_DPS = m_AvgDmg / m_ReloadTimeMin;
			m_FireRate = m_DPS / m_AvgDmg;
		}
		else
		{
			m_DPS = m_AvgDmg;
			m_FireRate = 1;
		}

		//! Weapon type
		if (weapon.IsInherited(Archery_Base))
		{
			m_WeaponType = ExpansionWeaponType.Archery;
		}
		else if (weapon.IsKindOf("Pistol_Base"))
		{
			m_WeaponType = ExpansionWeaponType.Handgun;
		}
		else if (weapon.IsKindOf("Shotgun_Base") || m_ChamberableFrom.Find("Ammo_12gaPellets") > -1)
		{
			m_WeaponType = ExpansionWeaponType.Shotgun;
		}
		else if (m_ReloadTimeMin < 0.1 && (m_FireModes.Contains(ExpansionFireMode.FullAuto) || m_FireModes.Contains(ExpansionFireMode.Burst)))
		{
			if (m_AvgDmg < 75)  //! Use Bullet_9x39 health dmg as threshold
				m_WeaponType = ExpansionWeaponType.Submachinegun;
			else
				m_WeaponType = ExpansionWeaponType.Assault;
		}
		else if (weapon.ShootsExplosiveAmmo())
		{
			m_WeaponType = ExpansionWeaponType.Launcher;
		}
		else if (m_AvgDmg >= 110 && weapon.GetMuzzleCount() == 1 && weapon.Expansion_HasAttachmentSlot(OPTICS))
		{
			//! @note SSG-82 creates SSG82Optic on weaponOpticsAug slot during init
			m_WeaponType = ExpansionWeaponType.Marksman;
		}
		else
		{
			m_WeaponType = ExpansionWeaponType.GenericRifle;
		}

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.WEAPONS, null, type + " " + typename.EnumToString(ExpansionWeaponType, m_WeaponType) + " avg dmg " + m_AvgDmg + " DPS " + m_DPS);
	#endif
	}
}

modded class Weapon_Base
{
	static ref map<string, ref ExpansionWeaponInfo> s_Expansion_WeaponInfo = new map<string, ref ExpansionWeaponInfo>();
	ref ExpansionWeaponInfo m_Expansion_WeaponInfo;

	int m_Expansion_FireModeIndexSync;

	void Weapon_Base()
	{
		string type = GetType();

		if (!s_Expansion_WeaponInfo.Find(type, m_Expansion_WeaponInfo))
		{
			m_Expansion_WeaponInfo = new ExpansionWeaponInfo(this);
			s_Expansion_WeaponInfo[type] = m_Expansion_WeaponInfo;
		}

		RegisterNetSyncVariableInt("m_Expansion_FireModeIndexSync", 0, 15); 
	}

	void ~Weapon_Base()
	{
		if (!g_Game)
			return;

		//! MagDetached also fires if the weapon is deleted; we cannot detect that beforehand on client, only after-the-fact
		//! and remove the queued call to Expansion_ValidateAndRepair
		if (m_fsm && m_fsm.m_Expansion_ValidateAndRepairQueued)
		{
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_ValidateAndRepair);
			m_fsm.m_Expansion_ValidateAndRepairQueued = "";
		}
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		m_Expansion_FireModeIndexSync = GetCurrentMode(GetCurrentMuzzle());
		SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		Expansion_UpdateFireMode();
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (item.IsMagazine())
			Expansion_CheckDelayedValidateAndRepair("MagAttached");
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (item.IsMagazine())
			Expansion_CheckDelayedValidateAndRepair("MagDetached");
	}

	bool Expansion_IsDelayedValidationTimerRunning()
	{
		if (m_DelayedValidationTimer)
			return m_DelayedValidationTimer.IsRunning();

		return false;
	}

	void Expansion_CheckDelayedValidateAndRepair(string name)
	{
		if (!g_Game.IsDedicatedServer() && IsInitialized() && !IsSetForDeletion())
		{
			//! Delayed validate and repair for AI - fixes occasional state mismatch that vanilla doesn't deal with
			PlayerBase player;
			if (Class.CastTo(player, GetHierarchyRootPlayer()) && player.GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_AI_REMOTE)
			{
				//! MagAttached also fires if the gun was newly created in inventory; we can detect that by checking if the vanilla
				//! validation timer is running which fires OnInventoryEnter
				if (!Expansion_IsDelayedValidationTimerRunning())
					Expansion_DelayedValidateAndRepair(name);
			}
		}
	}

	void Expansion_DelayedValidateAndRepair(string name)
	{
		if (m_fsm)
		{
			//! Always remove any existing queued call and queue new call
			if (m_fsm.m_Expansion_ValidateAndRepairQueued)
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Expansion_ValidateAndRepair);

			m_fsm.m_Expansion_ValidateAndRepairQueued = name;
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_ValidateAndRepair, 3000, false, name);
		}
	}

	void Expansion_ValidateAndRepair(string name)
	{
		if (m_fsm)
			m_fsm.Expansion_ValidateAndRepair(name);
	}

	void Expansion_UpdateFireMode()
	{
		int muzzleCount = GetMuzzleCount();
		bool changed;

		for (int muzzleIndex = 0; muzzleIndex < muzzleCount; muzzleIndex++)
		{
			int mode = GetCurrentMode(muzzleIndex);

		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.WEAPONS, this, "Expansion_UpdateFireMode muzzle=" + muzzleIndex + " mode=" + mode + " m_Expansion_FireModeIndexSync=" + m_Expansion_FireModeIndexSync);
		#endif

			//! @note there will only ever be mismatch if firemode wasn't changed by a player (e.g. AI)
			if (mode != m_Expansion_FireModeIndexSync)
			{
				SetCurrentMode(muzzleIndex, m_Expansion_FireModeIndexSync);
				changed = true;
			}
		}

		if (changed)
			OnFireModeChange(m_Expansion_FireModeIndexSync);
	}

	override float Expansion_GetDPS()
	{
		return m_Expansion_WeaponInfo.m_DPS;
	}

	/**
	 * @brief return current DPS capability of the weapon (depending on presence of mag)
	 */
	float Expansion_GetCurrentDPS()
	{
		int mi = GetCurrentMuzzle();

		if (!HasInternalMagazine(mi) && !GetMagazine(mi))
			return m_Expansion_WeaponInfo.m_AvgDmg;

		return m_Expansion_WeaponInfo.m_DPS;
	}

	float Expansion_GetAvgDmgPerShot()
	{
		return m_Expansion_WeaponInfo.m_AvgDmg;
	}

	ExpansionWeaponType Expansion_GetWeaponType()
	{
		return m_Expansion_WeaponInfo.m_WeaponType;
	}

	/**
	 * @brief check if mag or ammo pile is compatible with weapon
	 */
	bool Expansion_IsCompatibleMag(int muzzleIndex, Magazine mag)
	{
		if (mag.IsAmmoPile())
		{
			if (CanChamberFromMag(muzzleIndex, mag))
				return true;
		}
		else if (TestAttachMagazine(muzzleIndex, mag, false, true))
		{
			return true;
		}

		return false;
	}

	//! Return -1 on unknown state
	int ExpansionGetMagAttachedFSMStateID()
	{
		//! https://feedback.bistudio.com/T149240

		//! BU0 = no bullet
		//! BU1 = bullet in chamber
		//! BUF = fired out bullet cartridge in chamber
		//! MA0 = no magazine present
		//! MA1 = magazine present

		int stateId = -1;

		int mi = GetCurrentMuzzle();

		if ( IsInherited( SKS_Base ) )
		{
		} else if ( IsInherited( BoltActionRifle_InnerMagazine_Base ) )
		{
			//! E.g. Mosin, Winchester 70
		} else if ( IsInherited( DoubleBarrel_Base ) )
		{
		} else if ( IsInherited( Magnum_Base ) )
		{
			//stateId = MagnumStableStateID.DEFAULT;
		} else if ( IsInherited( Pistol_Base ) )
		{
			if ( IsChamberEmpty( mi ) )
				stateId = PistolStableStateID.CLO_DIS_BU0_MA1;
			else if ( IsChamberJammed( mi ) )
				stateId = PistolStableStateID.CLO_JAM_BU1_MA1;
			else
				stateId = PistolStableStateID.CLO_CHG_BU1_MA1;
		//! DayZ 1.16 or above
		} else if ( IsInherited( BoltActionRifle_ExternalMagazine_Base ) )
		{
			//! E.g. CZ527, Scout
			if ( IsChamberEmpty( mi ) )
				stateId = BAREMStableStateID.BAREM_CLO_BU0_MA1;
			else if ( IsChamberJammed( mi ) )
				stateId = BAREMStableStateID.BAREM_JAM_BU1_MA1;
			else if ( IsChamberFiredOut( mi ) )
				stateId = BAREMStableStateID.BAREM_CLO_BUF_MA1;
			else
				stateId = BAREMStableStateID.BAREM_CLO_BU1_MA1;
		} else if ( IsInherited( Repeater_Base ) )
		{
		} else if ( IsInherited( RifleBoltFree_Base ) )
		{
			//! E.g. AKM, Sporter 22, Saiga, MP5, VSS
			if ( IsChamberEmpty( mi ) )
				stateId = RBFStableStateID.RBF_CLO_BU0_MA1;
			else if ( IsChamberJammed( mi ) )
				stateId = RBFStableStateID.RBF_JAM_BU1_MA1;
			else
				stateId = RBFStableStateID.RBF_CLO_BU1_MA1;
		} else if ( IsInherited( RifleBoltLock_Base ) )
		{
			//! E.g. FAL, M4A1, SVD, CZ61, UMP-45
			if ( IsChamberEmpty( mi ) )
				stateId = RBLStableStateID.RBL_CLO_BU0_MA1;
			else if ( IsChamberJammed( mi ) )
				stateId = RBLStableStateID.RBL_JAM_BU1_MA1;
			else
				stateId = RBLStableStateID.RBL_CLO_BU1_MA1;
		} else if ( IsInherited( RifleSingleShot_Base ) )
		{
			//! e.g. Izh-18
		} else if ( IsInherited( Mp133Shotgun_Base ) )
		{
		}

		return stateId;
	}

	bool Expansion_IsChambered()
	{
		int mi = GetCurrentMuzzle();

		return Expansion_IsChambered(mi);
	}

	bool Expansion_IsChambered(int mi)
	{
		if (!IsChamberEmpty(mi) && !IsChamberFiredOut(mi))
			return true;

		return false;
	}

	bool Expansion_HasAmmo(out Magazine mag = null)
	{
		mag = null;

		int mi = GetCurrentMuzzle();

		if (!IsChamberEmpty(mi) && !IsChamberFiredOut(mi))
			return true;

		if (Expansion_GetMagazineAmmoCount(mi, mag) > 0)
			return true;

		return false;
	}

	int Expansion_GetMagazineAmmoCount(out Magazine mag = null)
	{
		int mi = GetCurrentMuzzle();

		return Expansion_GetMagazineAmmoCount(mi, mag);
	}

	int Expansion_GetMagazineAmmoCount(int mi, out Magazine mag = null)
	{
		if (HasInternalMagazine(mi))
			return GetInternalMagazineCartridgeCount(mi);

		mag = GetMagazine(mi);
		if (mag)
			return mag.GetAmmoCount();

		return 0;
	}

	/**
	 * @brief Set fire mode for all muzzles
	 * 
	 * @param fireMode
	 * @param [out] changed  true if fire mode was changed on at least one muzzle
	 * 
	 * @return true if fire mode is valid
	 * 
	 * @note Expansion fire mode enum is NOT the same as fire mode index used by SetCurrentMode/GetCurrentMode,
	 * so translation between the two is handled.
	 */
	bool Expansion_SetFireMode(ExpansionFireMode fireMode, out bool changed = false)
	{
		int fireModeIndex;
		if (!m_Expansion_WeaponInfo.m_FireModes.Find(fireMode, fireModeIndex))
			return false;

		int muzzleCount = GetMuzzleCount();

		for (int muzzleIndex = 0; muzzleIndex < muzzleCount; muzzleIndex++)
		{
			if (GetCurrentMode(muzzleIndex) != fireModeIndex)
			{
			#ifdef DIAG_DEVELOPER
				EXTrace.Print(EXTrace.WEAPONS, this, "::Expansion_SetFireMode - setting muzzle " + muzzleIndex + " mode " + typename.EnumToString(ExpansionFireMode, fireMode));
			#endif
				SetCurrentMode(muzzleIndex, fireModeIndex);
				changed = true;
			}
		}

		if (changed)
		{
			OnFireModeChange(fireModeIndex);
		#ifdef SERVER
			Synchronize();
		#endif
		}

		return true;
	}

	//! @note called by engine when changing mode with SetNextMuzzleMode, on both client+server if player was initiating (client gets called first),
	//! but needs to be called explicitly if firemode is changed in other ways.
	//! We also need to make sure to always sync firemode index to client.
	override void OnFireModeChange(int fireMode)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.StartStack(EXTrace.WEAPONS, this, "fireMode=" + fireMode);
	#endif

		super.OnFireModeChange(fireMode);

		m_Expansion_FireModeIndexSync = fireMode;

	#ifdef SERVER
		SetSynchDirty();
	#else
	#ifdef DIAG_DEVELOPER
		Man player = GetHierarchyRootPlayer();
		string msg = string.Format("%1 %2<%3> fireMode %4", player, GetType(), ExpansionStatic.GetInstanceID(this), fireMode);
		g_Game.Chat(msg, "colorAction");
	#endif
	#endif
	}

	ExpansionFireMode Expansion_GetFireMode()
	{
		int muzzleIndex = GetCurrentMuzzle();
		int mode = GetCurrentMode(muzzleIndex);
		foreach (ExpansionFireMode fireMode, int i: m_Expansion_WeaponInfo.m_FireModes)
		{
			if (mode == i)
			{
			#ifdef DIAG_DEVELOPER
				EXTrace.Print(EXTrace.WEAPONS, this, "::Expansion_GetFireMode - current mode " + typename.EnumToString(ExpansionFireMode, fireMode));
			#endif
				return fireMode;
			}
		}
		return ExpansionFireMode.INVALID;
	}

	void Expansion_ResetMuzzleModes()
	{
		int muzzleCount = GetMuzzleCount();

		for (int i = 0; i < muzzleCount; i++)
		{
			SetCurrentMode(i, 0);
		}
	}
}
