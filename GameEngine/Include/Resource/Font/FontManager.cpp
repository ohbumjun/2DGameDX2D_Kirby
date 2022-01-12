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
	// Direct2D �� DirectWrite �� �̿��Ͽ�, �ؽ�Ʈ�� ����ϱ� ���� ��ü��
	// - ID2D1Factory ��ü ����
	// - IDWriteFactory ��ü ����
	// - ID2D1RenderTarget ��ü ����
	// - IDWriteTextFormat ��ü ����
	// - ID2D1SolidColorBrush ��ü ����

	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_WriteFactory),
		(IUnknown**)&m_WriteFactory)))
		return false;

	return true;
}

bool CFontManager::CreateFontFile(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	return false;
}
