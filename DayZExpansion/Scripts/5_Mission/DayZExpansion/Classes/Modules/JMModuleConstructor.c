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
		// modules.Insert( ExpansionMarketModule );			
		#endif		
		modules.Insert( ExpansionSafeZoneModule );
		modules.Insert( ExpansionLocatorModule );
		modules.Insert( ExpansionPartyModule );
		modules.Insert( ExpansionMissionModule ); 
		modules.Insert( Expansion3DMarkerModule );
		modules.Insert( ExpansionMapMarkerModule );
		modules.Insert( ExpansionInteriorBuildingModule );
		modules.Insert( ExpansionTerritoryModule );
		modules.Insert( ExpansionWorldMappingModule );
		modules.Insert( ExpansionSkinModule );						
		modules.Insert( ExpansionAutorunModule );
		modules.Insert( ExpansionMonitorModule );		
		
		// COT Modules
		// modules.Insert( ExpansionGameMissionSelecterModule );
		// modules.Insert( ExpansionAnimalAITestModule );			
		// modules.Insert( ExpansionAITestModule );					
		// modules.Insert( ExpansionMarketCOTModule );					
		// modules.Insert( ExpansionMissionCOTModule );	

		// modules.Insert( ExpansionCOTBookModule );
		modules.Insert( ExpansionCOTGeneralModule );
	   	// modules.Insert( ExpansionCOTMapModule );
		// modules.Insert( ExpansionCOTPartyModule );
		// modules.Insert( ExpansionCOTSafeZoneModule );
		// modules.Insert( ExpansionCOTTerritoriesModule );
		modules.Insert( ExpansionCOTVehiclesModule ); // TODO: move to COT
		
		super.RegisterModules( modules );
	}
}