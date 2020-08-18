class Expansion_Guitar extends ItemBase
{
	override bool IsOpen()
	{
		return false;
	}
	
	void AnimateGuitar( bool state )
	{
		SetAnimationPhase( "AnimateGuitar", state );
	}
};