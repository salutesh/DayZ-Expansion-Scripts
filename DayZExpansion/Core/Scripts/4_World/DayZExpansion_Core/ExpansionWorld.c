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
	static ref ExpansionSoundSet s_Expansion_BushFallHard_SoundSet;
	static ref ExpansionSoundSet s_Expansion_BushFallSoft_SoundSet;
	static ref ExpansionSoundSet s_Expansion_TreeFallHard_SoundSet;
	static ref ExpansionSoundSet s_Expansion_TreeFallSoft_SoundSet;

	void ExpansionWorld()
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
#endif

		s_Expansion_BushFallHard_SoundSet = ExpansionSoundSet.Register("hardBushFall_SoundSet");
		s_Expansion_BushFallSoft_SoundSet = ExpansionSoundSet.Register("softBushFall_SoundSet");
		s_Expansion_TreeFallHard_SoundSet = ExpansionSoundSet.Register("hardTreeFall_SoundSet");
		s_Expansion_TreeFallSoft_SoundSet = ExpansionSoundSet.Register("softTreeFall_SoundSet");

		ExpansionSoundSet.Register("cartent_deploy_SoundSet");
		ExpansionSoundSet.Register("GoatBleat_A_SoundSet");
		ExpansionSoundSet.Register("GoatBleat_E_SoundSet");

		GetPermissionsManager().RegisterPermission("Expansion.Creature.Lobotomize");

		m_RPCManager.RegisterClient("RPC_LobotomySync");
	}

	void ~ExpansionWorld()
	{
		if (!GetGame())
			return;

#ifdef DIAG_DEVELOPER
		Print("~ExpansionWorld");
#endif
	}

	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType) 
	{
#ifdef DIAG_DEVELOPER
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

	override void Expansion_SendFar(ExpansionScriptRPC rpc, vector position, Object target = null, bool guaranteed = false)
	{
		PlayerBase.Expansion_SendFar(rpc, position, target, guaranteed);
	}

	override void Expansion_SendNear(ExpansionScriptRPC rpc, vector position, float distance, Object target = null, bool guaranteed = false)
	{
		PlayerBase.Expansion_SendNear(rpc, position, distance, target, guaranteed);
	}

	static void CheckTreeContact(IEntity other, float impulse, bool sendToClient = false)
	{
		if (impulse < 7500)
			return;

		Plant plant;
		if (!Plant.CastTo(plant, other))
			return;

		if (GetGame().IsServer())
		{
			PlayFellPlantSound(plant, sendToClient);
			plant.SetHealth(0);
		}

		dBodyDestroy(plant);
	}

	static void PlayFellPlantSound(Object plant, bool sendToClient = false)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.SOUND, ExpansionWorld, "" + sendToClient);
	#endif

#ifdef SERVER
		if (!sendToClient)
			return;
#endif

		if (plant.IsInherited(TreeHard))
			s_Expansion_TreeFallHard_SoundSet.Play(plant.GetPosition());
		else if (plant.IsInherited(TreeSoft))
			s_Expansion_TreeFallSoft_SoundSet.Play(plant.GetPosition());
		else if (plant.IsInherited(BushHard))
			s_Expansion_BushFallHard_SoundSet.Play(plant.GetPosition());
		else if (plant.IsInherited(BushSoft))
			s_Expansion_BushFallSoft_SoundSet.Play(plant.GetPosition());
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
	#ifndef DAYZ_1_25
		else if (entity.IsInherited(BoatScript))
			return BoatScript;
	#endif
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

	/**
	 * @brief Lobotomize/unlobotomize creature
	 */
	override void Lobotomize(DayZCreatureAI creature)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this, "" + creature, "" + creature.GetAIAgent());
	#endif

		if (!creature || !creature.IsAlive())
			return;

		bool isLobotomyInProgress;
		bool isLobotomized;

		AnimalBase animal;
		ZombieBase zombie;

		if (Class.CastTo(animal, creature))
		{
			isLobotomyInProgress = animal.m_Expansion_LobotomyInProgress;
			isLobotomized = animal.Expansion_IsLobotomized();
		}
		else if (Class.CastTo(zombie, creature))
		{
			isLobotomyInProgress = zombie.m_Expansion_LobotomyInProgress;
			isLobotomized = zombie.Expansion_IsLobotomized();
		}

		if (creature.GetAIAgent() && !isLobotomized)
		{
			if (!isLobotomyInProgress)
			{
				Error(string.Format("Cannot lobotomize, use %1::Expansion_StartLobotomy first", creature));
				return;
			}

		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.MISC, this, "Lobotomizing " + creature);
		#endif

			creature.GetAIAgent().SetKeepInIdle(true);
		}
		else
		{
			if (!creature.GetAIAgent())
			{
				string templateName = creature.ConfigGetString("aiAgentTemplate");
				AIWorld aiWorld = GetGame().GetWorld().GetAIWorld();
			#ifdef DIAG_DEVELOPER
				EXTrace.Print(EXTrace.MISC, this, "Creating AI group " + templateName);
			#endif
				AIGroup aiGroup = aiWorld.CreateGroup(templateName);
			#ifdef DIAG_DEVELOPER
				EXTrace.Print(EXTrace.MISC, this, "Initializing AI agent " + aiGroup);
			#endif
				creature.InitAIAgent(aiGroup);
			}
			else
			{
				creature.GetAIAgent().SetKeepInIdle(false);
			}
		}

		if (animal)
		{
			animal.Expansion_SetLobotomized(!isLobotomized);
			animal.Expansion_EndLobotomy();
		}
		else if (zombie)
		{
			zombie.Expansion_SetLobotomized(!isLobotomized);
			zombie.Expansion_EndLobotomy();
		}

		LobotomySync(creature, !isLobotomized);
	}

	override void LobotomySync(DayZCreatureAI creature, bool isLobotomized = false)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this, "" + creature, "" + creature.GetAIAgent());
	#endif

		auto rpc = m_RPCManager.CreateRPC("RPC_LobotomySync");

		if (!creature.GetAIAgent())
			isLobotomized = true;

		rpc.Write(isLobotomized);

		Expansion_SendFar(rpc, creature.GetPosition(), creature, true);
	}

	void RPC_LobotomySync(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this);
	#endif

		bool isLobotomized;
		if (!ctx.Read(isLobotomized))
			return;

	#ifdef EXTRACE_DIAG
		EXTrace.Add(trace, isLobotomized);
	#endif

		AnimalBase animal;
		ZombieBase zombie;
		if (Class.CastTo(animal, target))
			animal.Expansion_SetLobotomized(isLobotomized);
		else if (Class.CastTo(zombie, target))
			zombie.Expansion_SetLobotomized(isLobotomized);
	}
};
