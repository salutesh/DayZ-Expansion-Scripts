/**
 * BuildingBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class BuildingBase
{
	//Array of weak references is needed
	static autoptr array<BuildingBase> m_AllBuldingsInteriors = new array<BuildingBase>;
	
	protected ExpansionInteriorBuildingModule m_InteriorModule;
	
	private bool m_Loading;

	private bool m_IsIvyBuilding;
	
	void BuildingBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::BuildingBase - Start");
		#endif

		m_Loading = false;
		
		if ( GetGame() && HasInterior() )
		{
			m_AllBuldingsInteriors.Insert( this );
			
			//Load mapping here for runtime
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.LoadInterior, 8000);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::BuildingBase - End");
		#endif
	}
	
	void ~BuildingBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::~BuildingBase - Start");
		#endif

		RemoveFromInteriorsBuildings();

		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::~BuildingBase - End");
		#endif
	}
	
	void RemoveFromInteriorsBuildings()
	{
		if ( GetGame() && HasInterior() )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.LoadInterior);
			
			int remove_index = m_AllBuldingsInteriors.Find( this );
			if ( remove_index >= 0 )
			{
				m_AllBuldingsInteriors.Remove( remove_index );
			}
		}
	}
	
	private void ConvertTransformToWorld( vector posms, vector orims, out vector posws, out vector oriws )
	{
		Transform snapTrans = Transform.GetObject( this );
		Transform localTrans = Transform.YawPitchRoll( orims, posms );
		Transform worldTrans = new Transform();

		Math3D.MatrixMultiply4( snapTrans.data, localTrans.data, worldTrans.data );
		worldTrans.UpdateUnion();

		posws = worldTrans.GetOrigin();
		oriws = worldTrans.GetYawPitchRoll();
	}

	private void FixObjectCollision( Object obj )
	{
		vector roll = obj.GetOrientation();
		roll[2] = roll[2] - 1;
		obj.SetOrientation( roll );
		roll[2] = roll[2] + 1;
		obj.SetOrientation( roll );
	}

	protected Object SpawnInteriorIvy( string type, vector position, vector orientation )
	{
		if ( !m_IsIvyBuilding )
			return NULL;

		ConvertTransformToWorld( position, orientation, position, orientation );

		Object obj = GetGame().CreateObject_WIP( type, position, ECE_CREATELOCAL );
		if ( !obj )
			return NULL;
		
		//Tell engine it will represent static object
		obj.SetFlags( EntityFlags.STATIC, false );

		obj.SetPosition( position );
		obj.SetOrientation( orientation );

		FixObjectCollision( obj );
		
		Entity ent;
		if ( Class.CastTo( ent, obj ) )
		{
			ent.DisableSimulation( true );
		}

		m_InteriorModule.m_InteriorObjects.Insert( obj );

		return obj;
	}
	
	protected Object SpawnInteriorObject( string type, vector position, vector orientation, float random = 1.0 )
	{
		if ( random == -1 )
		{
			random = Math.RandomFloatInclusive( 0, 1 );
		}
		
		if ( Math.RandomFloatInclusive( 0, 1 ) <= random )
		{
			bool hasCollision = false;
			
			ConvertTransformToWorld( position, orientation, position, orientation );

			Object obj;
			if ( !m_InteriorModule.m_CachedCollision.Find( type, hasCollision ) )
			{
				obj = GetGame().CreateObject_WIP( type, position, ECE_CREATEPHYSICS | ECE_CREATELOCAL );
				if ( !obj )
				{
					m_InteriorModule.m_CachedCollision.Insert( type, false );
					return NULL;
				}
				
				vector minMax[2];
				hasCollision = obj.GetCollisionBox( minMax );
				
				m_InteriorModule.m_CachedCollision.Insert( type, hasCollision );
				
				GetGame().ObjectDelete( obj );
			}

			//Only spawn object with collision at server side, and object without collision at client side 
			if ( hasCollision )
			{
				if ( GetGame().IsClient() )
					return NULL;
			} else
			{
				if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
					return NULL;
			}
			
			#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::SpawnInteriorObject spawning " + type + " for " + GetType() + " at " + position);
		#endif

			// don't use CreateObject, it does a bunch of unnecessary stuff
			int flags = ECE_CREATEPHYSICS;
			if ( IsMissionClient() )
				flags |= ECE_CREATELOCAL; // create_local
			
			obj = GetGame().CreateObject_WIP( type, position, flags );
			if ( !obj )
				return NULL;
			
			//Tell engine it will represent static object
			obj.SetFlags( EntityFlags.STATIC, false );

			obj.SetPosition( position );
			obj.SetOrientation( orientation );

			FixObjectCollision( obj );
		
			if ( obj.CanAffectPathgraph() )
			{
				obj.SetAffectPathgraph( true, false );
			}

			Entity ent = Entity.Cast( obj );
			if ( ent )
			{
				EntityAI entityAI = EntityAI.Cast( obj );
				if ( entityAI )
				{
					//Make it not CE saved	
					if (IsMissionHost()) entityAI.SetLifetime(1.0);
				}
				
				ItemBase item = ItemBase.Cast( ent );
				if ( item )
				{
					//Make it not takeable
					item.SetTakeable( false );
				}
			
				ent.DisableSimulation( true );
			}

			m_InteriorModule.m_InteriorObjects.Insert( obj );

			return obj;
		}
		
		return NULL;
	}
	
	void LoadInterior()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("BuildingBase::LoadInterior - Start");
		#endif

		if ( !m_InteriorModule )
		{
			Class.CastTo( m_InteriorModule, GetModuleManager().GetModule( ExpansionInteriorBuildingModule ) );
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.LoadInterior);
		
		if ( m_InteriorModule && !g_Game.IsLoading() && GetExpansionSettings() && GetExpansionSettings().GetGeneral().Mapping.InteriorBuilding )
		{
			if ( !m_InteriorModule.AlreadySpawned( GetType(), GetPosition() ) )
			{
				m_InteriorModule.AddBuildingSpawned( GetType(), GetPosition() );
			
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("BuildingBase::LoadInterior - reload it");
				#endif
				
				if ( GetExpansionSettings().GetGeneral().Mapping.Ivies )
				{
					m_IsIvyBuilding = false;
		
					if ( IsMissionClient() )
					{
						for ( int i = 0; i < m_InteriorModule.m_WhereIviesObjectsSpawn.Count(); i++ )
						{
							if ( vector.Distance( m_InteriorModule.m_WhereIviesObjectsSpawn[i].position, this.GetPosition() ) <= m_InteriorModule.m_WhereIviesObjectsSpawn[i].radius )
							{
								m_IsIvyBuilding = true;
								break;
							}
						}
					}
				} else
				{
					m_IsIvyBuilding = false;
				}
				
				SpawnInterior();
		
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, this );
				
				m_InteriorModule.SaveCachedCollisions();
			} else
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("BuildingBase::LoadInterior - Dont reload it");
				#endif
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("BuildingBase::LoadInterior - End");
		#endif
	}
	
	bool HasInterior()
	{
		return false;
	}

	protected void SpawnInterior()
	{
	}

	/*void RemoveInterior()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("BuildingBase::RemoveInterior - Start GetPosition() : " + GetPosition());
		#endif
		if ( GetGame() )
		{
			for ( int i = 0; i < m_InteriorObjects.Count(); ++i )
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().ObjectDelete, m_InteriorObjects[i]);
			}
			
			BuildingBaseSpawned.AllSpawned.RemoveItem(GetPosition());
		}
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("BuildingBase::RemoveInterior - End GetPosition() : " + GetPosition());
		#endif
	}*/
}
