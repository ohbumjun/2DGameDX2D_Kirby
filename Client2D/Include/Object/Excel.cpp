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
        // Logo �̹����� �ִ´�
        int logoId = book->addPicture(L"C:\\Users\\dhsys\\\Desktop\\\\Kirby.jpg");

        // ���� ���� �ȿ� Sheet �� ����� �� ���̴�.
        Sheet* sheet = book->addSheet(L"Sales Receipt");

        if (sheet)
        {
            // 2 �޸� 1 ���� 
            // sheet->writeStr(2, 1, L"Hello, World !");
            // sheet->writeNum(3, 1, 1000);

            Font* textFont = book->addFont();

            // fontTitle->setName(L"���� ���");
            textFont->setName(L"Century Gothic");
            textFont->setBold(true);
            textFont->setSize(10);

            Font* dateFont = book->addFont();

            // fontTitle->setName(L"���� ���");
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

            // ���� 2��° ĭ�� �� �ø���. 
            sheet->setCol(1, 1, 36);
            sheet->setCol(0, 0, 17);
            sheet->setCol(2, 8, 17);

            // row
            // 2��° row �� ũ�⸦ ũ�� �����Ѵ�.
            sheet->setRow(2, 47.25);

            // 2�� 1 ���� �ش� ���� ����, titleFormat�� �����Ѵ�.
            sheet->writeStr(2, 1, L"Monster Stats", titleFormat);

            // 2�� --> 1�� ~ 4�� ��ü�� Merge �� ���̴�.
            sheet->setMerge(2, 2, 1, 4);
            sheet->setPicture(2, 1, logoId);

            sheet->writeStr(4, 0, L"OH BUM JUN", companyFormat);
            sheet->writeStr(4, 3, L"Date : ", textFormat);
            sheet->writeFormula(4, 4, L"TODAY()", dateFormat);

            // Name, Monster ����, HPMax, AttackDistance, DashDistance, MoveVelocity, m_IsGroundObject, m_AttackAbility
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

        // �ٸ� Sheet ���� Kirby Sheet �����ϱ� 

        book->save(L"C:\\Users\\dhsys\\\Desktop\\TextExcel.xlsx");
         book->release();
    }
}

void CExcel::SaveExcel()
{}
