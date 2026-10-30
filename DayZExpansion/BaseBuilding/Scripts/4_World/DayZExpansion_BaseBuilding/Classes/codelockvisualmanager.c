modded class CodeLockVisualManager
{
	static float FRONT_PART_EXBBWALL = 0.98;
	static float BACK_PART_EXBBWALL = 0.3;

	override protected float GetAnimationPhaseForFrontPart(EntityAI hierarchyParent, bool reversed)
	{
		float animationPhase = super.GetAnimationPhaseForFrontPart(hierarchyParent, reversed);

		if (hierarchyParent.IsInherited(ExpansionWallBase))
			animationPhase = FRONT_PART_EXBBWALL;
		
		return animationPhase;
	}
	
	override protected float GetAnimationPhaseForBackPart(EntityAI hierarchyParent, bool reversed)
	{
		float animationPhase = super.GetAnimationPhaseForBackPart(hierarchyParent, reversed);

		if (hierarchyParent.IsInherited(ExpansionWallBase))
			animationPhase = BACK_PART_EXBBWALL;
		
		return animationPhase;
	}
}
