
#include "UINode.h"

class UIText : public UINode
{
public:

	void         SetText(const std::string& _text);
	std::string  GetText() const;

private:
	std::string m_Text;
};