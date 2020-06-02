class ExpansionPlayerCamera extends Camera
{
	void ExpansionPlayerCamera()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlayerCamera::ExpansionPlayerCamera Start");
		#endif
		
		SetEventMask( EntityEvent.FRAME );
		
		SetFlags( EntityFlags.ACTIVE | EntityFlags.VISIBLE, true );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPlayerCamera::ExpansionPlayerCamera End");
		#endif
	}
}