/**
 * ExpansionTeleporterMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/



class ExpansionTeleporterMenu: ExpansionScriptViewMenu
{
	protected ref ExpansionTeleporterMenuController m_TeleporterMenuController;
	protected ref ExpansionTeleportData m_TeleporterClientData;
	
	protected Widget TeleporterListPanel;
	protected Widget ButtonsPanel;
	protected Widget PositionMapPanel;
	protected MapWidget PositionMap;
	protected Widget TeleportMapPanel;
	protected MapWidget TeleportMap;
	protected Widget TeleporterDetailsPanel;
	
	protected ButtonWidget Accept;
	protected TextWidget AcceptLable;
	protected Widget AcceptBackground;
	protected ButtonWidget Complete;
	protected TextWidget CompleteLable;
	protected Widget CompleteBackground;
	protected ButtonWidget Cancel;
	protected TextWidget CancelLable;
	protected Widget CancelBackground;
	protected ButtonWidget Close;
	protected TextWidget CloseLable;
	protected ButtonWidget CloseMenu;
	protected TextWidget CloseMenuCharacter;
	protected ImageWidget CloseMenuImage;
	protected ButtonWidget Back;
	protected Widget BackBackground;
	protected ImageWidget BackImage;
	protected Widget CloseBackground;
	
	protected PlayerBase m_Player;
	
	protected ref ExpansionTeleportPosition m_SelectedTeleport;
	protected ref ExpansionTeleportPositionEntry m_SelectedTeleportPos;
	
	protected bool m_InDetailView;
	protected bool m_RequestLocked;
	
	protected ref ExpansionMapMarkerPlayerArrow m_PlayerArrowMarker;
	protected ref ExpansionTeleporterMenuMarker m_LocationMarker;
	
	void ExpansionTeleporterMenu()
	{
		Class.CastTo(m_TeleporterMenuController, GetController());
		
		ExpansionTeleporterModule.GetModuleInstance().GetTeleporterMenuSI().Insert(SetTeleportLocations);
		ExpansionTeleporterModule.GetModuleInstance().GetTeleporterMenuCallbackSI().Insert(MenuCallback);
	}

	void ~ExpansionTeleporterMenu()
	{
		ExpansionTeleporterModule.GetModuleInstance().GetTeleporterMenuSI().Remove(SetTeleportLocations);
		ExpansionTeleporterModule.GetModuleInstance().GetTeleporterMenuCallbackSI().Remove(MenuCallback);
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Teleporter/GUI/layouts/expansion_teleporter_menu.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionTeleporterMenuController;
	}
	
	void SetTeleportLocations()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		m_TeleporterClientData = ExpansionTeleporterModule.GetModuleInstance().GetTeleporterClientData();
		if (!m_TeleporterClientData)
			return;
		
		vector playerPos = GetGame().GetPlayer().GetPosition();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetMapPosition, 100, false, playerPos);
		
		m_PlayerArrowMarker = new ExpansionMapMarkerPlayerArrow(PositionMapPanel, PositionMap, true);
		m_PlayerArrowMarker.SetName(GetGame().GetPlayer().GetIdentity().GetName());
		m_PlayerArrowMarker.Show();

		m_LocationMarker = new ExpansionTeleporterMenuMarker(TeleportMapPanel, TeleportMap, true);
		
		m_TeleporterMenuController.Positions.Clear();
		
		string displayName = m_TeleporterClientData.GetDisplayName();
	#ifdef EXPANSIONMODAI
		string factionName = m_TeleporterClientData.GetFaction();
		if (factionName != string.Empty)
			displayName = "[" + factionName + "] " + displayName;
	#endif
		
		if (displayName != string.Empty)
		{
			m_TeleporterMenuController.DisplayName = displayName;
			m_TeleporterMenuController.NotifyPropertyChanged("DisplayName");
		}
		
		array<ref ExpansionTeleportPosition> teleportPositions = m_TeleporterClientData.GetTeleportPositions();
		foreach (ExpansionTeleportPosition position: teleportPositions)
		{
			ExpansionTeleporterMenuListEntry teleportEntry = new ExpansionTeleporterMenuListEntry(position, this);
			m_TeleporterMenuController.Positions.Insert(teleportEntry);
		}
	}
	
	protected bool CanUseTeleportPosition(ExpansionTeleportPosition position)
	{
		#ifdef EXPANSIONMODQUESTS
		if (GetExpansionSettings().GetQuest().EnableQuests)
		{
			int questID = position.GetQuestID();
			Print(ToString() + "::OnExecuteServer - Need to complete quest with ID: " + questID);
			if (questID > -1)
			{
				//! Check if player has completed required quest
				if (!ExpansionQuestModule.GetModuleInstance().HasCompletedQuest(questID, m_Player.GetIdentity().GetId()))
				{
					ExpansionQuestConfig questConig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(questID);
					if (!questConig)
					{
						Error(ToString() + "::OnExecuteServer - Could not get quest config for quest ID: " + questID);
						return false;
					}
		
					ExpansionNotification(new StringLocaliser("Teleporter is locked!"), new StringLocaliser("You have no access to this teleporter yet. You need to compelete the quest " + questConig.GetTitle() + " first to use this teleporter."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create();
					return false;
				}
			}
		}
	#endif
		
	#ifdef EXPANSIONMODHARDLINE
		if (GetExpansionSettings().GetHardline().UseReputation)
		{
			int reputationRequirement = position.GetReputation();
			Print(ToString() + "::OnExecuteServer - Need to have reputation: " + reputationRequirement);
			if (reputationRequirement > 0)
			{
				int reputation = m_Player.Expansion_GetReputation();
				Print(ToString() + "::OnExecuteServer - Player reputation: " + reputation);
				if (reputation < reputationRequirement)
				{
					ExpansionNotification(new StringLocaliser("Teleporter is locked!"), new StringLocaliser("You have no access to this teleporter yet. You need at least " + reputationRequirement + " reputation points first to use this teleporter."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create();
					return false;
				}
			}
		}
	#endif
		
	#ifdef EXPANSIONMODAI
		string factionName = position.GetFaction();
		bool isInFaction;
		bool isInInOtherFaction;
		Print(ToString() + "::OnExecuteServer - Need to be in faction: " + factionName);
		if (factionName != string.Empty)
		{
			eAIGroup group = m_Player.GetGroup();
			if (!group)
				group = eAIGroup.GetGroupByLeader(m_Player);
			
			Print(ToString() + "::OnExecuteServer - Player group: " + group.ToString());
			if (group)
			{
				eAIFaction playerFaction = group.GetFaction();
				Print(ToString() + "::OnExecuteServer - Player faction: " + playerFaction.ToString());
				if (playerFaction)
				{
					string playerFactionName = playerFaction.GetName();
					Print(ToString() + "::OnExecuteServer - Player faction name: " + playerFactionName);
					if (playerFactionName == factionName)
					{
						isInFaction = true;
					}
					else
					{
						if (playerFactionName != string.Empty)
							isInInOtherFaction = true;
					}
				}
			}
			
			if (!isInFaction)
			{
				string message;
				if (isInInOtherFaction)
					message = "You have no access to this teleporter. You need to be a member of the " + factionName + " faction. You are a member of the " + playerFactionName + " faction.";
				else
					message = "You have no access to this teleporter. You need to be a member of the " + factionName + " faction.";
				
				ExpansionNotification(new StringLocaliser("Teleporter is locked!"), new StringLocaliser(message), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create();
				return false;
			}
		}
	#endif
		
		return true;
	}
	
	protected void SetMapPosition(vector position)
	{
	    auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		PositionMap.SetScale(0.1);
	    PositionMap.SetMapPos(position);
	}
	
	protected void SetLocationMapPosition(vector position, string displayName = "")
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		TeleportMap.SetScale(0.1);
	    TeleportMap.SetMapPos(position);
	
		m_LocationMarker.SetName(displayName);
		m_LocationMarker.SetPosition(position);
		m_LocationMarker.SetPrimaryColor(ARGB(255, 255, 255, 255));
		m_LocationMarker.SetHoverColour(ARGB(255, 255, 255, 255));
		m_LocationMarker.Show();
	}
	
	void SetTeleportPosition(ExpansionTeleportPosition pos)
	{
		m_SelectedTeleport = null;
		
		m_SelectedTeleport = pos;
		m_InDetailView = true;
		
		TeleporterListPanel.Show(false);
		TeleporterDetailsPanel.Show(true);
		ButtonsPanel.Show(true);
		
		ExpansionTeleportPositionEntry randomPos = pos.GetPositions().GetRandomElement();
		if (randomPos)
		{
			m_SelectedTeleportPos = randomPos;
			vector tempPos = randomPos.GetPosition();
			string displayName;
			displayName = pos.GetDisplayName();
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(SetLocationMapPosition, 100, false, tempPos, displayName);
		}
	}
	
	void OnCancelButtonClick()
	{
		
	}
	
	void OnAcceptButtonClick()
	{
		if (!m_SelectedTeleport || !m_SelectedTeleportPos || m_RequestLocked)
			return;
		
		if (!CanUseTeleportPosition(m_SelectedTeleport))
			return;
		
		m_RequestLocked = true;
		ExpansionTeleporterModule.GetModuleInstance().RequestTeleport(m_SelectedTeleportPos, m_TeleporterClientData.GetObjectPosition());
		CloseMenu();
	}
	
	void OnCloseButtonClick()
	{
		if (!m_InDetailView)
		{
			CloseMenu();
		}
		else
		{
			TeleporterListPanel.Show(true);
			ButtonsPanel.Show(false);
			TeleporterDetailsPanel.Show(false);
			m_InDetailView = false;
		}
	}
	
	void MenuCallback()
	{
		
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == Accept)
		{
			AcceptBackground.SetColor(ARGB(200, 220, 220, 220));
			AcceptLable.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == Complete)
		{
			CompleteBackground.SetColor(ARGB(200, 220, 220, 220));
			CompleteLable.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == Cancel)
		{
			CancelBackground.SetColor(ARGB(200, 220, 220, 220));
			CancelLable.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == Close)
		{
			CloseBackground.SetColor(ARGB(200, 220, 220, 220));
			CloseLable.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == CloseMenu)
		{
			CloseMenuCharacter.SetColor(ARGB(255, 0, 0, 0));
			CloseMenuImage.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == Back)
		{
			BackBackground.SetColor(ARGB(200, 220, 220, 220));
			BackImage.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == Accept)
		{
			AcceptBackground.SetColor(ARGB(200, 0, 0, 0));
			AcceptLable.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == Complete)
		{
			CompleteBackground.SetColor(ARGB(200, 0, 0, 0));
			CompleteLable.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == Cancel)
		{
			CancelBackground.SetColor(ARGB(200, 0, 0, 0));
			CancelLable.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == Close)
		{
			CloseBackground.SetColor(ARGB(200, 0, 0, 0));
			CloseLable.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == CloseMenu)
		{
			CloseMenuCharacter.SetColor(ARGB(255, 220, 220, 220));
			CloseMenuImage.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == Back)
		{
			BackBackground.SetColor(ARGB(200, 0, 0, 0));
			BackImage.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}

		return false;
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		SetFocus(GetLayoutRoot());
		GetUApi().UpdateControls();
	}

	override bool CanClose()
	{
		return true;
	}
	
	override float GetUpdateTickRate()
	{
		return 0.1;
	}
	
	override void Expansion_Update()
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		
		if (m_PlayerArrowMarker)
			m_PlayerArrowMarker.Update(0.1);
		
		if (m_LocationMarker)
			m_LocationMarker.Update(0.1);
	}
};
class ExpansionTeleporterMenuController: ExpansionViewController
{
	string DisplayName;
	ref ObservableCollection<ref ExpansionTeleporterMenuListEntry> Positions = new ObservableCollection<ref ExpansionTeleporterMenuListEntry>(this);
};

class ExpansionTeleporterMenuMarker: ExpansionMapWidgetBase
{
	void ExpansionTeleporterMenuMarker(Widget parent, MapWidget mapWidget, bool autoInit = true)
	{
		SetIgnorePointer(true);
		SetIcon(ExpansionIcons.GetPath("Map Marker"));
		SetPrimaryColor(-1048336);
	}

	override void Update(float pDt)
	{
		vector mapPos = GetMapWidget().MapToScreen(m_WorldPosition);
		float x, y;
		GetLayoutRoot().GetParent().GetScreenPos(x, y);
		GetLayoutRoot().SetPos(mapPos[0] - x, mapPos[1] - y, true);
	}

	override bool IsEditButton( Widget w )
	{
		return false;
	}

	override bool CanDrag()
	{
		return false;
	}
};