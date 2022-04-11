/**
 * JMModuleConstructor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class JMModuleConstructor
{
	override void RegisterModules( out TTypenameArray modules )
	{
		// Gameplay modules
		modules.Insert( ExpansionNotificationModule );
		modules.Insert( ExpansionSkinModule );
		modules.Insert( ExpansionZoneModule );
		modules.Insert( ExpansionItemBaseModule );
#ifdef EXPANSIONMONITORMODULE
		modules.Insert( ExpansionMonitorModule );
#endif
		
		modules.Insert( ExpansionClientSettingsModule );
		
#ifdef JM_COT
#ifdef EXPANSIONCOTESPMODULE
		modules.Insert( ExpansionESPModificationModule );
#endif
#endif
		
		super.RegisterModules( modules );
	}
}
