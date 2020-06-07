#include "GameObject.h"

GameObject::GameObject()
{

}

const char* GameObject::GetSymbol() const
{
   return "G";
}

std::ostream& operator<<(std::ostream& os, const GameObject& go)
{
    os << go.GetSymbol();
    return os;
}