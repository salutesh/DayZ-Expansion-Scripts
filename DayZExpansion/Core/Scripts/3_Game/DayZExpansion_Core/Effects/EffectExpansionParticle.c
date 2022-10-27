/**
 * EffectExpansionParticle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class EffectExpansionParticle : Effect
{
	protected vector			m_ParticleOrientation;
	protected bool				m_ForceRotationRelativeToWorld = false;
	protected Particle 			m_ParticleObj;
	protected int				m_ParticleID;
	protected vector			m_Orientation;
	protected Object 			m_Object;
	//=====================================
	// Constructor
	//=====================================
	void EffectExpansionParticle()
	{
		
	}
	
	//=====================================
	// Destructor
	//=====================================
	void ~EffectExpansionParticle()
	{
		Stop();
	}
	
	//=====================================
	// SetParticleID
	//=====================================
	void SetParticleID( int id )
	{
		m_ParticleID = id;
	}
	
	//! Set orientation in degrees (yaw, pitch, roll)
	//=====================================
	// SoundSetWaveKind
	//=====================================
	void SetOrientation( vector ori )
	{
		m_Orientation = ori;
	}
	
	//=====================================
	// SoundSetWaveKind
	//=====================================
	vector GetOrientation()
	{
		return m_Orientation;
	}
	
	//=====================================
	// GetAttachmentParent
	//=====================================
	override Object GetAttachmentParent()
	{
		return m_ParentObject;
	}
	
	//=====================================
	// GetAttachedLocalPos
	//=====================================
	override vector GetAttachedLocalPos()
	{
		return m_LocalPos;
	}
	
	//=====================================
	// GetAttachedLocalOri
	//=====================================
	override vector GetAttachedLocalOri()
	{
		return m_LocalOri;
	}
	
	//=====================================
	// SetAttachmentParent
	//=====================================
	override void SetAttachmentParent(Object obj)
	{
		m_ParentObject = obj;
	}
	
	//=====================================
	// ForceParticleRotationRelativeToWorld
	//=====================================
	void ForceParticleRotationRelativeToWorld(bool state)
	{
		m_ForceRotationRelativeToWorld = state;
	}
	
	//=====================================
	// IsParticleRotationRelativeToWorld
	//=====================================
	bool IsParticleRotationRelativeToWorld()
	{
		return m_ForceRotationRelativeToWorld;
	}
	
	//=====================================
	// SetAttachedLocalPos
	//=====================================
	override void SetAttachedLocalPos(vector pos)
	{
		m_LocalPos = pos;
	}
	
	//=====================================
	// SetAttachedLocalOri
	//=====================================
	override void SetAttachedLocalOri(vector ori)
	{
		m_LocalOri = ori;
	}

	//=====================================
	// SoundSetWaveKind
	//=====================================
	Particle GetParticle()
	{
		return m_ParticleObj;
	}
	
	//=====================================
	// SoundSetWaveKind
	//=====================================
	void SetParticle(Particle p)
	{
		m_ParticleObj = p;
	}
	
	//=====================================
	// CheckLifeSpan
	//=====================================
	void CheckLifeSpan()
	{
		if ( !m_ParticleObj )
		{
			delete this;
		}
		
		OnCheckUpdate();
	}
	
	//=====================================
	// AttachTo
	//=====================================
	void AttachTo(Object obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_rotation_to_world = false)
	{
		GetParticle().AddAsChild(obj, local_pos, local_ori, force_rotation_to_world);
	}
	
	// !Plays all elements this effects consists of
	//=====================================
	// Start
	//=====================================
	override void Start()
	{		
		if (m_ParticleID > 0)
		{
			Particle p;
			p = Particle.PlayInWorld(m_ParticleID, GetPosition());
			
			p.SetOrientation( GetOrientation() );
			//p.SetEffectHolder(this);
			
			SetParticle(p);
		}
		
		if ( GetAttachmentParent() )
		{
			AttachTo( GetAttachmentParent(), GetAttachedLocalPos(), GetAttachedLocalOri(), IsParticleRotationRelativeToWorld());
		}
		
		super.Start();
	}
	
	//! Stops all elements this effect consists of. Alternatively use SEffectManager.Stop( effect_id )
	//=====================================
	// Stop
	//=====================================
	override void Stop()
	{
		if ( GetParticle() )
		{
			GetParticle().Stop();
			SetParticle(NULL);
		}
	}
	
	//=====================================
	// Event_OnFrameUpdate
	//=====================================
	override void Event_OnFrameUpdate(float time_delta)
	{
		// Override this method for own use
	}
	
	//=====================================
	// Event_OnPlayStart
	//=====================================
	void Event_OnPlayStart()
	{
		// Override this method for own use
	}
	
	//=====================================
	// Event_OnPlayStarted
	//=====================================
	void Event_OnPlayStarted()
	{
		// Override this method for own use
	}
	
	//=====================================
	// OnCheckUpdate
	//=====================================
	override void OnCheckUpdate()
	{
		
	}
	
	//=====================================
	// SetDecalOwner
	//=====================================	
	void SetDecalOwner(Object o)
	{
		m_Object = o;
	}
};