//! Defines that depend on other mod's defines and thus need to load late

#ifdef DIAG_DEVELOPER
	#ifdef EXPANSIONMODNAVIGATION
		#define EAI_DEBUG_EVENTPATROL
	#endif
#endif
