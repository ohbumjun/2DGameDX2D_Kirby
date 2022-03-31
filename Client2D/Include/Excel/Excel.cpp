#include "Excel.h"
#include <fstream>
#include <sstream>
#include "../libxl-4.0.3.1/include_cpp/libxl.h"

CExcel* CExcel::m_Inst = nullptr;

CExcel::CExcel()
{}

CExcel::~CExcel()
{
    auto iter = m_MapMonsterStats.begin();
    auto iterEnd = m_MapMonsterStats.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE(iter->second);
    }
}

void CExcel::LoadExcel()
{
    using namespace libxl;

    // Monster Stats

    Book* book = xlCreateXMLBook();

    TCHAR LExcel_Path[MAX_PATH] = {};

    int ConvertLength = MultiByteToWideChar(CP_ACP, 0, EXCEL_PATH, -1, 0, 0);
    MultiByteToWideChar(CP_ACP, 0, EXCEL_PATH, -1, LExcel_Path, ConvertLength);

    if (book->load(LExcel_Path))
    // if (book->load(EXCEL_PATH))
    {
        Sheet* sheet = book->getSheet(0);

        if (sheet)
        {
            // for (int row = sheet->firstRow(); row < sheet->lastRow(); ++row)
            for (int row = 8; row < sheet->lastRow(); ++row)
            {
                CellType cellType = sheet->cellType(row, 0);

                switch (cellType)
                {
                case CELLTYPE_STRING:
                {
                    const wchar_t* s = sheet->readStr(row, 0);

                    std::wstring w(s);

                    Monster_Stat* Stat = new Monster_Stat;

                    m_MapMonsterStats.insert(std::make_pair(std::wstring(s), Stat));

                    bool IsAbility = false;
                    bool IsBoss = false;

                    for (int col = 1; col < sheet->lastCol(); ++col)
                    {
                        CellType cellType = sheet->cellType(row, col);

                        switch (col)
                        {
                        case 1:
                        {
                           // std::cout << "Type : ";
                            std::wstring TypeName = std::wstring(sheet->readStr(row, col));

                            if (TypeName == L"Normal")
                            {
                                Stat->m_Type = Monster_Type::Normal;
                            }
                            else if (TypeName == L"Water")
                            {
                                Stat->m_Type = Monster_Type::Water;
                            }
                            else if (TypeName == L"Ability")
                            {
                                IsAbility = true;
                                Stat->m_Type = Monster_Type::Ability;
                            }
                            else
                            {
                                Stat->m_Type = Monster_Type::Boss;
                            }
                        }
                        break;
                        case 2:
                        {
                            float d = (float)sheet->readNum(row, col);
                            Stat->m_HP = d;
                        }
                        break;
                        case 3:
                        {
                            float d = (float)sheet->readNum(row, col);
                            Stat->m_AttackDist = d;
                        }
                        break;
                        case 4:
                        {
                            float d = (float)sheet->readNum(row, col);
                            Stat->m_DashDist = d;
                        }
                        break;
                        case 5:
                        {
                            float d = (float)sheet->readNum(row, col);
                            Stat->m_MoveVelocity = d;
                        }
                        break;
                        case 6:
                        {
                            bool d = sheet->readBool(row, col);
                            Stat->m_IsGroundObject = d;
                        }
                        break;
                        case 7:
                        {
                            float d = (float)sheet->readNum(row, col);
                            Stat->m_AttackAbility = d;
                        }
                        break;
                        case 8:
                        {
                            float d = (float)sheet->readNum(row, col);
                            Stat->m_Scale = d;
                        }
                        break;
                        case 9:
                        {
                            float d = (float)sheet->readNum(row, col);
                            Stat->m_JumpVelocity = d;
                        }
                        break;
                        case 10:
                        {
                            float d = (float)sheet->readNum(row, col);
                            Stat->m_CloseAttackDist = d;
                        }
                        break;
                        case 11:
                        {
                            float d = (float)sheet->readNum(row, col);
                            Stat->m_FarAttackDist = d;
                        }
                        break;
                        case 12:
                        {
                            if (IsAbility)
                            {
                                std::wstring AbilityState = std::wstring(sheet->readStr(row, col));

								if (AbilityState == L"Bomb")
								{
                                    Stat->m_AbilityState = Ability_State::Bomb;
								}
                                else if (AbilityState == L"Beam")
                                {
                                    Stat->m_AbilityState = Ability_State::Beam;
                                }
                                else if (AbilityState == L"Fight")
                                {
                                    Stat->m_AbilityState = Ability_State::Fight;
                                }
                                else if (AbilityState == L"Fire")
                                {
                                    Stat->m_AbilityState = Ability_State::Fire;
                                }
                                else if (AbilityState == L"Sword")
                                {
                                    Stat->m_AbilityState = Ability_State::Sword;
                                }
                            }
                        }
                        break;
                        } // enum 문
                    } // for 문
                } // case 문
                break;
                case CELLTYPE_EMPTY:
                {
                    return ;
                }
                }
            }
        }
    }
}

void CExcel::SaveExcel()
{
    // Lib
    using namespace libxl;

    Book* book = xlCreateXMLBook();

    if (book)
    {
        // Logo 이미지를 넣는다
        int logoId = book->addPicture(L"C:\\Users\\dhsys\\Desktop\\Kirby.jpg");

        // 이제 엑셀 안에 Sheet 을 만들게 될 것이다.
        Sheet* sheet = book->addSheet(L"MonsterStat");


        if (sheet)
        {
            // 2 콤마 1 번지 
            // sheet->writeStr(2, 1, L"Hello, World !");
            // sheet->writeNum(3, 1, 1000);

            Font* textFont = book->addFont();

            // fontTitle->setName(L"맑은 고딕");
            textFont->setName(L"Century Gothic");
            textFont->setBold(true);
            textFont->setSize(10);

            Font* dateFont = book->addFont();

            // fontTitle->setName(L"맑은 고딕");
            dateFont->setName(L"Century Gothic");
            dateFont->setBold(true);
            dateFont->setSize(8);

            Font* titleFont = book->addFont(textFont);
            titleFont->setSize(38);
            titleFont->setColor(COLOR_GRAY25);

            Font* font12 = book->addFont(textFont);
            font12->setSize(14);

            Font* font10 = book->addFont(textFont);
            font10->setSize(12);

            // Formats
            Format* textFormat = book->addFormat();
            textFormat->setFont(textFont);
            textFormat->setAlignH(ALIGNH_LEFT);


            Format* titleFormat = book->addFormat();
            titleFormat->setFont(titleFont);
            titleFormat->setAlignH(ALIGNH_RIGHT);

            Format* companyFormat = book->addFormat();
            companyFormat->setFont(font12);


            Format* dateFormat = book->addFormat();
            dateFormat->setFont(dateFont);
            dateFormat->setAlignH(ALIGNH_LEFT);
            dateFormat->setNumFormat(book->addCustomNumFormat(L"[$-409]mmmm\\ d\\,\\ yyyy;@"));

            Format* borderFormat = book->addFormat(textFormat);
            borderFormat->setBorder();
            borderFormat->setBorderColor(COLOR_GRAY25);
            borderFormat->setAlignV(ALIGNV_CENTER);

            Format* textRightFormat = book->addFormat(textFormat);
            textRightFormat->setAlignH(ALIGNH_RIGHT);
            textRightFormat->setAlignV(ALIGNV_CENTER);

            Format* thankFormat = book->addFormat();
            thankFormat->setFont(font10);
            thankFormat->setAlignH(ALIGNH_CENTER);

            // actions
            sheet->setDisplayGridlines(false);

            // 세로 2번째 칸을 쫙 늘린다. 
            sheet->setCol(1, 1, 17);
            sheet->setCol(0, 0, 15);
            sheet->setCol(2, 13, 15);

            // row
            // 2번째 row 의 크기를 크게 세팅한다.
            sheet->setRow(2, 47.25);

            // 2행 1 열에 해당 값을 쓰고, titleFormat을 적용한다.
            sheet->writeStr(2, 1, L"Monster Stats", titleFormat);

            // 2행 --> 1열 ~ 4열 전체를 Merge 할 것이다.
            sheet->setMerge(2, 2, 1, 4);
            sheet->setPicture(2, 1, logoId);

            sheet->writeStr(4, 0, L"OH BUM JUN", companyFormat);
            sheet->writeStr(4, 3, L"Date : ", textFormat);
            sheet->writeFormula(4, 4, L"TODAY()", dateFormat);

            // Name, Monster 종류, Monster_Type, HPMax, AttackDistance, DashDistance, MoveVelocity, m_IsGroundObject, m_AttackAbility
            sheet->writeStr(7, 0, L"Name", textFormat);
            sheet->writeStr(7, 1, L"Type", textFormat);
            sheet->writeStr(7, 2, L"HP", textFormat);
            sheet->writeStr(7, 3, L"AttackDist", textFormat);
            sheet->writeStr(7, 4, L"DashDist", textFormat);
            sheet->writeStr(7, 5, L"MoveVelocity", textFormat);
            sheet->writeStr(7, 6, L"IsGroundObject", textFormat);
            sheet->writeStr(7, 7, L"AttackAbility", textFormat);
            sheet->writeStr(7, 8, L"Scale", textFormat);
            sheet->writeStr(7, 9, L"JumpVelocity", textFormat);
            sheet->writeStr(7, 10, L"CloseAttackDist", textFormat);
            sheet->writeStr(7, 11, L"FarAttackDist", textFormat);
            sheet->writeStr(7, 12, L"AbilityState", textFormat);

            for (int row = 8; row < 30; ++row)
            {
                sheet->setRow(row, 28);

                for (int col = 0; col < 13; ++col)
                {
                    sheet->writeBlank(row, col, borderFormat);
                }
            }

        }

        // 다른 Sheet 에는 Kirby Sheet 세팅하기 

        // book->save(L"C:\\Users\\dhsys\\\Desktop\\TextExcel.xlsx");
        book->save(L"C:\\Users\\dhsys\\DX_Project\\TextExcel.xlsx");
        book->release();

        getchar();
    }
}

bool CExcel::Init()
{
    return true;;
}

Monster_Stat* CExcel::FindMonsterState(const std::wstring& MonsterName)
{
    auto iter = m_MapMonsterStats.find(MonsterName);

    if (iter == m_MapMonsterStats.end())
        return nullptr;

    return iter->second;
}
