/**
 * ExpansionNotificationHUD.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNotificationHUD: ScriptView
{
	protected ExpansionNotificationHUDController m_NotificationHUDController;
	
	void ExpansionNotificationHUD()
	{
		if (!m_NotificationHUDController)
			m_NotificationHUDController = ExpansionNotificationHUDController.Cast(GetController());
	}
	
	override typename GetControllerType() 
	{
		return ExpansionNotificationHUDController;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/GUI/layouts/expansion_notification_hud.layout";
	}
	
	void AddNotificationToatsElemement(ExpansionNotificationViewToast element)
	{
		m_NotificationHUDController.NotificationToastElements.Insert(element);
	}
	
	void RemoveNotificationToastElemement(ExpansionNotificationViewToast element)
	{
		int index = -1;
		index = m_NotificationHUDController.NotificationToastElements.Find(element);
		if (index > -1)
			m_NotificationHUDController.NotificationToastElements.Remove(index);
	}
	
	void AddNotificationBaguetteElemement(ExpansionNotificationViewBaguette element)
	{
		if (m_NotificationHUDController.NotificationBaguetteElements.Count() > 2)
			HideBaguetteElements();
		
		m_NotificationHUDController.NotificationBaguetteElements.Insert(element);
	}
	
	void RemoveNotificationBaguetteElemement(ExpansionNotificationViewBaguette element)
	{
		int index = -1;
		index = m_NotificationHUDController.NotificationBaguetteElements.Find(element);
		if (index > -1)
			m_NotificationHUDController.NotificationBaguetteElements.Remove(index);
	}
	
	void HideBaguetteElements()
	{		
		for (int i = 0; i < m_NotificationHUDController.NotificationBaguetteElements.Count(); i++)
		{
			ExpansionNotificationViewBaguette view = m_NotificationHUDController.NotificationBaguetteElements[i];
			view.GetLayoutRoot().Show(false);
		}
	}
	
	void AddNotificationActivityElemement(ExpansionNotificationViewActivity element)
	{		
		if (m_NotificationHUDController.NotificationActivityElements.Count() > 2)
			HideActivityElements();
		
		m_NotificationHUDController.NotificationActivityElements.Insert(element);
	}
	
	void RemoveNotificationActivityElemement(ExpansionNotificationViewActivity element)
	{
		int index = -1;
		index = m_NotificationHUDController.NotificationActivityElements.Find(element);
		if (index > -1)
			m_NotificationHUDController.NotificationActivityElements.Remove(index);
	}
	
	void HideActivityElements()
	{
		for (int i = 0; i < m_NotificationHUDController.NotificationActivityElements.Count(); i++)
		{
			ExpansionNotificationViewActivity view = m_NotificationHUDController.NotificationActivityElements[i];
			view.GetLayoutRoot().Show(false);
		}
	}
	
	void AddNotificationKillfeedElemement(ExpansionNotificationViewKillfeed element)
	{				
		m_NotificationHUDController.NotificationKillfeedElements.Insert(element);
	}
	
	void RemoveNotificationKillfeedElemement(ExpansionNotificationViewKillfeed element)
	{
		int index = -1;
		index = m_NotificationHUDController.NotificationKillfeedElements.Find(element);
		if (index > -1)
			m_NotificationHUDController.NotificationKillfeedElements.Remove(index);
	}
}

class ExpansionNotificationHUDController: ViewController
{
	ref ObservableCollection<ref ExpansionNotificationViewToast> NotificationToastElements = new ObservableCollection<ref ExpansionNotificationViewToast>(this);
	ref ObservableCollection<ref ExpansionNotificationViewBaguette> NotificationBaguetteElements = new ObservableCollection<ref ExpansionNotificationViewBaguette>(this);
	ref ObservableCollection<ref ExpansionNotificationViewActivity> NotificationActivityElements = new ObservableCollection<ref ExpansionNotificationViewActivity>(this);
	ref ObservableCollection<ref ExpansionNotificationViewKillfeed> NotificationKillfeedElements = new ObservableCollection<ref ExpansionNotificationViewKillfeed>(this);
}