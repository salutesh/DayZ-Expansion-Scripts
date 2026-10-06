/**
 * IngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class IngameHud
{
	protected float SCREEN_X = 0.5615;
	protected float SCREEN_Y = 0.55;

	//! Player Tag
	protected ImageWidget m_PlayerTagIcon;

	protected EntityAI m_CurrentTaggedItem;
	protected DayZPlayerImplement m_CurrentTaggedNPC;
	protected EntityAI m_CurrentTaggedObject;
	protected bool m_IsMember = false;
	protected bool m_IsFriendly = false;
	#ifdef EXPANSIONMODAI
	protected string m_FactionName;
	#endif

	#ifdef EXPANSIONMODHARDLINE
	protected ExpansionHardlineItemRarity m_CurrentTaggedItemRarity = -1;
	#endif

	protected string m_CurrentTaggedItemIcon;

	protected ExpansionNameTagsSettings m_Expansion_NameTagsSettings;

	#ifdef DIAG_DEVELOPER
	static bool s_Expansion_Debug;
	ref DebugTextWorldSpace m_Expansion_DebugText;
	#endif
	
	void ~IngameHud()
	{
		if (m_PlayerTag && m_PlayerTag.ToString() != "INVALID")
			m_PlayerTag.Unlink();

		#ifdef DIAG_DEVELOPER
		if (m_Expansion_DebugText)
			Debug.RemoveTextWS(m_Expansion_DebugText);
		#endif
	}

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		m_Expansion_NameTagsSettings = GetExpansionSettings().GetNameTags(false);
	}

	protected void Expansion_CreatePlayerTagWigets()
	{
		m_PlayerTag = g_Game.GetWorkspace().CreateWidgets("DayZExpansion/NameTags/GUI/layouts/expansion_hud_player_tag.layout");
		m_PlayerTagText = TextWidget.Cast(m_PlayerTag.FindAnyWidget("TagText"));
		m_PlayerTagIcon = ImageWidget.Cast(m_PlayerTag.FindAnyWidget("TagIcon"));
	}

	protected void Expansion_ClearPlayerTagWidgets(float timeslice)
	{
		float new_alpha = Math.Clamp(m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1);
		m_PlayerTagText.SetAlpha(new_alpha);
		m_PlayerTagIcon.SetAlpha(Math.Clamp(m_PlayerTagIcon.GetAlpha() - timeslice * 10, 0, 1));
		if (new_alpha == 0)
		{
			m_PlayerTagText.SetText("");
			m_PlayerTagIcon.Show(false);
			m_CurrentTaggedPlayer = null;
		}
	}

	[Obsolete("no replacement")]
	protected void Expansion_SetPlayerTagIconPath()
	{
	}

	protected bool Expansion_IsPlayerTagAllowed(DayZPlayerImplement player, bool safeZone, bool territory)
	{
		if (!player)
			return false;

		if (!safeZone && !territory)
			return true;

		if (safeZone && player.Expansion_IsInSafeZone())
			return true;

		#ifdef EXPANSIONMODBASEBUILDING
		if (territory)
		{
			PlayerBase pb;
			if (Class.CastTo(pb, player) && pb.IsInTerritory())
				return true;
		}
		#endif

		return false;
	}

	protected bool Expansion_ShouldShowPlayerTag(EntityAI entity)
	{
		if (!entity)
			return false;

		vector head_pos = g_Game.GetCurrentCameraPosition();
		vector target_pos;

		if (entity.IsMan())
			target_pos = entity.GetBonePositionWS(entity.GetBoneIndex("Spine2"));
		else
			target_pos =  entity.GetPosition();

		float distanceSq = vector.DistanceSq(head_pos, target_pos);
		if (distanceSq > Math.SqrFloat(m_Expansion_NameTagsSettings.PlayerTagViewRange))
			return false;

		vector screen_pos = g_Game.GetScreenPosRelative(target_pos);
		return screen_pos[2] > 0 && screen_pos[0] > 0 && screen_pos[0] < 1 && screen_pos[1] > 0 && screen_pos[1] < 1;
	}

	protected void Expansion_RefreshPlayerTagsEx()
	{
		m_CurrentTaggedPlayer = null;
		m_CurrentTaggedItem = null;
		m_CurrentTaggedItemIcon = "";
		m_CurrentTaggedObject = null;
		m_CurrentTaggedNPC = null;

		#ifdef EXPANSIONMODHARDLINE
		m_CurrentTaggedItemRarity = -1;
		#endif

		#ifdef EXPANSIONMODAI
		m_FactionName = "";
		#endif

		m_IsMember = false;
		m_IsFriendly = false;

		bool safeZone = m_Expansion_NameTagsSettings.OnlyInSafeZones;
		bool territory = m_Expansion_NameTagsSettings.OnlyInTerritories;

		if (!Expansion_IsPlayerTagAllowed(DayZPlayerImplement.Cast(g_Game.GetPlayer()), safeZone, territory))
			return;

		vector head_pos = g_Game.GetCurrentCameraPosition();
		vector end_pos = head_pos + g_Game.GetCurrentCameraDirection() * m_Expansion_NameTagsSettings.PlayerTagViewRange;
		RaycastRVParams params = new RaycastRVParams(head_pos, end_pos, g_Game.GetPlayer(), 0);
		params.sorted = true;

		if (m_Expansion_NameTagsSettings.ShowPlayerItemInHands)
			params.flags = CollisionFlags.ALLOBJECTS;

		array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;
		DayZPhysics.RaycastRVProxy(params, results);

		#ifdef DIAG_DEVELOPER
		string dbgTxt;
		vector hitPos;
		#endif

		 //! @note when sorted = true, closest result will be last in the list, so we iterate in reverse order
		for (int i = results.Count() - 1; i >= 0; --i)
		{
			RaycastRVResult result = results[i];

			#ifdef DIAG_DEVELOPER
			if (s_Expansion_Debug)
			{
				if (result.obj)
					dbgTxt += result.obj.GetDebugNameNative();
				if (result.parent)
					dbgTxt += " (root " + result.parent.GetDebugNameNative() + ")";
				dbgTxt += "\n";

				hitPos = result.pos;
			}
			#endif

			Object obj;

			if (!m_Expansion_NameTagsSettings.ShowPlayerItemInHands && result.hierLevel > 0)
				obj = result.parent;
			else
				obj = result.obj;

			EntityAI entity = EntityAI.Cast(obj);

			if (!entity)
				continue;

			EntityAI parent = entity.GetHierarchyParent();
			if (parent && !parent.IsMan())
				entity = parent;

			DayZPlayerImplement targetPlayer = DayZPlayerImplement.Cast(entity.GetHierarchyRoot());

			if (targetPlayer)
			{
				if (!targetPlayer.IsAlive())
					break;

				if (!Expansion_IsPlayerTagAllowed(targetPlayer, safeZone, territory))
					break;

				if (targetPlayer == entity)
				{
					if (targetPlayer.GetIdentity())
					{
						if (m_Expansion_NameTagsSettings.EnablePlayerTags)
							m_CurrentTaggedPlayer = PlayerBase.Cast(targetPlayer);

						break;
					}
				}
				else if (m_Expansion_NameTagsSettings.ShowPlayerItemInHands && targetPlayer.GetHumanInventory().GetEntityInHands() == entity)
				{
					m_CurrentTaggedItem = entity;

					#ifdef EXPANSIONMODHARDLINE
					if (m_Expansion_NameTagsSettings.UseRarityColorForItemInHands)
					{
						ItemBase itemIB;
						Class.CastTo(itemIB, m_CurrentTaggedItem);
						if (itemIB)
							m_CurrentTaggedItemRarity = itemIB.Expansion_GetRarity();
					}
					#endif

					if (entity.IsWeapon() && (!entity.ConfigIsExisting("DisplayMagazine") || entity.ConfigGetBool("DisplayMagazine")))
					{
						string path = "CfgSlots Slot_";

						//! Show different magazine icon for firearms and pistols
						if (!entity.IsInherited(Pistol_Base))
							path += "magazine2";
						else
							path += "magazine";

						string icon_name = "";
						if (g_Game.ConfigGetText(path + " ghostIcon", icon_name) && icon_name != "")
							m_CurrentTaggedItemIcon = StaticGUIUtils.VerifyIconImageString(StaticGUIUtils.IMAGESETGROUP_INVENTORY, icon_name);
					}

					break;
				}
			}

			if (m_Expansion_NameTagsSettings.ShowNPCTags)
			{
				ExpansionNPCBase npc;
				#ifdef EXPANSIONMODAI
				eAIBase ai;
				#endif
				ExpansionStaticObjectBase staticObject;

				if (Class.CastTo(npc, entity))
					m_CurrentTaggedNPC = npc;
				#ifdef EXPANSIONMODAI
				else if (Class.CastTo(ai, entity))
					m_CurrentTaggedNPC = ai;
				#endif
				else if (Class.CastTo(staticObject, entity))
					m_CurrentTaggedObject = staticObject;
			}

			break;
		}

		#ifdef DIAG_DEVELOPER
		if (s_Expansion_Debug)
		{
			if (m_Expansion_DebugText)
			{
				m_Expansion_DebugText.SetText(dbgTxt);
				TStringArray dbgTxtLines = {};
				dbgTxt.Split("\n", dbgTxtLines);
				m_Expansion_DebugText.SetPosition(hitPos[0], hitPos[1] + dbgTxtLines.Count() * 0.05, hitPos[2]);
			}
			else
				m_Expansion_DebugText = Debug.DrawTextWS(dbgTxt, hitPos, 15);
		}
		#endif
	}

	protected void Expansion_ShowPlayerTagEx(float timeslice)
	{
		if (m_CurrentTaggedPlayer)
		{
			if (HandleCurrentTaggedCharacter(m_CurrentTaggedPlayer, timeslice))
				return;
		}
		else if (m_CurrentTaggedNPC)
		{
			if (HandleCurrentTaggedCharacter(m_CurrentTaggedNPC, timeslice))
				return;
		}
		else if (m_CurrentTaggedItem)
		{
			if (HandleCurrentTaggedItem(timeslice))
				return;
		}
		else if (m_CurrentTaggedObject)
		{
			if (HandleCurrentTaggedObject(timeslice))
				return;
		}

		if (m_PlayerTag)
			Expansion_ClearPlayerTagWidgets(timeslice);
	}

	//! Players
	protected bool HandleCurrentTaggedPlayer(float timeslice)
	{
		return HandleCurrentTaggedCharacter(m_CurrentTaggedPlayer, timeslice);
	}

	//! NPC/AI
	protected bool HandleCurrentTaggedNPC(float timeslice)
	{
		return HandleCurrentTaggedCharacter(m_CurrentTaggedNPC, timeslice);
	}

	//! Players and NPC/AI
	protected bool HandleCurrentTaggedCharacter(DayZPlayerImplement character, float timeslice)
	{
		if (!Expansion_ShouldShowPlayerTag(character))
			return false;

		int iconColor = m_Expansion_NameTagsSettings.PlayerTagsColor;
		int textColor;
		string icon = m_Expansion_NameTagsSettings.PlayerTagsIcon;
		string nameText;

		if (character == m_CurrentTaggedPlayer)
		{
			//! Player
			nameText = m_CurrentTaggedPlayer.GetIdentityName();

			#ifdef EXPANSIONMODGROUPS
			GetGroup(m_CurrentTaggedPlayer);
			#endif
		}
		else
		{
			//! NPC/AI
			nameText = character.GetDisplayName();
			int npcID = -1;

			if (character.IsInherited(ExpansionNPCBase))
			{
				#ifdef EXPANSIONMODQUESTS
				ExpansionQuestNPCBase questNPC;
				if (Class.CastTo(questNPC, character))
					npcID =  questNPC.GetQuestNPCID();
				#endif
			}
			#ifdef EXPANSIONMODAI
			else if (character.IsInherited(eAINPCBase))
			{
				#ifdef EXPANSIONMODQUESTS
				ExpansionQuestNPCAIBase questNPCAI;
				if (Class.CastTo(questNPCAI, character))
					npcID =  questNPCAI.GetQuestNPCID();
				#endif
			}
			else if (character.IsInherited(eAIBase))
			{
				icon = "Soldier";
			}
			#endif

			character.m_Expansion_NetsyncData.Get(1, icon);

			#ifdef EXPANSIONMODQUESTS
			if (npcID != -1 && ShowQuestMarker(PlayerBase.Cast(g_Game.GetPlayer()), npcID))
			{
				icon = "Exclamationmark";
				iconColor = ARGB(255, 255, 180, 24);
			}
			#endif
		}

		#ifdef EXPANSIONMODAI
		if (m_Expansion_NameTagsSettings.ShowPlayerFaction)
		{
			GetFaction(character);

			if (m_FactionName != string.Empty)
				nameText = string.Format("[%1] %2", m_FactionName, nameText);
		}
		#endif

		if (m_IsFriendly || m_IsMember)
			textColor = COLOR_EXPANSION_NOTIFICATION_SUCCESS;
		else
			textColor = COLOR_EXPANSION_NOTIFICATION_ERROR;

		#ifndef EXPANSIONMODAI
		#ifndef EXPANSIONMODGROUPS
		textColor = m_Expansion_NameTagsSettings.PlayerNameColor;
		#endif
		#endif

		Expansion_SetPlayerTag(icon, iconColor, nameText, textColor, timeslice);

		return true;
	}

	//! Entity in hands
	protected bool HandleCurrentTaggedItem(float timeslice)
	{
		if (!Expansion_ShouldShowPlayerTag(m_CurrentTaggedItem))
			return false;

		string icon = "Eye";
		string nameText = m_CurrentTaggedItem.GetDisplayName();
		int iconColor = m_Expansion_NameTagsSettings.PlayerTagsColor;
		int textColor = m_Expansion_NameTagsSettings.PlayerNameColor;

		#ifdef EXPANSIONMODHARDLINE
		if (m_CurrentTaggedItemRarity > ExpansionHardlineItemRarity.NONE)
		{
			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, m_CurrentTaggedItemRarity);
			int color, rarityColor;
			typename type = ExpansionHardlineItemRarityColor;
			ExpansionStatic.GetVariableIntByName(type, rarityName, color);
			int a, r, g, b;
			ExpansionStatic.IntToARGB(color, a, r, g, b);
			rarityColor = ExpansionStatic.ARGBtoInt(255, r, g, b);
			textColor = rarityColor;
		}
		#endif

		if (m_CurrentTaggedItemIcon != string.Empty)
			icon = m_CurrentTaggedItemIcon;

		Expansion_SetPlayerTag(icon, iconColor, nameText, textColor, timeslice);

		return true;
	}

	//! Object
	protected bool HandleCurrentTaggedObject(float timeslice)
	{
		if (!Expansion_ShouldShowPlayerTag(m_CurrentTaggedObject))
			return false;
		
		string nameText = m_CurrentTaggedObject.GetDisplayName();
		string icon = "Eye";
		int iconColor = m_Expansion_NameTagsSettings.PlayerTagsColor;
		int textColor = m_Expansion_NameTagsSettings.PlayerNameColor;

		ExpansionStaticObjectBase staticObject;
		if (Class.CastTo(staticObject, m_CurrentTaggedObject) && staticObject.m_Expansion_NetsyncData)
		{
			staticObject.m_Expansion_NetsyncData.Get(1, icon);
		}

		#ifdef EXPANSIONMODQUESTS
		ExpansionQuestStaticObject staticQuestObject;
		if (Class.CastTo(staticQuestObject, staticObject))
		{
			bool showQuestMarker = ShowQuestMarker(PlayerBase.Cast(g_Game.GetPlayer()), staticQuestObject.GetQuestNPCID());
			if (showQuestMarker)
			{
				icon = "Exclamationmark";
				iconColor = ARGB(255, 255, 180, 24);
			}
		}
		#endif

		Expansion_SetPlayerTag(icon, iconColor, nameText, textColor, timeslice);

		return true;
	}

	protected void Expansion_SetPlayerTag(string icon, int iconColor, string nameText, int textColor, float timeslice)
	{
		if (!m_PlayerTag)
		{
			Expansion_CreatePlayerTagWigets();
			m_PlayerTag.SetPos(SCREEN_X, SCREEN_Y);
		}

		string iconPath;

		if (icon != string.Empty)
		{
			iconPath = ExpansionIcons.GetPath(icon);
			if (iconPath == string.Empty)
				iconPath = icon;
		}

		m_PlayerTagIcon.LoadImageFile(0, iconPath);
		m_PlayerTagIcon.SetImage(0);
		m_PlayerTagIcon.SetColor(iconColor);
		m_PlayerTagIcon.SetAlpha(Math.Clamp(m_PlayerTagIcon.GetAlpha() + timeslice * 10, 0, 1));
		m_PlayerTagIcon.Show(true);
		m_PlayerTagText.SetColor(textColor);
		m_PlayerTagText.SetAlpha(Math.Clamp(m_PlayerTagText.GetAlpha() + timeslice * 10, 0, 1));
		m_PlayerTagText.SetText(nameText);
	}

	#ifdef EXPANSIONMODAI
	protected void GetFaction(DayZPlayerImplement player)
	{
		eAIGroup localGroup;
		eAIFaction localFaction;
		eAIFaction playerFaction;
		string factionName = "N/A";
		DayZPlayerImplement localPlayer = DayZPlayerImplement.Cast(g_Game.GetPlayer());
		eAIGroup playerGroup = player.GetGroup();
		if (!playerGroup)
			return;

		playerFaction = playerGroup.GetFaction();
		if (playerFaction)
			factionName = playerFaction.GetDisplayName();
		m_FactionName = factionName;

		localGroup = localPlayer.GetGroup();
		if (!localGroup)
			return;

		localFaction = localGroup.GetFaction();
		if (!localFaction || !playerFaction)
			return;

		if (playerFaction.IsFriendly(localFaction))
			m_IsFriendly = true;
	}
	#endif

	#ifdef EXPANSIONMODGROUPS
	protected void GetGroup(Man player)
	{
		ExpansionPartyModule partyModule;
		if (Class.CastTo(partyModule, CF_ModuleCoreManager.Get(ExpansionPartyModule)))
		{
			string playerUID = player.GetIdentity().GetId();
			ExpansionPartyData party = partyModule.GetParty();
			if (party && party.IsMember(playerUID))
				m_IsMember = true;
		}
	}
	#endif

	#ifdef EXPANSIONMODQUESTS
	bool ShowQuestMarker(PlayerBase player, int questNPCID)
	{
		map<int, ref ExpansionQuestConfig> questConfigs = ExpansionQuestModule.GetModuleInstance().GetQuestConfigs();
		if (!questConfigs || questConfigs.Count() == 0)
			return false;

		ExpansionQuestPersistentData playerQuestData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		if (!playerQuestData)
			return false;

		foreach (ExpansionQuestConfig questConfig: questConfigs)
		{
			if (ExpansionQuestModule.GetModuleInstance().QuestDisplayConditions(questConfig, player, playerQuestData, questNPCID))
				return true;
		}

		return false;
	}
	#endif

	override void Update(float timeslice)
	{
		super.Update(timeslice);

		if (!Expansion_CanShowHUDElements())
		{
			if (m_PlayerTag)
				Expansion_ClearPlayerTagWidgets(timeslice);
		}
		else
		{
			if (m_Expansion_NameTagsSettings.IsLoaded() && (m_Expansion_NameTagsSettings.EnablePlayerTags || m_Expansion_NameTagsSettings.ShowPlayerItemInHands || m_Expansion_NameTagsSettings.ShowNPCTags))
			{
				Expansion_RefreshPlayerTagsEx();
				//! Always make sure to fade the fucker out :-)
				Expansion_ShowPlayerTagEx(timeslice);
			}
		}

	}
}
