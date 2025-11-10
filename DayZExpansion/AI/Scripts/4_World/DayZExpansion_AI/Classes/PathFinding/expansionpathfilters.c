class ExpansionPathFilters
{
	protected static autoptr ExpansionPathFilters s_Instance;

	autoptr PGFilter m_PathFilter;
	autoptr PGFilter m_PathFilter_NoJumpClimb;
	autoptr PGFilter m_PathFilter_NoClosedDoors;
	autoptr PGFilter m_PathFilter_NoClosedDoors_NoJumpClimb;
	autoptr PGFilter m_PathFilter_Swimming;
	autoptr PGFilter m_PathFilter_NoJumpClimb_Swimming;
	autoptr PGFilter m_PathFilter_NoClosedDoors_Swimming;
	autoptr PGFilter m_PathFilter_NoClosedDoors_NoJumpClimb_Swimming;
	autoptr PGFilter m_BlockFilter;
	autoptr PGFilter m_AllFilter;
	
#ifdef EAI_DEBUG_PATH
	autoptr PGFilter m_PointFilter;
#endif

	int m_IncludeFlags;
	int m_IncludeFlags_NoJumpClimb;
	int m_IncludeFlags_NoClosedDoors;
	int m_IncludeFlags_NoClosedDoors_NoJumpClimb;
	int m_ExcludeFlags;
	int m_ExcludeFlags_NoJumpClimb;
	int m_ExcludeFlags_NoClosedDoors;
	int m_ExcludeFlags_NoClosedDoors_NoJumpClimb;
	int m_IncludeFlags_Swimming;
	int m_IncludeFlags_NoJumpClimb_Swimming;
	int m_IncludeFlags_NoClosedDoors_Swimming;
	int m_IncludeFlags_NoClosedDoors_NoJumpClimb_Swimming;
	int m_ExcludeFlags_Swimming;
	int m_ExcludeFlags_NoJumpClimb_Swimming;
	int m_ExcludeFlags_NoClosedDoors_Swimming;
	int m_ExcludeFlags_NoClosedDoors_NoJumpClimb_Swimming;
	int m_ExclusiveFlags;

	private void ExpansionPathFilters()
	{
		m_PathFilter = new PGFilter();
		m_PathFilter_NoJumpClimb = new PGFilter();
		m_PathFilter_NoClosedDoors = new PGFilter();
		m_PathFilter_NoClosedDoors_NoJumpClimb = new PGFilter();
		m_PathFilter_Swimming = new PGFilter();
		m_PathFilter_NoJumpClimb_Swimming = new PGFilter();
		m_PathFilter_NoClosedDoors_Swimming = new PGFilter();
		m_PathFilter_NoClosedDoors_NoJumpClimb_Swimming = new PGFilter();
		m_BlockFilter = new PGFilter();
		m_AllFilter = new PGFilter();

		/** @note
		 * PGPolyFlags.DISABLED is for closed doors
		 * PGPolyFlags.DOOR is for opened doors
		 * PGPolyFlags.INSIDE is for inside buildings
		 * 
		 * High cost for DOOR_OPENED to path around door (the physics object, the doorway will still be used)
		 */
		m_IncludeFlags = PGPolyFlags.UNREACHABLE | PGPolyFlags.DISABLED | PGPolyFlags.WALK | PGPolyFlags.DOOR | PGPolyFlags.INSIDE | PGPolyFlags.LADDER;
		m_ExcludeFlags = PGPolyFlags.CRAWL | PGPolyFlags.CROUCH | PGPolyFlags.SWIM_SEA | PGPolyFlags.SWIM;
		m_ExclusiveFlags = PGPolyFlags.NONE;

		if (eAIBase.AI_HANDLEVAULTING)
		{
			m_IncludeFlags |= PGPolyFlags.SPECIAL;
		}
		else
		{
			m_ExcludeFlags |= PGPolyFlags.SPECIAL;
		}

		m_IncludeFlags_NoJumpClimb = m_IncludeFlags & ~PGPolyFlags.SPECIAL;
		m_IncludeFlags_NoClosedDoors = m_IncludeFlags & ~PGPolyFlags.DISABLED;
		m_IncludeFlags_NoClosedDoors_NoJumpClimb = m_IncludeFlags_NoJumpClimb & ~PGPolyFlags.DISABLED;
		m_ExcludeFlags_NoJumpClimb = m_ExcludeFlags | PGPolyFlags.SPECIAL;
		m_ExcludeFlags_NoClosedDoors = m_ExcludeFlags | PGPolyFlags.DISABLED;
		m_ExcludeFlags_NoClosedDoors_NoJumpClimb = m_ExcludeFlags_NoJumpClimb | PGPolyFlags.DISABLED;

		int swim = PGPolyFlags.SWIM_SEA | PGPolyFlags.SWIM;
		m_IncludeFlags_Swimming = m_IncludeFlags | swim;
		m_IncludeFlags_NoJumpClimb_Swimming = m_IncludeFlags_NoJumpClimb | swim;
		m_IncludeFlags_NoClosedDoors_Swimming = m_IncludeFlags_Swimming & ~PGPolyFlags.DISABLED;
		m_IncludeFlags_NoClosedDoors_NoJumpClimb_Swimming = m_IncludeFlags_NoJumpClimb_Swimming & ~PGPolyFlags.DISABLED;
		m_ExcludeFlags_Swimming = m_ExcludeFlags & ~swim;
		m_ExcludeFlags_NoJumpClimb_Swimming = m_ExcludeFlags_NoJumpClimb & ~swim;
		m_ExcludeFlags_NoClosedDoors_Swimming = m_ExcludeFlags_Swimming | PGPolyFlags.DISABLED;
		m_ExcludeFlags_NoClosedDoors_NoJumpClimb_Swimming = m_ExcludeFlags_NoJumpClimb_Swimming | PGPolyFlags.DISABLED;

		SetFilterCost(m_PathFilter, m_PathFilter_NoJumpClimb);
		SetFilterCost(m_PathFilter_NoClosedDoors, m_PathFilter_NoClosedDoors_NoJumpClimb);
		SetFilterCost(m_PathFilter_Swimming, m_PathFilter_NoJumpClimb_Swimming);
		SetFilterCost(m_PathFilter_NoClosedDoors_Swimming, m_PathFilter_NoClosedDoors_NoJumpClimb_Swimming);

		m_PathFilter.SetFlags(m_IncludeFlags, m_ExcludeFlags, m_ExclusiveFlags);
		m_PathFilter_NoClosedDoors.SetFlags(m_IncludeFlags_NoClosedDoors, m_ExcludeFlags_NoClosedDoors, m_ExclusiveFlags);
		m_PathFilter_NoJumpClimb.SetFlags(m_IncludeFlags_NoJumpClimb, m_ExcludeFlags_NoJumpClimb, m_ExclusiveFlags);
		m_PathFilter_NoClosedDoors_NoJumpClimb.SetFlags(m_IncludeFlags_NoClosedDoors_NoJumpClimb, m_ExcludeFlags_NoClosedDoors_NoJumpClimb, m_ExclusiveFlags);
		m_PathFilter_Swimming.SetFlags(m_IncludeFlags_Swimming, m_ExcludeFlags_Swimming, m_ExclusiveFlags);
		m_PathFilter_NoJumpClimb_Swimming.SetFlags(m_IncludeFlags_NoJumpClimb_Swimming, m_ExcludeFlags_NoJumpClimb_Swimming, m_ExclusiveFlags);
		m_PathFilter_NoClosedDoors_Swimming.SetFlags(m_IncludeFlags_NoClosedDoors_Swimming, m_ExcludeFlags_NoClosedDoors_Swimming, m_ExclusiveFlags);
		m_PathFilter_NoClosedDoors_NoJumpClimb_Swimming.SetFlags(m_IncludeFlags_NoClosedDoors_NoJumpClimb_Swimming, m_ExcludeFlags_NoClosedDoors_NoJumpClimb_Swimming, m_ExclusiveFlags);

		//! Block filter - only used to check if path is blocked. MUST use SAME flags as normal pathfilter EXCEPT door
		m_BlockFilter.SetFlags(m_IncludeFlags & ~(PGPolyFlags.DOOR | PGPolyFlags.DISABLED), m_ExcludeFlags | PGPolyFlags.DOOR | PGPolyFlags.DISABLED, m_ExclusiveFlags);

		//! 'All' filter - only used to check if point can be sampled
		m_AllFilter.SetFlags(PGPolyFlags.ALL & ~(PGPolyFlags.CRAWL | PGPolyFlags.CROUCH), PGPolyFlags.CRAWL | PGPolyFlags.CROUCH, PGPolyFlags.NONE);

	#ifdef EAI_DEBUG_PATH
		m_PointFilter = new PGFilter();

		m_PointFilter.SetCost(PGAreaType.NONE, 10.0);
		m_PointFilter.SetCost(PGAreaType.TERRAIN, 10.0);
		m_PointFilter.SetCost(PGAreaType.DOOR_OPENED, 10.0);
		m_PointFilter.SetCost(PGAreaType.DOOR_CLOSED, 10.0);
		m_PointFilter.SetCost(PGAreaType.OBJECTS, 10.0);
		m_PointFilter.SetCost(PGAreaType.BUILDING, 10.0);
		m_PointFilter.SetCost(PGAreaType.ROADWAY, 10.0);
		m_PointFilter.SetCost(PGAreaType.ROADWAY_BUILDING, 10.0);
		m_PointFilter.SetCost(PGAreaType.LADDER, 10.0);
		m_PointFilter.SetCost(PGAreaType.CRAWL, 10.0);
		m_PointFilter.SetCost(PGAreaType.CROUCH, 10.0);
		m_PointFilter.SetCost(PGAreaType.FENCE_WALL, 10.0);
		m_PointFilter.SetCost(PGAreaType.JUMP, 10.0);
	#endif
	}

	private void ~ExpansionPathFilters()
	{
	}

	private void SetFilterCost(PGFilter filter, PGFilter filterNoJumpClimb)
	{
		filter.SetCost(PGAreaType.LADDER, 1.0);
		filter.SetCost(PGAreaType.CRAWL, 10.0);
		filter.SetCost(PGAreaType.CROUCH, 10.0);
		filter.SetCost(PGAreaType.FENCE_WALL, 5.0);  //! Vault
		filter.SetCost(PGAreaType.JUMP, 10.0);  //! Climb
		filter.SetCost(PGAreaType.WATER, 5.0);
		filter.SetCost(PGAreaType.WATER_DEEP, 10.0);
		filter.SetCost(PGAreaType.WATER_SEA, 5.0);
		filter.SetCost(PGAreaType.WATER_SEA_DEEP, 10.0);

		filter.SetCost(PGAreaType.DOOR_CLOSED, 4.0);
		filter.SetCost(PGAreaType.DOOR_OPENED, 10000.0);

		filter.SetCost(PGAreaType.ROADWAY, 4.0);
		filter.SetCost(PGAreaType.TREE, 1.0);

		filter.SetCost(PGAreaType.OBJECTS_NOFFCON, 5.0);
		filter.SetCost(PGAreaType.OBJECTS, 5.0);
		filter.SetCost(PGAreaType.TERRAIN, 4.0);
		filter.SetCost(PGAreaType.BUILDING, 4.0);
		filter.SetCost(PGAreaType.ROADWAY_BUILDING, 1.0);

		filterNoJumpClimb.SetCost(PGAreaType.LADDER, 1.0);
		filterNoJumpClimb.SetCost(PGAreaType.CRAWL, 10.0);
		filterNoJumpClimb.SetCost(PGAreaType.CROUCH, 10.0);
		filterNoJumpClimb.SetCost(PGAreaType.FENCE_WALL, 1000.0);  //! Vault
		filterNoJumpClimb.SetCost(PGAreaType.JUMP, 1000.0);  //! Climb
		filterNoJumpClimb.SetCost(PGAreaType.WATER, 5.0);
		filterNoJumpClimb.SetCost(PGAreaType.WATER_DEEP, 10.0);
		filterNoJumpClimb.SetCost(PGAreaType.WATER_SEA, 5.0);
		filterNoJumpClimb.SetCost(PGAreaType.WATER_SEA_DEEP, 10.0);

		filterNoJumpClimb.SetCost(PGAreaType.DOOR_CLOSED, 4.0);
		filterNoJumpClimb.SetCost(PGAreaType.DOOR_OPENED, 10000.0);

		filterNoJumpClimb.SetCost(PGAreaType.ROADWAY, 4.0);
		filterNoJumpClimb.SetCost(PGAreaType.TREE, 1.0);

		filterNoJumpClimb.SetCost(PGAreaType.OBJECTS_NOFFCON, 5.0);
		filterNoJumpClimb.SetCost(PGAreaType.OBJECTS, 5.0);
		filterNoJumpClimb.SetCost(PGAreaType.TERRAIN, 4.0);
		filterNoJumpClimb.SetCost(PGAreaType.BUILDING, 4.0);
		filterNoJumpClimb.SetCost(PGAreaType.ROADWAY_BUILDING, 1.0);
	}

	static ExpansionPathFilters GetInstance()
	{
		if (!s_Instance)
			s_Instance = new ExpansionPathFilters();

		return s_Instance;
	}
}
