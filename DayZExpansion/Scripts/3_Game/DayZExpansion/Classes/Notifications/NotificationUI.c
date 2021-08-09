/**
 * ExpansionNotificationUI.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class NotificationUI
{
	void NotificationUI()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::NotificationUI - Start");
		#endif
		
		NotificationSystem.UnbindOnAdd( AddNotification );
		NotificationSystem.UnbindOnRemove( RemoveNotification );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::NotificationUI - End");
		#endif
	}
};