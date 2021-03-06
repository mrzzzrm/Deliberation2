#include <Deliberation/Resource/PrototypeManager.h>

#include <fstream>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Json.h>

#include <Deliberation/Resource/AbstractPrototype.h>

namespace deliberation
{

template<typename PrototypeType>
void PrototypeManager::addPath(const std::string & path)
{
    const auto typeID = TypeID::value<PrototypeManager, PrototypeType>();
    auto iter = m_prototypeTypeContainersByTypeID.find(typeID);
    if (iter == m_prototypeTypeContainersByTypeID.end())
    {
        std::function<std::shared_ptr<AbstractPrototype>()> prototypeBuilder = [this]() {
            auto prototype = std::make_shared<PrototypeType>(shared_from_this());
            return std::static_pointer_cast<AbstractPrototype>(prototype);
        };

        iter = m_prototypeTypeContainersByTypeID.emplace(typeID, prototypeBuilder).first;
    }

    auto & container = iter->second;
    container.paths.emplace_back(path);
}

template<typename PrototypeType>
std::shared_ptr<const PrototypeType> PrototypeManager::prototype(const std::string & name) const
{
    const auto iter = m_prototypeTypeContainersByTypeID.find(TypeID::value<PrototypeManager, PrototypeType>());
    AssertM(iter != m_prototypeTypeContainersByTypeID.end(),
           std::string("No such prototype container for '") + typeid(PrototypeType).name() + "'")

    const auto & container = iter->second;

    const auto iter2 = container.prototypeByName.find(name);
    AssertM(iter2 != container.prototypeByName.end(),
           "No such prototype '" + name + "' of type '" + typeid(name).name() + "'");

    return std::static_pointer_cast<const PrototypeType>(iter2->second);
}

template<typename PrototypeType>
std::shared_ptr<const PrototypeType> PrototypeManager::getOrCreatePrototype(const std::string &name) {
    const auto iter = m_prototypeTypeContainersByTypeID.find(TypeID::value<PrototypeManager, PrototypeType>());
    AssertM(iter != m_prototypeTypeContainersByTypeID.end(),
           std::string("No such prototype container for '") + typeid(PrototypeType).name() + "'")

    auto & container = iter->second;

    auto iter2 = container.prototypeByName.find(name);
    if (iter2 == container.prototypeByName.end()) {
        iter2 = container.prototypeByName.emplace(name, container.prototypeFactory()).first;
    }

    return std::static_pointer_cast<const PrototypeType>(iter2->second);
}

}