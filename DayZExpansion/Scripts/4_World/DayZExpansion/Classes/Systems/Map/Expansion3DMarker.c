/**
 * Expansion3DMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion3DMarker extends UIScriptedMenu
{
    protected float             m_OriginalHeight;
    protected float             m_OriginalWidth;

    protected Object            m_LinkedObject;
    protected bool              m_UsingLinkedObject;
    protected vector            m_OffsetPosition;
    protected int               m_BoneIndex;

    protected vector            m_MarkerPosition;

    protected TextWidget        m_MarkerDistanceText;
    protected Widget            m_MarkerFrame;

    protected TextWidget        m_MarkerText;

    protected ImageWidget       m_MarkerIcon;

    private int                 m_AlphaColor;
    private int                 m_MinAlphaColor = 80;
    private int                 m_RedColor;
    private int                 m_GreenColor;
    private int                 m_BlueColor;

    void Expansion3DMarker( string name, string iconpath, int color, vector position, Object linkedObject = NULL, vector offset = "0 0 0", int bone = -1 )
    {
       	layoutRoot = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/expansion_dynamic_marker.layout" );
		if ( layoutRoot )
		{
	        m_MarkerIcon                        = ImageWidget.Cast( layoutRoot.FindAnyWidget( "MarkerImage" ) );
	        m_MarkerText                        = TextWidget.Cast( layoutRoot.FindAnyWidget( "MarkerText" ) );
	        m_MarkerDistanceText                = TextWidget.Cast( layoutRoot.FindAnyWidget( "MarkerDistance" ) );
	        m_MarkerFrame                       = layoutRoot.FindAnyWidget( "MarkerFrame" );
	
	        if ( linkedObject != NULL )
	        {
	            m_LinkedObject = linkedObject;
	        }
	        else
	        {
	            m_MarkerPosition = Vector( position[0], GetGame().SurfaceY( position[0], position[2] ), position[2] );
	        }
	        
	        m_UsingLinkedObject = ( linkedObject != NULL );
	
	        m_OffsetPosition = offset;
	        m_BoneIndex = bone;
	
	        m_MarkerFrame.GetSize( m_OriginalWidth, m_OriginalHeight );
	
	        m_MarkerText.SetText( name );

            int alpha;
            int red;
            int green;
            int blue;
            GetARGB(color, alpha, red, green, blue);
           
            m_AlphaColor = Math.Max(m_MinAlphaColor, alpha);
            m_RedColor = red;
            m_GreenColor = green;
            m_BlueColor = blue;

            m_MarkerText.SetColor( ARGB(alpha, 255, 255, 255) );
            m_MarkerDistanceText.SetColor( ARGB(alpha, 255, 255, 255) );

	        m_MarkerIcon.LoadImageFile( 0, iconpath );
	        m_MarkerIcon.SetImage( 0 );
	        m_MarkerIcon.SetColor( color );
		}
    }

    void ~Expansion3DMarker()
    {
		if ( layoutRoot )
	        layoutRoot.Unlink();
    }

    override Widget Init()
    {
        return layoutRoot;
    }

    override bool UseMouse()
    {
		return false;
	}

	override bool UseKeyboard()
    {
		return false;
	}
	
	override bool UseGamepad()
    {
		return false;
	}

    override void Update( float timeslice )
	{
        UpdateUI();
    }

    void UpdateUI()
    {
        if ( m_UsingLinkedObject && m_LinkedObject == NULL )
        {
            Close();
            return;
        }

        vector worldPosition;

        if ( m_UsingLinkedObject )
        {
            Human manOB = Human.Cast( m_LinkedObject );
            if ( m_BoneIndex != -1 && manOB )
            {
                worldPosition = manOB.GetBonePositionWS( m_BoneIndex );
            }
            else
            {
                worldPosition = m_LinkedObject.GetPosition();
            }
        }
        else
        {
            worldPosition = m_MarkerPosition;
        }
        //Add offset to the vector, example: player
        worldPosition += m_OffsetPosition;

        vector posRelative = GetGame().GetScreenPosRelative( worldPosition );

        if ( posRelative[0] >= 1 || posRelative[0] <= 0 || posRelative[1] >= 1 || posRelative[1] <= 0 || posRelative[2] <= 0 )
        {
			if (layoutRoot)
			{
           		layoutRoot.Show( false );
           		return;
			}
        }

        //Detect if we are pointing at it, if so reduce the alpha accordingly
        float dist = vector.Distance(posRelative, Vector(0.5, 0.5, posRelative[2]));
        float alpha = LinearConversion( 0, 0.15, dist, m_MinAlphaColor, m_AlphaColor );

        m_MarkerText.SetColor( ARGB(alpha, 255, 255, 255) );
        m_MarkerDistanceText.SetColor( ARGB(alpha, 255, 255, 255) );
        m_MarkerIcon.SetColor( ARGB(alpha, m_RedColor, m_GreenColor, m_BlueColor) );

        Hud inGameHUD;
        Mission mission = Mission.Cast( GetGame().GetMission() );
		if( mission )
		{
            inGameHUD = mission.GetHud();
        }

        if ( layoutRoot && inGameHUD && !inGameHUD.GetHudState() )
        {
            layoutRoot.Show( false );
            return;
        }

        if ( layoutRoot && !layoutRoot.IsVisible() )
            layoutRoot.Show( true );

        //Set text here with the distance
        float distance = vector.Distance( GetGame().GetCurrentCameraPosition(), worldPosition );
        m_MarkerDistanceText.SetText( Math.Ceil( distance ).ToString() + "m" );

        //scale widget from the distance of player
        if ( m_MarkerFrame )
        {
            float scale = LinearConversion( 2000, 100, distance, 0.6, 1 );
            m_MarkerFrame.SetSize( m_OriginalWidth * scale, m_OriginalHeight * scale );
        }
		
		if ( layoutRoot )
        	layoutRoot.SetPos( posRelative[0], posRelative[1] );
    }
}