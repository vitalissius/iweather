###Использование API доступа к погодным данным

- - -
Файлы, расположенные в папке `core/` являются ядром данного приложения, и их можно использовать отдельно. На данный момент можно воспользоваться одним из двух провайдеров погоды:  [Yahho](http://www.yahoo.com) или [Accuweather](http://www.accuweather.com).
Для получения списка городов подпадающих под введённый запрос необходимо использовать класс наследник от `AbstractPlaces`, а именно, класс `YahooWeather` или `AccuWeather`, соответствующие одноимённым провайдерам.
В тестовом примере для выбора провайдера используется функция `chooseProvider()`. В зависимости от введённого значения она вернёт значение `units::WeatherProvider::Yahoo` или `units::WeatherProvider::Accuweather`. Возвращённое значение необходимо передать в качестве аргумента в функцию `makePlaces()`. Данная функция вернёт указатель на экземпляр класса наследника от `AbstractPlaces`. Данный экземпляр класса будет являться статической переменной (класс `AbstractPlaces` реализует паттерн «Одиночка»).
В данной примере функция `makePlaces()` возвращает умный указатель на класс `AbstractPlaces`, но это не обязательно, можно возвращать **raw**-указатель на класс `AbstractPlaces`, так как созданный объект класса `YahooPlaces` или `AccuPlaces`, являясь статическим и не требует уничтожения до завершения приложения. При этом необходимо указать пользовательский удалитель с пустым телом, т.к. для указателя `AbstractPlaces*` не нужно вызывать оператор `delete`.
```cpp
auto deleter = [](AbstractPlaces* places) {};

std::unique_ptr<AbstractPlaces, decltype(deleter)> makePlaces(units::WeatherProvider provider)
{
    std::unique_ptr<AbstractPlaces, decltype(deleter)> places(nullptr, deleter);
    switch (provider)
    {
        case units::WeatherProvider::Yahoo:
            places.reset(YahooPlaces::Instance());
            break;
        default:
            places.reset(AccuPlaces::Instance());
            break;
    }
    return places;
}
```
Далее, для обновления состояния экземпляра класса наследника от `AbstractPlaces` (`YahooPlaces` или `AccuPlaces`), вызывается функция `updatePlaces()`, в которой требуется ввести город/место для поиска соответствий. При этом может выброситься исключение типа `IncorrectFile` в случае, если соответствий не будет найдено (в связи с тем что **xml**- или **json**-файл не будет содержать достаточных данных для обработки или будет пустым). При этом состояние экземпляра класса наследника `AbstractPlaces` не изменится – останется пустым. В таком случае необходимо повторить ввод искомого города. Также может выброситься исключение типа `NetworkError`, при отсутствии соединения к Интернету.
**Примечание!** В случае повторного запроса на поиск соответствий с другим городом/местом (или с тем же), состояние экземпляра класса наследника от `AbstractPlaces` (`YahooPlaces` или `AccuPlaces`) будет очищено и заполнено новыми данными.
```cpp
void updatePlaces(const std::unique_ptr<AbstractPlaces, decltype(deleter)>& places)
{
    while (true)
    {
        std::string place = inputPlace();
        try {
            places->Update(place);
            break;
        }
        catch (IncorrectFile&)
        {
            std::cout << "\nNot found. Try input another place! ";
            continue;
        }
    }
}
```
Каждый элемент внутри экземпляра класса наследника от `AbstractPlaces` (`YahooPlaces` или `AccuPlaces`), представляющий отдельный город/место, состоит из двух компонентов: *уникальный ключ* места и *название* города/места.
Если выполнение функции `updatePlaces()` прошло успешно, и был получен список соответствий, то доступ к нему можно получить с помощью итераторов на первый и следующий за последним элементы, используя цикл `for-each`. Также в данной функции запрашивается ввод индекса полученного списка элементов, чтобы с его помощью получить название города/места. А с помощью города/места получить доступ к уникальному ключу, соответствующему этому городу/месту (такая реализация заложена для возможности использования её с выпадающим списком). В дальнейшем по данному ключу будет производиться запрос данных о погоде.
Поиск конкретного ключа в списке осуществляется по значению города/места, с помощью метода `GetKey()`.
```cpp
std::string choosePlace(const AbstractPlaces* places)
{
    for (size_t i = 0; i < places->size(); ++i)
    {
        std::cout << i << ") " << (places->begin() + i)->place << "\n";
    }

    std::cout << "\nChoose place (index): ";

    std::string choose;
    do {
        std::getline(std::cin, choose);
        if (std::regex_match(choose, std::regex("[0-9]+")) && (std::stoul(choose) < places->size()))
        {
            break;
        }
    } while (std::cout << "Choose correct place (index): ");

    return places->GetKey((places->begin() + std::stoi(choose))->place);
}
```
После получения ключа из функции `choosePlace()`, который будет соответствовать запрашиваемому городу/месту, данное значение передаётся в виде аргумента функции `makeWeather()`. Данная функция возвращает экземпляр наследника класса `AbstractWeather`, которым может быть экземпляр класса `YahooWeather`, или класса `AccuWeather`, для соответствующего провайдера. Данная функция возвращает указатель на полноценный объект, так что для возвращаемого указателя на объект используется умный указатель с удалителем по умолчанию.
```cpp
std::unique_ptr<AbstractWeather> makeWeather(units::WeatherProvider provider)
{
    switch (provider)
    {
        case units::WeatherProvider::Yahoo:
            return std::make_unique<YahooWeather>();
        default:
            return std::make_unique<AccuWeather>();
    }
}
```
Получив конкретный объект наследника `AbstractWeather` (`YahooWeather` или `AccuWeather`), можно обновить его состояние в соответствии со значением ключа города/места. Это делается с помощью его метода `Update()`. При этом, до обновления состояния можно указать желаемый *язык* получаемых погодных данных. До или после обновления можно указать *формат времени*, указать *единицы измерения атмосферного давления*, *скорости ветра* и *температуры*, для выводимых данных. По умолчанию используется английский язык, время выводится в 12-часовом формате, атмосферное давление в атмосферах (atm), скорость ветра в километрах в час, температура в градусах Цельсия.
Формат времени указывается при помощи посылки сообщения экземпляру наследнику от `AbstractWeather` (`YahooWeather` или `AccuWeather`), соответствующим методом: `ChangeClockSystemTo12h()` или `ChangeClockSystemTo24h()`.
Единица измерения атмосферного давления задаётся методом `ChangePressureUnitTo()` с указанием конкретной единицы, например, `units::PressureUnits::PU_MILLIBARS` (по умолчанию используется `units::PressureUnits::PU_ATMOSPHERES`).
Единица измерения скорости ветра задаётся при помощи метода `ChangeSpeedUnitTo()` с указанием конкретной единицы, например, `units::SpeedUnits::SU_METERSPERSECOND` (по умолчанию используется `units::SpeedUnits::SU_KILOMETERSPERHOUR`).
Единица измерения температуры задаётся методом `ChangeTemperatureUnitTo()` с указанием конкретной единицы, например, `units::TemperatureUnits::TU_FAHRENHEIT` (по умолчанию используется `units::TemperatureUnits::TU_CELSIUS`).
А желаемый язык задаётся методом `ChangeLanguageTo()` с указанием конкретного языка, например, `LanguagePack::LanguageUnits::RUS` (по умолчанию используется `LanguagePack::LanguageUnits::ENG`).
При этом введённые данные сохраняются в экземпляре класса наследника от `AbstractWeather` (`YahooWeather` или `AccuWeather`), так что при обновлении его состояния нет необходимости каждый раз указывать желаемые единицы измерения или язык.
При получении данных о погоде (обновление состояния экземпляра класса наследника от `AbstractWeather`) доступ к ним можно получить с помощью соответствующих **get**-теров.
Дата (время) публикации: `GetPublicationDateLine()`.
Местоположение (формат: Город): `GetLocationCLine()`.
Местоположение (формат: Город, Страна): `GetLocationCcLine()`.
Местоположение (формат: Город, Страна (Регион/Район)): `GetLocationCcrLine()`.
Текущая температура: `GetConditionTemt()`.
Описание текущей погоды: `GetConditionText()`.
Текущая влажность: `GetHumidityLine()`.
Текущее атмосферное давление: `GetPressureLine()`.
Текущее состояние атмосферного давления: `GetPressureStateLine()`.
Текущая скорость ветра: `GetWindSpeedLine()`.
Текущее направление ветра: `GetWindDirectionLine()`.
Восход солнца: `GetSunriseLine()`.
Закат солнца: `GetSunsetLine()`.
Данные о прогнозе погоды на текущий и четыре последующих дня хранятся в векторе из пяти элементов, доступ к которому можно получить с помощью метода `GetForecastVector()`. Каждый элемент вектора хранит следующие данные, к которым можно получить доступ, используя соответствующие методы:
Код погодных условий (используется для соответствия картинке описывающей погодные условия): `GetCode()`.
Дата, на которую расчитан прогноз: `GetDate()`.
Сокращённое название дня: `GetDayName()`.
Максимальная температура (не обязательно дневная): `GetHighTemp()`.
Минимальная температура (не обязательно ночная): `GetLowTemp()`.
Описание погодных условий: `GetDescriptionText()`.
Функция `printWeather()` демонстрирует использование объекта наследника класса `AbstractWeather` (`YahooWeather` или `AccuWeather`), в котором содержатся указанные данные:
```cpp
void printWeather(const std::unique_ptr<AbstractWeather>& weather)
{
    static auto prints = [](const char* descr, const std::string& msg) {
        std::cout << std::setw(24) << descr << msg << "\n";
    };
    static auto printss = [](const char* descr, const std::string& msg1, const std::string& msg2) {
        std::cout << std::setw(24) << descr << msg1 << " (" << msg2 << ")" << "\n";
    };
    static auto printi = [](const char* descr, int msg) {
        std::cout << std::setw(24) << descr << msg << "\n";
    };

    prints("\nPublication date: ", weather->GetPublicationDateLine());
    prints("City: ", weather->GetLocationCLine());
    prints("City, country: ", weather->GetLocationCcLine());
    prints("City, country (region): ", weather->GetLocationCcrLine());
    printi("Temp: ", weather->GetConditionTemt());
    prints("Description: ", weather->GetConditionText());
    prints("Humidity: ", weather->GetHumidityLine());
    printss("Pressure: ", weather->GetPressureLine(), weather->GetPressureStateLine());
    printss("Pressure: ", weather->GetWindSpeedLine(), weather->GetWindDirectionLine());
    prints("Sunrise: ", weather->GetSunriseLine());
    prints("Sunset: ", weather->GetSunsetLine());

    std::cout << "Forecast:\nCode:\tDate:\t\tDay:\tHigh:\tLow:\tText:\n";
    for (const auto& forecast : weather->GetForecastVector())
    {
        std::cout << forecast.GetCode()
            << "\t" << forecast.GetDate()
            << "\t" << forecast.GetDayName()
            << "\t" << forecast.GetHighTemp()
            << "\t" << forecast.GetLowTemp()
            << "\t" << forecast.GetDescriptionText() << "\n";
    }
}
```
При этом необходимо учитывать, что при обновлении состояния объекта наследника класса `AbstractWeather` (вызове метода `Update()`) может быть выброшено исключение типа `NetworkError`, при отсутствии соединения к Интернету. Так же может быть выброшено исключение `IncorrectForecastSize` из-за того, что по какой-то причине прогноз погоды получен менее чем на пять дней (маловероятно), но при этом часто бывают случаи недоступности данных провайдера **Yahoo** (из-за использования устаревшего способа доступа к данным), в таком случае выбрасывается исключение `IncorrectFile`, указывающее на недоступность данных или их неполноту. По этой причине рекомендуется использовать провайдер **Accuweather**. Также необходимо учитывать, что при получении данных от **Accuweather** используется идентификатор приложения, который можно получить при регистрации на их сайте [developer.accuweather.com](http://developer.accuweather.com).
