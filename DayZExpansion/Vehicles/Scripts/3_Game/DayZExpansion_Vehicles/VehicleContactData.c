modded class VehicleContactData
{
	static float s_OverrideImpulse = -1;

	override void SetData(vector localPos, IEntity other, float impulse)
	{
		if (s_OverrideImpulse >= 0)
			impulse = s_OverrideImpulse;

		super.SetData(localPos, other, impulse);
	}
}
