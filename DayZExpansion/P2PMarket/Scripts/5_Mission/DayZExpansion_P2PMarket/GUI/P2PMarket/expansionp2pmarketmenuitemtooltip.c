/**
 * ExpansionP2PMarketMenuItemTooltip.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuItemTooltip: ExpansionScriptView
{
	protected ref ExpansionP2PMarketMenuItemTooltipController m_TooltipController;
	protected ExpansionP2PMarketListingBase m_Listing;
	protected ExpansionP2PMarketMenuViewBase m_ParentView;
	
	protected ImageWidget tooltip_icon;
	protected Widget tooltip_header;
	protected Widget tooltip_content;
	protected HtmlWidget tooltip_title;
	
	protected bool m_IsEquipped = false;
	protected bool m_HasItems = false;
	protected bool m_HasAttachments = false;
	protected bool m_HasAmmo = false;
	protected bool m_IsAttached = false;
	protected bool m_InHands = false;
	
	void ExpansionP2PMarketMenuItemTooltip(ExpansionP2PMarketMenuViewBase viewBase)
	{
		m_ParentView = viewBase;
		
		if (!m_TooltipController)
			m_TooltipController = ExpansionP2PMarketMenuItemTooltipController.Cast(GetController());
		
		tooltip_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoIcon"));
		tooltip_header.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorTooltipsHeaders"));
		tooltip_content.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorTooltipsBackground"));
	}
	
	void ~ExpansionP2PMarketMenuItemTooltip()
	{
		for (int i = 0; i < m_TooltipController.SpacerEntries.Count(); ++i)
		{
			ExpansionP2PMarketMenuItemTooltipEntryItemInfo infoItem = m_TooltipController.SpacerEntries[i];
			infoItem.Destroy();
		}

		m_TooltipController.SpacerEntries.Clear();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/P2PMarket/GUI/layouts/expansion_p2pmarket_item_element_tooltip.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionP2PMarketMenuItemTooltipController;
	}
	
	void SetView()
	{
		m_TooltipController.TooltipTitle = ExpansionStatic.GetItemDisplayNameWithType(m_Listing.GetClassName());
		m_TooltipController.NotifyPropertyChanged("TooltipTitle");
		tooltip_title.SetColor(ARGB(255, 255, 255, 255));
	}
	
	void SetListing(ExpansionP2PMarketListingBase listing)
	{
		m_Listing = listing;
			
		SetView();
		CheckForItemInfos();
	}
	
	void CheckForItemInfos()
	{
		EntityAI item = m_Listing.GetItem();
		if (m_Listing && item)
		{
			ExpansionP2PMarketMenuItemTooltipEntryItemInfo itemInfoEntry;
			StringLocaliser text;
			
			if (m_Listing.GetContainerItems().Count() > 0)
			{
				itemInfoEntry = new ExpansionP2PMarketMenuItemTooltipEntryItemInfo(this);
				string textID;
				string colorID;
				if (!item.IsWeapon())
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

				text = new StringLocaliser(textID, m_Listing.GetContainerItems().Count().ToString());			
				itemInfoEntry.SetText(text.Format());
				itemInfoEntry.SetIcon("Info");
				itemInfoEntry.SetColor(ARGB(255, 255, 255, 255));
				m_TooltipController.SpacerEntries.Insert(itemInfoEntry);
			}
			
			if (HasItemOnInventorySlot(item))
			{
				itemInfoEntry = new ExpansionP2PMarketMenuItemTooltipEntryItemInfo(this);
				itemInfoEntry.SetText("#STR_EXPANSION_MARKET_ITEM_TOOLTIP_ONSLOT");
				itemInfoEntry.SetIcon("Info");
				itemInfoEntry.SetColor(ARGB(255, 255, 255, 255));
				m_TooltipController.SpacerEntries.Insert(itemInfoEntry);
				
				m_IsEquipped = true;
			}
			
			if (item.IsMagazine())
			{
				MagazineStorage magStorage = MagazineStorage.Cast(item);
				if (magStorage && magStorage.GetAmmoCount() > 0)
				{
					itemInfoEntry = new ExpansionP2PMarketMenuItemTooltipEntryItemInfo(this);
					int countAmmo;
					
					text = new StringLocaliser("STR_EXPANSION_MARKET_ITEM_TOOLTIP_BULLETS", magStorage.GetAmmoCount().ToString());
					itemInfoEntry.SetText(text.Format());
					itemInfoEntry.SetIcon("Info");
					itemInfoEntry.SetColor(ARGB(255, 255, 255, 255));
					m_TooltipController.SpacerEntries.Insert(itemInfoEntry);
					
					m_HasAmmo = true;
				}
			}

			if (item.GetInventory().IsAttachment())
			{
				itemInfoEntry = new ExpansionP2PMarketMenuItemTooltipEntryItemInfo(this);					
				if (item.GetHierarchyParent())
				{
					string name = item.GetHierarchyParent().GetDisplayName();
					if (item.GetHierarchyParent().IsWeapon())
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
				itemInfoEntry.SetColor(ARGB(255, 255, 255, 255));
				m_TooltipController.SpacerEntries.Insert(itemInfoEntry);
				
				m_IsAttached = true;
			}
			
			if (item.GetHierarchyRootPlayer() && item.GetHierarchyRootPlayer().GetHumanInventory().GetEntityInHands() && item.GetHierarchyRootPlayer().GetHumanInventory().GetEntityInHands() == item)
			{
				itemInfoEntry = new ExpansionP2PMarketMenuItemTooltipEntryItemInfo(this);
				text = new StringLocaliser("STR_EXPANSION_MARKET_ITEM_TOOLTIP_ITEM_IN_HANDS");
				
				itemInfoEntry.SetText(text.Format());
				itemInfoEntry.SetIcon("Info");
				itemInfoEntry.SetColor(ARGB(255, 255, 255, 255));
				m_TooltipController.SpacerEntries.Insert(itemInfoEntry);

				m_InHands = true;
			}
			
			if (m_HasItems || m_HasAttachments || m_IsEquipped || m_HasAmmo || m_IsAttached || m_InHands)
			{
				m_ParentView.ShowInfoButton(true);
			}
		}
	}
	
	bool HasItemOnInventorySlot(EntityAI item)
	{
		array<string> slots = {"Back", "Vest", "Legs", "Body", "Hands", "Shoulder", "Melee", "Bow", "Hips", "Feet", "Armband", "Headgear", "Mask", "Eyewear", "LeftHand", "Gloves"};
		foreach (string slot: slots)
		{
			if (g_Game.GetPlayer().FindAttachmentBySlotName(slot) == item /*&& MiscGameplayFunctions.Expansion_HasAnyCargo(item)*/)
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

	bool HasItems()
	{
		return m_HasItems;
	}
	
	bool HasAttachments()
	{
		return m_HasAttachments;
	}
	
	bool IsEquipped()
	{
		return m_IsEquipped;
	}
	
	bool IsAttached()
	{
		return m_IsAttached;
	}
	
	bool InHands()
	{
		return m_InHands;
	}
};

class ExpansionP2PMarketMenuItemTooltipController: ExpansionViewController
{
	string TooltipTitle;
	ref ObservableCollection<ref ExpansionP2PMarketMenuItemTooltipEntryItemInfo> SpacerEntries = new ObservableCollection<ref ExpansionP2PMarketMenuItemTooltipEntryItemInfo>(this);
};