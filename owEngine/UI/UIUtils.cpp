#include "stdafx.h"

// General
#include "UIUtils.h"

glm::vec2 owUIUtils::CalculateChildsVerticalSize(const std::vector<std::shared_ptr<CUIBaseNode>>& Childs, float HorizontalSpace)
{
    glm::vec2 size = glm::vec2(0.0f, 0.0f);

    for (auto childs : Childs)
    {
        if (childs->GetSize().x > size.x)
            size.x = childs->GetSize().x;

        size.y += childs->GetSize().y;
    }

    return size;
}
