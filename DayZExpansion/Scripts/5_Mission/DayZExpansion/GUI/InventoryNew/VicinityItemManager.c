modded class VicinityItemManager
{
	bool IsObstructed (Object filtered_object)
	{
			if ( filtered_object.IsInherited(ExpansionBaseBuilding) )
			{
				return false;
			}
			return super.IsObstructed(filtered_object);
	}
};