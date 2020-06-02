class ExpansionSkinCarDoorFromObjectLambda : ReplaceItemWithNewLambdaBase
{
	private string m_Skin;
    private ExpansionSkinModule m_Module;

	void ExpansionSkinCarDoorFromObjectLambda( EntityAI old_item, string new_item_type, ref ExpansionSkinModule module, string skin )
	{
		m_OldItem = old_item;
		m_NewItemType = new_item_type;
        m_Module = module;
		m_Skin = skin;
	}
	
	override void OnSuccess( EntityAI new_item )
	{
		super.OnSuccess( new_item );

		if ( m_Skin == "" )
			return;
		
		CarDoor item;
		Class.CastTo( item, new_item );

		int skinIndex = m_Module.GetSkinIndex( m_NewItemType, m_Skin );
		item.ExpansionSetSkin( skinIndex );
	}
}