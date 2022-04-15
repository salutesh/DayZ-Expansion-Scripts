/**
 * ExpansionMarketMenuItemManagerCategoryItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuItemManagerCategoryItem: ExpansionScriptView
{
	protected ref ExpansionMarketMenuItemManagerCategoryItemController m_MarketItemManagerCategoryItemController;
	protected ExpansionMarketMenu m_MarketMenu;
	ref ExpansionMarketMenuItemManager m_MarketMenuItemManager;
	protected string m_ItemClassName;
	protected ButtonWidget item_element_lmbutton;
	protected Widget item_element_background;
	protected ButtonWidget item_element_increment;
	protected ButtonWidget item_element_decrement;
	protected ButtonWidget item_element_tooltip;
	protected ImageWidget item_element_tooltip_icon;
	protected bool m_CanBeAttached;
	protected bool m_CanBeAttachedOrReplaceConflicting;
	protected ref ExpansionMarketMenuTooltip m_Tooltip;
	
	EntityAI m_Object;
	protected ref ExpansionItemTooltip m_ItemTooltip;
	
	void ExpansionMarketMenuItemManagerCategoryItem(string itemClassName, ExpansionMarketMenu menu, ExpansionMarketMenuItemManager itemManager)
	{
		m_ItemClassName = itemClassName;
		m_MarketMenu = menu;
		m_MarketMenuItemManager = itemManager;
		
		if (!m_MarketItemManagerCategoryItemController)
			m_MarketItemManagerCategoryItemController = ExpansionMarketMenuItemManagerCategoryItemController.Cast(GetController());
		
		SetView();
	}
	
	void ~ExpansionMarketMenuItemManagerCategoryItem()
	{
		if (m_Object)
			GetGame().ObjectDelete(m_Object);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_item_manager_category_item_element.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuItemManagerCategoryItemController;
	}
	
	void SetView()
	{	
		GetCategoryItemController().ItemName = ExpansionStatic.GetItemDisplayNameWithType(m_ItemClassName);
		GetCategoryItemController().NotifyPropertyChanged("ItemName");

		UpdateView();
		CreatePreviewObject();
	}

	int GetCount()
	{
		int count;
		string classNameLower = m_ItemClassName;
		classNameLower.ToLower();
		foreach (string attachment: m_MarketMenu.GetSelectedMarketItem().SpawnAttachments)
		{
			if (attachment == classNameLower)
				count++;
		}
EXPrint("Attached: " + count + " " + classNameLower);
		return count;
	}
	
	void UpdateView()
	{
		EntityAI parent = EntityAI.Cast(m_MarketMenu.GetSelectedMarketItemElement().GetPreviewObject());

		if (parent)
			m_CanBeAttached = CanAttachAttachment(parent, m_ItemClassName);

		m_CanBeAttachedOrReplaceConflicting = m_CanBeAttached;

		if (m_CanBeAttached)
		{
			//! This item can be attached - nothing to do here
		}
		else if (IsSpecialCase(parent))
		{
			//! Special case: This item can only be attached to specific other item(s)
			Hide();
		}
		else
		{
			//! Item cannot be attached. Check if another attachment is conflicting (slot only)
			m_CanBeAttachedOrReplaceConflicting = IsAttachmentConflicting(m_ItemClassName, true);
		}

		string color;
		if (m_CanBeAttachedOrReplaceConflicting)
			color = "BaseColorHeaders";
		else
			color = "ColorSellButton";
		item_element_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get(color));

		int count = GetCount();
		item_element_decrement.Show(count > 0);
		item_element_increment.Show(m_CanBeAttached || (count == 0 && m_CanBeAttachedOrReplaceConflicting));
		item_element_tooltip.Show(!m_CanBeAttachedOrReplaceConflicting);
	}

	bool IsSpecialCase(EntityAI parent)
	{
		if (!parent)
			return false;

		if ((m_ItemClassName == "M4_CarryHandleOptic" || m_ItemClassName == "BUISOptic") && !parent.IsKindOf("M4A1"))
			return true;

		return false;
	}

	ExpansionMarketMenuItemManagerCategoryItemController GetCategoryItemController()
	{
		return m_MarketItemManagerCategoryItemController;
	}
	
	void OnLMButtonClick()
	{
		if (item_element_decrement.IsVisible())
		{
			OnItemCountDecrement();
		}
		else if (item_element_increment.IsVisible())
		{
			OnItemCountIncrement();
		}
	}
	
	//! Check if this item can be attached to parent at all
	bool CanAttachAttachment(EntityAI parent, string attachment, bool includeExisting = false)
	{
		if (includeExisting)
		{
			//! If an item of the same class is already attached, check if it can be removed & reattached
			ExpansionString exAttachment = new ExpansionString(attachment);
			InventoryLocation loc = new InventoryLocation;

			for (int i = parent.GetInventory().AttachmentCount() - 1; i >= 0; i--)
			{
				EntityAI item = parent.GetInventory().GetAttachmentFromIndex(i);
				if (item)
				{
					if (exAttachment.EqualsCaseInsensitive(item.GetType()))
					{
						item.GetInventory().GetCurrentInventoryLocation(loc);
						if (!GameInventory.LocationRemoveEntity(loc))
						{
							EXPrint("Could NOT remove existing " + item.GetType());
						}
						else if (!GameInventory.LocationAddEntity(loc))
						{
							EXPrint("Could NOT reattach existing " + item.GetType());
							GetGame().ObjectDelete(item);
						}
						else
						{
							EXPrint("Can reattach existing " + item.GetType());
							return true;
						}
						return false;
					}
				}
			}
		}

		//! Try to create attachment on parent. If this succeeds, attaching is possible.
		EntityAI attachmentEntity = ExpansionItemSpawnHelper.SpawnAttachment(attachment, parent);
		if (attachmentEntity)
		{
			GetGame().ObjectDelete(attachmentEntity);

EXPrint("Can attach " + attachment);
			return true;
		}

EXPrint("Can NOT attach " + attachment);
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		super.OnMouseEnter(w, x, y);

		switch (w)
		{
			case item_element_lmbutton:
			{
				if (m_CanBeAttachedOrReplaceConflicting)
				{
					item_element_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorSearchFilterButton"));
				}
				
				if (!m_ItemTooltip && m_Object)
				{
					m_ItemTooltip = new ExpansionItemTooltip(m_Object);
					m_ItemTooltip.SetContentOffset(0.019531, -0.076389);
					m_ItemTooltip.SetShowItemPreview(true);
					m_ItemTooltip.Show();
				}
				break;
			}
			case item_element_tooltip:
			{
				item_element_tooltip_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoAttachments"));
				if (!m_Tooltip) 
				{
					m_Tooltip = new ExpansionMarketMenuTooltip();
					m_Tooltip.SetTitle("#STR_EXPANSION_MARKET_TOOLTIP_ATTACHMENTS_TITLE");
					m_Tooltip.SetText("#STR_EXPANSION_MARKET_TOOLTIP_ATTACHMENTS_DESC");
					m_Tooltip.SetContentOffset(0.019531, 0);
					m_Tooltip.Show();
				}
				break;
			}
			case item_element_increment:
			{
				item_element_increment.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorIncreaseQuantityButton"));
				break;
			}
			case item_element_decrement:
			{
				item_element_decrement.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorDecreaseQuantityButton"));
				break;
			}
		}		
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		super.OnMouseLeave(w, enterW, x, y);

		switch (w)
		{
			case item_element_lmbutton:
			{
				if (m_CanBeAttachedOrReplaceConflicting)
				{
					item_element_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorHeaders"));
				}
				
				if (m_ItemTooltip)
				{
					m_ItemTooltip.Hide();
					delete m_ItemTooltip;
				}
				break;
			}
			case item_element_tooltip:
			{
				item_element_tooltip_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
				if (m_Tooltip) 
				{
					m_Tooltip.Hide();
					delete m_Tooltip;
				}
				break;
			}
			case item_element_increment:
			{
				item_element_increment.SetColor(ARGB(0,0,0,0));
				break;
			}
			case item_element_decrement:
			{
				item_element_decrement.SetColor(ARGB(0,0,0,0));
				break;
			}
		}
			
		return false;
	}

	void OnItemCountIncrement()
	{
		string classNameToLower = m_ItemClassName;
		classNameToLower.ToLower();
		ExpansionMarketMenuItemManagerCategoryItem listItem;
		EntityAI parent = EntityAI.Cast(m_MarketMenu.GetSelectedMarketItemElement().GetPreviewObject());
		
		int findIndexAttachment = -1;
		int findIndexListItem = -1;

		string attachmentToReplace;
		string listItemNameToLower;
		
		//! If it cannot be attached, another attachment may be conflicting.
		//! Check by only looking at the slot for non-weapons and also looking at attachments of the same type for weapons.
		//! Note this is different from the IsAttachmentConflicting call in UpdateView!
		if (!m_CanBeAttached && IsAttachmentConflicting(m_ItemClassName, parent && !parent.IsWeapon(), attachmentToReplace))
		{
			if (attachmentToReplace != "")
			{
				RemoveAttachment(attachmentToReplace);
			}
		}
		
EXPrint("Adding " + classNameToLower);
		m_MarketMenu.GetSelectedMarketItem().SpawnAttachments.Insert(classNameToLower);
		
		//! UpdatePreview needs to be called before calling UpdateAttachments
		m_MarketMenuItemManager.UpdatePreview();
		UpdateAttachments();
		m_MarketMenuItemManager.UpdateMenuViews(false);
	}
		
	string GetSlotName(string className)
	{
		string slotName;
		TStringArray inventory_slots = {};
		string slot;
		string path;
		
		if (GetGame().ConfigIsExisting("CfgVehicles " + className))
			path = "CfgVehicles " + className;
		else if (GetGame().ConfigIsExisting("CfgMagazines " + className))
			path = "CfgMagazines " + className;
			
		switch (GetGame().ConfigGetType(path + " inventorySlot")) 
		{
			case CT_ARRAY: 
			{
				GetGame().ConfigGetTextArray(path + " inventorySlot", inventory_slots);
				break;
			}
			
			case CT_STRING: 
			{
				GetGame().ConfigGetText(path + " inventorySlot", slot);
				break;
			}
		}
		
		if (inventory_slots.Count() > 0)
		{
			slotName = inventory_slots[0];
		}
		else if (slot != "")
		{
			slotName = slot;
		}
		
		return slotName;
	}

	void OnItemCountDecrement()
	{		
		if (RemoveAttachment() > -1)
		{	
			//! UpdatePreview needs to be called before calling UpdateAttachments
			m_MarketMenuItemManager.UpdatePreview();
			UpdateAttachments();
			m_MarketMenuItemManager.UpdateMenuViews(false);
		}
	}

	int RemoveAttachment(string className)
	{
		string classNameToLower = className;
		classNameToLower.ToLower();

		int findIndexAttachment = m_MarketMenu.GetSelectedMarketItem().SpawnAttachments.Find(classNameToLower);
		if (findIndexAttachment > -1)
		{
EXPrint("Removing " + classNameToLower);
			m_MarketMenu.GetSelectedMarketItem().SpawnAttachments.RemoveOrdered(findIndexAttachment);
		}

		return findIndexAttachment;
	}

	int RemoveAttachment()
	{
		return RemoveAttachment(m_ItemClassName);
	}

	void UpdateAttachments()
	{
		//! Check other attachments if they can still be attached and if not, remove them
		EntityAI parent = EntityAI.Cast(m_MarketMenu.GetSelectedMarketItemElement().GetPreviewObject());
		TStringArray attachments = m_MarketMenu.GetSelectedMarketItem().SpawnAttachments;
		for (int j = attachments.Count() - 1; j >= 0; j--)
		{
			if (!CanAttachAttachment(parent, attachments[j], true))
			{
				attachments.Remove(j);
			}
		}
	}

	bool IsInSpawnAttachments(string className)
	{
		return (m_MarketMenu.GetSelectedMarketItem().SpawnAttachments.Find(className) > -1);
	}
	
	bool IsAttachmentConflicting(string className, bool checkSlotOnly, out string conflictingAttachment = "")
	{
		//! Check if the attachment we want to add to the spawn attachment is conflicting
		//! with an existing attachment (same slot name or same attachment type)
		bool hasSameSlotName = false;
		TStringArray inventory_slots = {};
		TStringArray tmp;
		string slot;
		string path;
		
		if (GetGame().ConfigIsExisting("CfgVehicles " + className))
			path = "CfgVehicles " + className;
		else if (GetGame().ConfigIsExisting("CfgMagazines " + className))
			path = "CfgMagazines " + className;
			
		switch (GetGame().ConfigGetType(path + " inventorySlot")) 
		{
			case CT_ARRAY: 
			{
				GetGame().ConfigGetTextArray(path + " inventorySlot", inventory_slots);
				tmp = inventory_slots;
				inventory_slots = new TStringArray;
				foreach (string inventory_slot: tmp)
				{
					inventory_slot.ToLower();
					inventory_slots.Insert(inventory_slot);
				}
				break;
			}
			
			case CT_STRING: 
			{
				GetGame().ConfigGetText(path + " inventorySlot", slot);
				slot.ToLower();
				break;
			}
		}
		
		if (!checkSlotOnly)
		{
			//! Only used for weapons
			ExpansionMarketAttachType attachType1 = ExpansionMarketFilters.GetWeaponAttachmentAttachType(className);
		}

		foreach (string attachment_name: m_MarketMenu.GetSelectedMarketItem().SpawnAttachments)
		{
			if (!checkSlotOnly)
			{
				//! Only used for weapons
				ExpansionMarketAttachType attachType2 = ExpansionMarketFilters.GetWeaponAttachmentAttachType(attachment_name);
				if (attachType1 == attachType2 && attachType1 != ExpansionMarketAttachType.OTHER)
				{
					//! Only allow one of each attachment type
					conflictingAttachment = attachment_name;
EXPrint(className + " is conflicting with existing " + conflictingAttachment);
					return true;
				}
			}

			TStringArray attachment_inventory_slots = {};
			string attachment_slot;
			string attachment_path;
			
			if (GetGame().ConfigIsExisting("CfgVehicles " + attachment_name))
				attachment_path = "CfgVehicles " + attachment_name;
			else if (GetGame().ConfigIsExisting("CfgMagazines " + attachment_name))
				attachment_path = "CfgMagazines " + attachment_name;
			
			switch (GetGame().ConfigGetType(attachment_path + " inventorySlot")) 
			{
				case CT_ARRAY: 
				{
					GetGame().ConfigGetTextArray(attachment_path + " inventorySlot", attachment_inventory_slots);
					tmp = attachment_inventory_slots;
					attachment_inventory_slots = new TStringArray;
					foreach (string attachment_inventory_slot: tmp)
					{
						attachment_inventory_slot.ToLower();
						attachment_inventory_slots.Insert(attachment_inventory_slot);
					}
					break;
				}
				
				case CT_STRING: 
				{
					GetGame().ConfigGetText(attachment_path + " inventorySlot", attachment_slot);
					attachment_slot.ToLower();
					break;
				}
			}
			
			if (inventory_slots.Count() > 0)
			{
				if (attachment_inventory_slots.Count() > 0)
				{
					foreach(string slotName1: inventory_slots)
					{
						if (attachment_inventory_slots.Find(slotName1) > -1)
						{
							hasSameSlotName = true;
							conflictingAttachment = attachment_name;
							break;
						}
					}
				}
				else if (attachment_slot != "")
				{
					if (inventory_slots.Find(attachment_slot) > -1)
					{
						hasSameSlotName = true;
						conflictingAttachment = attachment_name;
					}
				}
			}
			else if (slot != "")
			{
				if (attachment_inventory_slots.Count() > 0)
				{
					if (attachment_inventory_slots.Find(slot) > -1)
					{
						hasSameSlotName = true;
						conflictingAttachment = attachment_name;
					}
				}
				else if (attachment_slot != "")
				{
					if (attachment_slot == slot)
					{
						hasSameSlotName = true;
						conflictingAttachment = attachment_name;
					}
				}
			}

			if (hasSameSlotName)
			{
EXPrint(className + " is conflicting with existing " + conflictingAttachment);
				return true;
			}
		}
		
		return false;
	}
	
	void CreatePreviewObject()
	{
		if (m_Object)
		{
			GetGame().ObjectDelete(m_Object);
		}
		
		if (!GetGame().IsKindOf(m_ItemClassName, "DZ_LightAI"))
			m_Object = EntityAI.Cast(GetGame().CreateObjectEx(m_ItemClassName, vector.Zero, ECE_LOCAL|ECE_NOLIFETIME));
		else
			m_Object = NULL;
	}
	
	string GetItemClassName()
	{
		return m_ItemClassName;
	}
};
class ExpansionMarketMenuItemManagerCategoryItemController: ExpansionViewController
{
	string ItemName;
};