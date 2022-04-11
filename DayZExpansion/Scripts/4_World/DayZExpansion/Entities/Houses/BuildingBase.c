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
	
	protected ExpansionInteriorBuildingModule m_InteriorModule;
	
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
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, this, "BuildingBase");
#endif
		
		ExpansionSettings.SI_General.Insert( OnSettingsUpdated );
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ReloadCustomObjects, 8000, false);
		
		if( GetExpansionSettings().GetGeneral().DisableShootToUnlock )
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetGodMode, 8000, false);
	}
		
	// ------------------------------------------------------------
	// BuildingBase Desturctor
	// ------------------------------------------------------------
	void ~BuildingBase()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, this, "~BuildingBase");
#endif

		ExpansionSettings.SI_General.Remove( OnSettingsUpdated );
		//UnloadInterior();
		//UnloadIvys();
	}
	
	// ------------------------------------------------------------
	// IsBuildingListed
	// ------------------------------------------------------------
	bool IsBuildingListed()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, this, "IsBuildingListed");
#endif
		
		for ( int x = 0; x < GetExpansionSettings().GetGeneral().Mapping.Interiors.Count(); ++x )
		{
			if ( this.IsKindOf( GetExpansionSettings().GetGeneral().Mapping.Interiors[x] ) )
			{
				return true;
			}
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// OnSettingsUpdated
	// ------------------------------------------------------------
	void OnSettingsUpdated()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, this, "OnSettingsUpdated");
#endif
		
		if ( IsMissionHost() )
		{
			if ( HasInterior() ) {
				if ( GetExpansionSettings().GetGeneral().Mapping.BuildingInteriors && IsBuildingListed() )
					LoadInterior();
				else 
					UnloadInterior();
			}
		
			if ( HasIvys() ) {
				if ( GetExpansionSettings().GetGeneral().Mapping.BuildingIvys )
					LoadIvys();	
				else
					UnloadIvys();
			}
		}
	}
	
	// ------------------------------------------------------------
	// BuildingBase ReloadCustomObjects
	// ------------------------------------------------------------
	void ReloadCustomObjects()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, this, "ReloadCustomObjects");
#endif
		
		if ( HasInterior() ) {
			if ( GetExpansionSettings().GetGeneral().Mapping.BuildingInteriors && IsBuildingListed() )
				LoadInterior();
			else 
				UnloadInterior();
		}
	
		if ( HasIvys() ) {
			if ( GetExpansionSettings().GetGeneral().Mapping.BuildingIvys )
				LoadIvys();	
			else
				UnloadIvys();
		}
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
	// BuildingBase FixObjectCollision
	// ------------------------------------------------------------
	private void FixObjectCollision( Object obj )
	{
		vector roll = obj.GetOrientation();
		roll[2] = roll[2] - 1;
		obj.SetOrientation( roll );
		roll[2] = roll[2] + 1;
		obj.SetOrientation( roll );
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
		obj.SetPosition( position );
		obj.SetOrientation( orientation );

		FixObjectCollision( obj );
		
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
		if ( !m_InteriorModule.m_CachedCollision.Find( type, hasCollision ) )
		{
			obj = GetGame().CreateObjectEx( type, position, ECE_CREATEPHYSICS | ECE_LOCAL );
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
		
		// don't use CreateObject, it does a bunch of unnecessary stuff
		int flags = ECE_CREATEPHYSICS;
		if ( IsMissionClient() )
			flags |= ECE_LOCAL; // create_local
		
		obj = GetGame().CreateObjectEx( type, position, flags );
		if ( !obj )
			return NULL;
		
		//Tell engine it will represent static object
		//obj.SetFlags( EntityFlags.STATIC, false );
		obj.SetPosition( position );
		obj.SetOrientation( orientation );

		FixObjectCollision( obj );
		
		if ( obj.CanAffectPathgraph() ) {
			obj.SetAffectPathgraph( true, false );
		}

		Entity ent = Entity.Cast(obj);
		if (ent)
		{			
			ItemBase item = ItemBase.Cast( ent );
			EntityAI ent_ai;
			if (item)
			{
				// TODO: Do we actually need this, i.e. is there anything that we spawn that inherits from ItemBase?

				item.SetTakeable(false);
				
				// Make it not CE saved (would this actually work as intended?)
				if ( IsMissionHost() ) 
					item.SetLifetimeMax(1.0);
			} else if ( Class.CastTo( ent_ai, obj ) )
			{
				// Set lifetime to 45 days, otherwise it would default to roughly half an hour
				if ( IsMissionHost() )
					ent_ai.SetLifetimeMax(3888000);
			}
		
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
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, this, "LoadInterior");
#endif
		
		if (m_InteriorsLoaded) {
			return;
		}
				

		if ( !m_InteriorModule ) {
			Class.CastTo( m_InteriorModule, GetModuleManager().GetModule( ExpansionInteriorBuildingModule ) );
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.LoadInterior);
		
		SpawnInterior();
		m_InteriorsLoaded = true;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, this);
		m_InteriorModule.SaveCachedCollisions();
	}
	
	// ------------------------------------------------------------
	// BuildingBase LoadIvys
	// ------------------------------------------------------------	
	void LoadIvys()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, this, "LoadIvys");
#endif
		
		if (m_IvysLoaded || !IsMissionClient()) {
			return;
		}
				
		if (!m_InteriorModule) {
			Class.CastTo(m_InteriorModule, GetModuleManager().GetModule(ExpansionInteriorBuildingModule));
		}
		
		if (m_InteriorModule.ShouldIvySpawn(GetPosition())) {
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
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, this, "UnloadInterior");
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
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, this, "UnloadIvys");
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
		this.SetAllowDamage(false);
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

