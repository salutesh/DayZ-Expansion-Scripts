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
		if (!g_Game)
			return;

#ifdef DIAG_DEVELOPER
		Print("~ExpansionWorld");
#endif
	}

	override string GetPlayerID(Man player)
	{
		PlayerIdentity identity = player.GetIdentity();
		if (identity)
			return identity.GetId();

		PlayerBase pb;
		if (Class.CastTo(pb, player))
			return pb.GetCachedID();

		return string.Empty;
	}

	override string GetPlayerName(Man player)
	{
		PlayerIdentity identity = player.GetIdentity();
		if (identity)
			return identity.GetName();

		PlayerBase pb;
		if (Class.CastTo(pb, player))
			return pb.GetCachedName();

		return string.Empty;
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

		if (g_Game.IsServer())
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
		//! Impulse examples:
		//! ~15000 = Offroadhatchback at ~40 km/h
		//! ~24000 = OffroadHatchback at ~70 km/h
		//! ~75000 = MH6 at ~160 km/h
		//! ~200000-300000 = Merlin at ~160 km/h

		if (impulse < 7500)
			return;

		Plant plant;
		if (!Plant.CastTo(plant, other) || plant.IsDamageDestroyed())
			return;

		vector minMax[2];
		if (plant.GetCollisionBox(minMax))
		{
			float height = minMax[1][1] - minMax[0][1];

			//! Tree tiers:
			//! _1s/_1sb ~4 m
			//! _1f ~8 m
			//! _2d ~16 m
			//! _2s ~17 m
			//! _2sb ~18 m
			//! _3d ~24 m
			//! _3s ~26 m
			//! _3f ~28 m

			//! Require a minimum impulse based on tree height. E.g. a 5 m tree will require an impulse of 15k to be felled.
			if (impulse < height * 3000)
				return;
		}

		//! @note server segfault under DayZ 1.28 Experimental if setting plant health to zero in same frame as contact event, so defer using Call
		if (g_Game.IsServer())
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(FellPlant, plant, sendToClient);
	}

	static void FellPlant(Plant plant, bool sendToClient = false)
	{
		if (plant && !plant.IsDamageDestroyed())
		{
			PlayFellPlantSound(plant, sendToClient);
			plant.SetHealth(0);
		}
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
		else if (entity.IsInherited(BoatScript))
			return BoatScript;
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
		g_Game.ConfigGetTextRaw("CfgLiquidDefinitions " + clsName +  " displayName", displayName);
		g_Game.FormatRawConfigStringKeys(displayName);

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

			color = g_Game.ConfigGetInt(colorPath);

			if (!color)
			{
				string colorConstantName;
				g_Game.ConfigGetTextRaw(colorPath, colorConstantName);

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
				AIWorld aiWorld = g_Game.GetWorld().GetAIWorld();
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

	static bool IsStoreLoaded(EntityAI entity)
	{
		ItemBase item;
		ExpansionVehicle vehicle;
		if (Class.CastTo(item, entity))
			return item.Expansion_IsStoreLoaded();
		else if (ExpansionVehicle.Get(vehicle, entity))
			return vehicle.IsStoreLoaded();

		return false;
	}

	static bool IsStoreSaved(EntityAI entity)
	{
		ItemBase item;
		ExpansionVehicle vehicle;
		if (Class.CastTo(item, entity))
			return item.Expansion_IsStoreSaved();
		else if (ExpansionVehicle.Get(vehicle, entity))
			return vehicle.IsStoreSaved();

		return false;
	}
};
