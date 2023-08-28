/**
 * ExpansionGarageMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGarageMenu: ExpansionScriptViewMenu
{
	protected ref ExpansionGarageMenuController m_GarageMenuController;
	protected ref ExpansionGarageModule m_GarageModule;

#ifdef EXPANSIONMODBASEBUILDING
	protected ref ExpansionTerritoryModule m_TerritoryModule;
	protected ref ExpansionTerritory m_Territory;
#endif
#ifdef EXPANSIONMODMARKET
	protected ref ExpansionMarketModule m_MarketModule;
#endif
	protected autoptr array<ref ExpansionGarageVehicleData> m_WorldVehicles;
	protected autoptr array<ref ExpansionGarageVehicleData> m_StoredVehicles;
	protected autoptr array<ref ExpansionGarageMenuInfo> m_MenuInfos;

	protected Widget garage_loading;
	protected Widget player_money_panel;

	protected autoptr ExpansionGarageMenuDialog m_GarageDialog;
	protected GridSpacerWidget garage_menu_info_spacer;

	void ExpansionGarageMenu()
	{
		Class.CastTo(m_GarageMenuController, GetController());
		Class.CastTo(m_GarageModule, CF_ModuleCoreManager.Get(ExpansionGarageModule));
	#ifdef EXPANSIONMODBASEBUILDING
		Class.CastTo(m_TerritoryModule, CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
	#endif
	#ifdef EXPANSIONMODMARKET
		Class.CastTo(m_MarketModule, CF_ModuleCoreManager.Get(ExpansionMarketModule));
	#endif

		m_GarageModule.GetGarageMenuSI().Insert(SetVehicles);
		m_GarageModule.GetGarageMenuCallbackSI().Insert(OnModuleCallback);

		m_WorldVehicles = new array<ref ExpansionGarageVehicleData>;
		m_StoredVehicles = new array<ref ExpansionGarageVehicleData>;

		m_MenuInfos = new array<ref ExpansionGarageMenuInfo>;
	}
	
	void ~ExpansionGarageMenu()
	{
		if (m_GarageModule)
		{
			m_GarageModule.GetGarageMenuSI().Remove(SetVehicles);
			m_GarageModule.GetGarageMenuCallbackSI().Remove(OnModuleCallback);
		}
	}

	void SetView()
	{
		m_GarageMenuController.WorldVehicles.Clear();
		foreach (ExpansionGarageVehicleData worldData: m_WorldVehicles)
		{
			ExpansionGarageMenuVehicle worldVehicleEntry = new ExpansionGarageMenuVehicle(this, worldData);
			m_GarageMenuController.WorldVehicles.Insert(worldVehicleEntry);
		}

		m_GarageMenuController.StoredVehicles.Clear();
		foreach (ExpansionGarageVehicleData storedData: m_StoredVehicles)
		{
			ExpansionGarageMenuVehicle storedVehicleEntry = new ExpansionGarageMenuVehicle(this, storedData, true);
			m_GarageMenuController.StoredVehicles.Insert(storedVehicleEntry);
		}

		garage_loading.Show(false);
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Garage/GUI/layouts/expansion_garage_menu.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionGarageMenuController;
	}

	override void OnShow()
	{
		super.OnShow();

		SetMenuInfos();
		garage_loading.Show(true);
		m_GarageModule.RequestPlayerVehicles();
	}

	void SetMenuInfos()
	{
		m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_STORE", ARGB(255, 220, 220, 220), garage_menu_info_spacer));
		m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_RETRIEVE", ARGB(255, 220, 220, 220), garage_menu_info_spacer));

		if (GetExpansionSettings().GetGarage().GarageMode == ExpansionGarageMode.Territory)
			m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_TERRITORY", ARGB(255, 220, 220, 220), garage_menu_info_spacer));

		if (!GetExpansionSettings().GetGarage().CanStoreWithCargo)
			m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_CARGO", ARGB(255, 220, 220, 220), garage_menu_info_spacer));

		if (GetExpansionSettings().GetGarage().NeedKeyToStore)
			m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_KEY", ARGB(255, 220, 220, 220), garage_menu_info_spacer));

		m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_WARRANTY", ARGB(255, 241, 196, 15), garage_menu_info_spacer));
		
		if (GetExpansionSettings().GetGarage().GarageMode == ExpansionGarageMode.Territory)
			m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_RAIDING", ARGB(255, 231, 76, 60), garage_menu_info_spacer));

	#ifdef EXPANSIONMODGROUPS
		if (GetExpansionSettings().GetGarage().EnableGroupFeatures)
		{
			switch (GetExpansionSettings().GetGarage().GroupStoreMode)
			{
				case ExpansionGarageGroupStoreMode.StoreOnly:
				{
					m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_GROUP_STORE", ARGB(255, 155, 89, 182), garage_menu_info_spacer));
				}
				break;
				case ExpansionGarageGroupStoreMode.RetrieveOnly:
				{
					m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_GROUP_RETRIEVE", ARGB(255, 155, 89, 182), garage_menu_info_spacer));
				}
				break;
				case ExpansionGarageGroupStoreMode.StoreAndRetrieve:
				{
					m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_GROUP_STOREANDRETRIEVE", ARGB(255, 155, 89, 182), garage_menu_info_spacer));
				}
				break;
			}
		}
	#endif
		
	#ifdef EXPANSIONMODMARKET
		if (GetExpansionSettings().GetGarage().EnableMarketFeatures && GetExpansionSettings().GetMarket().MarketSystemEnabled)
		{
			m_MenuInfos.Insert(new ExpansionGarageMenuInfo("#STR_EXPANSION_GARAGE_INFO_PRICE", ARGB(255, 52, 152, 219), garage_menu_info_spacer));
			if (GetExpansionSettings().GetGarage().StorePricePercent > 0 || GetExpansionSettings().GetGarage().StaticStorePrice > 0)
			{
				player_money_panel.Show(true);
				UpdatePlayerCurrency();
			}
		}
	#endif

		garage_menu_info_spacer.Update();
	}

#ifdef EXPANSIONMODMARKET
	void UpdatePlayerCurrency()
	{		
		array<int> monies = new array<int>;
		int worth = m_MarketModule.GetPlayerWorth(PlayerBase.Cast(GetGame().GetPlayer()), monies);	
		
		string currencyPlayerTotalMoneyString = ExpansionStatic.IntToCurrencyString(worth, ",");
		m_GarageMenuController.PlayerMoney = currencyPlayerTotalMoneyString + " #STR_EXPANSION_GARAGE_MENU_CURRENCY_TOTAL";
		m_GarageMenuController.NotifyPropertyChanged("PlayerMoney");
	}
#endif

	void SetVehicles(array<ref ExpansionGarageVehicleData> worldVehicles, array<ref ExpansionGarageVehicleData> storedVehicles)
	{
		m_WorldVehicles.Clear();
		foreach (ExpansionGarageVehicleData worldData: worldVehicles)
		{
			m_WorldVehicles.Insert(worldData);
		}

		m_StoredVehicles.Clear();
		foreach (ExpansionGarageVehicleData storedData: storedVehicles)
		{
			m_StoredVehicles.Insert(storedData);
		}

		SetView();
	}

	void RequestDeposit(ExpansionGarageVehicleData vehicle)
	{
		if (!m_GarageModule)
			return;

		m_GarageModule.DepositVehicleRequest(vehicle.m_VehicleObject);
	}

	void RequestRetrieve(ExpansionGarageVehicleData vehicle)
	{
		if (!m_GarageModule)
			return;

		m_GarageModule.RetrieveVehicleRequest(vehicle);
	}

	void OnModuleCallback(ExpansionGarageModuleCallback callback)
	{
		switch (callback)
		{
			case ExpansionGarageModuleCallback.VehicleStored:
			{
				CloseMenu();
			}
			break;

			case ExpansionGarageModuleCallback.VehicleRetrieved:
			{
				CloseMenu();
			}
			break;

			case ExpansionGarageModuleCallback.NoVehicles:
			{
				garage_loading.Show(false);
				
				if (!m_GarageDialog)
					m_GarageDialog = new ExpansionGarageMenuDialog(this);

				m_GarageDialog.SetText("#STR_EXPANSION_GARAGE_MENU_ERROR_NOVEHICLES");
				m_GarageDialog.Show();
			}
			break;
			
			case ExpansionGarageModuleCallback.NoTerritory:
			{
				garage_loading.Show(false);
				
				if (!m_GarageDialog)
					m_GarageDialog = new ExpansionGarageMenuDialog(this);

				m_GarageDialog.SetText("#STR_EXPANSION_GARAGE_MENU_ERROR_NOTERRITORY");
				m_GarageDialog.Show();
			}
			break;
			
			case ExpansionGarageModuleCallback.Update:
			{
				garage_loading.Show(true);
				m_GarageModule.RequestPlayerVehicles();
			}
			break;
		}
	}

	void OnConfirmDialog()
	{
		CloseMenu();
	}

	override bool CanClose()
	{
		return !m_GarageDialog || !m_GarageDialog.IsVisible();
	}
};

class ExpansionGarageMenuController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionGarageMenuVehicle> WorldVehicles = new ObservableCollection<ref ExpansionGarageMenuVehicle>(this);
	ref ObservableCollection<ref ExpansionGarageMenuVehicle> StoredVehicles = new ObservableCollection<ref ExpansionGarageMenuVehicle>(this);
#ifdef EXPANSIONMODMARKET
	string PlayerMoney;
#endif
};
