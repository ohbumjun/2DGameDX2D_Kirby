#include "Excel.h"
#include <fstream>
#include <sstream>
#include "../libxl-4.0.3.1/include_cpp/libxl.h"

CExcel* CExcel::m_Inst = nullptr;

CExcel::CExcel()
{}

CExcel::~CExcel()
{}

void CExcel::LoadExcel()
{
    // Lib
    using namespace libxl;

    libxl::Book* book = xlCreateXMLBook();

    if (book)
    {
        // Logo 이미지를 넣는다
        int logoId = book->addPicture(L"C:\\Users\\dhsys\\\Desktop\\\\Kirby.jpg");

        // 이제 엑셀 안에 Sheet 을 만들게 될 것이다.
        Sheet* sheet = book->addSheet(L"Sales Receipt");

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
            sheet->setCol(1, 1, 36);
            sheet->setCol(0, 0, 17);
            sheet->setCol(2, 8, 17);

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

            // Name, Monster 종류, HPMax, AttackDistance, DashDistance, MoveVelocity, m_IsGroundObject, m_AttackAbility
            sheet->writeStr(7, 0, L"Name", textFormat);
            sheet->writeStr(7, 1, L"Kind", textFormat);
            sheet->writeStr(7, 2, L"HP", textFormat);
            sheet->writeStr(7, 3, L"AttackDist", textFormat);
            sheet->writeStr(7, 4, L"DashDist", textFormat);
            sheet->writeStr(7, 5, L"MoveVelocity", textFormat);
            sheet->writeStr(7, 6, L"IsGroundObject", textFormat);
            sheet->writeStr(7, 7, L"AttackAbility", textFormat);

            for (int row = 8; row < 30; ++row)
            {
                sheet->setRow(row, 28);

                for (int col = 0; col < 8; ++col)
                {
                    sheet->writeBlank(row, col, borderFormat);
                }
            }
        }

        // 다른 Sheet 에는 Kirby Sheet 세팅하기 

        book->save(L"C:\\Users\\dhsys\\\Desktop\\TextExcel.xlsx");
         book->release();
    }
}

void CExcel::SaveExcel()
{}
