#include "AccuPlaces.h"

AccuPlaces* AccuPlaces::Instance()
{
    static AccuPlaces places;
    return &places;
}

void AccuPlaces::Update(const std::string& place, const LanguagePack::LanguageUnits lang)
{
    if (!m_places.empty())
    {
        m_places.clear();
    }
    std::string page = downloadData("dataservice.accuweather.com", place,
        DataResource::ACCU_LOCATIONS, lang);
    parse(std::move(page));
}

void AccuPlaces::parse(std::string page)
{
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(page, root))
    {
        throw IncorrectFile("Json places file: parsing error");
    }

    if (root.empty())
    {
        throw IncorrectFile("No data: empty json file");
    }

    for (Json::ArrayIndex i = 0; i < root.size(); ++i)
    {
        Json::Value value = root[i];

        std::string key = value["Key"].asString();
        std::string type = value["Type"].asString();
        std::string name = value["LocalizedName"].asString();
        std::string country = value["Country"]["LocalizedName"].asString();
        std::string area = value["AdministrativeArea"]["LocalizedName"].asString();

        std::string place = makePlace(std::move(type), std::move(name), std::move(country), std::move(area));
        m_places.emplace_back(std::move(key), std::move(place));
    }
}

std::string AccuPlaces::makePlace(std::string type, std::string name, std::string country, std::string area)
{
    std::stringstream place;
    place << type << ": " << name << " " << country << " (" << area << ")";
    return place.str();
}
