/**
 * Construction.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Construction
{
	//============================================
	// IsColliding
	//============================================
	override bool IsColliding( string part_name )
	{
		if ( GetExpansionSettings().GetBaseBuilding().CanBuildAnywhere )
		{
			return false;
		}
		else
		{
			return super.IsColliding(part_name);
		}
	}
	override bool CanBuildPart( string part_name, ItemBase tool, bool use_tool )
	{
		if ( tool.GetType() == "ExpansionAdminHammer" )
		{
			if ( !IsPartConstructed( part_name ) && HasRequiredPart( part_name ) && !HasConflictPart( part_name ) )
			{
				return true;
			}
		}

		return super.CanBuildPart( part_name, tool, use_tool );
	}
	
	override protected void UpdateConstructionParts()
	{
		super.UpdateConstructionParts();

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Construction::UpdateConstructionParts - Start");
		#endif

		string construction_path = "cfgVehicles" + " " + GetParent().GetType() + " " + "Construction";

		if ( GetGame().ConfigIsExisting( construction_path ) )
		{
			//main parts
			for ( int i = 0; i < GetGame().ConfigGetChildrenCount( construction_path ); ++i )
			{
				string main_part_name;
				GetGame().ConfigGetChildName( construction_path, i, main_part_name );
				string part_path = construction_path + " " + main_part_name;
				
				//parts
				for ( int j = 0; j < GetGame().ConfigGetChildrenCount( part_path ); ++j )
				{
					string part_name;
					GetGame().ConfigGetChildName( part_path, j, part_name );

					ConstructionPart part;
					if (  m_ConstructionParts.Find( part_name, part ) )
					{
						string snapping_show_path = part_path + " " + part_name + " snapping_show";
						if ( GetGame().ConfigIsExisting( snapping_show_path ) )
						{
							GetGame().ConfigGetIntArray( snapping_show_path, part.m_SnappingShow );
						}
						
						part.m_Order = GetGame().ConfigGetInt( part_path + " " + part_name + " order" );
						#ifdef EXPANSIONEXLOGPRINT
						EXLogPrint("Construction::UpdateConstructionParts " + part.GetName() + " part.m_Order : " + part.m_Order);
						#endif
					}
				}
			}
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Construction::UpdateConstructionParts - End");
		#endif
	}

	bool IsPartBuiltForSnapPoint( int idx )
	{
		for ( int i = 0; i < m_ConstructionParts.Count(); i++ )
		{
			string key = m_ConstructionParts.GetKey( i );
			ConstructionPart value = m_ConstructionParts.Get( key );
			if ( value.IsBuilt() && value.m_SnappingShow.Find( idx ) > -1 )
				return true;
		}

		return false;
	}
	
	//Get all construction parts that can be build (at that current time)
	override void GetConstructionPartsToBuild( string main_part_name, out array<ConstructionPart> construction_parts, ItemBase tool, out string real_constructionTarget, bool use_tool )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Construction::GetConstructionPartsToBuild - Start");
		#endif
		
		super.GetConstructionPartsToBuild( main_part_name, construction_parts, tool, real_constructionTarget, use_tool );
		
		//Descending order
		for ( int j = 0; j < construction_parts.Count(); ++j )
		{
			for ( int k = 0; k < construction_parts.Count(); ++k )
			{
				if ( construction_parts[k].m_Order < construction_parts[j].m_Order )
				{
					construction_parts.SwapItems(j,k);
				}  
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Construction::GetConstructionPartsToBuild - End");
		#endif
	}
};