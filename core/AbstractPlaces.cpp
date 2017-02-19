#include "AbstractPlaces.h"

AbstractPlaces::Place::Place(std::string key, std::string place)
    : key(std::move(key))
    , place(std::move(place))
{}

std::vector<AbstractPlaces::Place>::const_iterator AbstractPlaces::begin() const
{
    return m_places.begin();
}

std::vector<AbstractPlaces::Place>::const_iterator AbstractPlaces::end() const
{
    return m_places.end();
}

std::vector<AbstractPlaces::Place>::size_type AbstractPlaces::size() const
{
    return m_places.size();
}

const AbstractPlaces::Place& AbstractPlaces::at(size_t pos) const
{
    return m_places.at(pos);
}

std::string AbstractPlaces::GetKey(const std::string& place) const
{
    auto it = std::find_if(m_places.begin(), m_places.end(), [&place](const Place& e)
    {
        return place == e.place;
    });
    return (it != m_places.end()) ? it->key : "";
}
