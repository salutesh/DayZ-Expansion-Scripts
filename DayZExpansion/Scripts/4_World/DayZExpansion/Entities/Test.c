/*
modded class Apple
{
	void Apple()
	{
		SetFlags(EntityFlags.ACTIVE | EntityFlags.VISIBLE, false);
		SetEventMask(EntityEvent.FRAME | EntityEvent.INIT);
	}

	void OnCreatePhysics()
	{
		SetFlags(EntityFlags.ACTIVE | EntityFlags.VISIBLE, false);
		SetEventMask(EntityEvent.FRAME | EntityEvent.INIT);

		Print( "OnCreatePhysics" );
		dBodyDestroy( this );

		vobject vobj;
		vobj = GetObject( "DayZExpansion/Vehicles/Air/Mh6/mh6wreck.xob" );
		SetObject( vobj, "" );
		ReleaseObject( vobj, false );

		if(!dBodyCreateDynamic(this, 1.0, 0xffffffff))
		{
			Print( "dBodyCreateDynamic fail" );

			//create implicit box
			vector mins, maxs;
			GetBounds(mins, maxs);
			vector center = (mins + maxs) * 0.5;
			vector size = maxs - mins;

			Print( center );
			Print( size );
							
			ref PhysicsGeomDef geoms[] =
			{
				PhysicsGeomDef("", dGeomCreateBox(size), "material/default", 0xffffffff)
			};

			dBodyCreateDynamicEx(this, center, 1, geoms);
		} else
		{
			Print( "dBodyCreateDynamic success" );
		}
		if(dBodyIsSet(this))
		{
			Print( "dBodyIsSet success" );
			dBodySetMass(this, 1.0);
			dBodyActive(this, ActiveState.ACTIVE);
			dBodyDynamic(this, true);
		} else
		{
			Print( "dBodyIsSet fail" );
		}
		Print( "OnCreatePhysics end" );
	}
};
*/