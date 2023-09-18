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

modded class Weapon_Base
{
	static ref map<string, ref map<ExpansionFireMode, int>> s_Expansion_FireModes = new map<string, ref map<ExpansionFireMode, int>>();

	void Weapon_Base()
	{
		string type = GetType();
		if (!s_Expansion_FireModes[type])
		{
			auto fireModes = new map<ExpansionFireMode, int>();
			s_Expansion_FireModes[type] = fireModes;
			TStringArray modes = new TStringArray();
			ConfigGetTextArray("modes", modes);
			for (int i = 0; i < modes.Count(); i++)
			{
				ExpansionFireMode fireMode = typename.StringToEnum(ExpansionFireMode, modes[i]);
				if (fireMode != -1)
				{
				#ifdef DIAG
					EXTrace.Print(EXTrace.WEAPONS, this, "mode " + modes[i]);
				#endif
					fireModes[fireMode] = i;
				}
			}
		}
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
		return !IsChamberEmpty(mi) && !IsChamberFiredOut(mi);
	}

	bool Expansion_HasAmmo(out Magazine mag = null)
	{
		mag = null;

		int mi = GetCurrentMuzzle();

		if (!IsChamberEmpty(mi) && !IsChamberFiredOut(mi))
			return true;

		if (HasInternalMagazine(mi) && GetInternalMagazineCartridgeCount(mi))
			return true;

		mag = GetMagazine(mi);
		if (mag && mag.GetAmmoCount())
			return true;

		return false;
	}

	bool Expansion_SetFireMode(ExpansionFireMode fireMode)
	{
		string type = GetType();
		auto fireModes = s_Expansion_FireModes[type];
		if (!fireModes)
			return false;

		int fireModeIndex;
		if (!fireModes.Find(fireMode, fireModeIndex))
			return false;

		int muzzleIndex = GetCurrentMuzzle();
		if (GetCurrentMode(muzzleIndex) != fireModeIndex)
		{
		#ifdef DIAG
			EXTrace.Print(EXTrace.WEAPONS, this, "::Expansion_SetFireMode - setting mode " + typename.EnumToString(ExpansionFireMode, fireMode));
		#endif
			OnFireModeChange(fireModeIndex);
			SetCurrentMode(muzzleIndex, fireModeIndex);
		}

		return true;
	}

	ExpansionFireMode Expansion_GetFireMode()
	{
		int muzzleIndex = GetCurrentMuzzle();
		int mode = GetCurrentMode(muzzleIndex);
		string type = GetType();
		foreach (ExpansionFireMode fireMode, int i: s_Expansion_FireModes[type])
		{
			if (mode == i)
			{
			#ifdef DIAG
				EXTrace.Print(EXTrace.WEAPONS, this, "::Expansion_GetFireMode - current mode " + typename.EnumToString(ExpansionFireMode, fireMode));
			#endif
				return fireMode;
			}
		}
		return ExpansionFireMode.INVALID;
	}
}
