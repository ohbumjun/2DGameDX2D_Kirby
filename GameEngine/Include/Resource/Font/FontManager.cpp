#include "FontManager.h"

CFontManager::CFontManager() :
m_WriteFactory(nullptr)
{}

CFontManager::~CFontManager()
{
	SAFE_RELEASE(m_WriteFactory);
}

bool CFontManager::Init()
{
	// Direct2D 및 DirectWrite 을 이용하여, 텍스트를 출력하기 위한 객체들
	// - ID2D1Factory 객체 생성
	// - IDWriteFactory 객체 생성
	// - ID2D1RenderTarget 객체 생성
	// - IDWriteTextFormat 객체 생성
	// - ID2D1SolidColorBrush 객체 생성

	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_WriteFactory),
		(IUnknown**)&m_WriteFactory)))
		return false;

	return true;
}

bool CFontManager::CreateFontFile(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	return false;
}
