/**
 * ExpansionNotificationHUD.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONUI
class ExpansionNotificationHUD: ExpansionScriptViewBase
{
	protected ExpansionNotificationHUDController m_NotificationHUDController;
	
	protected Widget MarketContent;
	protected bool m_ElementPosAdjustment = false;
	
	void ExpansionNotificationHUD()
	{
		if (!m_NotificationHUDController)
			m_NotificationHUDController = ExpansionNotificationHUDController.Cast(GetController());
	}
	
	void ~ExpansionNotificationHUD()
	{

	}
	
	override typename GetControllerType() 
	{
		return ExpansionNotificationHUDController;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/expansion_notification_hud.layout";
	}
	
	void AddNotificationToatsElemement(ExpansionNotificationViewToast element)
	{
		if (!m_NotificationHUDController)
			return;
		
		m_NotificationHUDController.NotificationToastElements.Insert(element);
	}
	
	void RemoveNotificationToastElemement(ExpansionNotificationViewToast element)
	{
		if (!m_NotificationHUDController)
			return;
		
		int index = -1;
		index = m_NotificationHUDController.NotificationToastElements.Find(element);
		if (index > -1)
			m_NotificationHUDController.NotificationToastElements.Remove(index);
	}
	
	void AddNotificationBaguetteElemement(ExpansionNotificationViewBaguette element)
	{
		if (!m_NotificationHUDController)
			return;
		
		m_NotificationHUDController.NotificationBaguetteElements.Insert(element);
	}
	
	void RemoveNotificationBaguetteElemement(ExpansionNotificationViewBaguette element)
	{
		if (!m_NotificationHUDController)
			return;
		
		int index = -1;
		index = m_NotificationHUDController.NotificationBaguetteElements.Find(element);
		if (index > -1)
			m_NotificationHUDController.NotificationBaguetteElements.Remove(index);
	}
	
	void HideBaguetteElements()
	{
		if (!m_NotificationHUDController)
			return;
		
		for (int i = 0; i < m_NotificationHUDController.NotificationBaguetteElements.Count(); i++)
		{
			ExpansionNotificationViewBaguette view = m_NotificationHUDController.NotificationBaguetteElements[i];
			view.GetLayoutRoot().Show(false);
		}
	}
	
	void AddNotificationActivityElemement(ExpansionNotificationViewActivity element)
	{
		if (!m_NotificationHUDController)
			return;
		
		m_NotificationHUDController.NotificationActivityElements.Insert(element);
	}
	
	void RemoveNotificationActivityElemement(ExpansionNotificationViewActivity element)
	{
		if (!m_NotificationHUDController)
			return;
		
		int index = -1;
		index = m_NotificationHUDController.NotificationActivityElements.Find(element);
		if (index > -1)
			m_NotificationHUDController.NotificationActivityElements.Remove(index);
	}
	
	void HideActivityElements()
	{
		if (!m_NotificationHUDController)
			return;
		
		for (int i = 0; i < m_NotificationHUDController.NotificationActivityElements.Count(); i++)
		{
			ExpansionNotificationViewActivity view = m_NotificationHUDController.NotificationActivityElements[i];
			view.GetLayoutRoot().Show(false);
		}
	}
	
	void AddNotificationKillfeedElemement(ExpansionNotificationViewKillfeed element)
	{
		if (!m_NotificationHUDController)
			return;
				
		m_NotificationHUDController.NotificationKillfeedElements.Insert(element);
	}
	
	void RemoveNotificationKillfeedElemement(ExpansionNotificationViewKillfeed element)
	{
		if (!m_NotificationHUDController)
			return;
		
		int index = -1;
		index = m_NotificationHUDController.NotificationKillfeedElements.Find(element);
		if (index > -1)
			m_NotificationHUDController.NotificationKillfeedElements.Remove(index);
	}
	
	void AddNotificationMarketElemement(ExpansionNotificationViewMarket element)
	{
		if (!m_NotificationHUDController)
			return;
			
		m_NotificationHUDController.NotificationMarketElements.Insert(element);
	}
	
	void RemoveNotificationMarketElemement(ExpansionNotificationViewMarket element)
	{
		if (!m_NotificationHUDController)
			return;
		
		int index = -1;
		index = m_NotificationHUDController.NotificationMarketElements.Find(element);
		if (index > -1)
			m_NotificationHUDController.NotificationMarketElements.Remove(index);
	}
	
	void AddNotificationGarageElemement(ExpansionNotificationViewGarage element)
	{	
		if (!m_NotificationHUDController)
			return;

		if (!m_ElementPosAdjustment)
		{
			Print(ToString() + "::AddNotificationGarageElemement - Adjust element position!");
			
			float x1, y1;
			Widget content = Widget.Cast(GetLayoutRoot().FindAnyWidget("MarketContent"));
			content.GetPos(x1, y1);
			float yNew1 = y1 + 80.0;
			content.SetPos(x1, yNew1);
			m_ElementPosAdjustment = true;
		}
		
		m_NotificationHUDController.NotificationMarketElements.Insert(element);
	}
	
	void RemoveNotificationGarageElemement(ExpansionNotificationViewGarage element)
	{
		if (!m_NotificationHUDController)
			return;
		
		int index = -1;
		index = m_NotificationHUDController.NotificationMarketElements.Find(element);
		if (index > -1)
			m_NotificationHUDController.NotificationMarketElements.Remove(index);
		
		if (m_ElementPosAdjustment)
		{
			Print(ToString() + "::RemoveNotificationGarageElemement - Adjust element position!");
			
			float x1, y1;
			Widget content = Widget.Cast(GetLayoutRoot().FindAnyWidget("MarketContent"));
			content.GetPos(x1, y1);
			float yNew1 = y1 - 80.0;
			content.SetPos(x1, yNew1);
			m_ElementPosAdjustment = false;
		}
	}
}

class ExpansionNotificationHUDController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionNotificationViewToast> NotificationToastElements = new ObservableCollection<ref ExpansionNotificationViewToast>(this);
	ref ObservableCollection<ref ExpansionNotificationViewBaguette> NotificationBaguetteElements = new ObservableCollection<ref ExpansionNotificationViewBaguette>(this);
	ref ObservableCollection<ref ExpansionNotificationViewActivity> NotificationActivityElements = new ObservableCollection<ref ExpansionNotificationViewActivity>(this);
	ref ObservableCollection<ref ExpansionNotificationViewKillfeed> NotificationKillfeedElements = new ObservableCollection<ref ExpansionNotificationViewKillfeed>(this);
	ref ObservableCollection<ref ExpansionNotificationViewMarket> NotificationMarketElements = new ObservableCollection<ref ExpansionNotificationViewMarket>(this);
}
#endif
