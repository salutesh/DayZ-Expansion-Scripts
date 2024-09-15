/**
 * IngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/


modded class IngameHud
{
	protected float RAYCAST_RANGE = 500.0;
	protected float MEMBER_RANGE = 500.0;
	protected float SCREEN_X = 0.5615;
	protected float SCREEN_Y = 0.55;

	//! Player Tag
	protected ImageWidget m_PlayerTagIcon;
	protected string m_PlayerTagIconPath;
	protected int m_PlayerTagIconColor;
	protected int m_PlayerNameIconColor;

	protected EntityAI m_CurrentTaggedItem;
	protected DayZPlayerImplement m_CurrentTaggedNPC;
	protected EntityAI m_CurrentTaggedObject;
	protected float m_MaxViewRange;
	protected bool m_IsMember = false;
	protected bool m_IsFriendly = false;
#ifdef EXPANSIONMODAI
	protected bool m_ShowFaction;
	protected string m_FactionName;
#endif

#ifdef EXPANSIONMODHARDLINE
	protected ExpansionHardlineItemRarity m_CurrentTaggedItemRarity = -1;
#endif

	protected ref array<string>	m_AttachmentSlotNames;
	protected string m_CurrentTaggedItemIcon;

	array<string> GetItemSlots(EntityAI e)
	{
		TStringArray searching_in = new TStringArray;
		searching_in.Insert(CFG_VEHICLESPATH);
		searching_in.Insert(CFG_WEAPONSPATH);
		searching_in.Insert(CFG_MAGAZINESPATH);

		array<string> attachments_slots	= new array<string>;
		for (int s = 0; s < searching_in.Count(); ++s)
		{
			string cfg_name = searching_in.Get( s );
			string path = cfg_name + " " + e.GetType();

			if ( GetGame().ConfigIsExisting( path ) )
			{
				GetGame().ConfigGetTextArray(path + " attachments", attachments_slots);
				if (e.IsWeapon() && (!e.ConfigIsExisting("DisplayMagazine") || e.ConfigGetBool("DisplayMagazine")))
				{
					attachments_slots.Insert( "magazine" );
				}

				return attachments_slots;
			}
		}
		if (e.IsWeapon() && (!e.ConfigIsExisting("DisplayMagazine") || e.ConfigGetBool("DisplayMagazine")))
		{
			attachments_slots.Insert("magazine");
		}

		return attachments_slots;
	}

	protected void Expansion_CreatePlayerTagWigets()
	{
		m_PlayerTag = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/NameTags/GUI/layouts/expansion_hud_player_tag.layout");
		m_PlayerTagText = TextWidget.Cast(m_PlayerTag.FindAnyWidget("TagText"));
		m_PlayerTagIcon = ImageWidget.Cast(m_PlayerTag.FindAnyWidget("TagIcon"));

		if (!m_PlayerTagIconPath)
			Expansion_SetPlayerTagIconPath();

		m_PlayerTagIcon.SetColor(m_PlayerTagIconColor);
		m_PlayerTagText.SetColor(m_PlayerNameIconColor);
		m_PlayerTagIcon.LoadImageFile(0, m_PlayerTagIconPath);
		m_PlayerTagIcon.LoadImageFile(1, "{C5A0666669DF90D2}DayZExpansion/Core/GUI/icons/hud/eye_64x64.edds");
		m_PlayerTagIcon.SetImage(0);
	}

	protected void Expansion_ClearPlayerTagWidgets(float timeslice)
	{
		float new_alpha = Math.Clamp(m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1);
		m_PlayerTagText.SetAlpha(Math.Clamp(m_PlayerTagText.GetAlpha() - timeslice * 10, 0, 1));
		m_PlayerTagIcon.SetAlpha(Math.Clamp(m_PlayerTagIcon.GetAlpha() - timeslice * 10, 0, 1));
		if (new_alpha == 0)
		{
			m_PlayerTagText.SetText("");
			m_PlayerTagIcon.Show(false);
			m_CurrentTaggedPlayer = null;
		}
	}

	protected void Expansion_SetPlayerTagIconPath()
	{
		string path = GetExpansionSettings().GetNameTags().PlayerTagsIcon;
		if (path != string.Empty)
		{
			string icon = ExpansionIcons.GetPath(path);
			if (icon != string.Empty)
			{
				 m_PlayerTagIconPath = icon;
			}
			else
			{
				m_PlayerTagIconPath = path;
			}
		}
		else
		{
			m_PlayerTagIconPath = "{287CF3AEB0F2A6E7}DayZExpansion/Core/GUI/icons/hud/persona_64x64.edds";
		}

		m_PlayerTagIconColor = GetExpansionSettings().GetNameTags().PlayerTagsColor;
		m_PlayerNameIconColor = GetExpansionSettings().GetNameTags().PlayerNameColor;

	}

	protected void Expansion_RefreshPlayerTagsEx()
	{
		if (!GetGame().GetPlayer())
			return;

		m_MaxViewRange = GetExpansionSettings().GetNameTags().PlayerTagViewRange;
	#ifdef EXPANSIONMODAI
		m_ShowFaction = GetExpansionSettings().GetNameTags().ShowPlayerFaction;
	#endif

		bool showPlayerTags = GetExpansionSettings().GetNameTags().EnablePlayerTags;
		bool showNPCTags = GetExpansionSettings().GetNameTags().ShowNPCTags;
		bool useRarityColor;
	#ifdef EXPANSIONMODHARDLINE
		useRarityColor = GetExpansionSettings().GetNameTags().UseRarityColorForItemInHands;
	#endif
		bool showPlayerItem = GetExpansionSettings().GetNameTags().ShowPlayerItemInHands;
		bool safeZone = GetExpansionSettings().GetNameTags().OnlyInSafeZones;
		bool territory = GetExpansionSettings().GetNameTags().OnlyInTerritories;
		vector head_pos = GetGame().GetCurrentCameraPosition();
		float distance;

		vector end_pos;
		RaycastRVParams params;
		array<ref RaycastRVResult> results;

		m_CurrentTaggedPlayer = null;
		m_CurrentTaggedItem = null;
		m_CurrentTaggedObject = null;
		m_CurrentTaggedNPC = null;
		m_AttachmentSlotNames = null;

	#ifdef EXPANSIONMODHARDLINE
		m_CurrentTaggedItemRarity = -1;
	#endif

	#ifdef EXPANSIONMODAI
		m_FactionName = "";
	#endif

		m_IsMember = false;
		m_IsFriendly = false;

		PlayerBase playerA = PlayerBase.Cast(GetGame().GetPlayer());
		PlayerBase playerB;

		bool isInSafeZone;
		if (safeZone && playerA && playerA.Expansion_IsInSafeZone())
			isInSafeZone = true;
	#ifdef EXPANSIONMODBASEBUILDING
		bool isInTerritory;
		if (territory && playerA && playerA.IsInTerritory())
			isInTerritory = true;
	#endif

		foreach (Man player : ClientData.m_PlayerBaseList)
		{
			EntityAI entityInHands;
			if (player.IsAlive() && player != GetGame().GetPlayer())
			{
				Class.CastTo(playerB, player);

				bool check = false;

				if (!safeZone && !territory)
					check = true;
				else if (safeZone && isInSafeZone && playerB && playerB.Expansion_IsInSafeZone())
					check = true;
			#ifdef EXPANSIONMODBASEBUILDING
				else if (territory && isInTerritory && playerB && playerB.IsInTerritory())
					check = true;
			#endif

				if (!check)
					continue;

				if (showPlayerItem && player.GetHumanInventory().GetEntityInHands())
					entityInHands = player.GetHumanInventory().GetEntityInHands();

				end_pos = head_pos + GetGame().GetCurrentCameraDirection() * RAYCAST_RANGE;
				params = new RaycastRVParams(head_pos, end_pos, GetGame().GetPlayer(), 0);
				params.sorted = true;

				results = new array<ref RaycastRVResult>;
				DayZPhysics.RaycastRVProxy(params, results);
				if (results.Count() == 0)
					return;

				Object resultObj = results.Get(0).obj;
				if (!resultObj)
					return;

				ExpansionNPCBase expNPCBase;
				Class.CastTo(expNPCBase, resultObj);
			#ifdef EXPANSIONMODAI
				eAINPCBase expAINPCBase;
				Class.CastTo(expAINPCBase, resultObj);
				eAIBase eAI;
				Class.CastTo(eAI, resultObj);
			#endif

				if (resultObj == player)
				{
				#ifdef EXPANSIONMODAI
					if (!playerB.IsAI() && playerB.GetIdentity() && showPlayerTags)
				#else
					if (playerB.GetIdentity() && showPlayerTags)
				#endif
					{
						m_CurrentTaggedPlayer = playerB;
					#ifdef EXPANSIONMODGROUPS
						GetGroup(m_CurrentTaggedPlayer);
					#endif
						break;
					}
				/*#ifdef EXPANSIONMODAI
					else if (playerB.IsAI() && showNPCTags)
					{
						m_CurrentTaggedNPC = playerB;
						break;
					}
				#endif*/
				}
				else if (resultObj == entityInHands && showPlayerItem)
				{
					m_CurrentTaggedItem = entityInHands;
				#ifdef EXPANSIONMODHARDLINE
					if (useRarityColor)
					{
						ItemBase itemIB;
						Class.CastTo(itemIB, m_CurrentTaggedItem);
						if (itemIB)
							m_CurrentTaggedItemRarity = itemIB.Expansion_GetRarity();
					}
				#endif

					m_AttachmentSlotNames = GetItemSlots(entityInHands);
					for (int i = 0; i < m_AttachmentSlotNames.Count(); i++ )
					{
						string path = "CfgSlots" + " Slot_" + m_AttachmentSlotNames[i];
						//! Show different magazine icon for firearms and pistols
						if (m_AttachmentSlotNames[i] == "magazine")
						{
							if (!entityInHands.IsInherited(Pistol_Base))
								path = "CfgSlots" + " Slot_" + "magazine2";
						}

						string icon_name = ""; //! icon_name must be in format "set:<setname> image:<imagename>"
						if (GetGame().ConfigGetText(path + " ghostIcon", icon_name) && icon_name != "")
							m_CurrentTaggedItemIcon = StaticGUIUtils.VerifyIconImageString(StaticGUIUtils.IMAGESETGROUP_INVENTORY, icon_name);
					}

					break;
				}
				else if (expNPCBase && showNPCTags)
				{
					m_CurrentTaggedNPC = expNPCBase;
					break;
				}
			#ifdef EXPANSIONMODAI
				else if (expAINPCBase && showNPCTags)
				{
					m_CurrentTaggedNPC = expAINPCBase;
					break;
				}
				else if (eAI && showNPCTags)
				{
					m_CurrentTaggedNPC = eAI;
					break;
				}
			#endif
				else
				{
					if (showNPCTags)
					{
						auto staticObject = ExpansionStaticObjectBase.Cast(resultObj);
						if (staticObject)
						{
							m_CurrentTaggedObject = staticObject;
							break;
						}
					}
				}
			}
		}
	}

	protected void Expansion_ShowPlayerTagEx(float timeslice)
	{
		if (m_CurrentTaggedPlayer && m_CurrentTaggedPlayer.GetIdentity() && m_CurrentTaggedPlayer.IsAlive())
		{
			if (HandleCurrentTaggedPlayer(timeslice))
				return;
		}
		else if (m_CurrentTaggedNPC && m_CurrentTaggedNPC.IsAlive())
		{
			if (HandleCurrentTaggedNPC(timeslice))
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
		vector target_pos = m_CurrentTaggedPlayer.GetPosition();
		vector head_pos = GetGame().GetCurrentCameraPosition();
		float distance = vector.Distance(head_pos, target_pos);
		float range = m_MaxViewRange;
		bool isMember;

		if (distance > range)
			return false;

		if (!m_PlayerTag)
		{
			Expansion_CreatePlayerTagWigets();
			m_PlayerTag.SetPos(SCREEN_X, SCREEN_Y);
		}

	#ifdef EXPANSIONMODAI
		if (m_ShowFaction)
			GetFaction(m_CurrentTaggedPlayer);
	#endif

		m_PlayerSpineIndex = m_CurrentTaggedPlayer.GetBoneIndex("Spine2");
		vector player_pos = m_CurrentTaggedPlayer.GetBonePositionWS(m_PlayerSpineIndex);
		vector screen_pos = GetGame().GetScreenPosRelative(player_pos);
		string nameText = m_CurrentTaggedPlayer.GetIdentityName();
		int iconColor = ARGB(255, 230, 230, 230);

		if (screen_pos[2] > 0)
		{
			if (screen_pos[0] > 0 && screen_pos[0] < 1)
			{
				if (screen_pos[1] > 0 && screen_pos[1] < 1)
				{
					m_PlayerTagText.SetColor(ARGB(255, 230, 230, 230));
					m_PlayerTagText.SetAlpha(Math.Clamp(m_PlayerTagText.GetAlpha() + timeslice * 10, 0, 1));

				#ifdef EXPANSIONMODAI
					if (m_FactionName != string.Empty)
					{
						nameText = string.Format("[%1] %2", m_FactionName, nameText);
					}
				#endif

					if (m_IsFriendly || m_IsMember)
					{
						m_PlayerTagText.SetColor(COLOR_EXPANSION_NOTIFICATION_SUCCESS);
					}
					else
					{
						m_PlayerTagText.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
					}

					m_PlayerTagIcon.LoadImageFile(0, m_PlayerTagIconPath);
					m_PlayerTagText.SetText(nameText);
					m_PlayerTagIcon.SetImage(0);
					m_PlayerTagIcon.SetColor(iconColor);
					m_PlayerTagIcon.SetAlpha(Math.Clamp(m_PlayerTagIcon.GetAlpha() + timeslice * 10, 0, 1));
					m_PlayerTagIcon.Show(true);
					return true;
				}
			}
		}

		return false;
	}

	//! NPC/AI
	protected bool HandleCurrentTaggedNPC(float timeslice)
	{
		if (!m_PlayerTag)
		{
			Expansion_CreatePlayerTagWigets();
			m_PlayerTag.SetPos(SCREEN_X, SCREEN_Y);
		}

	#ifdef EXPANSIONMODAI
		if (m_ShowFaction)
			GetFaction(m_CurrentTaggedNPC);
	#endif

		m_PlayerSpineIndex = m_CurrentTaggedNPC.GetBoneIndex("Spine2");
		vector player_pos = m_CurrentTaggedNPC.GetBonePositionWS(m_PlayerSpineIndex);
		vector screen_pos = GetGame().GetScreenPosRelative(player_pos);
		string npcName = "Unknown";
		string iconPath = m_PlayerTagIconPath;
		int iconColor = ARGB(255, 230, 230, 230);
		string icon;
		string nameText;
		bool showQuestMarker;

		if (screen_pos[2] > 0)
		{
			if (screen_pos[0] > 0 && screen_pos[0] < 1)
			{
				if (screen_pos[1] > 0 && screen_pos[1] < 1)
				{
					m_PlayerTagText.SetColor(ARGB(255, 230, 230, 230));
					m_PlayerTagText.SetAlpha(Math.Clamp(m_PlayerTagText.GetAlpha() + timeslice * 10, 0, 1));

					DayZPlayerImplement npcPlayer = m_CurrentTaggedNPC;
					ExpansionNPCBase expNPCBase;
					Class.CastTo(expNPCBase, npcPlayer);
				#ifdef EXPANSIONMODAI
					eAINPCBase expAINPCBase;
					Class.CastTo(expAINPCBase, npcPlayer);
					eAIBase eAI;
					Class.CastTo(eAI, npcPlayer);
				#endif

					if (expNPCBase)
					{
						expNPCBase.m_Expansion_NetsyncData.Get(0, npcName);
						expNPCBase.m_Expansion_NetsyncData.Get(1, icon);
					#ifdef EXPANSIONMODQUESTS
						ExpansionQuestNPCBase expQuestNPCBase;
						if (Class.CastTo(expQuestNPCBase, expNPCBase))
						{
							showQuestMarker = ShowQuestMarker(PlayerBase.Cast(GetGame().GetPlayer()), expQuestNPCBase.GetQuestNPCID());
							if (showQuestMarker)
							{
								icon = "{2F55C3FCBE849589}DayZExpansion/Core/GUI/icons/hud/exclamationmark_64x64.edds";
								iconColor = ARGB(255, 255, 180, 24);
							}
						}
					#endif
					}
				#ifdef EXPANSIONMODAI
					else if (expAINPCBase)
					{
						expAINPCBase.m_Expansion_NetsyncData.Get(0, npcName);
						expAINPCBase.m_Expansion_NetsyncData.Get(1, icon);
					#ifdef EXPANSIONMODQUESTS
						ExpansionQuestNPCAIBase expQuestNPCAIBase;
						if (Class.CastTo(expQuestNPCAIBase, expAINPCBase))
						{
							showQuestMarker = ShowQuestMarker(PlayerBase.Cast(GetGame().GetPlayer()), expQuestNPCAIBase.GetQuestNPCID());
							if (showQuestMarker)
							{
								icon = "{2F55C3FCBE849589}DayZExpansion/Core/GUI/icons/hud/exclamationmark_64x64.edds";
								iconColor = ARGB(255, 255, 180, 24);
							}
						}
					#endif
					}
					else if (eAI)
					{
						if (!eAI.m_Expansion_NetsyncData.Get(0, npcName))
							npcName = "#str_cfgvehicles_survivor0";
						
						icon = "{3364F58EF7F7FBE3}DayZExpansion/Core/GUI/icons/misc/T_Soldier_256x256.edds";
					}
				#endif
					else if (npcPlayer)
					{
						npcPlayer.m_Expansion_NetsyncData.Get(0, npcName);
						npcPlayer.m_Expansion_NetsyncData.Get(1, icon);
					}

					nameText = npcName;

				#ifdef EXPANSIONMODAI
					if (m_FactionName != string.Empty)
					{
						nameText = "[" + m_FactionName + "] " + nameText;
					}
				#endif

					if (m_IsFriendly)
					{
						m_PlayerTagText.SetColor(COLOR_EXPANSION_NOTIFICATION_SUCCESS);
					}
					else
					{
						m_PlayerTagText.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
					}

					if (icon != string.Empty)
					{
						iconPath = ExpansionIcons.GetPath(icon);
						if (iconPath == string.Empty)
							iconPath = icon;
					}

					m_PlayerTagIcon.LoadImageFile(0, iconPath);
					m_PlayerTagText.SetText(nameText);
					m_PlayerTagIcon.SetImage(0);
					m_PlayerTagIcon.SetColor(iconColor);
					m_PlayerTagIcon.SetAlpha(Math.Clamp(m_PlayerTagIcon.GetAlpha() + timeslice * 10, 0, 1));
					m_PlayerTagIcon.Show(true);
					return true;
				}
			}
		}

		return false;
	}

	//! Entity in hands
	protected bool HandleCurrentTaggedItem(float timeslice)
	{
		vector target_pos = m_CurrentTaggedItem.GetPosition();
		vector head_pos = GetGame().GetCurrentCameraPosition();
		float distance = vector.Distance(head_pos, target_pos);

		if (distance > m_MaxViewRange)
			return false;

		if (!m_PlayerTag)
		{
			Expansion_CreatePlayerTagWigets();
			m_PlayerTag.SetPos(SCREEN_X, SCREEN_Y);
		}

		vector screen_pos = GetGame().GetScreenPosRelative(m_CurrentTaggedItem.GetPosition());
		string iconPath = "{C5A0666669DF90D2}DayZExpansion/Core/GUI/icons/hud/eye_64x64.edds";
		string nameText = m_CurrentTaggedItem.GetDisplayName();
		int iconColor = ARGB(255, 230, 230, 230);
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
			m_PlayerTagText.SetColor(rarityColor);
		}
		else
		{
			m_PlayerTagText.SetColor(ARGB(255, 230, 230, 230));
		}
	#else
		m_PlayerTagText.SetColor(ARGB(255, 230, 230, 230));
	#endif

		if (m_CurrentTaggedItemIcon != string.Empty)
		{
			iconPath = m_CurrentTaggedItemIcon;
		}

		if (screen_pos[2] > 0)
		{
			if (screen_pos[0] > 0 && screen_pos[0] < 1)
			{
				if (screen_pos[1] > 0 && screen_pos[1] < 1)
				{
					m_PlayerTagIcon.LoadImageFile(0, iconPath);
					m_PlayerTagText.SetAlpha(Math.Clamp(m_PlayerTagText.GetAlpha() + timeslice * 10, 0, 1));
					m_PlayerTagText.SetText(nameText);
					m_PlayerTagIcon.SetImage(0);
					m_PlayerTagIcon.SetColor(iconColor);
					m_PlayerTagIcon.SetAlpha(Math.Clamp(m_PlayerTagIcon.GetAlpha() + timeslice * 10, 0, 1));
					m_PlayerTagIcon.Show(true);
					return true;
				}
			}
		}

		return false;
	}

	//! Object
	protected bool HandleCurrentTaggedObject(float timeslice)
	{
		vector target_pos = m_CurrentTaggedObject.GetPosition();
		vector head_pos = GetGame().GetCurrentCameraPosition();
		float distance = vector.Distance(head_pos, target_pos);

		if (distance > m_MaxViewRange)
			return false;

		if (!m_PlayerTag)
		{
			Expansion_CreatePlayerTagWigets();
			m_PlayerTag.SetPos(SCREEN_X, SCREEN_Y);
		}

		vector screen_pos = GetGame().GetScreenPosRelative(m_CurrentTaggedObject.GetPosition());
		if (screen_pos[2] > 0)
		{
			if (screen_pos[0] > 0 && screen_pos[0] < 2)
			{
				if (screen_pos[1] > 0 && screen_pos[1] < 2)
				{
					string nameText = "Unknown";
					string iconPath = "{C5A0666669DF90D2}DayZExpansion/Core/GUI/icons/hud/eye_64x64.edds";
					int iconColor = ARGB(255, 230, 230, 230);
					string icon;
					m_PlayerTagText.SetColor(ARGB(255, 230, 230, 230));

					auto staticObject = ExpansionStaticObjectBase.Cast(m_CurrentTaggedObject);
					if (staticObject && staticObject.m_Expansion_NetsyncData)
					{
						staticObject.m_Expansion_NetsyncData.Get(0, nameText);
						staticObject.m_Expansion_NetsyncData.Get(1, icon);
					}

				#ifdef EXPANSIONMODQUESTS
					ExpansionQuestStaticObject staticQuestObject;
					if (Class.CastTo(staticQuestObject, staticObject))
					{
						bool showQuestMarker = ShowQuestMarker(PlayerBase.Cast(GetGame().GetPlayer()), staticQuestObject.GetQuestNPCID());
						if (showQuestMarker)
						{
							icon = "{2F55C3FCBE849589}DayZExpansion/Core/GUI/icons/hud/exclamationmark_64x64.edds";
							iconColor = ARGB(255, 255, 180, 24);
						}
					}
				#endif

					if (icon != string.Empty)
					{
						iconPath = ExpansionIcons.GetPath(icon);
						if (iconPath == string.Empty)
							iconPath = icon;
					}

					m_PlayerTagIcon.LoadImageFile(0, iconPath);
					m_PlayerTagText.SetAlpha(Math.Clamp(m_PlayerTagText.GetAlpha() + timeslice * 10, 0, 1));
					m_PlayerTagText.SetText(nameText);
					m_PlayerTagIcon.SetImage(0);
					m_PlayerTagIcon.SetColor(iconColor);
					m_PlayerTagIcon.SetAlpha(Math.Clamp(m_PlayerTagIcon.GetAlpha() + timeslice * 10, 0, 1));
					m_PlayerTagIcon.Show(true);
					return true;
				}
			}
		}

		return false;
	}

#ifdef EXPANSIONMODAI
	protected void GetFaction(DayZPlayerImplement player)
	{
		eAIGroup localGroup;
		eAIFaction localFaction;
		eAIFaction playerFaction;
		string factionName = "N/A";
		DayZPlayerImplement localPlayer = DayZPlayerImplement.Cast(GetGame().GetPlayer());
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

		UIScriptedMenu topMenu = GetGame().GetUIManager().GetMenu();
		ExpansionScriptViewMenuBase viewMenu = GetDayZExpansion().GetExpansionUIManager().GetMenu();
		if ((topMenu || viewMenu || IsHideHudPlayer()) && m_PlayerTag)
		{
			Expansion_ClearPlayerTagWidgets(timeslice);
		}
		else if (!topMenu && !viewMenu && !IsHideHudPlayer())
		{
			//! Player Tags
			if (GetExpansionSettings().GetNameTags(false).IsLoaded() && (GetExpansionSettings().GetNameTags().EnablePlayerTags || GetExpansionSettings().GetNameTags().ShowPlayerItemInHands))
			{
				Expansion_RefreshPlayerTagsEx();
				//! Always make sure to fade the fucker out :-)
				Expansion_ShowPlayerTagEx(timeslice);
			}
		}

	}
};