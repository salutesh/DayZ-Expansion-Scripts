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
	//! Player Tag
	protected ImageWidget m_PlayerTagIcon;
	protected string m_PlayerTagIconPath;
	
	override void Update( float timeslice )
	{
		super.Update(timeslice);
	
		//! Player Tags		
		if (GetExpansionSettings().GetNameTags().EnablePlayerTags)
		{
			RefreshPlayerTagsEx();

			//! Always make sure to fade the fucker out :-)
			ShowPlayerTagEx(timeslice);
		}
	}

	void RefreshPlayerTagsEx()
	{
		if (!GetGame().GetPlayer())
			return;

		bool safeZone = GetExpansionSettings().GetNameTags().ShowPlayerTagsInSafeZones;
		bool territory = GetExpansionSettings().GetNameTags().ShowPlayerTagsInTerritories;
		float viewrange = GetExpansionSettings().GetNameTags().PlayerTagViewRange;
		vector head_pos = GetGame().GetCurrentCameraPosition();
		float distance;

		m_CurrentTaggedPlayer = null;

		PlayerBase playerA = PlayerBase.Cast(GetGame().GetPlayer());
		PlayerBase playerB;

		bool isInSafeZone = safeZone && playerA && playerA.IsInSafeZone();

	#ifdef EXPANSIONMODBASEBUILDING
		bool isInTerritory = territory && playerA && playerA.IsInTerritory();
	#endif

		foreach (Man player : ClientData.m_PlayerBaseList)
		{
			if (!player.IsAlive() || player == GetGame().GetPlayer())
				continue;

			vector target_player_pos = player.GetPosition();
			distance = vector.Distance(head_pos, target_player_pos);
			
			if (distance > viewrange)
				continue;

			Class.CastTo(playerB, player);

			bool check = !safeZone && !territory;

			if (safeZone && isInSafeZone && playerB && playerB.IsInSafeZone())
				check = true;

		#ifdef EXPANSIONMODBASEBUILDING
			if (territory && isInTerritory && playerB && playerB.IsInTerritory())
				check = true;
		#endif

			if (!check)
				continue;

			vector end_pos = head_pos + GetGame().GetCurrentCameraDirection() * viewrange;
			RaycastRVParams params = new RaycastRVParams(head_pos, end_pos, GetGame().GetPlayer(), 0);
			params.sorted = true;
			
			array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;
			DayZPhysics.RaycastRVProxy(params, results);
			if (results.Count() > 0 && results.Get(0).obj == player)
			{
				m_CurrentTaggedPlayer = playerB;
				break;
			}
		}
	}
	
	void ShowPlayerTagEx( float timeslice )
	{
		if (m_CurrentTaggedPlayer && m_CurrentTaggedPlayer.GetIdentity() && m_CurrentTaggedPlayer.IsAlive())
		{
			if (!m_PlayerTag)
			{
				m_PlayerTag = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/NameTags/GUI/layouts/expansion_hud_player_tag.layout");
				m_PlayerTagText = TextWidget.Cast(m_PlayerTag.FindAnyWidget("TagText"));
				m_PlayerTagIcon = ImageWidget.Cast(m_PlayerTag.FindAnyWidget("TagIcon"));
				if (!m_PlayerTagIconPath)
					Expansion_SetPlayerTagIconPath();
				m_PlayerTagIcon.LoadImageFile(0, m_PlayerTagIconPath);
				m_PlayerTagIcon.SetImage(0);
			}
			
			m_PlayerSpineIndex = m_CurrentTaggedPlayer.GetBoneIndex("Spine2");
			vector player_pos = m_CurrentTaggedPlayer.GetBonePositionWS(m_PlayerSpineIndex);
			vector screen_pos = GetGame().GetScreenPosRelative(player_pos);
			
			if (screen_pos[2] > 0)
			{
				if (screen_pos[0] > 0 && screen_pos[0] < 1)
				{
					if (screen_pos[1] > 0 && screen_pos[1] < 1)
					{
						m_PlayerTagText.SetAlpha(Math.Clamp(m_PlayerTagText.GetAlpha() + timeslice * 10, 0, 1));
						m_PlayerTag.SetPos(0.55, 0.55);
						m_PlayerTagText.SetText(m_CurrentTaggedPlayer.GetIdentityName());
						m_PlayerTagIcon.SetAlpha(Math.Clamp(m_PlayerTagIcon.GetAlpha() + timeslice * 10, 0, 1));
						m_PlayerTagIcon.Show(true);
						return;
					}
				}
			}
		}
		
		if (m_PlayerTag)
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
	}

	void Expansion_SetPlayerTagIconPath()
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
			m_PlayerTagIconPath = "DayZExpansion\\Core\\GUI\\icons\\hud\\persona_64x64.edds";
		}
	}
};
