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
		bool debugPrint;
#ifdef DIAG
		debugPrint = EXTrace.WEAPONS;
#else
	#ifdef EXPANSIONMODAI
		EntityAI sourceEntity;
		if (Class.CastTo(sourceEntity, source))
		{
			Man player = sourceEntity.GetHierarchyRootPlayer();
			if (player && player.IsInherited(eAIBase))
			{
				if (m_eAI_FirearmEffectsCallCount == m_eAI_FirearmEffectsCallCountPrev)  //! This should NOT happen
					debugPrint = true;
				else
					m_eAI_FirearmEffectsCallCountPrev = m_eAI_FirearmEffectsCallCount;
			}
		}
	#endif
#endif
		if (debugPrint)
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
		ExpansionWorldObjectsModule.RPC_RemoveObjects(ctx);
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
};
