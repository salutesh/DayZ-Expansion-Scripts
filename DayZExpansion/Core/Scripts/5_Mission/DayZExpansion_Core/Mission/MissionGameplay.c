/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		MissionGameplay
 * @brief		
 **/
modded class MissionGameplay
{
	protected static ref ExpansionItemTooltip m_EXItemTooltip;
	protected static ref ExpansionItemInspection m_EXItemInspection;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void MissionGameplay()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "MissionGameplay");
#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~MissionGameplay()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "~MissionGameplay");
#endif
		
		DestroyNotificationSystem();
	}
	
	// ------------------------------------------------------------
	// Override PlayerControlDisable
	// ------------------------------------------------------------
	override void PlayerControlDisable(int mode)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "PlayerControlDisable");
#endif
	
		switch (mode)
		{
			case INPUT_EXCLUDE_ALL:
			{
				GetUApi().ActivateExclude("menuexpansion");
				GetUApi().UpdateControls();
				break;
			}
		}
		
		super.PlayerControlDisable(mode);
	}

	// ------------------------------------------------------------
	// OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate(float timeslice)
	{	
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnUpdate");
#endif
		
		super.OnUpdate(timeslice);
		
		if (!m_bLoaded)
		{
			return;
		}

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		bool isAliveConscious = player && player.GetPlayerState() == EPlayerStates.ALIVE && !player.IsUnconscious();

		Input input = GetGame().GetInput();

		//! Checking for keyboard focus
		Widget focus = GetFocus();
		bool inputIsFocused = focus && (focus.IsInherited(EditBoxWidget) || focus.IsInherited(MultilineEditBoxWidget)) && focus.IsVisible();

		UIScriptedMenu menu = m_UIManager.GetMenu();
		ExpansionScriptViewMenuBase viewMenu = GetDayZExpansion().GetExpansionUIManager().GetMenu();

		Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);
	}

	void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		//! Close current opened expansion script view menu when ESC is pressed
		if (viewMenu && viewMenu.IsVisible() && input.LocalPress("UAUIBack", false))
		{
			if (!m_EXItemInspection && viewMenu.CanClose())
			{
				GetDayZExpansion().GetExpansionUIManager().CloseMenu();
			}
			else if (m_EXItemInspection)
			{
				Expansion_DestroyItemInspection();
				viewMenu.GetLayoutRoot().Show(true);
			}
		}
	}

	// ------------------------------------------------------------
	// Pause
	// ------------------------------------------------------------	
	override void Pause()
	{
		if (GetDayZExpansion().GetExpansionUIManager().GetMenu())
			return;

		super.Pause();
	}
	
	// ------------------------------------------------------------
	// Override CloseAllMenus
	// ------------------------------------------------------------
	override void CloseAllMenus()
	{
		super.CloseAllMenus();
		
		if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			GetDayZGame().GetExpansionGame().GetExpansionUIManager().CloseAll();
	}

	// ------------------------------------------------------------
	// IsMenuOpened
	// ------------------------------------------------------------
	bool IsMenuOpened()
	{
		if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			return true;
		
		return false;
	}
	
	override void OnMissionLoaded()
	{
		ExpansionGlobalID.s_IsMissionLoaded = true;

		super.OnMissionLoaded();
	}

	// ------------------------------------------------------------
	// OnMissionFinish
	// ------------------------------------------------------------
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		//! Close and destroy any open ScriptView menus. This will return control to the player and unlock all inputs
		ExpansionUIManager manager = GetDayZGame().GetExpansionGame().GetExpansionUIManager();
		ExpansionScriptViewMenuBase menu = manager.GetMenu();
		if (menu)
		{
			manager.CloseMenu();
		}
	}
	
	static ExpansionItemTooltip Expansion_GetItemTooltip()
	{
		if (!m_EXItemTooltip)
			m_EXItemTooltip = new ExpansionItemTooltip();

		return m_EXItemTooltip;
	}
	
	static void Expansion_DestroyItemTooltip()
	{
		if (m_EXItemTooltip)
		{
			m_EXItemTooltip.Destroy();
			m_EXItemTooltip = null;
		}
	}
	
	static ExpansionItemTooltip SetItemTooltip(Object object)
	{
		ExpansionItemTooltip itemTooltip = Expansion_GetItemTooltip();
		if (itemTooltip)
		{
			itemTooltip.SetItem(EntityAI.Cast(object));
			itemTooltip.SetShowItemPreview(true);
			itemTooltip.SetUpdateStatsOnShow(true);
			itemTooltip.Show();
		}
		
		return itemTooltip;
	}
	
	static ExpansionItemTooltip SetItemTooltip(Object object, int healthLevel, int liquidType, bool isBloodContainer, int quantityType, float quantity, int quantityMax, int foodStage, string className, int rarity = -1)
	{
		ExpansionItemTooltip itemTooltip = Expansion_GetItemTooltip();
		if (itemTooltip)
		{
			itemTooltip.SetItem(EntityAI.Cast(object));
			itemTooltip.SetShowItemPreview(true);
			itemTooltip.SetUpdateStatsOnShow(false);		
			if (!object.IsInherited(Car))
			{
				itemTooltip.UpdateItemInfoDamage(healthLevel);
				itemTooltip.UpdateItemInfoLiquidType(liquidType, isBloodContainer);
				itemTooltip.UpdateItemInfoQuantity(quantityType, quantity, quantityMax, false);
				itemTooltip.UpdateItemInfoFoodStage(foodStage);
				itemTooltip.UpdateItemInfoCargoSize(className);
			#ifdef EXPANSIONMODHARDLINE
				if (rarity != -1)
					itemTooltip.UpdateItemRarity(rarity);
			#endif
			}
			
			itemTooltip.Show();
		}
		
		return itemTooltip;
	}
	
	
	static ExpansionItemInspection Expansion_GetItemInspection()
	{
		if (!m_EXItemInspection)
			m_EXItemInspection = new ExpansionItemInspection();
		
		return m_EXItemInspection;
	}

	static void Expansion_DestroyItemInspection()
	{
		if (m_EXItemInspection)
		{
			m_EXItemInspection.Destroy();
			m_EXItemInspection = null;
		}
	}

	static void InspectItem(ScriptView parentView, Object object)
	{
		ExpansionItemInspection itemInspection = Expansion_GetItemInspection();
		if (itemInspection)
		{
			itemInspection.SetItem(object);
			itemInspection.UpdateItemStats();
			itemInspection.SetParentView(parentView);
			parentView.GetLayoutRoot().Show(false);
		}
	}
	
	static void InspectItem(ScriptView parentView, Object object, int healthLevel, int liquidType, bool isBloodContainer, int quantityType, float quantity, int quantityMax, int foodStage, string className, int rarity = -1)
	{
		ExpansionItemInspection itemInspection = Expansion_GetItemInspection();
		if (itemInspection)
		{
			itemInspection.SetItem(object);			
			if (!object.IsInherited(Car))
			{
				itemInspection.UpdateItemInfoDamage(healthLevel);
				itemInspection.UpdateItemInfoLiquidType(liquidType, isBloodContainer);
				itemInspection.UpdateItemInfoQuantity(quantityType, quantity, quantityMax, false);
				itemInspection.UpdateItemInfoFoodStage(foodStage);
				itemInspection.UpdateItemInfoCargoSize(className);
			#ifdef EXPANSIONMODHARDLINE
				if (rarity != -1)
					itemInspection.UpdateItemRarity(rarity);
			#endif
			}
			
			itemInspection.SetParentView(parentView);
			parentView.GetLayoutRoot().Show(false);
		}
	}
};
