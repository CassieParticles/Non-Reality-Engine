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

	//Calculates the oblique projection matrix from the existing projection matrix
	DirectX::XMFLOAT4X4 calcProjectionMatrixNearClip(DirectX::XMFLOAT4 nearClipPlane);
protected:

};