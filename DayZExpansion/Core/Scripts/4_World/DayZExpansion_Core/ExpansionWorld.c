/**
 * ExpansionWorld.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionWorld: ExpansionGame
{
	void ExpansionWorld()
	{
		ExpansionAttachmentHelper.Init();
	}

	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType) 
	{
#ifdef DIAG
		if (EXTrace.WEAPONS)
		{
			string msg = "::FirearmEffects ";
			msg += "" + source + " ";
			msg += "" + directHit + " ";
			msg += "" + componentIndex + " ";
			msg += "" + surface + " ";
			msg += "" + pos + " ";
			msg += "" + surfNormal + " ";
			msg += "" + exitPos + " ";
			msg += "" + inSpeed.Length() + " ";
			msg += "" + outSpeed.Length() + " ";
			msg += "" + isWater + " ";
			msg += "" + deflected + " ";
			msg += "" + ammoType;
			EXTrace.Print(true, this, msg);
		}
#endif

		if (GetGame().IsServer())
		{
			if (source && source.ShootsExplosiveAmmo() && !deflected && outSpeed == vector.Zero)
			{
				if (GetExpansionSettings().GetDamageSystem().Enabled)
				{
					string explosionAmmoType = GetExpansionSettings().GetDamageSystem().ExplosiveProjectiles[ammoType];
					if (explosionAmmoType)
						ExpansionDamageSystem.OnBeforeExplode(EntityAI.Cast(source), DT_EXPLOSION, explosionAmmoType, pos);
				}
			}
		}

		super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);
	}

	override void WriteRemovedWorldObjects(ParamsReadContext ctx)
	{
		ExpansionWorldObjectsModule.WriteRemovedObjects(ctx);
	}

	override void ReadRemovedWorldObjects(ParamsReadContext ctx)
	{
		ExpansionWorldObjectsModule.RPC_RemoveObjects(null, null, ctx);
	}

	static void CheckTreeContact(IEntity other, float impulse, bool sendToClient = false)
	{
		if (impulse < 7500)
			return;

		Plant plant;
		if (!Plant.CastTo(plant, other))
			return;

		PlayFellPlantSound(plant, sendToClient);

		if (GetGame().IsServer())
			plant.SetHealth(0);

		dBodyDestroy(plant);
	}

	static void PlayFellPlantSound(Object plant, bool sendToClient = false)
	{
#ifdef SERVER
		if (!sendToClient)
			return;
#endif
		
		string soundSet;

		if (plant.IsInherited(TreeHard))
			soundSet =	"hardTreeFall_SoundSet";
		else if (plant.IsInherited(TreeSoft))
			soundSet =	"softTreeFall_SoundSet";
		else if (plant.IsInherited(BushHard))
			soundSet =	"hardBushFall_SoundSet";
		else if (plant.IsInherited(BushSoft))
			soundSet =	"softBushFall_SoundSet";
		else
			return;

#ifdef SERVER
		ExpansionItemBaseModule.s_Instance.PlaySound(plant.GetPosition(), soundSet);
#else
		EffectSound sound =	SEffectManager.Expansion_PlaySound(soundSet, plant.GetPosition());
#endif
	}

	/**
	 * @brief get moddable root type of entity
	 * 
	 * @param entity Entity to get moddable root type of
	 * 
	 * @return typename
	 */
	static typename GetModdableRootType(EntityAI entity)
	{
		if (entity.IsInherited(AdvancedCommunication))
			return AdvancedCommunication;
		else if (entity.IsInherited(AnimalBase))
			return AnimalBase;
		else if (entity.IsInherited(BuildingBase))
			return BuildingBase;
		else if (entity.IsInherited(CarScript))
			return CarScript;
		else if (entity.IsInherited(DayZPlayerImplement))
			return DayZPlayerImplement;
		//else if (entity.IsInherited(HelicopterScript))
			//return HelicopterScript;
		else if (entity.IsInherited(ItemBase))
			return ItemBase;
		else if (entity.IsInherited(ZombieBase))
			return ZombieBase;

		typename type;
		return type;
	}
};
