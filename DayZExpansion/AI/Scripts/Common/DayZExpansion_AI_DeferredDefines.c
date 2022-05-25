//! Defines that depend on other mod's defines and thus need to load late

#ifdef DIAG
	#ifdef EXPANSIONMODNAVIGATION
		#define EAI_DEBUG_EVENTPATROL
	#endif
#endif

#ifdef GAMELABS
	#define EXPANSIONMODAI_ENABLEPOLICEWRECK
#else
	#ifdef DIAG
		//! @note won't work with GameLabs until they fix their define not being loaded
		#define EXPANSIONMODAI_ENABLEPOLICEWRECK
	#endif
#endif
