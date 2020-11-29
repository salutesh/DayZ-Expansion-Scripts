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
	
	autoptr array<Object> m_InteriorObjects = {};
	autoptr array<Object> m_IvyObjects = {};
	
	private bool m_InteriorsLoaded;
	private bool m_IvysLoaded;
	
	// ------------------------------------------------------------
	// BuildingBase Constructor
	// ------------------------------------------------------------
	void BuildingBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::BuildingBase - Start");
		#endif
		
		ExpansionSettings.SI_General.Insert( OnSettingsUpdated );
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ReloadCustomObjects, 8000, false);
		
		if( GetExpansionSettings().GetGeneral().DisableShootToUnlock )
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetGodMode, 8000, false);
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::BuildingBase - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// BuildingBase Desturctor
	// ------------------------------------------------------------
	void ~BuildingBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::~BuildingBase - Start");
		#endif

		ExpansionSettings.SI_General.Remove( OnSettingsUpdated );
		//UnloadInterior();
		//UnloadIvys();

		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::~BuildingBase - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// IsBuildingListed
	// ------------------------------------------------------------
	bool IsBuildingListed()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::IsBuildingListed - Start");
		#endif
		
		for ( int x = 0; x < GetExpansionSettings().GetGeneral().Mapping.Interiors.Count(); ++x )
		{
			if ( this.IsKindOf( GetExpansionSettings().GetGeneral().Mapping.Interiors[x] ) )
			{
				return true;
			}
		}

		return false;

		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::IsBuildingListed - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnSettingsUpdated
	// ------------------------------------------------------------
	void OnSettingsUpdated()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::OnSettingsUpdated - Start");
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::OnSettingsUpdated - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// BuildingBase ReloadCustomObjects
	// ------------------------------------------------------------
	void ReloadCustomObjects()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::ReloadCustomObjects - Start");
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::ReloadCustomObjects - End");
		#endif
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("BuildingBase::SpawnInteriorObject spawning " + type + " for " + GetType() + " at " + position);
		#endif

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
			if (item)
			{
				//Make it not takeable
				item.SetTakeable(false);
				
				if ( IsMissionHost() ) 
					item.SetLifetimeMax(1.0);
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
		if (m_InteriorsLoaded) {
			return;
		}
		
		//Print("Loading Interior");
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("BuildingBase::LoadInterior - Start");
		#endif

		if ( !m_InteriorModule ) {
			Class.CastTo( m_InteriorModule, GetModuleManager().GetModule( ExpansionInteriorBuildingModule ) );
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.LoadInterior);
		
		thread SpawnInterior();
		m_InteriorsLoaded = true;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, this);
		m_InteriorModule.SaveCachedCollisions();
		
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("BuildingBase::LoadInterior - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// BuildingBase LoadIvys
	// ------------------------------------------------------------	
	void LoadIvys()
	{
		if (m_IvysLoaded || !IsMissionClient()) {
			return;
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("BuildingBase::LoadIvys - Start");
		#endif
		
		if (!m_InteriorModule) {
			Class.CastTo(m_InteriorModule, GetModuleManager().GetModule(ExpansionInteriorBuildingModule));
		}
		
		if (m_InteriorModule.ShouldIvySpawn(GetPosition())) {
			thread SpawnIvys();
		}
		
		m_IvysLoaded = true;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("BuildingBase::LoadIvys - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// BuildingBase UnloadInterior
	// ------------------------------------------------------------		
	void UnloadInterior()
	{		
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
	
	// ------------------------------------------------------------
	// BuildingBase Explode
	// ------------------------------------------------------------
 	override void Explode(int damageType, string ammoType = "")
	{
		float explosionDamageMultiplier = GetExpansionSettings().GetRaid().ExplosionDamageMultiplier;
		float blastDropoff = 1;
		float blastDistance;
		float blastRange = 5;
		float blastDropoffRange = 2.5;
		super.Explode(damageType, ammoType);
		//(point - min ) / (max - min ) 
		if (ammoType == "")
			ammoType = this.ConfigGetString("ammoType");
		

		string dmgPath = "CfgAmmo" + " " + ammoType + " " + "DamageApplied" + " " + "Health" + " " + "Damage";
		int explosionDamage = GetGame().ConfigGetInt(dmgPath);
		
		ref array<Object> nearest_objects = new array<Object>;
		ref array<CargoBase> proxy_cargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D( this.GetPosition(), blastRange, nearest_objects, proxy_cargos );
		for ( int i = 0; i < nearest_objects.Count(); i++ )
		{
			bool dealDamage = !GetExpansionSettings().GetRaid().EnableExplosiveWhitelist;
			Object nearest_object = nearest_objects.Get(i);

			if ( nearest_object.IsInherited( ExpansionBaseBuilding ) )
			{
				blastDistance = vector.Distance(nearest_object.GetPosition(), this.GetPosition());
				if (blastDistance > blastDropoffRange)
					blastDropoff = (1 - (blastDistance - blastDropoffRange) / (blastRange - blastDropoffRange));
				else 
					blastDropoff = 1;
				
				
				for (int x = 0; x < GetExpansionSettings().GetRaid().ExplosiveDamageWhitelist.Count(); ++x)
				{

					if (this.IsKindOf(GetExpansionSettings().GetRaid().ExplosiveDamageWhitelist[x]))
					{
						dealDamage = true;
					}
				}
				if (dealDamage)
					nearest_object.AddHealth( "GlobalHealth", "Health", ( explosionDamage * blastDropoff * explosionDamageMultiplier * -1) ); 
			}
		}
	}

	void SetGodMode()
	{
		this.SetAllowDamage(false);
	}
	// ------------------------------------------------------------
	// BuildingBase RemoveInterior
	// ------------------------------------------------------------
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
