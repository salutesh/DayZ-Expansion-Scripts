class ExpansionCameraHandler
{
	DayZPlayerCameraBase m_Camera;

	bool m_CenterCamera;
	bool m_ZoomOutToggle;
	bool m_StdFovUpdateCalled;

	bool m_WasFreeLook;
	float m_UpDownAngle;
	float m_UpDownAngleAdd;
	float m_LeftRightAngle;
	float m_LeftRightAngleAdd;

	float m_LRAngleCorr;

#ifdef DIAG_DEVELOPER
	float m_LookUp;
	float m_LookDown;
	float m_LookLeft;
	float m_LookRight;
#endif

	void ExpansionCameraHandler(DayZPlayerCameraBase camera)
	{
		m_Camera = camera;
	}

	float GetLRAngleCorr()
	{
		float lrAngleCorr;
		
		PlayerBase player = m_Camera.m_Ex_Player;
		HumanCommandVehicle hcv = player.GetCommand_Vehicle();
		if (hcv && !hcv.IsGettingIn() && player.Expansion_IsDriver())
		{
			Transport transport = hcv.GetTransport();

			if (transport)
			{
				vector tTransform[4];
				transport.GetTransform(tTransform);

				vector playerDir = player.GetDirection();
				vector playerDirLS = playerDir.InvMultiply3(tTransform);

				//Print(playerDirLS.VectorToAngles()[0]);
				lrAngleCorr = ExpansionMath.AngleDiff2(playerDirLS.VectorToAngles()[0], 0.0);
			}
		}

		return lrAngleCorr;
	}

	void OnActivate(inout float lrAngle, inout float lrAngleAdd, inout float udAngle, inout float udAngleAdd, DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
	{
		DayZPlayerCameraBase cameraBase;

		if (Class.CastTo(cameraBase, pPrevCamera))
		{
			m_WasFreeLook = cameraBase.m_ExIsFreeLook;

			ExpansionCameraHandler handler = cameraBase.m_Expansion_CameraHandler;

			if (handler)
				m_ZoomOutToggle = handler.m_ZoomOutToggle;

			if (!cameraBase.m_ExIsFreeLook)
			{
				if (handler)
				{
					m_UpDownAngle = handler.m_UpDownAngle;
					m_UpDownAngleAdd = handler.m_UpDownAngleAdd;
					m_LeftRightAngle = handler.m_LeftRightAngle;
					m_LeftRightAngleAdd	= handler.m_LeftRightAngleAdd;
				}
			}
			else
			{
				if (m_Camera.m_ExpansionVehicle.IsHelicopter() && !cameraBase.IsInherited(DayZPlayerCamera1stPersonVehicle) && !cameraBase.IsInherited(DayZPlayerCamera3rdPersonVehicle))
				{
					//! Center camera on initial vehicle get in
					m_CenterCamera = true;
				}
				else
				{
					m_UpDownAngle = udAngle;
					m_UpDownAngleAdd = udAngleAdd;
					m_LeftRightAngle = lrAngle;
					m_LeftRightAngleAdd	= lrAngleAdd;
				}
			}
		}
	}

	void OnUpdate(float pDt, inout float lrAngle, inout float lrAngleAdd, inout float udAngle, inout float udAngleAdd, ExpansionVehicleHelicopter simulation = null, ExpansionPhysicsState pState = null)
	{
	#ifdef COMPONENT_SYSTEM
		UAInput centerCamera = GetUApi().GetInputByName("UAExpansionVehicleCenterCamera");
	#else
		UAInput centerCamera = GetUApi().GetInputByID(UAExpansionVehicleCenterCamera);
	#endif

		if (centerCamera.LocalPress())
			m_CenterCamera = !m_CenterCamera;

		if (!m_Camera.m_ExIsFreeLook && m_WasFreeLook)
		{
			lrAngleAdd -= GetLRAngleCorr();

			m_UpDownAngle = udAngle;
			m_UpDownAngleAdd = udAngleAdd;
			m_LeftRightAngle = lrAngle;
			m_LeftRightAngleAdd = lrAngleAdd;

		#ifdef DIAG_DEVELOPER
			PrintFormat("FreeLook OFF %1 %2 %3 %4", udAngle, udAngleAdd, lrAngle, lrAngleAdd);
		#endif
		}
		else if (m_Camera.m_ExIsFreeLook && !m_WasFreeLook)
		{
			udAngle = m_UpDownAngle;
			udAngleAdd = m_UpDownAngleAdd;
			lrAngle = m_LeftRightAngle;
			lrAngleAdd = m_LeftRightAngleAdd;

			lrAngleAdd += GetLRAngleCorr();

		#ifdef DIAG_DEVELOPER
			PrintFormat("FreeLook ON %1 %2 %3 %4", udAngle, udAngleAdd, lrAngle, lrAngleAdd);
		#endif
		}

		m_WasFreeLook = m_Camera.m_ExIsFreeLook;

		//! ExpansionCameraHandler::OnUpdate gets called before camera OnUpdate super
		//! Reset angleAdd so we can add VRS shake and not have it break during heli mouse ctrl or when using TrackIR
		HumanInputController input = m_Camera.Expansion_GetInput();
		if ((input.CameraIsTracking() && m_Camera.m_ExpansionVehicle.IsHelicopter()) || !m_Camera.m_ExIsFreeLook)
		{
			lrAngleAdd = 0;
			udAngleAdd = 0;
		}

		if (simulation)
		{
			if (simulation.m_VRSSeverity > 0.0)
			{
				float shakeStrength = pState.m_LinearVelocity[1] / -30.0 * simulation.m_VRSSeverity;
				lrAngleAdd += Math.RandomFloat(-shakeStrength, shakeStrength);
				udAngleAdd += Math.RandomFloat(-shakeStrength, shakeStrength);
			}
		}

		//! Vanilla calls StdFovUpdate up to three times in camera OnUpdate super chain (in each inherited camera and once in camera_base)
		if (m_StdFovUpdateCalled)
			m_StdFovUpdateCalled = false;
	}

	//! Similar to vanilla, but NOT handling freelook (autocenter)
	float UpdateUDAngleUnlocked(out float pAngle, out float pAngleAdd, float pMin, float pMax, float pDt)
	{
		float target;

		HumanInputController input = m_Camera.Expansion_GetInput();
		float udAngleVel[1];
		m_Camera.Expansion_GetUDAngleVel(udAngleVel);

		if (input.CameraIsTracking() || !m_Camera.m_ExIsFreeLook)	
 		{
			pAngleAdd += input.GetTracking()[1] * Math.RAD2DEG;

			if (input.CameraIsTracking())
				target = m_Camera.Expansion_GetCommandWeapons().GetBaseAimingAngleUD();

			pAngle = target;

		#ifdef DIAG_DEVELOPER
			float lookUp = KeyState(KeyCode.KC_UP);
			float lookDown = KeyState(KeyCode.KC_DOWN);
			float lookUpDecay;
			if (lookUp)
				lookUpDecay = lookUp;
			else
				lookUpDecay = m_LookUp;
			float lookDownDecay;
			if (lookDown)
				lookDownDecay = lookDown;
			else
				lookDownDecay = m_LookDown;
			m_LookUp = Math.Lerp(m_LookUp, lookUp, pDt * 4 * (1.25 - lookUpDecay));
			m_LookDown = Math.Lerp(m_LookDown, lookDown, pDt * 4 * (1.25 - lookDownDecay));
			pAngleAdd += (m_LookUp - m_LookDown) * 85;
		#endif
 
			udAngleVel[0] = 0;
 		}
		else
		{
			if (Math.AbsFloat(pAngleAdd) > 0.001)
			{
				pAngleAdd 	= pAngle + pAngleAdd - target;
				pAngle 		= target;
			}

			PlayerBase player = m_Camera.m_Ex_Player;
			if (player)	// handle locking of camera U/D angle movement when performing an action and not in freelook
			{		
				float pitch = pAngle + pAngleAdd; 
				float downLimit, upLimit, leftLimit, rightLimit;
				player.GetLookLimits(downLimit, upLimit, leftLimit, rightLimit);
				
				if (pitch < (upLimit + 1) && pitch > (downLimit - 1)) // stop smoothing once within sufficiently small difference
				{
					pMin = downLimit;
					pMax = upLimit;
				}
				else // smooth camera to locked angle, avoiding snap from the angle we started the action to the limit
				{
					float vel[1] = udAngleVel;
					pMin = Math.SmoothCD(pitch, downLimit, vel, 0.2, 1000, pDt);
					pMax = Math.SmoothCD(pitch, upLimit, vel, 0.2, 1000, pDt);
				}
			 }

			pAngleAdd = Math.SmoothCD(pAngleAdd, -pAngle, udAngleVel, 0.03, 10000, pDt);
		}
				
		pAngle = Limit(pAngle, pMin, pMax);
		pAngleAdd = Limit(pAngle + pAngleAdd, pMin, pMax) - pAngle;
		return pAngle + pAngleAdd;
 	}

	//! Similar to vanilla, but NOT handling freelook (autocenter)
	float UpdateLRAngleUnlocked(out float pAngle, out float pAngleAdd, float pMin, float pMax, float pDt)
	{
		float lrAngleCorr = GetLRAngleCorr();
		float target = 0;

		HumanInputController input = m_Camera.Expansion_GetInput();

		if (input.CameraIsTracking() || !m_Camera.m_ExIsFreeLook)
 		{
			pAngleAdd += input.GetTracking()[0] * Math.RAD2DEG;
			pAngle = target;

		#ifdef DIAG_DEVELOPER
			float lookLeft = KeyState(KeyCode.KC_LEFT);
			float lookRight = KeyState(KeyCode.KC_RIGHT);
			float lookLeftDecay;
			if (lookLeft)
				lookLeftDecay = lookLeft;
			else
				lookLeftDecay = m_LookLeft;
			float lookRightDecay;
			if (lookRight)
				lookRightDecay = lookRight;
			else
				lookRightDecay = m_LookRight;
			m_LookLeft = Math.Lerp(m_LookLeft, lookLeft, pDt * 4 * (1.25 - lookLeftDecay));
			m_LookRight = Math.Lerp(m_LookRight, lookRight, pDt * 4 * (1.25 - lookRightDecay));
			pAngleAdd += (m_LookRight - m_LookLeft) * 160;
		#endif

			pAngleAdd += lrAngleCorr;

			float udAngleVel[1];
			m_Camera.Expansion_GetUDAngleVel(udAngleVel);
			udAngleVel[0] = 0;

			m_CenterCamera = false;
 		}
		else
		{
			if (Math.AbsFloat(pAngleAdd) > 0.001)
			{
				pAngleAdd 	= pAngle + pAngleAdd - target;
				pAngle 		= target;
			}

			float lrAngleVel[1];
			m_Camera.Expansion_GetLRAngleVel(lrAngleVel);
			pAngleAdd = Math.SmoothCD(pAngleAdd, lrAngleCorr, lrAngleVel, 0.03, 10000, pDt);

			if (m_CenterCamera)
			{
				PlayerBase player = m_Camera.m_Ex_Player;
				HumanCommandVehicle hcv = player.GetCommand_Vehicle();
				if (hcv && !hcv.IsGettingIn() && Math.AbsFloat(lrAngleCorr - m_LRAngleCorr) < 0.001)
				{
					float udAngle;
					float udAngleAdd;
					m_Camera.Expansion_GetUDAngles(udAngle, udAngleAdd);
					if (Math.AbsFloat(pAngleAdd - lrAngleCorr) < 0.01 && Math.AbsFloat(udAngleAdd + udAngle) < 0.01)
					{
						pAngleAdd = lrAngleCorr;
						m_CenterCamera = false;
					}
				}
			}
			
			pAngleAdd = pAngleAdd + pAngle;
		}
		
		m_LRAngleCorr = lrAngleCorr;

		pAngle = Limit(pAngle, pMin, pMax);
		pAngleAdd = Limit(pAngle + pAngleAdd, pMin, pMax) - pAngle;
		
		return pAngle + pAngleAdd;
	}

	bool StdFovUpdate(float pDt, inout float fovAbsolute, inout float fovAbsVel[], out DayZPlayerCameraResult pOutResult)
	{
		//! Vanilla calls StdFovUpdate up to three times in camera OnUpdate super chain (in each inherited camera and once in camera_base)
		if (m_StdFovUpdateCalled)
			return true;

		m_StdFovUpdateCalled = true;

	#ifdef COMPONENT_SYSTEM
		UAInput zoomOut = GetUApi().GetInputByName("UAExpansionVehicleCameraZoomOut");
	#else
		UAInput zoomOut = GetUApi().GetInputByID(UAExpansionVehicleCameraZoomOut);
	#endif

		UAInput zoomIn = GetUApi().GetInputByID(UAZoomIn);
		UAInput zoomInToggle = GetUApi().GetInputByID(UAZoomInToggle);

		if (zoomOut.LocalDoubleClick())
			m_ZoomOutToggle = !m_ZoomOutToggle;
		else if (zoomOut.LocalPress() || zoomInToggle.LocalPress())
			m_ZoomOutToggle = false;

		if (zoomIn.LocalValue())
			return false;

		if (m_ZoomOutToggle || zoomOut.LocalValue())
		{
			fovAbsolute = Math.SmoothCD(fovAbsolute, g_Game.GetUserFOV() * 1.3, fovAbsVel, 0.1, 1000, pDt);
			pOutResult.m_fFovAbsolute = fovAbsolute;
			return true;
		}

		return false;
	}
}
