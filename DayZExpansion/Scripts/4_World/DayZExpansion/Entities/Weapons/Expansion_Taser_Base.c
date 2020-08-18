class Expansion_Taser_Base extends FNX45_Base
{
	override void SetActions()
	{
		super.SetActions();
		RemoveAction( FirearmActionLoadBullet );
		RemoveAction( FirearmActionMechanicManipulate );
		RemoveAction( ActionTurnOnWeaponFlashlight );
		RemoveAction( ActionTurnOffWeaponFlashlight );
		RemoveAction( FirearmActionLoadBulletQuick );
	}
};

class Mag_Expansion_Taser extends Mag_FNX45_15Rnd
{
	override void SetActions()
	{
		super.SetActions();

		RemoveAction( ActionLoadMagazine );
		RemoveAction( ActionEmptyMagazine );
		RemoveAction( ActionLoadMagazineQuick );
	}
};