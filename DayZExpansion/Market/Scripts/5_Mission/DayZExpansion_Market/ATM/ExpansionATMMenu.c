/**
 * ExpansionATMMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionATMMenu: ExpansionScriptViewMenu
{
	protected ref ExpansionATMMenuController m_ATMMenuController;
	protected ref ExpansionMarketModule m_MarketModule;
	
	ref ExpansionMarketATM_Data m_ATMData;
	
	EditBoxWidget AmountValue;
	int m_Amount = 0;
	int m_PlayerMoney;
	
	protected Widget atm_transfer;
	protected Widget atm_party;
	
	ref SyncPlayer m_SelectedPlayer;
	ref ExpansionATMMenuTransferDialog m_TransferDialog;
	ref ExpansionATMMenuPartyTransferDialog m_PartyTransferDialog;
	
	#ifdef EXPANSIONMOD
	ref ExpansionPartyData m_Party;
	#endif
	
	protected EditBoxWidget atm_filter_box;
	protected ButtonWidget PartyButtonWithdraw;
	protected Widget PartyButtonWithdrawBackground;
	protected ButtonWidget atm_filter_clear;
	protected ImageWidget atm_filter_clear_icon;
	
	protected ref ExpansionATMMenuColorHandler m_ColorHandler;	
	
	protected int m_PlayerSearchRadius = 25;
	
	// ------------------------------------------------------------
	// ExpansionATMMenu Constructor
	// ------------------------------------------------------------	
	void ExpansionATMMenu()
	{
		if (!m_ATMMenuController)
			m_ATMMenuController = ExpansionATMMenuController.Cast(GetController());
		
		if (!m_MarketModule)
			m_MarketModule = ExpansionMarketModule.Cast(GetModuleManager().GetModule(ExpansionMarketModule));
		
		ExpansionMarketModule.SI_ATMMenuInvoker.Insert(SetPlayerATMData);
		ExpansionMarketModule.SI_ATMMenuCallback.Insert(OnCallback);
		ExpansionMarketModule.SI_ATMMenuTransferCallback.Insert(OnTransferCallback);
		#ifdef EXPANSIONMOD
		ExpansionMarketModule.SI_ATMMenuPartyCallback.Insert(OnPartyCallback);
		#endif
		
		if (!m_ColorHandler)
			m_ColorHandler = new ExpansionATMMenuColorHandler(GetLayoutRoot());
	}

	// ------------------------------------------------------------
	// ExpansionATMMenu GetLayoutFile
	// ------------------------------------------------------------
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/atm/expansion_atm_menu.layout";
	}

	// ------------------------------------------------------------
	// ExpansionATMMenu GetControllerType
	// ------------------------------------------------------------
	override typename GetControllerType() 
	{
		return ExpansionATMMenuController;
	}
		
	// ------------------------------------------------------------
	// ExpansionATMMenu CanShow
	// ------------------------------------------------------------
	override bool CanShow()
	{
		return GetExpansionSettings().GetMarket().ATMSystemEnabled;
	}
		
	// ------------------------------------------------------------
	// ExpansionATMMenu OnHide
	// ------------------------------------------------------------	
	override void OnHide()
	{
		super.OnHide();
		
		if (GetExpansionSettings().GetMarket().ATMPlayerTransferEnabled)
			ClearPlayers();
	}
		
	// ------------------------------------------------------------
	// ExpansionATMMenu OnMouseButtonDown
	// ------------------------------------------------------------
	void SetPlayerATMData(ExpansionMarketATM_Data data)
	{
		m_ATMData = data;
		
		SetView();
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu SetView
	// ------------------------------------------------------------
	void SetView()
	{
		m_ATMMenuController.MaxValue = GetExpansionSettings().GetMarket().MaxDepositMoney.ToString();
		m_ATMMenuController.NotifyPropertyChanged("MaxValue");
		
		m_ATMMenuController.MoneyDepositValue = m_ATMData.MoneyDeposited.ToString();
		m_ATMMenuController.NotifyPropertyChanged("MoneyDepositValue");
		
		array<int> monies = new array<int>;
		m_MarketModule.EnumeratePlayerInventory(PlayerBase.Cast(GetGame().GetPlayer()));
		m_PlayerMoney = m_MarketModule.GetPlayerWorth(PlayerBase.Cast(GetGame().GetPlayer()), monies, NULL, true);		
		m_ATMMenuController.PlayerMoneyValue = m_PlayerMoney.ToString();
		m_ATMMenuController.NotifyPropertyChanged("PlayerMoneyValue");
		monies.Clear();
		
		SetFocus(AmountValue);
		
		if (GetExpansionSettings().GetMarket().ATMPlayerTransferEnabled)
		{
			atm_transfer.Show(true);
			LoadPlayers("");
		}
		
		if (GetExpansionSettings().GetMarket().ATMPartyLockerEnabled)
		{
			#ifdef EXPANSIONMOD
			//PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			ExpansionPartyModule module = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
			
			if (!module || !module.HasParty())
				return;
			
			atm_party.Show(true);
			
			m_Party = module.GetParty();
			
			m_ATMMenuController.PartyName = m_Party.GetPartyName();
			m_ATMMenuController.NotifyPropertyChanged("PartyName");
			
			m_ATMMenuController.PartyID = m_Party.GetPartyID().ToString();
			m_ATMMenuController.NotifyPropertyChanged("PartyID");
			
			m_ATMMenuController.PartyOwner = m_Party.GetOwnerName();
			m_ATMMenuController.NotifyPropertyChanged("PartyOwner");
			
			m_ATMMenuController.PartyMaxValue = GetExpansionSettings().GetMarket().MaxPartyDepositMoney.ToString();
			m_ATMMenuController.NotifyPropertyChanged("PartyMaxValue");
			
			m_ATMMenuController.PartyMoneyDepositValue = m_Party.GetMoneyDeposited().ToString();
			m_ATMMenuController.NotifyPropertyChanged("PartyMoneyDepositValue");
			
			Print("CanWithdrawMoney: " + GetPlayerPartyData().CanWithdrawMoney());
			Print("Permissons: " + GetPlayerPartyData().GetPermissions());
			
			if (m_Party && GetPlayerPartyData().CanWithdrawMoney())
			{
				PartyButtonWithdraw.Show(true);
				PartyButtonWithdrawBackground.Show(true);
			}
			#endif
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu UpdateView
	// ------------------------------------------------------------
	void UpdateView(bool updatePlayerMoney = true)
	{
		m_ATMMenuController.MoneyDepositValue = m_ATMData.MoneyDeposited.ToString();
		m_ATMMenuController.NotifyPropertyChanged("MoneyDepositValue");
		
		if (updatePlayerMoney)
		{
			array<int> monies = new array<int>;
			m_MarketModule.EnumeratePlayerInventory(PlayerBase.Cast(GetGame().GetPlayer()));
			m_PlayerMoney = m_MarketModule.GetPlayerWorth(PlayerBase.Cast(GetGame().GetPlayer()), monies, NULL, true);
		}

		m_ATMMenuController.PlayerMoneyValue = m_PlayerMoney.ToString();
		m_ATMMenuController.NotifyPropertyChanged("PlayerMoneyValue");
	
		#ifdef EXPANSIONMOD
		if (GetExpansionSettings().GetMarket().ATMPartyLockerEnabled && m_Party)
		{
			m_ATMMenuController.PartyMoneyDepositValue = m_Party.GetMoneyDeposited().ToString();
			m_ATMMenuController.NotifyPropertyChanged("PartyMoneyDepositValue");
		}
		#endif
	}

	//! Gets amount text and checks if it contains only numbers.
	//! Returns amount as positive int if check passed, else displays error notification and returns -1.
	int GetCheckAmount(string title)
	{
		string quantity = AmountValue.GetText();
		TStringArray allNumbers = {"0","1","2","3","4","5","6","7","8","9"};
		for (int i = 0; i < quantity.Length(); i++)
		{
			if (allNumbers.Find(quantity.Get(i)) == -1)
			{
				ExpansionNotification("STR_EXPANSION_" + title, "STR_EXPANSION_ATM_ONLY_NUMBERS").Error();
				return -1;
			}
		}

		return quantity.ToInt();
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu OnWithdrawButtonClick
	// ------------------------------------------------------------
	void OnWithdrawButtonClick()
	{		
		//! Only numbers are allowed
		int quantity = GetCheckAmount("ATM_WITHDRAW_FAILED");
		if (quantity == -1)
			return;
		
		m_Amount = quantity;
		
		//! Make sure we dont send negative or 0 amounts
		if (m_Amount <= 0)
		{
			ExpansionNotification("STR_EXPANSION_ATM_WITHDRAW_FAILED", "STR_EXPANSION_ATM_NONZERO").Error();
			return;
		}
				
		//! We only can withdraw what we have deposited
		if (m_Amount > m_ATMData.MoneyDeposited)
		{
			ExpansionNotification("STR_EXPANSION_ATM_WITHDRAW_FAILED", "STR_EXPANSION_ATM_AMOUNT_MAX_ERROR").Error();
			return;
		}
		
		m_MarketModule.RequestWithdrawMoney(m_Amount);
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu OnDepositButtonClick
	// ------------------------------------------------------------	
	void OnDepositButtonClick()
	{
		//! Only numbers are allowed
		int quantity = GetCheckAmount("ATM_DEPOSIT_FAILED");
		if (quantity == -1)
			return;
		
		m_Amount = quantity;
		
		//! Make sure we dont send negative or 0 amounts
		if (m_Amount <= 0)
		{
			ExpansionNotification("STR_EXPANSION_ATM_DEPOSIT_FAILED", "STR_EXPANSION_ATM_NONZERO").Error();
			return;
		}
		
		//! We can only deposit money until we reach max. depending on server setting
		if ((m_ATMData.MoneyDeposited + m_Amount) > GetExpansionSettings().GetMarket().MaxDepositMoney)
		{
			ExpansionNotification("STR_EXPANSION_ATM_DEPOSIT_FAILED", new StringLocaliser("STR_EXPANSION_ATM_DEPOSIT_MAX_ERROR", GetExpansionSettings().GetMarket().MaxDepositMoney.ToString())).Error();
			return;
		}
		
		//! Check if we try to deposit more then we have in inventory
		if (m_PlayerMoney < m_Amount)
		{
			ExpansionNotification("STR_EXPANSION_ATM_DEPOSIT_FAILED", new StringLocaliser("STR_EXPANSION_ATM_DEPOSIT_NOTENOUGHMONEY", m_PlayerMoney.ToString())).Error();
			return;
		}		
		
		m_MarketModule.RequestDepositMoney(m_Amount);
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu OnCallback
	// ------------------------------------------------------------	
	void OnCallback(int amount, ExpansionMarketATM_Data data, int state)
	{
		switch (state)
		{
			//! Deposit
			case 1:
				ExpansionNotification("STR_EXPANSION_ATM_DEPOSIT_SUCCESS", new StringLocaliser("STR_EXPANSION_ATM_DEPOSIT_SUCCESS_TEXT", amount.ToString())).Success();
				m_PlayerMoney -= amount;
				break;
			//! Withdraw
			case 2:
				ExpansionNotification("STR_EXPANSION_ATM_WITHDRAW_SUCCESS", new StringLocaliser("STR_EXPANSION_ATM_WITHDRAW_SUCCESS_TEXT", amount.ToString())).Success();
				m_PlayerMoney += amount;
				break;
		}
		
		m_ATMData = data;
		
		UpdateView(false);
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu LoadPlayers
	// ------------------------------------------------------------
	void LoadPlayers(string filter)
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player) 
			return;
		
		if (!GetGame().GetPlayer().GetIdentity())
			return;
		
		ClearPlayers();
		
		string filterNormal = filter;
		filter.ToLower();	
			
		string playerID = GetGame().GetPlayer().GetIdentity().GetId();
		ExpansionATMMenuPlayerEntry entry;
		SyncPlayer playerSync;
		int i;
		string playerName;
		
		if (GetExpansionSettings().GetMarket().UseWholeMapForATMPlayerList)
		{
			for (i = 0; i < ClientData.m_PlayerList.m_PlayerList.Count(); ++i)
			{
				playerSync = ClientData.m_PlayerList.m_PlayerList[i];
				
				if (!playerSync)
					continue;
					
				if (filter != "")
				{
					playerName = playerSync.m_PlayerName;
					playerName.ToLower();
		
					if (playerName.IndexOf(filter) == -1)
						continue;
				}
				
				if (playerSync && playerSync.m_RUID != "" && playerSync.m_RUID != playerID)
				{
					AddPlayerEntry(playerSync);
				}
			}
		}
		else
		{
			array<PlayerIdentity> identitys = GetNearbyPlayerIdentitys(player.GetPosition(), m_PlayerSearchRadius);			
			if (!identitys)
			{
				return;
			}
			
			for (i = 0; i < identitys.Count(); ++i)
			{
				for (int k = 0; k < ClientData.m_PlayerList.m_PlayerList.Count(); ++k)
				{				
					playerSync = ClientData.m_PlayerList.m_PlayerList[k];
					
					if (!playerSync)
						continue;
					
					if (filter != "")
					{
						playerName = playerSync.m_PlayerName;
						playerName.ToLower();
			
						if (playerName.IndexOf(filter) == -1)
							continue;
					}
										
					if (playerSync && playerSync.m_RUID == identitys[i].GetId() && playerSync.m_RUID != playerID)
					{
						AddPlayerEntry(playerSync);
					}
				}
			}
		}
	}
	
	void AddPlayerEntry(SyncPlayer playerSync)
	{
		bool isInList = false;
		for (int i = 0; i < m_ATMMenuController.PlayerEntries.Count(); ++i)
		{
			ExpansionATMMenuPlayerEntry entry = m_ATMMenuController.PlayerEntries[i];
			if (entry.m_Player.m_RUID == playerSync.m_RUID)
			{
				isInList = true;
				continue;
			}
		}
		
		if (!isInList)
		{
			ExpansionATMMenuPlayerEntry newEntry = new ExpansionATMMenuPlayerEntry(this, playerSync);
			m_ATMMenuController.PlayerEntries.Insert(newEntry);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu ClearPlayers
	// ------------------------------------------------------------
	void ClearPlayers()
	{
		if (m_ATMMenuController.PlayerEntries.Count() > 0)
		{
			for (int i = 0; i < m_ATMMenuController.PlayerEntries.Count(); ++i)
			{
				ExpansionATMMenuPlayerEntry entry = m_ATMMenuController.PlayerEntries[i];
				entry.Hide();
				entry = NULL;
			}
			
			m_ATMMenuController.PlayerEntries.Clear();
		}
	}	
	
	// ------------------------------------------------------------
	// ExpansionATMMenu SetPlayer
	// ------------------------------------------------------------
	void SetPlayer(SyncPlayer playerSync, ExpansionATMMenuPlayerEntry selectedEntry)
	{
		m_SelectedPlayer = playerSync;
				
		for (int i = 0; i < m_ATMMenuController.PlayerEntries.Count(); ++i)
		{
			ExpansionATMMenuPlayerEntry entry = m_ATMMenuController.PlayerEntries[i];
			if (entry != selectedEntry && entry.IsSelected())
				entry.SetSelected(false);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu OnFilterButtonClick
	// ------------------------------------------------------------
	void OnFilterButtonClick()
	{
		atm_filter_box.SetText("");
		LoadPlayers("");
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu OnTransferButtonClick
	// ------------------------------------------------------------
	void OnTransferButtonClick()
	{
		if (!m_SelectedPlayer)
			return;
	
		if (!GetExpansionSettings().GetMarket().ATMPlayerTransferEnabled)	
			return;
		
		//! Only numbers are allowed
		int quantity = GetCheckAmount("ATM_TRANSFER_FAILED");
		if (quantity == -1)
			return;
		
		m_Amount = quantity;
		
		//! Make sure we dont send negative or 0 amounts
		if (m_Amount <= 0)
		{
			ExpansionNotification("STR_EXPANSION_ATM_TRANSFER_FAILED", "STR_EXPANSION_ATM_NONZERO").Error();
			return;
		}
				
		//! We only can only transfer what we have deposited
		if (m_Amount > m_ATMData.MoneyDeposited)
		{
			ExpansionNotification("STR_EXPANSION_ATM_TRANSFER_FAILED", "STR_EXPANSION_ATM_AMOUNT_MAX_ERROR").Error();
			return;
		}
		
		if (!m_TransferDialog)
			m_TransferDialog = new ExpansionATMMenuTransferDialog(this, m_SelectedPlayer, m_Amount);	
		
		m_TransferDialog.Show();
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu ConfirmTransfer
	// ------------------------------------------------------------
	void ConfirmTransfer()
	{
		m_TransferDialog.Hide();
		m_MarketModule.RequestTransferMoneyToPlayer(m_Amount, m_SelectedPlayer.m_RUID);
	}
		
	// ------------------------------------------------------------
	// ExpansionATMMenu OnTransferPartyButtonClick
	// ------------------------------------------------------------
	void OnTransferPartyButtonClick()
	{
		#ifdef EXPANSIONMOD
		if (!m_Party)
			return;
				
		//! Only numbers are allowed
		int quantity = GetCheckAmount("ATM_DEPOSIT_FAILED");
		if (quantity == -1)
			return;
		
		m_Amount = quantity;
		
		//! Make sure we dont send negative or 0 amounts
		if (m_Amount <= 0)
		{
			ExpansionNotification("STR_EXPANSION_ATM_DEPOSIT_FAILED", "STR_EXPANSION_ATM_NONZERO").Error();
			return;
		}
		
		//! We can only deposit money until we reach max. depending on server setting
		if ((m_Party.GetMoneyDeposited() + m_Amount) > GetExpansionSettings().GetMarket().MaxPartyDepositMoney)
		{
			ExpansionNotification("STR_EXPANSION_ATM_DEPOSIT_FAILED",  new StringLocaliser("STR_EXPANSION_ATM_DEPOSIT_MAX_ERROR", GetExpansionSettings().GetMarket().MaxPartyDepositMoney.ToString())).Error();
			return;
		}
		
		//! Check if we try to deposit more then we have in deposit
		if (m_ATMData.MoneyDeposited < m_Amount)
		{
			ExpansionNotification("STR_EXPANSION_ATM_DEPOSIT_FAILED", new StringLocaliser("You don't have more then %1 in your deposit!", m_ATMData.MoneyDeposited.ToString())).Error();
			return;
		}
		
		if (!m_PartyTransferDialog)
			m_PartyTransferDialog = new 	ExpansionATMMenuPartyTransferDialog(this, m_Amount, m_Party);	
		
		m_PartyTransferDialog.Show();
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu OnWithdrawButtonClick
	// ------------------------------------------------------------
	void OnWithdrawPartyButtonClick()
	{
		#ifdef EXPANSIONMOD
		if (!m_Party)
			return;
		
		//! Only can withdaw money when promotet
		if (!GetPlayerPartyData() || !GetPlayerPartyData().CanWithdrawMoney())
			return;
				
		//! Only numbers are allowed
		int quantity = GetCheckAmount("ATM_WITHDRAW_FAILED");
		if (quantity == -1)
			return;
		
		m_Amount = quantity;
		
		//! Make sure we dont send negative or 0 amounts
		if (m_Amount <= 0)
		{
			ExpansionNotification("STR_EXPANSION_ATM_WITHDRAW_FAILED", "STR_EXPANSION_ATM_NONZERO").Error();
			return;
		}
				
		//! We only can withdraw what we have deposited
		if (m_Amount > m_Party.GetMoneyDeposited())
		{
			ExpansionNotification("STR_EXPANSION_ATM_WITHDRAW_FAILED", "STR_EXPANSION_ATM_AMOUNT_MAX_ERROR").Error();
			return;
		}
		
		m_MarketModule.RequestPartyWithdrawMoney(m_Amount, m_Party.GetPartyID());
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenu ConfirmPartyTransfer
	// ------------------------------------------------------------
	void ConfirmPartyTransfer()
	{
		#ifdef EXPANSIONMOD
		m_PartyTransferDialog.Hide();
		m_MarketModule.RequestPartyTransferMoney(m_Amount, m_Party.GetPartyID());
		#endif
	}
	
	#ifdef EXPANSIONMOD
	// ------------------------------------------------------------
	// ExpansionATMMenu GetPlayerPartyData
	// ------------------------------------------------------------
	ExpansionPartyPlayerData GetPlayerPartyData()
	{
		return m_Party.GetPlayer(GetGame().GetPlayer().GetIdentity().GetId());
	}
	#endif
	
	// ------------------------------------------------------------
	// ExpansionATMMenu OnTransferCallback
	// ------------------------------------------------------------
	void OnTransferCallback(ExpansionMarketATM_Data data)
	{
		m_ATMData = data;

		UpdateView();
	}
	
	#ifdef EXPANSIONMOD
	// ------------------------------------------------------------
	// ExpansionATMMenu OnPartyCallback
	// ------------------------------------------------------------
	void OnPartyCallback(int amount, ExpansionPartyData party, ExpansionMarketATM_Data data, int state)
	{
		Print("OnPartyCallback");
		m_Party = party;
		m_ATMData = data;
		
		switch (state)
		{
			//! Deposit
			case 1:
			{	
				ExpansionNotification("STR_EXPANSION_ATM_DEPOSIT_SUCCESS", new StringLocaliser("STR_EXPANSION_ATM_DEPOSIT_SUCCESS_TEXT", amount.ToString())).Success();
			}
			break;
			//! Withdraw
			case 2:
			{
				ExpansionNotification("STR_EXPANSION_ATM_WITHDRAW_SUCCESS", new StringLocaliser("STR_EXPANSION_ATM_WITHDRAW_SUCCESS_TEXT", amount.ToString())).Success();
			}
			break;
		}
		
		UpdateView();
	}
	#endif
	
	// ------------------------------------------------------------
	// ExpansionATMMenu OnChange
	// ------------------------------------------------------------
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w == atm_filter_box)
		{
			LoadPlayers(atm_filter_box.GetText());
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case atm_filter_clear:
			atm_filter_clear_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorSearchFilterButton"));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case atm_filter_clear:
			atm_filter_clear_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetNearbyPlayerIdentitys
	// ------------------------------------------------------------
	array<PlayerIdentity> GetNearbyPlayerIdentitys(vector position, int radius)
	{
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		array<PlayerIdentity> identitys = new array<PlayerIdentity>;
		
		GetGame().GetObjectsAtPosition(position, radius, objects, proxyCargos);
		
		if (objects.Count() > 0)
		{
			foreach (Object obj : objects)
			{
				PlayerBase player;
				if (Class.CastTo(player, obj))
				{
					identitys.Insert(player.GetIdentity());
				}
			}
		}
		
		return identitys;
	}
};

class ExpansionATMMenuController: ExpansionViewController
{
	string MaxValue;
	string MoneyDepositValue;
	string PlayerMoneyValue;
	
	string PartyName;
	string PartyID;
	string PartyOwner;
	string PartyMaxValue;
	string PartyMoneyDepositValue;
	
	ref ObservableCollection<ref ExpansionATMMenuPlayerEntry> PlayerEntries = new ObservableCollection<ref ExpansionATMMenuPlayerEntry>(this);
};