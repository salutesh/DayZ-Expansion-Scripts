modded class Math
{
	/**
	 * @brief Check whether point lies in rotated rectangle
	 * 
	 * @param min \p vector Minimums of rectangle ([0] and [2] will be used, as a rectangle is 2D)
	 * @param max \p vector Maximums of rectangle ([0] and [2] will be used, as a rectangle is 2D)
	 * @param width \p float Width of the short side of the rectangle
	 * @param p \p vector Point ([0] and [2] will be used, as a rectangle is 2D)
	 * 
	 * @return \p bool - True when point is in rectangle
	*/
	static bool IsPointInRotatedRectangle(vector min, vector max, float width, vector point)
    {
		float centerX = (min[0] + max[0]) * 0.5;
		float centerY = (min[2] + max[2]) * 0.5;

		float diagX = max[0] - min[0];
		float diagY = max[2] - min[2];
		float diagLength = Math.Sqrt(diagX * diagX + diagY * diagY);

		float diagUnitX = diagX / diagLength;
		float diagUnitY = diagY / diagLength;
		float perpUnitX = -diagUnitY;
		float perpUnitY = diagUnitX;

		float halfLength = diagLength * 0.5;
		float halfWidth = width * 0.5;

		//! Express point relative to rectangle's center
		float relX = point[0] - centerX;
		float relY = point[2] - centerY;

		//! Project the point onto the rectangle's axes
		float projLength = relX * diagUnitX + relY * diagUnitY;
		float projWidth = relX * perpUnitX + relY * perpUnitY;

		//! Check if the point is within bounds
		if (-halfLength <= projLength && projLength <= halfLength && -halfWidth <= projWidth && projWidth <= halfWidth)
			return true;

		return false;
    }
}
