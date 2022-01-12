#pragma once

#include "../../GameInfo.h"

// http://egloos.zum.com/Lusain/v/3184533

class CFontManager
{
	friend class CResourceManager;
private :
	CFontManager();
	~CFontManager();
private :
	IDWriteFactory5* m_WriteFactory;
	// DWrite 에서 시스템 폰트 이외의 폰트를 사용하는 방법은 Font Collection을 만드는 방법
	std::unordered_map<std::string, IDWriteFontCollection1*> m_mapFontFile;
	std::unordered_map<std::string, IDWriteTextFormat*> m_mapFont;
	std::unordered_map<std::string, ID2D1SolidColorBrush*> m_mapFontColor;
public :
	bool Init();
	bool CreateFontFile(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = FONT_PATH);
};

