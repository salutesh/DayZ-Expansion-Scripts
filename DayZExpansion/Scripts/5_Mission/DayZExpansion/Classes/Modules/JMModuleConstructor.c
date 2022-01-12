/**
 * JMModuleConstructor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		modules.Insert( ExpansionGlobalChatModule );
		modules.Insert( ExpansionNotificationModule );	
		modules.Insert( ExpansionLocatorModule );
		modules.Insert( ExpansionPartyModule );
		modules.Insert( ExpansionMarkerModule );
		modules.Insert( ExpansionTerritoryModule );
		modules.Insert( ExpansionInteriorBuildingModule );
		modules.Insert( ExpansionWorldMappingModule );
		modules.Insert( ExpansionAutorunModule );
		modules.Insert( ExpansionMonitorModule );		
		modules.Insert( ExpansionKillFeedModule );
		
#ifdef JM_COT
		modules.Insert( ExpansionCOTTerritoriesModule );

		modules.Insert( ExpansionESPModificationModule );
		modules.Insert( ExpansionCOTVehiclesModule ); //!TODO: Move to COT
#endif

		modules.Insert( ExpansionClientSettingsModule );
		modules.Insert( ExpansionRespawnHandlerModule );
		
		super.RegisterModules( modules );
	}
}