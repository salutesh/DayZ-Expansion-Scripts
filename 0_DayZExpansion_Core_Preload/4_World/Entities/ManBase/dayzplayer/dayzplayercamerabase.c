#ifndef DAYZ_1_29
//! 1.30+
//! Get rid of the camera U/D movement bounce-back bug that was introduced in 1.30 Experimental
//! (see https://report.bistudio.com/issues/DZEXP-124)
modded class DayZPlayerCameraBase
{
	override float UpdateUDAngleUnlocked(out float pAngle, out float pAngleAdd, float pMin, float pMax, float pDt)
	{
		float target = 0.0;
		
		if (m_pInput.CameraIsTracking())	
 		{
			pAngleAdd = m_pInput.GetTracking()[1] * Math.RAD2DEG;
			pAngle = target;
 
			m_fUDAngleVel[0] = 0;
 		}
		else
		{
			if (m_pInput.CameraIsFreeLook())	
	 		{
				pAngleAdd += m_pInput.GetAimDelta(pDt)[1] * Math.RAD2DEG;
	 
				m_fUDAngleVel[0] = 0;
	 		}
	 		else
	 		{
				target = m_CommandWeapons.GetBaseAimingAngleUD();

				if (Math.AbsFloat(pAngleAdd) > 0.001)
				{
					pAngleAdd 	= pAngle + pAngleAdd - target;
					pAngle 		= target;
				}

				PlayerBase player = PlayerBase.Cast(m_pPlayer);
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
						float velMin[1] = m_fUDAngleVel;
						pMin = Math.SmoothCD(pitch, downLimit, velMin, 0.2, 1000, pDt);

						float velMax[1] = m_fUDAngleVel;
						pMax = Math.SmoothCD(pitch, upLimit, velMax, 0.2, 1000, pDt);
					}
				}
				
				pAngleAdd = Math.SmoothCD(pAngleAdd, 0.0, m_fUDAngleVel, 0.14, 1000, pDt);
			}
	
			if (!m_pInput.CameraIsFreeLook())
			{
				pAngle += m_pInput.GetAimDelta(pDt)[1] * Math.RAD2DEG;
	 		}
		}

		pAngle = Limit(pAngle, pMin, pMax);
		pAngleAdd = Limit(pAngle + pAngleAdd, pMin, pMax) - pAngle;

		return pAngle + pAngleAdd;
 	}
}
#endif
