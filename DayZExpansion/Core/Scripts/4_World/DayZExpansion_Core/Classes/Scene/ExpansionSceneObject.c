//TODO: maybe continue work on this? Not really needed ATM
class ExpansionSceneObject : ExpansionPrefabObject
{
	vector Position;
	vector Orientation;
	vector Scale;

	autoptr array<ref ExpansionSceneObject> Children = new array<ref ExpansionSceneObject>();

	override Object Spawn(Object self)
	{
		self = super.Spawn(self);



		return self;
	}
};
