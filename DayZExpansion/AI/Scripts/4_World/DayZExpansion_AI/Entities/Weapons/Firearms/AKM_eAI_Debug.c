class AKM_eAI_Debug: RifleBoltFree_Base
{
	void AKM_eAI_Debug()
	{
		if (GetGame().IsServer())
			SetAllowDamage(false);
	}

	override RecoilBase SpawnRecoilObject()
	{
		return new AkmRecoil(this);
	}
	
	override bool CanEnterIronsights()
	{
		ItemOptics optic = GetAttachedOptics();
		if (optic && PSO1Optic.Cast(optic) || PSO11Optic.Cast(optic) || KazuarOptic.Cast(optic))
			return true;
		return super.CanEnterIronsights();
	}
	
	override void OnDebugSpawn()
	{
		GameInventory inventory = GetInventory();

		inventory.CreateInInventory( "AK_WoodBttstck" );
		inventory.CreateInInventory( "AK_RailHndgrd" );
		inventory.CreateInInventory( "AK_Suppressor" );
		inventory.CreateInInventory( "UniversalLight" );
		inventory.CreateInInventory( "KobraOptic" );
		inventory.CreateInInventory( "Battery9V" );
		inventory.CreateInInventory( "Battery9V" );

		SpawnAttachedMagazine("Mag_AKM_Drum_eAI_Debug");
	}

	override void EEFired(int muzzleType, int mode, string ammoType)
	{
		super.EEFired(muzzleType, mode, ammoType);

		//! Always have unlimited ammo, even if vanilla debug option is disabled
		if ((ItemBase.GetDebugActionsMask() & DebugActionType.UNLIMITED_AMMO) == 0)
		{
			Magazine magazine = GetMagazine(GetCurrentMuzzle());
			if (magazine)
			{
				if (GetGame().IsServer())
					magazine.ServerSetAmmoMax();
				else
					magazine.LocalSetAmmoMax();
			}
		}
	}
};

class Ammo_762x39Tracer_eAI_Debug: Ammunition_Base {};
class Mag_AKM_Drum_eAI_Debug: MagazineStorage {};
