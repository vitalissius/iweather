#pragma once

#include "LanguagePack.h"

#include <algorithm>
#include <string>
#include <vector>

class AbstractPlaces {
protected:
    struct Place {
        std::string key;
        std::string place;

        Place(std::string key, std::string place);
    };

protected:
    AbstractPlaces() = default;

public:
    AbstractPlaces(AbstractPlaces&&) = default;
    virtual ~AbstractPlaces() = default;
    AbstractPlaces& operator=(AbstractPlaces&&) = default;
    std::vector<Place>::const_iterator begin() const;
    std::vector<Place>::const_iterator end() const;
    std::vector<Place>::size_type size() const;
    std::string GetKey(const std::string& place) const;

public:
    virtual void Update(const std::string& place) = 0;

private:
    virtual void parse(std::string page) = 0;

protected:
    std::vector<Place> m_places;
};
