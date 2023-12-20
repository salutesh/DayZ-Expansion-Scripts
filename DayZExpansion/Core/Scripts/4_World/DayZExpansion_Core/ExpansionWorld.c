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
	static int s_Expansion_BushFallHard_NetworkedSoundID;
	static int s_Expansion_BushFallSoft_NetworkedSoundID;
	static int s_Expansion_TreeFallHard_NetworkedSoundID;
	static int s_Expansion_TreeFallSoft_NetworkedSoundID;

	void ExpansionWorld()
	{
		s_Expansion_BushFallHard_NetworkedSoundID = ExpansionItemBaseModule.s_Instance.RegisterSound("hardBushFall_SoundSet");
		s_Expansion_BushFallSoft_NetworkedSoundID = ExpansionItemBaseModule.s_Instance.RegisterSound("softBushFall_SoundSet");
		s_Expansion_TreeFallHard_NetworkedSoundID = ExpansionItemBaseModule.s_Instance.RegisterSound("hardTreeFall_SoundSet");
		s_Expansion_TreeFallSoft_NetworkedSoundID = ExpansionItemBaseModule.s_Instance.RegisterSound("softTreeFall_SoundSet");
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
		
		int soundID;

		if (plant.IsInherited(TreeHard))
			soundID =	s_Expansion_TreeFallHard_NetworkedSoundID;
		else if (plant.IsInherited(TreeSoft))
			soundID =	s_Expansion_TreeFallSoft_NetworkedSoundID;
		else if (plant.IsInherited(BushHard))
			soundID =	s_Expansion_BushFallHard_NetworkedSoundID;
		else if (plant.IsInherited(BushSoft))
			soundID =	s_Expansion_BushFallSoft_NetworkedSoundID;
		else
			return;

		ExpansionItemBaseModule.s_Instance.PlaySound(plant.GetPosition(), soundID);
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

	//! @note would make more sense to have this on `Liquid` but due to `GetLiquidConfigProperty` being private
	//! (although it's not used directly by this) it would result in a compile error :-(
	static string GetLiquidDisplayName(int liquidType, bool upperCase = true, out int color = 0)
	{
		auto nutritionalProfile = Liquid.GetNutritionalProfileByType(liquidType);
		if (!nutritionalProfile || !nutritionalProfile.IsLiquid())
			return "ERROR";

		string clsName = nutritionalProfile.GetLiquidClassname();
		string underscored = ExpansionString.CamelCaseToWords(clsName, "_");
		string displayName;
		GetGame().ConfigGetTextRaw("CfgLiquidDefinitions " + clsName +  " displayName", displayName);
		GetGame().FormatRawConfigStringKeys(displayName);

		string translated;
		if (displayName.IndexOf("#") == 0)
			translated = Widget.TranslateString(displayName);
		else
			translated = displayName;

		//! Fix up vanilla liquid display name
		if (displayName.IndexOf("#STR_cfgLiquidDefinitions_") == 0 && translated.IndexOf("$UNT$") == 0)
			translated = Widget.TranslateString("#inv_inspect_" + underscored);

		if (upperCase)
			translated.ToUpper();

		color = Colors.COLOR_LIQUID;

	#ifdef EXPANSION_INSPECT_MENU_NEW_ENABLE
		if (GetExpansionClientSettings().EnableLiquidTypeColors)
		{
			string colorPath = "CfgLiquidDefinitions " + clsName +  " color";

			color = GetGame().ConfigGetInt(colorPath);

			if (!color)
			{
				string colorConstantName;
				GetGame().ConfigGetTextRaw(colorPath, colorConstantName);

				if (!colorConstantName)
				{
					//! Fallback to liquid classname, all uppercase
					colorConstantName = clsName;
					colorConstantName.ToUpper();
				}

				bool found = ExpansionStatic.StringToEnumEx(Colors, colorConstantName, color);

				if (!found)
				{
					if (!colorConstantName.Contains("LIQUID"))
					{
						colorConstantName += "LIQUID";  //! e.g. RaG_Liquid_Framework
						found = ExpansionStatic.StringToEnumEx(Colors, colorConstantName, color);
					}

					if (!found)
					{
						//! Fallback to liquid classname, all uppercase, words delimited by underscore
						colorConstantName = underscored;
						colorConstantName.ToUpper();

						if (!ExpansionStatic.StringToEnumEx(Colors, colorConstantName, color))
						{
							switch (liquidType)
							{
								case LIQUID_BEER:
									color = Colors.ORANGE;
									break;
								case LIQUID_DIESEL:
								case LIQUID_GASOLINE:
									color = Colors.YELLOW;
									break;
								case LIQUID_DISINFECTANT:
								case LIQUID_VODKA:
									color = Colors.GRAY;
									break;
								default:
									color = Colors.COLOR_LIQUID;
									break;
							}
						}
					}
				}
			}
		}
	#endif

		return translated;
	}
};
