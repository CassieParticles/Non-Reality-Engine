#pragma once

#include <graphicsEngine/View.h>

//Class used to calculate oblique projection matrix used within portal

class ObliqueView :public View
{
public:
	ObliqueView();
	ObliqueView(ObliqueView &other);
	ObliqueView(ObliqueView&& other);
	~ObliqueView() = default;

	//Calculates a oblique perspective projection matrix, where the near clipping plane is described by nearClipPlane
	void setProjectionMatrixNearClip(float FOV, float aspectRatio, float nearZ, float farZ, DirectX::XMFLOAT4 nearClipPlane);
protected:

};