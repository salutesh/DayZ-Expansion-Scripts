class eAICommandMenuItem
{
	protected int m_ID;
	protected string m_Name;
	protected eAICommandCategories m_Category;
	//radial menu
	protected Widget m_RadialMenuSelector;
	protected Widget m_RadialMenuItemCard;

	void eAICommandMenuItem(int id, string name, eAICommandCategories category)
	{
		m_ID = id;
		m_Name = name;
		m_Category = category;
	}

	void SetName(string name)
	{
		m_Name = name;
	}

	string GetName()
	{
		return m_Name;
	}

	string GetCategoryName()
	{
		string name = m_Name;

		eAIGroup group = eAIGroup.GetGamePlayerGroup();

		switch (m_ID)
		{
			case eAICommandCategories.CAT_FACTION:
				if (group)
					name = group.GetFaction().GetName();
				break;

			//! Formation is not synced to client
			//case eAICommandCategories.CAT_FORMATION:
				//if (group)
					//name = group.GetFormation().GetName();
				//break;

			case eAICommandCategories.CAT_DAMAGE_IN:
				name = "Damage In";
				break;
		}

		return name;
	}

	int GetID()
	{
		return m_ID;
	}

	eAICommandCategories GetCategory()
	{
		return m_Category;
	}

	Widget GetRadialItemCard()
	{
		return m_RadialMenuItemCard;
	}

	void SetRadialItemCard(Widget widget)
	{
		m_RadialMenuItemCard = widget;
	}
};

class eAICommandMenu: UIScriptedMenu
{
	protected Widget m_GestureItemCardPanel;
	protected ref array < ref eAICommandMenuItem > m_GestureItems;

	protected TextWidget m_CategoryNameText;

	//
	const string RADIAL_TEXT = "RadialText";
	const string CATEGORY_NAME = "CategoryName";
	//selections
	protected Widget m_SelectedItem;
	protected bool m_IsCategorySelected;
	protected bool m_IsMenuClosing;
	protected int m_CurrentCategory;
	protected ref TIntArray m_MenuPathCategories = {};
	protected ref TStringArray m_MenuPathNames = {};

	//instance
	static ref eAICommandMenu instance;

	//============================================
	// eAICommandMenu
	//============================================
	void eAICommandMenu()
	{
		m_GestureItems = new array < ref eAICommandMenuItem > ;
	}

	void ~eAICommandMenu() {};

	static void OpenMenu()
	{
		#ifndef EDITOR
		if (!instance)
			instance = new eAICommandMenu();

		g_Game.GetUIManager().ShowScriptedMenu(instance, null);
		#endif
	}

	//============================================
	// Init &Widget Events
	//============================================
	override Widget Init()
	{
		layoutRoot = g_Game.GetWorkspace().CreateWidgets("DayZExpansion/AI/GUI/layouts/radial_menu/menu.layout");
		m_GestureItemCardPanel = layoutRoot.FindAnyWidget(RadialMenu.RADIAL_ITEM_CARD_CONTAINER);

		//register gestures menu
		RadialMenu.GetInstance().RegisterClass(this);

		//delay updates until fully initialized
		RadialMenu.GetInstance().SetWidgetInitialized(false);

		//set radial menu properties
		RadialMenu.GetInstance().SetWidgetProperties("DayZExpansion/AI/GUI/layouts/radial_menu/delimiter.layout");

		//create content (widgets) for items
		RefreshGestures();

		#ifdef PLATFORM_WINDOWS
		Widget toolbar_panel = layoutRoot.FindAnyWidget("toolbar_bg");
		toolbar_panel.Show(!RadialMenu.GetInstance().IsUsingMouse());
		#endif

		//clear category name text
		UpdateCategoryName("");

		return layoutRoot;
	}

	override void OnShow()
	{
		super.OnShow();

		if (g_Game.GetUIManager())
			g_Game.GetUIManager().ShowCursor(true);
	
		if (g_Game.GetMission())// thanks Wardog
			g_Game.GetMission().PlayerControlDisable(INPUT_EXCLUDE_MOUSE_RADIAL);
		
		m_IsMenuClosing = false;
	}
	
	override void OnHide()
	{
		super.OnHide();
	
		if (GetParentMenu()) // check if we're a child menu, we shouldn't give control back yet
			return;
	
		if (g_Game.GetUIManager())
			g_Game.GetUIManager().ShowCursor(false);
	
		if (g_Game.GetMission())
			g_Game.GetMission().PlayerControlEnable(true);
		
		m_IsMenuClosing = true;
	}

	override bool OnController(Widget w, int control, int value)
	{
		super.OnController(w, control, value);

		RadialMenu.GetInstance().SetControlType(RadialMenuControlType.CONTROLLER);

		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		super.OnMouseEnter(w, x, y);

		RadialMenu.GetInstance().SetControlType(RadialMenuControlType.MOUSE);

		return false;
	}

	override bool UseMouse()
	{
		return true;
	}

	override bool UseGamepad()
	{
		return true;
	}

	//============================================
	// Gestures
	//============================================
	protected void RefreshGestures(eAICommandCategories category_id = -1, string name = "")
	{
		//create gestures content (widgets) based on categories
		if (category_id > -1)
		{
			GetGestureItems(m_GestureItems, category_id);
			m_CurrentCategory = category_id;
			instance.m_IsCategorySelected = true;
			if (m_MenuPathCategories.Find(category_id) == -1)
			{
				m_MenuPathCategories.Insert(category_id);
				m_MenuPathNames.Insert(name);
			}
			else
			{
				int index = m_MenuPathCategories.Count() - 1;
				m_MenuPathCategories.Remove(index);
				m_MenuPathNames.Remove(index);
			}
		}
		else
		{
			GetGestureItems(m_GestureItems, eAICommandCategories.CATEGORIES);
			m_CurrentCategory = -1;
			instance.m_IsCategorySelected = false;
			m_MenuPathCategories.Clear();
			m_MenuPathNames.Clear();
		}

		CreateGestureContent();

		UpdateToolbar();
		UpdateCategoryName(name);
	}

	protected void GetGestureItems(out array < ref eAICommandMenuItem > gesture_items, eAICommandCategories category)
	{
		gesture_items.Clear();

		int i;

		switch (category)
		{
		case eAICommandCategories.CATEGORIES:
			/**
			 * @note the menu items are ordered in a way so that their positions in the menu
			 *       are preserved as much as possible to aid muscle memory
			 * 
			 * 
			 *       Admin w/o group:
			 * 
			 *             Spawn
			 *         -           -
			 *      /                 \
			 *                        
			 *    |                     |
			 *                           
			 *    |                     |
			 *   Status             Faction
			 *      \                 /
			 *         -           -
			 *             -----
			 * 
			 * 
			 *       Admin in group:
			 * 
			 *           Movement
			 *         -           -
			 *      /                 \
			 *   Spawn             Formation
			 *    |                     |
			 *                           
			 *    |                     |
			 *   Status             Faction
			 *      \                 /
			 *         -           -
			 *            Looting
			 * 
			 * 
			 *    Not an admin, in group:
			 * 
			 *           Movement
			 *         -           -
			 *      /                 \
			 *                        
			 *    |                     |
			 *   Report            Formation
			 *    |                     |
			 *                        
			 *      \                 /
			 *         -           -
			 *            Looting
			 */
			if (GetExpansionSettings().GetAI().IsAdmin())
			{
				if (GetDayZGame().GetExpansionGame().InGroup())
				{
					gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_MOVEMENT, "Movement", eAICommandCategories.CATEGORIES));
					gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_FORMATION, "Formation", eAICommandCategories.CATEGORIES));
				}
				else
				{
					gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_DEBUG, "Spawn", eAICommandCategories.CATEGORIES));
				}

				gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_FACTION, "Faction", eAICommandCategories.CATEGORIES));

				if (GetDayZGame().GetExpansionGame().InGroup())
				{
					gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_LOOTINGBEHAVIOUR, " Looting", eAICommandCategories.CATEGORIES));
				}

				gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_STATUS, "Status", eAICommandCategories.CATEGORIES));

				if (GetDayZGame().GetExpansionGame().InGroup())
				{
					gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_DEBUG, "Spawn", eAICommandCategories.CATEGORIES));
				}
			}
			else if (GetDayZGame().GetExpansionGame().InGroup())
			{
				gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_MOVEMENT, "Movement", eAICommandCategories.CATEGORIES));
				gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_FORMATION, "Formation", eAICommandCategories.CATEGORIES));
				gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_LOOTINGBEHAVIOUR, " Looting", eAICommandCategories.CATEGORIES));
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.STA_SITREP, "Report", eAICommandCategories.CATEGORIES));
			}
			break;

		case eAICommandCategories.CAT_MOVEMENT:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_STOP, "Stop", eAICommandCategories.CAT_MOVEMENT));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_FLANK, "Flank", eAICommandCategories.CAT_MOVEMENT));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_ROAM, "Roam", eAICommandCategories.CAT_MOVEMENT));
			//gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_GOTO, "Go To...", eAICommandCategories.CAT_MOVEMENT));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_RTF, "Rejoin", eAICommandCategories.CAT_MOVEMENT));
			//gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_GETIN, "Get In", eAICommandCategories.CAT_MOVEMENT));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_SETWP, "    Set\nWaypoint", eAICommandCategories.CAT_MOVEMENT));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_CLEARWP, "     Clear\nWaypoints  ", eAICommandCategories.CAT_MOVEMENT));
			if (GetExpansionSettings().GetAI().IsAdmin())
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_EXPORTPATROL, "Export\n Patrol", eAICommandCategories.CAT_MOVEMENT));

			gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_MOVEMENT_STANCE, "Stance", eAICommandCategories.CATEGORIES));
			gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_MOVEMENT_SPEED, "Speed", eAICommandCategories.CATEGORIES));
			break;

		case eAICommandCategories.CAT_MOVEMENT_SPEED:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_WALK, "Walk", eAICommandCategories.CAT_MOVEMENT_SPEED));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_JOG, "Jog", eAICommandCategories.CAT_MOVEMENT_SPEED));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_SPRINT, "Sprint", eAICommandCategories.CAT_MOVEMENT_SPEED));
			break;

		case eAICommandCategories.CAT_MOVEMENT_STANCE:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_ERECT, "Stand", eAICommandCategories.CAT_MOVEMENT_STANCE));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_CROUCH, "Crouch", eAICommandCategories.CAT_MOVEMENT_STANCE));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.MOV_PRONE, "Prone", eAICommandCategories.CAT_MOVEMENT_STANCE));
			break;

		case eAICommandCategories.CAT_FORMATION:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_VEE, "Vee", eAICommandCategories.CAT_FORMATION));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_INVVEE, "InvVee", eAICommandCategories.CAT_FORMATION));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_FILE, "File", eAICommandCategories.CAT_FORMATION));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_INVFILE, "InvFile", eAICommandCategories.CAT_FORMATION));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_WALL, "Wall", eAICommandCategories.CAT_FORMATION));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_COL, "Column", eAICommandCategories.CAT_FORMATION));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_INVCOL, "InvColumn", eAICommandCategories.CAT_FORMATION));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_CIRCLE, "Circle", eAICommandCategories.CAT_FORMATION));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_CIRCLEDOT, "CircleDot", eAICommandCategories.CAT_FORMATION));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_STAR, "Star", eAICommandCategories.CAT_FORMATION));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_STARDOT, "StarDot", eAICommandCategories.CAT_FORMATION));
			break;

		case eAICommandCategories.CAT_FORMATION_SCALE:
			for (i = 0; i < 5; i++)
			{
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_SCALE_1X + i, string.Format("%1x", i + 1), eAICommandCategories.CAT_FORMATION_SCALE));
			}
			break;

		case eAICommandCategories.CAT_FORMATION_LOOSENESS:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_LOOSENESS_00, "0 m", eAICommandCategories.CAT_FORMATION_LOOSENESS));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_LOOSENESS_05, "0.5 m", eAICommandCategories.CAT_FORMATION_LOOSENESS));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_LOOSENESS_10, "1 m", eAICommandCategories.CAT_FORMATION_LOOSENESS));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_LOOSENESS_15, "1.5 m", eAICommandCategories.CAT_FORMATION_LOOSENESS));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_LOOSENESS_20, "2 m", eAICommandCategories.CAT_FORMATION_LOOSENESS));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_LOOSENESS_30, "3 m", eAICommandCategories.CAT_FORMATION_LOOSENESS));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_LOOSENESS_40, "4 m", eAICommandCategories.CAT_FORMATION_LOOSENESS));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.FOR_LOOSENESS_50, "5 m", eAICommandCategories.CAT_FORMATION_LOOSENESS));
			break;

		case eAICommandCategories.CAT_LOOTINGBEHAVIOUR:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.ALL, "All", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.DEFAULT, "Default", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.BANDAGES, "Bandages", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.FOOD, "Food", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.WEAPONS, "Weapons", eAICommandCategories.CATEGORIES));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING, "Clothing", eAICommandCategories.CATEGORIES));
			gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_LOOTINGBEHAVIOUR_CLOTHING_BACK, "Backwear", eAICommandCategories.CATEGORIES));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_SIMILAR, "Similar", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_IDENTICAL, "Identical", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.UPGRADE, "Upgrade", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			break;

		case eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING, "All", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_VEST, "Vests", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_BODY, "Tops", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_HIPS, "Hipwear", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_LEGS, "Pants", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_HEADGEAR, "Headgear", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_MASK, "Masks", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_EYEWEAR, "Eyewear", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_GLOVES, "Gloves", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_FEET, "Shoes", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_ARMBAND, "Armbands", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			break;

		case eAICommands.BEH_LOOT + eAILootingBehavior.WEAPONS:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.WEAPONS, "All", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.WEAPONS_FIREARMS, "Firearms", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.WEAPONS_LAUNCHERS, "Launchers", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.WEAPONS_MELEE, "Melee", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			break;

		case eAICommandCategories.CAT_LOOTINGBEHAVIOUR_CLOTHING_BACK:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_BACK_LARGE + eAILootingBehavior.CLOTHING_BACK_MEDIUM + eAILootingBehavior.CLOTHING_BACK_SMALL, "All", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_BACK_LARGE, "Large", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_BACK_MEDIUM, "Medium", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.BEH_LOOT + eAILootingBehavior.CLOTHING_BACK_SMALL, "Small", eAICommandCategories.CAT_LOOTINGBEHAVIOUR));
			break;

		case eAICommandCategories.CAT_STATUS:
			//gesture_items.Insert(new eAICommandMenuItem(eAICommands.STA_SITREP, "Report Situation", eAICommandCategories.CAT_STATUS));
			//gesture_items.Insert(new eAICommandMenuItem(eAICommands.STA_POSITION, "Report Position", eAICommandCategories.CAT_STATUS));
			//gesture_items.Insert(new eAICommandMenuItem(eAICommands.STA_THREATS, "Report Threats", eAICommandCategories.CAT_STATUS));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.STA_DUMP, "Dump State", eAICommandCategories.CAT_STATUS));
			if (GetExpansionSettings().GetAI().IsAdmin())
			{
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_RESET_PATHFINDING, "    Reset\nPathfinding", eAICommandCategories.CAT_STATUS));
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_FORCEWEAPONRESYNC, "Force Weapon\n     Resync", eAICommandCategories.CAT_STATUS));
			#ifdef DIAG_DEVELOPER
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_DBGOBJECTS, " Debug\nObjects", eAICommandCategories.CAT_STATUS));
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_DBGDAMAGE, " Debug\nDamage", eAICommandCategories.CAT_STATUS));
			#endif
				gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_DAMAGE_IN, "Damage\n In/Out", eAICommandCategories.CATEGORIES));
				gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_UNLIMITEDRELOAD, "Unlimited\n  Reload", eAICommandCategories.CATEGORIES));
			#ifdef JM_COT
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_SPECTATE, "Spectate AI", eAICommandCategories.CAT_STATUS));
			#endif
			}
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.STA_SITREP, "Report", eAICommandCategories.CAT_STATUS));
			break;

		case eAICommandCategories.CAT_UNLIMITEDRELOAD:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_UNLIMITEDRELOAD_ALL, "All", eAICommandCategories.CAT_DEBUG));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_UNLIMITEDRELOAD_ANIMALS, "Animals", eAICommandCategories.CAT_DEBUG));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_UNLIMITEDRELOAD_INFECTED, "Infected", eAICommandCategories.CAT_DEBUG));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_UNLIMITEDRELOAD_PLAYERS, "Players", eAICommandCategories.CAT_DEBUG));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_UNLIMITEDRELOAD_VEHICLES, "Vehicles", eAICommandCategories.CAT_DEBUG));
			break;

		case eAICommandCategories.CAT_DAMAGE_IN:
		case eAICommandCategories.CAT_DAMAGE_OUT:
			for (i = 0; i < eAICommands.DEB_DAMAGE_COUNT; ++i)
			{
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_DAMAGE + i, string.Format("%1%%", 100 - i * 5), category));
			}
			break;

		case eAICommandCategories.CAT_HEADSHOTRESISTANCE:
			for (i = 0; i < eAICommands.DEB_DAMAGE_COUNT; ++i)
			{
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_DAMAGE + i, string.Format("%1%%", i * 5), category));
			}
			break;

		case eAICommandCategories.CAT_DEBUG:
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_SPAWNALLY, "Companion", eAICommandCategories.CAT_DEBUG));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_CLEARALL, "Clear AI", eAICommandCategories.CAT_DEBUG));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_SPAWNBEAR, "Bear", eAICommandCategories.CAT_DEBUG));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_SPAWNWOLF, "Wolf", eAICommandCategories.CAT_DEBUG));
			gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_SPAWNZOM, "Zombie", eAICommandCategories.CAT_DEBUG));
			gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_SPAWNFACTION, "Other", eAICommandCategories.CATEGORIES));
			if (IsMissionOffline())
			{
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_TARGET_CREATE, "Create Debug Apple", eAICommandCategories.CAT_DEBUG));
				gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_TARGET_DESTROY, "Destroy Debug Apple", eAICommandCategories.CAT_DEBUG));
			}
			//gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_AIMAP, "AI Menu", eAICommandCategories.CAT_DEBUG));
			//gesture_items.Insert(new eAICommandMenuItem(eAICommands.DEB_GRPMGR, "Group Manager", eAICommandCategories.CAT_DEBUG));
			break;

		case eAICommandCategories.CAT_FACTION:
		case eAICommandCategories.CAT_SPAWNFACTION:
			foreach (int factionId, typename factionType: eAIRegisterFaction.s_FactionTypes)
			{
				eAIFaction faction = eAIFaction.Cast(factionType.Spawn());
				if (!faction.IsInvincible())
				{
					string name = faction.GetName();
					switch (name)
					{
						case "YeetBrigade":
							name = "Yeet";
							break;
					}
					gesture_items.Insert(new eAICommandMenuItem(factionId, name, category));
				}
			}
			break;


		case eAICommandCategories.CAT_EMPTY:
			Close();
			break;
		}

		if (gesture_items.Count() == 1)
		{
			gesture_items.Insert(new eAICommandMenuItem(eAICommandCategories.CAT_EMPTY, "", eAICommandCategories.CATEGORIES));
		}
	}

	protected void CreateGestureContent()
	{
		//delete existing gesture widgets
		DeleteGestureItems();

		for (int i = 0; i < m_GestureItems.Count(); ++i)
		{
			eAICommandMenuItem gesture_item = m_GestureItems.Get(i);

			//create item card
			Widget gesture_item_card_widget = Widget.Cast(g_Game.GetWorkspace().CreateWidgets("DayZExpansion/AI/GUI/layouts/radial_menu/item_card.layout", m_GestureItemCardPanel));
			gesture_item.SetRadialItemCard(gesture_item_card_widget);

			//update item card widget
			UpdateQuickbarItemCard(gesture_item);

			//set data
			gesture_item_card_widget.SetUserData(gesture_item);
		}

		//set radial parameters for content
		if (m_GestureItems.Count() > 0)
		{
			RadialMenu radial_menu = RadialMenu.GetInstance();
			radial_menu.SetRadiusOffset(0);
			radial_menu.SetExecuteDistOffset(0.5);
			radial_menu.SetOffsetFromTop(0);
			radial_menu.SetItemCardRadiusOffset(0.25);
			radial_menu.ActivateControllerTimeout(false);
			radial_menu.SetSelectorColors(ARGB(255,21,84,21), ARGB(255,21,84,21));
		}

		//refresh radial menu
		RadialMenu.GetInstance().Refresh();
	}

	protected void UpdateQuickbarItemCard(eAICommandMenuItem gesture_item)
	{
		Widget gesture_item_card_widget = gesture_item.GetRadialItemCard();

		//set text
		ButtonWidget text_widget;
		if (Class.CastTo(text_widget, gesture_item_card_widget.FindAnyWidget(RADIAL_TEXT))) text_widget.SetText(gesture_item.GetName());
	}

	protected void DeleteGestureItems()
	{
		Widget child;
		Widget child_to_destroy;

		child = m_GestureItemCardPanel.GetChildren();
		while (child)
		{
			child_to_destroy = child;
			child = child.GetSibling();

			delete child_to_destroy;
		}
	}

	protected void UpdateToolbar()
	{
		#ifdef PLATFORM_CONSOLE
		//set controller toolbar icons
		string select_img;
		//string back_img;

		ImageWidget toolbar_select = ImageWidget.Cast(layoutRoot.FindAnyWidget("SelectIcon"));
		//ImageWidget toolbar_back = ImageWidget.Cast(layoutRoot.FindAnyWidget("BackIcon"));

		if (instance.m_IsCategorySelected)
		{
			#ifdef PLATFORM_XBOX
			select_img = "set:xbox_buttons image:A";
			//back_img = "set:xbox_buttons image:B";
			#endif
			#ifdef PLATFORM_PS4
			string confirm = "cross";
			if (g_Game.GetInput().GetEnterButton() == GamepadButton.A)
			{
				confirm = "cross";
			}
			else
			{
				confirm = "circle";
			}

			select_img = "set:playstation_buttons image:" + confirm;
			//back_img = "set:playstation_buttons image:circle";
			#endif
		}
		else
		{
			#ifdef PLATFORM_XBOX
			select_img = "set:xbox_buttons image:R_up";
			//back_img = "set:xbox_buttons image:B";
			#endif
			#ifdef PLATFORM_PS4
			select_img = "set:playstation_buttons image:R_up";
			//back_img = "set:playstation_buttons image:circle";
			#endif
		}

		toolbar_select.LoadImageFile(0, select_img);
		//toolbar_back.LoadImageFile(0, back_img);
		#endif
	}

	protected void UpdateCategoryName(string name)
	{
		if (!m_CategoryNameText)
		{
			m_CategoryNameText = TextWidget.Cast(layoutRoot.FindAnyWidget(CATEGORY_NAME));
		}

		m_CategoryNameText.SetText(name);
	}

	//============================================
	// Radial Menu Events
	//============================================
	//Common
	void OnControlsChanged(RadialMenuControlType type)
	{
		//show/hide controller toolbar
		Widget toolbar_panel = layoutRoot.FindAnyWidget("toolbar_bg");
		if (type == RadialMenuControlType.CONTROLLER)
		{
			#ifdef PLATFORM_CONSOLE
			toolbar_panel.Show(true);
			#endif
		}
		else
		{
			toolbar_panel.Show(false);
		}
	}

	//Mouse
	void OnMouseSelect(Widget w)
	{
		MarkSelected(w);
	}

	void OnMouseDeselect(Widget w)
	{
		UnmarkSelected(w);
	}

	// unused, press events used instead with 1.19
	void OnMouseExecute( Widget w )
	{
	}
	
	//! LMB
	void OnMousePressLeft( Widget w )
	{
		ExecuteSelected(w);
	}
	
	//! RMB
	void OnMousePressRight( Widget w )
	{
		BackOneLevel();
	}

	//Controller
	void OnControllerSelect(Widget w)
	{
		MarkSelected(w);
	}

	void OnControllerDeselect(Widget w)
	{
		UnmarkSelected(w);
	}

	// unused, press events used instead with 1.19
	void OnControllerExecute( Widget w )
	{
	}
	
	void OnControllerPressSelect( Widget w )
	{
		ExecuteSelected(w);
	}
	
	void OnControllerPressBack( Widget w )
	{
		BackOneLevel();
	}

	//!DEPRECATED with 1.19
	void OnMenuRelease()
	{
	}

	//Actions
	protected void MarkSelected(Widget w)
	{
		instance.m_SelectedItem = w;

		if (w)
		{
			eAICommandMenuItem gesture_item;
			w.GetUserData(gesture_item);

			//is not category
			/*
			if (gesture_item && gesture_item.GetCategory() != eAICommandCategories.CATEGORIES)
			{ 
			//alter item visual
			//TextWidget text_widget = TextWidget.Cast(gesture_item.GetRadialItemCard().FindAnyWidget(RADIAL_TEXT));
			//text_widget.SetColor(ARGB(255, 66, 175, 95));
			}

			*/
		}
	}

	protected void UnmarkSelected(Widget w)
	{
		instance.m_SelectedItem = NULL;

		if (w)
		{
			eAICommandMenuItem gesture_item;
			w.GetUserData(gesture_item);

			//is not category
			/*
			if (gesture_item && gesture_item.GetCategory() != eAICommandCategories.CATEGORIES)
			{ 
			//alter item visual
			//TextWidget text_widget = TextWidget.Cast(gesture_item.GetRadialItemCard().FindAnyWidget(RADIAL_TEXT));
			//text_widget.SetColor(ARGB(255, 255, 255, 255));
			}

			*/
		}
	}

	protected void ExecuteSelected(Widget w)
	{
		if (w)
		{
			eAICommandMenuItem gesture_item;
			w.GetUserData(gesture_item);

			if (gesture_item.GetCategory() == eAICommandCategories.CATEGORIES)
			{
				string name = gesture_item.GetCategoryName();

				RefreshGestures(gesture_item.GetID(), name);
			}
			else
			{
				ExecuteSelectedItem();
			}
		}
	}

	protected void ExecuteSelectedItem()
	{
		if (instance.m_IsCategorySelected && instance.m_SelectedItem)
		{
			if (!g_Game.IsDedicatedServer())
			{
				eAICommandMenuItem selected;
				instance.m_SelectedItem.GetUserData(selected);

				if (selected)
				{
					g_Game.GetExpansionGame().GetCommandManager().Send(selected.GetID(), selected.GetCategory());

					eAIGroup group = eAIGroup.GetGamePlayerGroup();

					switch (selected.GetCategory())
					{
						case eAICommandCategories.CAT_FACTION:
						case eAICommandCategories.CAT_SPAWNFACTION:
						case eAICommandCategories.CAT_FORMATION_LOOSENESS:
							UpdateCategoryName(selected.GetName());
							break;

						case eAICommandCategories.CAT_DAMAGE_IN:
							RefreshGestures(eAICommandCategories.CAT_DAMAGE_OUT, "Damage Out");
							break;

						case eAICommandCategories.CAT_DAMAGE_OUT:
							RefreshGestures(eAICommandCategories.CAT_HEADSHOTRESISTANCE, "Headshot Resistance");
							break;

						case eAICommandCategories.CAT_FORMATION:
							RefreshGestures(eAICommandCategories.CAT_FORMATION_SCALE, "Scale");
							//! Formation scale is not synced to client
							//if (group)
								//UpdateCategoryName("Scale " + group.GetFormation().GetScale() + "x");
							break;

						case eAICommandCategories.CAT_FORMATION_SCALE:
							RefreshGestures(eAICommandCategories.CAT_FORMATION_LOOSENESS, "Looseness");
							//! Formation loseness is not synced to client
							//if (group)
							//{
								//float looseness;
								//eAIFormation form = group.GetFormation();
								//EnScript.GetClassVar(form, "m_Looseness", 0, looseness);
								//UpdateCategoryName("Looseness " + looseness + " m");
							//}
							break;
					}
				}
			}
		}
	}
	
	protected void BackOneLevel()
	{
		eAICommandCategories category_id = -1;
		string name;
		int index = m_MenuPathCategories.Count() - 2;
		if (index >= 0)
		{
			category_id = m_MenuPathCategories[index];
			name = m_MenuPathNames[index];
		}

		RefreshGestures(category_id, name);
	}

	bool IsMenuClosing()
	{
		return m_IsMenuClosing;
	}

	void SetMenuClosing(bool state)
	{
		m_IsMenuClosing = state;
	}
};
