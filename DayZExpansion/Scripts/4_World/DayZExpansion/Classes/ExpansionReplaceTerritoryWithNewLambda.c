#ifdef DAYZ_1_09
class ExpansionReplaceTerritoryWithNewLambda : TurnItemIntoItemLambda
{
	private vector pos;
	private vector ori;

	void ExpansionReplaceTerritoryWithNewLambda( EntityAI old_item, string new_item_type, PlayerBase player ) {}

	override void CopyOldPropertiesToNew( notnull EntityAI old_item, EntityAI new_item )
	{
		super.CopyOldPropertiesToNew( old_item, new_item );

		ref ExpansionTerritory territory;
		int id;
		string ownerid;
		string flagtexture;

		ExpansionFlagBase old_flag_base;
		if ( Class.CastTo( old_flag_base, old_item ) )
		{
			flagtexture = old_flag_base.GetFlagTexturePath();
			ownerid = old_flag_base.GetOwnerID();
		}

		ExpansionTerritoryFlag old_flag;
		if ( Class.CastTo( old_flag, old_item ) )
		{
			territory = old_flag.GetTerritory();
			id = old_flag.GetTerritoryID();
		}

		TerritoryFlag new_flag;
		if ( Class.CastTo( new_flag, new_item ) )
		{
			if ( old_flag_base.IsTerritoryFlag() )
			{
				new_flag.SetTerritory( territory );
				new_flag.SetTerritoryID( id );
			}
			new_flag.SetOwnerID( ownerid );
			new_flag.SetFlagTexture( flagtexture );
		}

		pos = old_item.GetPosition();
		ori = old_item.GetOrientation();
	}
	
	override protected void OnSuccess( EntityAI new_item ) 
	{
		super.OnSuccess( new_item );

		TerritoryFlag flag;
		if ( Class.CastTo( flag, new_item ) )
		{
			flag.GetConstruction().COT_BuildPart( "base", false );				
			flag.GetConstruction().COT_BuildPart( "support", false );				
			flag.GetConstruction().COT_BuildPart( "pole", false );
			flag.GetInventory().CreateAttachment( "Flag_DayZ" );
			
			flag.SetIsExpansionTerritoryFlag( true );

			flag.SetPosition( pos );
			flag.SetOrientation( ori );
		}
	}
};
#endif