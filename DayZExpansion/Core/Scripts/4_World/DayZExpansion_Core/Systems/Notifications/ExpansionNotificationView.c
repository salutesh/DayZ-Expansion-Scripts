/**
 * ExpansionNotificationView.c
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
class ExpansionNotificationView: ScriptView
{
	ExpansionNotificationViewController m_NotificationViewController;	
	NotificationRuntimeData m_Data;
	ExpansionNotificationModule m_NotificationModule;
	ExpansionNotificationHUD m_NotificationHUD;
	
	protected bool m_Showing;
	protected bool m_Hiding;
	
	protected float m_ShowUpdateTime;
	protected float m_HideUpdateTime;
	
	protected float m_TotalShowUpdateTime;
	protected float m_TotalHideUpdateTime;
	
	protected AbstractWave m_Sound;
	
	protected Widget Notification;
	protected Widget NotificationColorSpacer
	protected Widget NotificationColor;
	protected ImageWidget NotificationIcon;
	protected RichTextWidget NotificationTitle;
	protected RichTextWidget NotificationText;
	protected GridSpacerWidget NotificationElement;
	
	protected Widget PreviewSpacer;
	protected ItemPreviewWidget NotificationPreview;
	
	string m_LayoutPath;
	
	void ExpansionNotificationView(NotificationRuntimeData data, ExpansionNotificationModule module, ExpansionNotificationHUD notificationHUD)
	{
		m_Data = data;
		m_NotificationModule = module;
		m_NotificationHUD = notificationHUD;
		
		if (!m_NotificationViewController)
			m_NotificationViewController = ExpansionNotificationViewController.Cast(GetController());
		
		m_TotalShowUpdateTime = m_Data.GetTime() / 2;
		m_TotalHideUpdateTime = m_Data.GetTime() / 2;
		
		m_Hiding = false;
		m_Showing = false;
	}
	
	void HideNotification() 
	{
		m_Hiding = true;
		m_HideUpdateTime = 0;
	}
	
	void ShowNotification() 
	{
		if (GetExpansionClientSettings().ShowNotifications && GetExpansionClientSettings().NotificationSound )
			PlaySound();
		
		GetLayoutRoot().Show(true);
		
		m_Showing = true;
		m_ShowUpdateTime = 0;
		
		SetView();
	}

	void PlaySound()
	{
		if ( g_Game && g_Game.GetPlayer() && (!m_Data.m_LeaveJoinNotif || (m_Data.m_LeaveJoinNotif && GetExpansionClientSettings().NotificationSoundLeaveJoin) ))
		{
			if (g_Game.ConfigIsExisting("CfgSoundSets "+ GetSoundSet()))  //! Hint sound is part of Licensed
				SEffectManager.PlaySoundOnObject(GetSoundSet(), g_Game.GetPlayer(), 0, 0.15, false);
		}
	}

	string GetSoundSet()
	{
		return "Expansion_Hint_Sound_SoundSet";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionNotificationViewController;
	}
	
	void SetView()
	{
		array<string> propertiesToChange = new array<string>;
			
		if (m_Data.GetIcon())
		{
			m_NotificationViewController.NotificationIcon = m_Data.GetIcon();
			propertiesToChange.Insert("NotificationIcon");
		}
		
		if (m_Data.GetTitleText() != "")
		{
			m_NotificationViewController.NotificationTitle = m_Data.GetTitleText();
			propertiesToChange.Insert("NotificationTitle");
		}
		
		if (m_Data.GetDetailText() != "")
		{
			m_NotificationViewController.NotificationText = m_Data.GetDetailText();
			propertiesToChange.Insert("NotificationText");
		}
				
		if (m_Data.GetColor())
		{
			if (NotificationColor)
				NotificationColor.SetColor(m_Data.GetColor());
			
			if (NotificationIcon)
				NotificationIcon.SetColor(m_Data.GetColor());
		}
		
		m_NotificationViewController.NotifyPropertiesChanged(propertiesToChange);
	}

	void AnimateShowNotification(float timeslice)
	{
		m_ShowUpdateTime += timeslice;
		
		if (m_ShowUpdateTime >= m_TotalShowUpdateTime)
		{	
			m_Showing = false;
			m_Data.m_HasShown = true;
			m_Data.m_NotificationTime = g_Game.GetTickTime() + m_Data.GetTime();
		}
	}
	
	void AnimateHideNotification(float timeslice)
	{
		m_HideUpdateTime += timeslice;
		
		SetHideAlpha(timeslice);
		
		if (m_HideUpdateTime >= m_TotalHideUpdateTime)
		{
			if (GetLayoutRoot() && GetLayoutRoot().IsVisible())	
				GetLayoutRoot().Show(false);
			
			m_NotificationModule.RemoveNotificationView(this);
			RemoveNotificationElement();
			m_Hiding = false;
		}
	}

	void SetSlide(float timeslice)
	{
		float currentX;
		float currentY;
		
		GetLayoutRoot().GetPos(currentX, currentY);
	}
	
	void SetHideAlpha(float timeslice)
	{
		float currentAlpha;
		float newAlpha;
		
		if (GetLayoutRoot())
		{
			currentAlpha = GetLayoutRoot().GetAlpha();
			newAlpha = Math.Clamp(GetLayoutRoot().GetAlpha() - timeslice / m_TotalHideUpdateTime, 0, 1);
			GetLayoutRoot().SetAlpha(newAlpha);
		}
		
		if (Notification)
		{
			currentAlpha = Notification.GetAlpha();
			newAlpha = Math.Clamp(Notification.GetAlpha() - timeslice / m_TotalHideUpdateTime, 0, 1);
			Notification.SetAlpha(newAlpha);
		}
		
		if (NotificationColor)
		{
			currentAlpha = NotificationColor.GetAlpha();
			newAlpha = Math.Clamp(NotificationColor.GetAlpha() - timeslice / m_TotalShowUpdateTime, 0, 1);
			NotificationColor.SetAlpha(newAlpha);
		}
		
		if (NotificationIcon)
		{
			currentAlpha = NotificationIcon.GetAlpha();
			newAlpha = Math.Clamp(NotificationIcon.GetAlpha() - timeslice / m_TotalShowUpdateTime, 0, 1);
			NotificationIcon.SetAlpha(newAlpha);
		}
		
		if (NotificationTitle)
		{
			currentAlpha = NotificationTitle.GetAlpha();
			newAlpha = Math.Clamp(NotificationTitle.GetAlpha() - timeslice / m_TotalShowUpdateTime, 0, 1);
			NotificationTitle.SetAlpha(newAlpha);
		}
		
		if (NotificationText)
		{
			currentAlpha = NotificationText.GetAlpha();
			newAlpha = Math.Clamp(NotificationText.GetAlpha() - timeslice / m_TotalShowUpdateTime, 0, 1);
			NotificationText.SetAlpha(newAlpha);
		}
		
		if (NotificationElement)
		{
			currentAlpha = NotificationElement.GetAlpha();
			newAlpha = Math.Clamp(NotificationElement.GetAlpha() - timeslice / m_TotalShowUpdateTime, 0, 1);
			NotificationElement.SetAlpha(newAlpha);
		}
	}
	
	void RemoveNotificationElement()
	{
		if (!m_Data || !m_Data.GetType())
			return;
		
		switch (m_Data.GetType())
		{
			case ExpansionNotificationType.TOAST:
				m_NotificationHUD.RemoveNotificationToastElemement(ExpansionNotificationViewToast.Cast(this));
				break;
			case ExpansionNotificationType.BAGUETTE:
				m_NotificationHUD.RemoveNotificationBaguetteElemement(ExpansionNotificationViewBaguette.Cast(this));
				break;
			case ExpansionNotificationType.ACTIVITY:
				m_NotificationHUD.RemoveNotificationActivityElemement(ExpansionNotificationViewActivity.Cast(this));
				break;
			case ExpansionNotificationType.KILLFEED:
				m_NotificationHUD.RemoveNotificationKillfeedElemement(ExpansionNotificationViewKillfeed.Cast(this));
				break;
			case ExpansionNotificationType.MARKET:
				m_NotificationHUD.RemoveNotificationMarketElemement(ExpansionNotificationViewMarket.Cast(this));
				break;
			case ExpansionNotificationType.GARAGE:
				m_NotificationHUD.RemoveNotificationGarageElemement(ExpansionNotificationViewGarage.Cast(this));
				break;
		}
	}
		
	void AnimateNotification(float timeslice) 
	{
		if (m_Hiding)
		{
			AnimateHideNotification(timeslice);
		}
	
		if (m_Showing)
		{
			AnimateShowNotification(timeslice);
		}
	}

	NotificationRuntimeData GetNotificationData()
	{
		return m_Data;
	}
};

class ExpansionNotificationViewController: ExpansionViewController
{
	string NotificationIcon;
	string NotificationTitle;
	string NotificationText;
};

class ExpansionNotificationViewToast: ExpansionNotificationView
{
	override typename GetControllerType() 
	{
		return ExpansionNotificationViewToastController;
	}
	
	override string GetLayoutFile() 
	{		
		return "DayZExpansion/Core/GUI/layouts/expansion_notification_toast.layout";
	}
};
class ExpansionNotificationViewToastController: ExpansionNotificationViewController {};

class ExpansionNotificationViewBaguette: ExpansionNotificationView
{
	override typename GetControllerType() 
	{
		return ExpansionNotificationViewBaguetteController;
	}
	
	override string GetLayoutFile() 
	{		
		return "DayZExpansion/Core/GUI/layouts/expansion_notification_baguette.layout";
	}
};
class ExpansionNotificationViewBaguetteController: ExpansionNotificationViewController {};

class ExpansionNotificationViewActivity: ExpansionNotificationView
{
	override typename GetControllerType() 
	{
		return ExpansionNotificationViewActivityController;
	}
	
	override void PlaySound() {}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/expansion_notification_activity.layout";
	}
};
class ExpansionNotificationViewActivityController: ExpansionNotificationViewController {};

class ExpansionNotificationViewKillfeed: ExpansionNotificationView
{
	override typename GetControllerType() 
	{
		return ExpansionNotificationViewKillfeedController;
	}
	
	override void PlaySound() {}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/expansion_notification_killfeed.layout";
	}
};
class ExpansionNotificationViewKillfeedController: ExpansionNotificationViewController {};

class ExpansionNotificationViewMarket: ExpansionNotificationView
{
	override typename GetControllerType() 
	{
		return ExpansionNotificationViewMarketController;
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/Core/GUI/layouts/expansion_notification_market.layout";
	}
};
class ExpansionNotificationViewMarketController: ExpansionNotificationViewController {};

class ExpansionNotificationViewGarage: ExpansionNotificationViewMarket
{
};
#endif
