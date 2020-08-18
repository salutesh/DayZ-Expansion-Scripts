class ExpansionSkinObjectLambda : TurnItemIntoItemLambda
{
	private vector m_OldPos;
	private vector m_OldOri;

	private string m_Skin;
	private ExpansionSkinModule m_Module;

	void ExpansionSkinObjectLambda( EntityAI old_item, string new_item_type, PlayerBase player, ref ExpansionSkinModule module, string skin )
	{
		m_OldItem = old_item;
		m_NewItemType = new_item_type;
		m_Module = module;
		m_Skin = skin;

		m_OldPos = old_item.GetPosition();
		m_OldOri = old_item.GetOrientation();
	}
	
	protected override bool CanExecuteLambda()
	{
		return true;
	}

	void CopyCarFluid( CarFluid fluid, Car old_car, Car new_car )
	{
		new_car.LeakAll( fluid );
		new_car.Fill( fluid, old_car.GetFluidFraction( fluid ) * old_car.GetFluidCapacity( fluid ) );
	}

	override void CopyOldPropertiesToNew( notnull EntityAI old_item, EntityAI new_item )
	{		
		if ( new_item )
		{
			TransferInventory( old_item, new_item );

			new_item.SetPosition( m_OldPos );
			new_item.SetOrientation( m_OldOri );

			int skinIndex = m_Module.GetSkinIndex( m_NewItemType, m_Skin );
			
			#ifdef EXPANSION_SKIN_LOGGING
			Print( skinIndex );
			#endif

			if ( skinIndex >= 0 )
			{
				ItemBase item;
				if ( Class.CastTo( item, new_item ) )
					item.ExpansionSetSkin( skinIndex );

				CarScript car;
				if ( Class.CastTo( car, new_item ) )
				{
					car.ExpansionSetSkin( skinIndex );

					Car old_car;
					if ( Class.CastTo( old_car, old_item ) )
					{
						CopyCarFluid( CarFluid.FUEL, old_car, car );
						CopyCarFluid( CarFluid.OIL, old_car, car );
						CopyCarFluid( CarFluid.BRAKE, old_car, car );
						CopyCarFluid( CarFluid.COOLANT, old_car, car );
					}
				}

				#ifdef EXPANSION_SKIN_LOGGING
				Print( item );
				Print( car );
				#endif
			}

			ItemBase old_item_ib = ItemBase.Cast( old_item );
			ItemBase new_item_ib = ItemBase.Cast( new_item );

			if ( new_item_ib && old_item_ib )
			{
				new_item_ib.TransferAgents( old_item_ib.GetAgents() );
				MiscGameplayFunctions.TransferItemVariables( ItemBase.Cast(old_item), ItemBase.Cast(new_item), false );
			}

			if ( !GetGame().IsClient() )
			{
				new_item.SetHealth( "", "", old_item.GetHealth01( "", "" ) * new_item.GetMaxHealth( "", "" ) );
			}
		} else
		{
			Print( "Error!" );
		}
	}

	TransferInventoryResult TransferInventory( EntityAI old_item, EntityAI new_item )
	{
		TransferInventoryResult result = TransferInventoryResult.Ok;

		array<EntityAI> children = new array<EntityAI>;
		old_item.GetInventory().EnumerateInventory( InventoryTraversalType.LEVELORDER, children );
		int count = children.Count();
		for ( int i = 0; i < count; ++i )
		{
			EntityAI child = children[i];
			if ( child )
			{
				InventoryLocation child_src = new InventoryLocation;
				child.GetInventory().GetCurrentInventoryLocation( child_src );
				
				InventoryLocation child_dst = new InventoryLocation;
				child_dst.Copy( child_src );
				child_dst.SetParent( new_item );

				bool drop = false;

				if ( GameInventory.LocationCanAddEntity( child_dst ) )
				{
					new_item.LocalTakeToDst( child_src, child_dst );
				} else
				{
					drop = true;
				}

				if ( drop )
				{
					new_item.LocalDropEntity( child );
					GetGame().RemoteObjectTreeCreate( child );
					result = TransferInventoryResult.DroppedSome;
				}
			}
		}
		return result;
	}
}