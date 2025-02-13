/**
 * ExpansionAIMissionMeta.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionAIMissionMeta
 * @brief		This class handle ai missions meta
 **/
class ExpansionAIMissionMeta
{
	ref ExpansionSettingNotificationData NotificationStart;
	ref ExpansionSettingNotificationData NotificationAction;
	ref ExpansionSettingNotificationData NotificationEnd;

	ref ExpansionSettingMarkerData Marker;
	
	// ------------------------------------------------------------
	// Expansion ExpansionAIMissionMeta
	// ------------------------------------------------------------
	void ExpansionAIMissionMeta(ExpansionSettingNotificationData notifstart, ExpansionSettingNotificationData notifaction, ExpansionSettingNotificationData notifend, ExpansionSettingMarkerData marker)
	{
		NotificationStart 	= notifstart;
		NotificationAction 	= notifaction;
		NotificationEnd 	= notifend;
		Marker 				= marker;
	}
};
