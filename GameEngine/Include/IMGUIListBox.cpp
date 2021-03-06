#include "IMGUIListBox.h"

CIMGUIListBox::CIMGUIListBox() :
	m_Select(false),
	m_SelectIndex(-1),
	m_PageItemCount(3),
	m_Sort(false)
{
}

CIMGUIListBox::~CIMGUIListBox()
{
}

bool CIMGUIListBox::Init()
{
	return true;
}

void CIMGUIListBox::Render()
{
	ImVec2 Size = ImVec2(m_Size.x, m_PageItemCount * ImGui::GetTextLineHeightWithSpacing());

	//ImGui::Text(m_Name.c_str());

	if (ImGui::BeginListBox(m_Name.c_str(), Size))
	{
		size_t ItemCount = m_vecItemUTF8.size();

		for (size_t i = 0; i < ItemCount; ++i)
		{
			m_Select = m_SelectIndex == i;

			if (ImGui::Selectable(m_vecItemUTF8[i].c_str(), m_Select))
			{
				// if (m_SelectIndex != i && m_SelectCallback)
				if (m_SelectCallback)
					m_SelectCallback(static_cast<int>(i), m_vecItem[i].c_str());

				// ???? Index ???? Update
				m_SelectIndex = static_cast<int>(i);
			}

			if (m_Select)
			{
				// https://stackoverflow.com/questions/61853584/how-can-i-change-text-color-of-my-inputtext-in-imgui-unanswered
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndListBox();
	}
}
