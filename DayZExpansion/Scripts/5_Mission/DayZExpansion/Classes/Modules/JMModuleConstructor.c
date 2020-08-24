/**
 * JMModuleConstructor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
		#ifdef EXPANSION_MARKET
		modules.Insert( ExpansionMarketModule );				// WIP			
		#endif		
		modules.Insert( ExpansionSafeZoneModule );
		modules.Insert( ExpansionLocatorModule );
		modules.Insert( ExpansionPartyModule );
		modules.Insert( ExpansionMarkerModule );
		modules.Insert( ExpansionMissionModule );
		modules.Insert( ExpansionTerritoryModule );
		modules.Insert( ExpansionInteriorBuildingModule );
		modules.Insert( ExpansionWorldMappingModule );
		modules.Insert( ExpansionSkinModule );
		modules.Insert( ExpansionAutorunModule );
		modules.Insert( ExpansionMonitorModule );		
		modules.Insert( ExpansionKillFeedModule );				// WIP
		
		modules.Insert( ExpansionESPModificationModule );

		modules.Insert( ExpansionCOTTerritoriesModule );
		modules.Insert( ExpansionCOTVehiclesModule ); 			// TODO: move to COT

		modules.Insert( ExpansionCOTAirdropModule );
		modules.Insert( ExpansionCOTBaseBuildingModule );
		modules.Insert( ExpansionCOTBookModule );
		modules.Insert( ExpansionCOTGeneralModule );
		modules.Insert( ExpansionCOTMapModule );
		modules.Insert( ExpansionCOTNotificationModule );
		modules.Insert( ExpansionCOTPartyModule );
		modules.Insert( ExpansionCOTRaidModule );
		modules.Insert( ExpansionCOTSpawningModule );
		modules.Insert( ExpansionCOTVehicleModule );

		modules.Insert( ExpansionGameMissionSelecterModule );
		modules.Insert( ExpansionClientSettingsModule );
		
		super.RegisterModules( modules );
	}
}