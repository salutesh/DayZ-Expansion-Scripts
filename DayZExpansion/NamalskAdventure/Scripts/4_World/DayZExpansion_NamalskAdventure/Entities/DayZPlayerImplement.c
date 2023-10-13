
//! Allow 3PP camera view while in a Expansion SZ.
//! Change back eye zoom level changes while in a Expansion SZ (introduced with 1.20)
modded class DayZPlayerImplement
{
    override void HandleView()
    {
        bool camera3rdPerson = m_Camera3rdPerson;

        super.HandleView();

        if (!IsAlive())
            return;

        HumanInputController hic = GetInputController();

        if (GetGame().GetWorld().Is3rdPersonDisabled() && Expansion_IsInSafeZone())
        {
            m_Camera3rdPerson = camera3rdPerson;

            if (!IsRaised() || !m_IsWeapon)
            {
                if (hic.CameraViewChanged())
                    m_Camera3rdPerson = !m_Camera3rdPerson;
            }
        }

        if (!IsRaised() && hic.IsZoomToggle())
            m_CameraEyeZoomLevel = ECameraZoomType.NORMAL;
        else
            m_CameraEyeZoomLevel = ECameraZoomType.NONE;
    }
};