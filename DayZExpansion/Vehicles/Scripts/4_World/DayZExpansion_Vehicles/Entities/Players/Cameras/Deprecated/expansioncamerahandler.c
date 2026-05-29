class ExpansionCameraHandler
{
	static float CAMERA_SMOOTHTIME = 0.05;
	static float CAMERA_MAXVEL = 10000;

	DayZPlayerCameraBase m_Camera;

	bool m_CenterCamera;
	float m_CenterCameraTime;

	void ExpansionCameraHandler(DayZPlayerCameraBase camera)
	{
		m_Camera = camera;
	}

	void OnUpdate(float pDt, inout float lrAngle, inout float lrAngleAdd, inout float udAngle, inout float udAngleAdd)
	{
	#ifdef COMPONENT_SYSTEM
		if (GetUApi().GetInputByName("UAExpansionVehicleCenterCamera").LocalPress())
	#else
		if (GetUApi().GetInputByID(UAExpansionVehicleCenterCamera).LocalPress())
	#endif
		{
			m_CenterCamera = !m_CenterCamera;

			float lr = Math.AbsFloat(lrAngleAdd - lrAngle) / 90.0;
			float ud = Math.AbsFloat(udAngleAdd + udAngle) / 90.0;

			if (lr > ud)
				m_CenterCameraTime = Math.Max(0.5 - lr, 0.0);
			else
				m_CenterCameraTime = Math.Max(0.5 - ud, 0.0);

			m_CenterCameraTime *= m_CenterCameraTime;
		}

		if (m_CenterCamera && m_CenterCameraTime < 1.0 && (Math.AbsFloat(lrAngleAdd - lrAngle) > 0.05 || Math.AbsFloat(udAngleAdd + udAngle) > 0.05) && m_Camera.m_ExIsFreeLook)
		{
			m_CenterCameraTime = Math.Min(m_CenterCameraTime + pDt, 1.0);
			float t = Easing.EaseOutSine(m_CenterCameraTime);
			lrAngleAdd = Math.Lerp(lrAngleAdd, lrAngle, t);
			udAngleAdd = Math.Lerp(udAngleAdd, -udAngle, t);
		}
		else
		{
			m_CenterCamera = false;
		}
	}
}
