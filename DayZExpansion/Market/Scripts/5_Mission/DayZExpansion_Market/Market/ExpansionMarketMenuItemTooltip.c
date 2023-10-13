/**
 * ExpansionMarketMenuItemTooltip.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuItemTooltip: ExpansionScriptView
{
	protected ref ExpansionMarketMenuItemTooltipController m_TooltipController;
	protected ref ExpansionMarketMenuItem m_ItemElement;
	protected ref ExpansionMarketPlayerItem m_PlayerItem;
	
	protected ImageWidget tooltip_icon;
	protected Widget tooltip_header;
	protected Widget tooltip_content;
	protected HtmlWidget tooltip_title;
	
	protected bool m_IsEquiped = false;
	protected bool m_HasItems = false;
	protected bool m_HasAttachments = false;
	protected bool m_HasAmmo = false;
	protected bool m_IsAttached = false;
	
	void ExpansionMarketMenuItemTooltip(ExpansionMarketMenuItem element)
	{
		m_ItemElement = element;
		
		if (!m_TooltipController)
			m_TooltipController = ExpansionMarketMenuItemTooltipController.Cast(GetController());
		
		tooltip_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoIcon"));
		tooltip_header.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorTooltipsHeaders"));
		tooltip_content.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorTooltipsBackground"));
		
		SetView();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_item_element_tooltip.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuItemTooltipController;
	}
	
	void SetView()
	{
		m_TooltipController.TooltipTitle = ExpansionStatic.GetItemDisplayNameWithType(m_ItemElement.GetMarketItem().ClassName);
		m_TooltipController.NotifyPropertyChanged("TooltipTitle");
		tooltip_title.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoTitle"));
		
		CheckForItemInfos();
	}
	
	void CheckForItemInfos()
	{			
		if (m_ItemElement.GetMarketMenu().HasPlayerItem(m_ItemElement.GetMarketItem().ClassName))
		{
			ExpansionMarketModule marketModule = ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));

			array<ref ExpansionMarketPlayerItem> items = m_ItemElement.GetMarketMenu().GetPlayerItems();
			
			m_TooltipController.SpacerEntries.Clear();
			
			for (int j = 0; j < items.Count(); j++)
			{
				string itemName = items[j].ClassName;
				itemName.ToLower();
				
				itemName = marketModule.GetMarketItemClassName(marketModule.GetTrader().GetTraderMarket(), itemName);

				if (itemName == m_ItemElement.GetMarketItem().ClassName)
				{
					m_PlayerItem = items[j];
				}
			}
			
			if (m_PlayerItem)
			{
				ExpansionMarketMenuItemTooltipEntryItemInfo itemInfoEntry;
				StringLocaliser text;
				
				if (m_PlayerItem.ContainerItemsCount > 0)
				{
					itemInfoEntry = new ExpansionMarketMenuItemTooltipEntryItemInfo(this);
					string textID;
					string colorID;
					if (!m_PlayerItem.IsWeapon())
					{
						textID = "STR_EXPANSION_MARKET_ITEM_TOOLTIP_OTHERITEMS";
						colorID = "ColorItemInfoHasContainerItems";
						
						m_HasItems = true;
					}
					else
					{
						textID = "STR_EXPANSION_MARKET_ITEM_TOOLTIP_ATTACHMENTS";
						colorID = "ColorItemInfoHasAttachments";
						
						m_HasAttachments = true;
					}
					text = new StringLocaliser(textID, m_PlayerItem.ContainerItemsCount.ToString());			
					itemInfoEntry.SetText(text.Format());
					itemInfoEntry.SetIcon("Info");
					itemInfoEntry.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get(colorID));
					m_TooltipController.SpacerEntries.Insert(itemInfoEntry);
					
				}
				
				if (HasItemOnInventorySlot(m_PlayerItem.Item))
				{
					itemInfoEntry = new ExpansionMarketMenuItemTooltipEntryItemInfo(this);
					itemInfoEntry.SetText("#STR_EXPANSION_MARKET_ITEM_TOOLTIP_ONSLOT");
					itemInfoEntry.SetIcon("Info");
					itemInfoEntry.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoIsEquipped"));
					m_TooltipController.SpacerEntries.Insert(itemInfoEntry);
					
					m_IsEquiped = true;
				}
				
				if (m_PlayerItem.IsMagazine())
				{
					MagazineStorage magStorage = MagazineStorage.Cast(m_PlayerItem.Item);
					if (magStorage.GetAmmoCount() > 0)
					{
						itemInfoEntry = new ExpansionMarketMenuItemTooltipEntryItemInfo(this);
						int countAmmo;
						
						text = new StringLocaliser("STR_EXPANSION_MARKET_ITEM_TOOLTIP_BULLETS", magStorage.GetAmmoCount().ToString());
						itemInfoEntry.SetText(text.Format());
						itemInfoEntry.SetIcon("Info");
						itemInfoEntry.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoHasBullets"));
						m_TooltipController.SpacerEntries.Insert(itemInfoEntry);
						
						m_HasAmmo = true;
					}
				}
				
				if (m_PlayerItem.IsAttached())
				{
					itemInfoEntry = new ExpansionMarketMenuItemTooltipEntryItemInfo(this);
					string name;					
					if (m_PlayerItem.Item.GetHierarchyParent())
					{
						name = m_PlayerItem.Item.GetHierarchyParent().GetDisplayName();
						
						if (m_PlayerItem.Item.GetHierarchyParent().IsWeapon())
						{
							text = new StringLocaliser("STR_EXPANSION_MARKET_ITEM_TOOLTIP_ATTACHED_WEAPON", name);
						}
						else
						{
							text = new StringLocaliser("STR_EXPANSION_MARKET_ITEM_TOOLTIP_ATTACHED_ITEM", name);
						}
					}
					
					itemInfoEntry.SetText(text.Format());
					itemInfoEntry.SetIcon("Info");
					itemInfoEntry.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoIsAttachment"));
					m_TooltipController.SpacerEntries.Insert(itemInfoEntry);
					
					m_IsAttached = true;
				}
				
				if (m_HasItems || m_HasAttachments || m_IsEquiped || m_HasAmmo || m_IsAttached)
				{
					m_ItemElement.ShowInfoButton(true);
				}
			}
		}
	}
	
	bool HasItemOnInventorySlot(EntityAI item)
	{
		array<string> slots = {"Back", "Vest", "Legs", "Body", "Hands", "Shoulder", "Melee", "Bow", "Hips", "Feet", "Armband", "Headgear", "Mask", "Eyewear", "LeftHand", "Gloves"};
		foreach (string slot: slots)
		{
			if (GetGame().GetPlayer().FindAttachmentBySlotName(slot) == item /*&& MiscGameplayFunctions.Expansion_HasAnyCargo(item)*/)
			{
				return true;
			}
		}
		return false;
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		int x, y;
		GetMousePos(x, y);
		GetLayoutRoot().SetPos(x - 10.0, y, true);
	}
	
	override void OnHide()
	{
		super.OnHide();
	}
	
	ExpansionMarketPlayerItem GetPlayerItem()
	{
		return m_PlayerItem;
	}
	
	bool HasItems()
	{
		return m_HasItems;
	}
	
	bool HasAttachments()
	{
		return m_HasAttachments;
	}
	
	bool IsEquiped()
	{
		return m_IsEquiped;
	}
	
	bool IsAttached()
	{
		return m_IsAttached;
	}
};

class ExpansionMarketMenuItemTooltipController: ExpansionViewController
{
	string TooltipTitle;
	ref ObservableCollection<ref ExpansionMarketMenuItemTooltipEntryBase> SpacerEntries = new ObservableCollection<ref ExpansionMarketMenuItemTooltipEntryBase>(this);
};