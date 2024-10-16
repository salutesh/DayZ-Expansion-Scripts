enum eAIWaypointBehavior
{
	HALT,
	LOOP,
	ALTERNATE,
	ONCE,
	HALT_OR_LOOP,
	HALT_OR_ALTERNATE,
	ROAMING,
	MIXED  //! TODO
};

enum eAILootingBehavior
{
	NONE = 0,
	WEAPONS_FIREARMS = 1,
	WEAPONS_LAUNCHERS = 2,
	WEAPONS_MELEE = 4,
	WEAPONS = 7,  //! WEAPONS_FIREARMS | WEAPONS_LAUNCHERS | WEAPONS_MELEE
	BANDAGES = 8,
	CLOTHING = 16,
	UPGRADE = 32,
	DEFAULT = 7,  //! WEAPONS_FIREARMS | WEAPONS_LAUNCHERS | WEAPONS_MELEE
	ALL = 63  //! WEAPONS | BANDAGES | CLOTHING | UPGRADE
};

enum eAIGroupFormationState
{
	NONE,
	IN,
	FLANK
};

enum eAIMovementSpeed
{
	STATIC,
	WALK,
	JOG,
	SPRINT,
	RANDOM,
	RANDOM_NONSTATIC
};

enum eAITargetType
{
	NONE = 0,
	ALL = 1,
	ANIMAL = 2,
	INFECTED = 4,
	PLAYER = 8,
	VEHICLE = 16,
	ALL_OR = 30  //! AI | ANIMAL | INFECTED | PLAYER | VEHICLE
};
