class ExpansionLootPosition
{
    vector pos;
    float height
    float range;
    int flags;
	
	[NonSerialized()]
	Object cylinder;

	void ClearVisual()
	{
		if ( cylinder == NULL )
			return;

		GetGame().ObjectDelete( cylinder );
	}

	void GenerateVisual( Object target, vector bbCenter )
	{
		ClearVisual();

		cylinder = g_Game.CreateObjectEx( "ExpansionDebugCylinder", "0 0 0", ECE_CREATEPHYSICS ); 
		target.AddChild( cylinder, -1 );

        vector transform[4] =
		{ 
            "1 0 0 0"
            "0 1 0 0" 
            "0 0 1 0"
            "0 0 0 1"
		};

		transform[0][0] = range * 2.0;
		transform[1][1] = height * 2.0;
		transform[2][2] = range * 2.0;

        transform[3][0] = bbCenter[0] - pos[2];
        transform[3][1] = bbCenter[1] + pos[1];
        transform[3][2] = bbCenter[2] + pos[0];
        transform[3][3] = 1.0;

        cylinder.SetTransform( transform );

		target.Update();
	}
};

class ExpansionXMLMapGroupCallback : XMLCallback
{
	private map< string, ref array< ref ExpansionLootPosition > > _positions;

	private bool _isFinished;
	
	void ExpansionXMLMapGroupCallback( ref map< string, ref array< ref ExpansionLootPosition > > positions )
	{
		//Print( "+ExpansionXMLMapGroupCallback::ExpansionXMLMapGroupCallback" );

		_positions = positions;

		// Print( "-ExpansionXMLMapGroupCallback::ExpansionXMLMapGroupCallback" );
	}

	bool IsFinished()
	{
		return _isFinished;
	}

    override void OnStart( ref XMLDocument document )
    {
		//Print( "+ExpansionXMLMapGroupCallback::OnStart document=" + document );
		//Print( "-ExpansionXMLMapGroupCallback::OnStart" );
    }

    override void OnSuccess( ref XMLDocument document )
    {
		//Print( "+ExpansionXMLMapGroupCallback::OnSuccess document=" + document );

		XMLElement ele = document.Get( 1 ).GetContent();

		for ( int i = 0; i < ele.Count(); ++i )
		{
			XMLTag tag = ele.Get( i );
			if ( tag.GetName() != "group" )
				continue;

			array< ref ExpansionLootPosition > positions = new array< ref ExpansionLootPosition >;
			_positions.Insert( tag.GetAttribute( "name" ).ValueAsString(), positions );
			
			array< XMLTag > containers = tag.GetContent().Get( "container" );
			
			for ( int j = 0; j < containers.Count(); ++j )
			{
				array< XMLTag > points = containers[j].GetContent().Get( "point" );

				for ( int k = 0; k < points.Count(); ++k )
				{
					ExpansionLootPosition position = new ExpansionLootPosition;
					XMLAttribute attrib = NULL;

					attrib = points[k].GetAttribute( "pos" );
					if ( attrib )
						position.pos = attrib.ValueAsVector();

					attrib = points[k].GetAttribute( "height" );
					if ( attrib )
						position.height = attrib.ValueAsFloat();

					attrib = points[k].GetAttribute( "range" );
					if ( attrib )
						position.range = attrib.ValueAsFloat();

					attrib = points[k].GetAttribute( "flags" );
					if ( attrib )
						position.flags = attrib.ValueAsInt();

					positions.Insert( position );
				}
			}
		}

		_isFinished = true;

		//Print( "-ExpansionXMLMapGroupCallback::OnStart" );
    }

    override void OnFailure( ref XMLDocument document )
    {
		//Print( "+ExpansionXMLMapGroupCallback::OnFailure document=" + document );
		//Print( "-ExpansionXMLMapGroupCallback::OnFailure" );
    }
};

static ref map< string, ref array< ref ExpansionLootPosition > > _positions;
static ref ExpansionXMLMapGroupCallback _callback;

static void CreateLootPositionsData()
{
	//Print( "+::CreateLootPositionsData _callback=" + _callback );

	if ( _callback == NULL )
	{
		_positions = new map< string, ref array< ref ExpansionLootPosition > >;
		_callback = new ExpansionXMLMapGroupCallback( _positions );

		GetXMLApi().Read( "$CurrentDir:missions\\ExpansionCOM.sandbox\\mapGroupProto.xml", _callback );
	}
	
	//Print( "-::CreateLootPositionsData" );
}

static void ExpansionLootPositions_OnKeyPress( int key )
{
	array< ref ExpansionLootPosition > positions;
	Object selectedObject;
	int j;

	if ( key == KeyCode.KC_J )
	{
		CreateLootPositionsData();

		if ( _callback.IsFinished() )
		{
			selectedObject = ( ( ObjectEditor ) GetCOMModuleManager().GetModule( ObjectEditor ) ).m_SelectedObject;
			if ( selectedObject )
			{
				positions = _positions.Get( selectedObject.GetType() );
				if ( positions )
				{
					vector min, max;
					selectedObject.GetBounds( min, max );   

					float x1 = min[0];
					float z1 = min[1];
					float y1 = min[2];
				
					float x2 = max[0];
					float z2 = max[1];
					float y2 = max[2];

					vector bbCenter = Vector( x1 + x2, z1 + z2, y1 + y2 ) * 0.5;

					for ( j = 0; j < positions.Count(); ++j )
					{
						positions[j].GenerateVisual( selectedObject, bbCenter );
					}
				}
			}
		}
	} else if ( key == KeyCode.KC_L )
	{
		if ( _callback.IsFinished() )
		{
			selectedObject = ( ( ObjectEditor ) GetCOMModuleManager().GetModule( ObjectEditor ) ).m_SelectedObject;
			if ( selectedObject )
			{
				positions = _positions.Get( selectedObject.GetType() );

				for ( j = 0; j < positions.Count(); ++j )
				{
					positions[j].ClearVisual();
				}
			}
		}
	}
}