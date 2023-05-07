class PluginCleaning extends PluginRepairing 
{
	void PluginCleaning()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}
	
	override bool CanRepair( ItemBase repair_kit, Object item, string damage_zone = "" )
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		int state = item.GetHealthLevel(damage_zone);
		
		if ( state != GameConstants.STATE_RUINED && (item.CanBeRepairedToPristine() && state >= GameConstants.STATE_WORN) || (!item.CanBeRepairedToPristine() && state >= GameConstants.STATE_DAMAGED ) )
		{
			int repair_kit_type = repair_kit.ConfigGetInt( "repairKitType" );
			
			PluginDebugPrint("::CanRepair - Kit type: " + repair_kit_type);
			
			array<int> repairable_with_types = new array<int>;
			item.ConfigGetIntArray( "repairableWithKits", repairable_with_types );	
			
			for ( int i = 0; i < repairable_with_types.Count(); i++ )
			{
				int repairable_with_type = repairable_with_types.Get(i);
				
				if ( ExIsRepairValid( repair_kit_type, repairable_with_type ) )
				{
					return true;
				}
			}
		}
		return false;
		
	}
	
	private bool ExIsRepairValid(int repair_kit_type, int repairable_with_type)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (repair_kit_type > 0 && repairable_with_type > 0)
		{
			PluginDebugPrint("::ExIsRepairValid - Kit type: " + repair_kit_type);
			PluginDebugPrint("::ExIsRepairValid - Repairable with kit: " + repairable_with_type);
			
			return repair_kit_type == repairable_with_type;
		}
		
		return false;
	}
	
	void PluginDebugPrint(string text)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	}
};