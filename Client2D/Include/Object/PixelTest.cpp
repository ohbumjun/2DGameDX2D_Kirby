#include "PixelTest.h"
#include "Component/ColliderPixel.h"

CPixelTest::CPixelTest()
{}

CPixelTest::CPixelTest(const CPixelTest& Pixel) : CGameObject(Pixel)
{
	m_Body = Pixel.m_Body;
}

CPixelTest::~CPixelTest()
{}

bool CPixelTest::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Body = CreateComponent<CColliderPixel>("PixelBody");

	SetRootComponent(m_Body);

	m_Body->SetInfo(TEXT("PixelCollision.png"));
	m_Body->SetPixelCollisionType(PixelCollision_Type::Color_Ignore);
	m_Body->SetPixelColor(255, 0, 255);

	return true;
}

CPixelTest* CPixelTest::Clone()
{
	return new CPixelTest(*this);
}
