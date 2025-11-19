/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
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
#ifdef EXPANSIONUI
	protected static ref ExpansionItemTooltip m_EXItemTooltip;
	protected static ref ExpansionItemInspection m_EXItemInspection;
#endif

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

	void Expansion_EnableAllInputs()
	{
		TIntArray inputIDs = new TIntArray;
		GetUApi().GetActiveInputs(inputIDs);

		foreach (int input : inputIDs)
		{
			GetUApi().GetInputByID(input).ForceDisable(false);
		}

		GetUApi().UpdateControls();
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

		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());

		bool isAliveConscious;
		if (player && player.GetPlayerState() == EPlayerStates.ALIVE && !player.IsUnconscious())
			isAliveConscious = true;

		Input input = g_Game.GetInput();

		//! Checking for keyboard focus
		Widget focus = GetFocus();
		bool inputIsFocused;
		if (focus && (focus.IsInherited(EditBoxWidget) || focus.IsInherited(MultilineEditBoxWidget)) && focus.IsVisible())
			inputIsFocused = true;

		UIScriptedMenu menu = m_UIManager.GetMenu();
	#ifdef EXPANSIONUI
		ExpansionScriptViewMenuBase viewMenu = GetDayZExpansion().GetExpansionUIManager().GetMenu();
	#else
		ExpansionScriptViewMenuBase viewMenu;
	#endif

		Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);
	}

	void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
	#ifdef EXPANSIONUI
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
	#endif
	}

#ifdef EXPANSIONUI
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
#endif

	// ------------------------------------------------------------
	// IsMenuOpened
	// ------------------------------------------------------------
	bool IsMenuOpened()
	{
	#ifdef EXPANSIONUI
		if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			return true;
	#endif
		
		return false;
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		
		GetDayZExpansion().OnStart();
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
		
	#ifdef EXPANSIONUI
		//! Close and destroy any ScriptViews
		ExpansionScriptViewBase.DestroyAll();
	#endif

		//! This will return control to the player and unlock all inputs
		Expansion_EnableAllInputs();
		if (PPEffects.m_BlurMenu > 0)
			PPEffects.SetBlurMenu(0.0);

		GetDayZExpansion().OnFinish();
	}
	
#ifdef EXPANSIONUI
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
			m_EXItemTooltip.Hide();
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
			itemTooltip.UpdateItemInfoDamage(healthLevel);
			if (!object.IsTransport())
			{
				itemTooltip.UpdateItemInfoLiquidType(liquidType, isBloodContainer);
				itemTooltip.UpdateItemInfoQuantity(quantityType, quantity, quantityMax, false);
				itemTooltip.UpdateItemInfoFoodStage(foodStage);
			}
			itemTooltip.UpdateItemInfoCargoSize();
		#ifdef EXPANSIONMODHARDLINE
			if (GetExpansionSettings().GetHardline().EnableItemRarity && rarity != -1)
				itemTooltip.UpdateItemRarityEx(rarity);
		#endif
			
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

	static ExpansionItemInspection InspectItem(ScriptView parentView, Object object)
	{
		ExpansionItemInspection itemInspection = Expansion_GetItemInspection();
		if (itemInspection)
		{
			itemInspection.SetItem(object);
			itemInspection.UpdateItemStats();
			itemInspection.SetParentView(parentView);
			parentView.GetLayoutRoot().Show(false);
		}
		
		return itemInspection;
	}
	
	static ExpansionItemInspection InspectItem(ScriptView parentView, Object object, int healthLevel, int liquidType, bool isBloodContainer, int quantityType, float quantity, int quantityMax, int foodStage, string className, int rarity = -1)
	{
		ExpansionItemInspection itemInspection = Expansion_GetItemInspection();
		if (itemInspection)
		{
			itemInspection.SetItem(object);
			itemInspection.UpdateItemInfoDamage(healthLevel);
			if (!object.IsTransport())
			{
				itemInspection.UpdateItemInfoLiquidType(liquidType, isBloodContainer);
				itemInspection.UpdateItemInfoQuantity(quantityType, quantity, quantityMax, false);
				itemInspection.UpdateItemInfoFoodStage(foodStage);
			}
			itemInspection.UpdateItemInfoCargoSize();
		#ifdef EXPANSIONMODHARDLINE
			if (GetExpansionSettings().GetHardline().EnableItemRarity && rarity != -1)
				itemInspection.UpdateItemRarityEx(rarity);
		#endif
			
			itemInspection.SetParentView(parentView);
			parentView.GetLayoutRoot().Show(false);
		}
		
		return itemInspection;
	}
#endif
};
