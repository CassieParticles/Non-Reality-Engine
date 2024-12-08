#include "ObliqueView.h"
#include <graphicsEngine/VectorMathOverloads.h>

ObliqueView::ObliqueView():View{}
{
}

ObliqueView::ObliqueView(ObliqueView& other):View{other}
{

}

ObliqueView::ObliqueView(ObliqueView&& other) :View{ other }
{

}

//Derived from : https://terathon.com/lengyel/Lengyel-Oblique.pdf
void ObliqueView::setProjectionMatrixNearClip(float FOV, float aspectRatio, float nearZ, float farZ, DirectX::XMFLOAT4 nearClipPlane)
{
	//Calculate the projection matrix to be modified
	setProjectionMatrixPespective(FOV, aspectRatio, nearZ, farZ);

	DirectX::XMVECTOR c = DirectX::XMLoadFloat4(&nearClipPlane);
	//Get the transpose of the inverse of the projection matrix
	DirectX::XMMATRIX m = DirectX::XMLoadFloat4x4(&projectionMatrix);
	DirectX::XMMATRIX invM = DirectX::XMMatrixInverse(nullptr, m);
	DirectX::XMMATRIX invTrM = DirectX::XMMatrixTranspose(invM);

	//Get plane C in clip space
	DirectX::XMVECTOR cClip = DirectX::XMVector4Transform(c,invTrM);
	//Get cClip as an XMFLOAT4 because I need to do that to access it's components for some reason
	DirectX::XMFLOAT4 cClipComp;
	DirectX::XMStoreFloat4(&cClipComp, cClip);

	//Get q in clip space (create as XMFLOAT4 then load into XMVECTOR cause this library was designed by a 7 year old)
	float xSign = (cClipComp.x > 0) ? 1.0f : ((cClipComp.x < 0) ? -1.0f : 0.0f);
	float ySign = (cClipComp.y > 0) ? 1.0f : ((cClipComp.y < 0) ? -1.0f : 0.0f);
	DirectX::XMFLOAT4 qClipComp = {xSign,ySign,1,1};
	DirectX::XMVECTOR qClip = DirectX::XMLoadFloat4(&qClipComp);

	//Get q in camera space by transforming it by the inverse of the projection matrix
	DirectX::XMVECTOR q = DirectX::XMVector4Transform(qClip, invM);
	//Get q as an XMFLOAT4 cause I need access to the z component, DirectXMath is really cool and practical guys
	DirectX::XMFLOAT4 qComp;
	DirectX::XMStoreFloat4(&qComp, q);
	
	//Get the dot product of c and q, this is returned as a vector where every component is the dot product, idk why
	DirectX::XMVECTOR cDotQVec=DirectX::XMVector4Dot(c,q);
	//Get the cDotQVec as a XMFLOAT4 to extract the dot product, this library fucking sucks 
	DirectX::XMFLOAT4 cDotQVecComp;
	DirectX::XMStoreFloat4(&cDotQVecComp, cDotQVec);
	float cDotQ = cDotQVecComp.x;

	float a = qComp.z / cDotQ;

	//Calculate the new column 3 of the matrix, then convert into an XMFLOAT4, since nothing is allowed to be easy
	DirectX::XMVECTOR m3= DirectX::XMVectorScale(c, a);
	DirectX::XMFLOAT4 m3Comp;
	DirectX::XMStoreFloat4(&m3Comp, m3);

	//Override column 3 in the matrix with updated 
	projectionMatrix._13 = m3Comp.x;
	projectionMatrix._23 = m3Comp.y;
	projectionMatrix._33 = m3Comp.z;
	projectionMatrix._43 = m3Comp.w;
}