/**
 * BuildingBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class BuildingBase
{
	//Array of weak references is needed
	static autoptr array<BuildingBase> m_AllBuldingsInteriors = new array<BuildingBase>;
	
	protected static ExpansionInteriorBuildingModule s_InteriorModule;
	static int s_Expansion_LoadCustomObjectsDelay;
	
	autoptr array<Object> m_InteriorObjects = {};
	autoptr array<Object> m_IvyObjects = {};
	
	private bool m_InteriorsLoaded;
	private bool m_IvysLoaded;
	
	// ------------------------------------------------------------
	// BuildingBase Constructor
	// ------------------------------------------------------------
	void BuildingBase()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this, GetType());
#endif

		ExpansionSettings.SI_General.Insert( OnSettingsUpdated );

#ifdef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(OnSettingsUpdated);
#endif
	}
		
	// ------------------------------------------------------------
	// BuildingBase Desturctor
	// ------------------------------------------------------------
	void ~BuildingBase()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this, GetType());
#endif

		ExpansionSettings.SI_General.Remove( OnSettingsUpdated );
	}
	
	// ------------------------------------------------------------
	// OnSettingsUpdated
	// ------------------------------------------------------------
	void OnSettingsUpdated()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif
		if (GetGame().IsDedicatedServer())
			SetGodMode();

		if (!HasInterior() && !HasIvys())
			return;

		if (ExpansionWorldObjectsModule.s_RemovedObjects[this])
			return;

		auto mapping = GetExpansionSettings().GetGeneral().Mapping;

		bool loadInterior = HasInterior() && mapping.BuildingInteriors && ExpansionStatic.IsAnyOf(this, mapping.Interiors, false);
		bool loadIvys = HasIvys() && mapping.BuildingIvys;

		if (!loadInterior && !loadIvys)
			return;

		if (GetGame().IsDedicatedServer())
		{
			LoadCustomObjects(loadInterior, loadIvys);
		}
		else
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LoadCustomObjects, s_Expansion_LoadCustomObjectsDelay, false, loadInterior, loadIvys);

			//! @note buildings are initialized map-wide even on client (unlike other entities)
			//! Load objects every 5 ms to avoid bogging down client
			s_Expansion_LoadCustomObjectsDelay += 5;
			if (ExpansionMath.Cmp(s_Expansion_LoadCustomObjectsDelay, 0) == -1)
				s_Expansion_LoadCustomObjectsDelay = 1000;
		}
	}
	
	// ------------------------------------------------------------
	// BuildingBase LoadCustomObjects
	// ------------------------------------------------------------
	void LoadCustomObjects(bool loadInterior, bool loadIvys)
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif
		
		if (loadInterior)
			LoadInterior();
	
		if (loadIvys)
			LoadIvys();
	}
	
	// ------------------------------------------------------------
	// BuildingBase ConvertTransformToWorld
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// BuildingBase SpawnInteriorIvy
	// ------------------------------------------------------------
	protected Object SpawnInteriorIvy( string type, vector position, vector orientation )
	{
		ConvertTransformToWorld( position, orientation, position, orientation );

		Object obj = GetGame().CreateObjectEx( type, position, ECE_LOCAL );
		if ( !obj )
			return NULL;
		
		//Tell engine it will represent static object
		obj.SetFlags( EntityFlags.STATIC, false );
		obj.SetOrientation( orientation );
		obj.Update();
		
		Entity ent;
		if (Class.CastTo(ent, obj)) {
			ent.DisableSimulation(true);
		}

		m_IvyObjects.Insert( obj );

		return obj;
	}
		
	// ------------------------------------------------------------
	// BuildingBase SpawnInteriorObject
	// ------------------------------------------------------------
	protected Object SpawnInteriorObject( string type, vector position, vector orientation, float random = 1.0 )
	{
		#ifdef EXPANSION_INTERIOR_RANDONMIZATION
		if ( random == -1 )
			random = Math.RandomFloatInclusive( 0, 1 );
		
		if ( !( Math.RandomFloatInclusive( 0, 1 ) <= random ) )
			return NULL;
		#endif

		bool hasCollision = false;
		
		ConvertTransformToWorld( position, orientation, position, orientation );

		Object obj;
		if ( !s_InteriorModule.m_CachedCollision.Find( type, hasCollision ) )
		{
			obj = GetGame().CreateObjectEx( type, position, ECE_CREATEPHYSICS | ECE_LOCAL );
			if ( !obj )
			{
				s_InteriorModule.m_CachedCollision.Insert( type, false );
				return NULL;
			}
			
			vector minMax[2];
			hasCollision = obj.GetCollisionBox( minMax );
			
			s_InteriorModule.m_CachedCollision.Insert( type, hasCollision );
			
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
		
		// don't use CreateObject, it does a bunch of unnecessary stuff
		int flags = ECE_CREATEPHYSICS | ECE_NOLIFETIME;
		if ( IsMissionClient() )
			flags |= ECE_LOCAL; // create_local
		
		obj = GetGame().CreateObjectEx( type, position, flags );
		if ( !obj )
			return NULL;
		
		//Tell engine it will represent static object
		obj.SetFlags( EntityFlags.STATIC, false );
		obj.SetOrientation( orientation );
		obj.Update();
		
		if (GetGame().IsServer())
		{
			obj.SetAffectPathgraph( true, false );
			if ( obj.CanAffectPathgraph() )
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
		}

		Entity ent = Entity.Cast(obj);
		if (ent)
		{
			ItemBase item = ItemBase.Cast( ent );
			if (item)
				item.SetTakeable(false);
		
			ent.DisableSimulation(true);
		}

		m_InteriorObjects.Insert(obj);

		return obj;
	}
			
	// ------------------------------------------------------------
	// BuildingBase LoadInterior
	// ------------------------------------------------------------
	void LoadInterior()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif

		if (m_InteriorObjects.Count())
		{
			int deleted;
			foreach (auto obj: m_InteriorObjects)
			{
				if (!obj)
					deleted++;
			}
			//! The game will automatically delete the objects on mission end (disconnect/quit)
			//! This condition will always be met on client reconnect
			if (deleted == m_InteriorObjects.Count())
				m_InteriorObjects.Clear();
			//! This condition should never be met
			else if (!deleted)
				return;
			//! This `else` just exists as a safety
			else
				UnloadInterior();
		}

		if ( !s_InteriorModule ) {
			CF_Modules<ExpansionInteriorBuildingModule>.Get(s_InteriorModule);
		}
		
		SpawnInterior();
		m_InteriorsLoaded = true;

		if (GetGame().IsDedicatedServer())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, this);
	}
	
	// ------------------------------------------------------------
	// BuildingBase LoadIvys
	// ------------------------------------------------------------	
	void LoadIvys()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif
		
		if (!IsMissionClient()) {
			return;
		}

		if (m_IvyObjects.Count())
		{
			int deleted;
			foreach (auto obj: m_IvyObjects)
			{
				if (!obj)
					deleted++;
			}
			//! The game will automatically delete the objects on mission end (disconnect/quit)
			//! This condition will always be met on client reconnect
			if (deleted == m_IvyObjects.Count())
				m_IvyObjects.Clear();
			//! This condition should never be met
			else if (!deleted)
				return;
			//! This `else` just exists as a safety
			else
				UnloadIvys();
		}
				
		if (!s_InteriorModule) {
			CF_Modules<ExpansionInteriorBuildingModule>.Get(s_InteriorModule);
		}
		
		if (s_InteriorModule.ShouldIvySpawn(GetPosition())) {
			SpawnIvys();
		}
		
		m_IvysLoaded = true;
	}
	
	// ------------------------------------------------------------
	// BuildingBase UnloadInterior
	// ------------------------------------------------------------		
	void UnloadInterior()
	{	
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif
		
		if (!m_InteriorsLoaded) {
			return;
		}		
		
		if (!m_InteriorObjects || m_InteriorObjects.Count() == 0) {
			return;	
		}
		
		foreach (Object int_obj: m_InteriorObjects)
		{
			if (int_obj)
				GetGame().ObjectDelete(int_obj);
		}
		
		
		m_InteriorObjects.Clear();
		m_InteriorsLoaded = false;
	}
	
	// ------------------------------------------------------------
	// BuildingBase UnloadIvys
	// ------------------------------------------------------------		
	void UnloadIvys()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);
#endif
		
		if (!m_IvysLoaded) {
			return;
		}
		
		if (!m_IvyObjects || m_IvyObjects.Count() == 0) {
			return;	
		}
		
		foreach (Object ivy_obj: m_IvyObjects) {
			GetGame().ObjectDelete(ivy_obj);
		}
		
		m_IvyObjects.Clear();
		m_IvysLoaded = false;
	}
	
	// ------------------------------------------------------------
	// BuildingBase HasInterior
	// ------------------------------------------------------------	
	bool HasInterior()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// BuildingBase HasIvys
	// ------------------------------------------------------------	
	bool HasIvys()
	{
		return false;
	}

	protected void SpawnInterior();
	protected void SpawnIvys();

	void SetGodMode()
	{
		if (GetExpansionSettings().GetGeneral().DisableShootToUnlock)
			SetAllowDamage(false);
	}

	// ------------------------------------------------------------
	// BuildingBase RemoveInterior
	// ------------------------------------------------------------
	/*
	void RemoveInterior()
	{
		if ( GetGame() )
		{
			for ( int i = 0; i < m_InteriorObjects.Count(); ++i )
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().ObjectDelete, m_InteriorObjects[i]);
			}
			
			BuildingBaseSpawned.AllSpawned.RemoveItem(GetPosition());
		}
	}
	*/
};

