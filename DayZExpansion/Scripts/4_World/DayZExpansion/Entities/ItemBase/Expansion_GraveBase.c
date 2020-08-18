class Expansion_GraveBase extends Container_Base
{
	protected bool m_ReceivedAttachments;
	protected int m_FliesIndex = -1;

	protected ref EffectParticle m_FliesEffect;
	protected ref EffectSound m_SoundFliesEffect;

	void Expansion_GraveBase()
	{
		RegisterNetSyncVariableBool("m_ReceivedAttachments");
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (!m_FliesEffect)
			m_FliesEffect = new EffSwarmingFlies();
				
		if (m_FliesEffect && !SEffectManager.IsEffectExist(m_FliesIndex))
		{
			m_FliesEffect.SetDecalOwner(this);
			m_FliesIndex = SEffectManager.PlayOnObject(m_FliesEffect, this, "0 0.25 0");

			Particle part = m_FliesEffect.GetParticle();
			AddChild(part, -1);

			if (!m_SoundFliesEffect)
				PlaySoundSetLoop(m_SoundFliesEffect,"Flies_SoundSet", 1.0, 1.0);
		}
	}

	override void EEDelete(EntityAI parent)
	{
		if (m_FliesEffect)
			m_FliesEffect.Stop();

		if (m_SoundFliesEffect)
			StopSoundSet(m_SoundFliesEffect);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(m_ReceivedAttachments))
			return false;

		return true;
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		ctx.Write(m_ReceivedAttachments);
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (m_ReceivedAttachments)
			return false;

		return super.CanReceiveAttachment(attachment, slotId);
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (GetInventory().AttachmentCount() < 1)
			Delete();
	}

	void SetReceivedAttachments(bool value)
	{
		m_ReceivedAttachments = value;
		SetSynchDirty();
	}
}

class Expansion_Gravecross extends Expansion_GraveBase {}
class Expansion_Gravecross_LowLifetime extends Expansion_GraveBase {}