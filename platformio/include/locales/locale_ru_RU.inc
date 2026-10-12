#include "_locale.h"
#include <vector>
#include <Arduino.h>

// !!! использование русских буковок ломает вызовы спецсимволов больше 255 таких как \260 

// LC_TIME
const char *LC_D_T_FMT     = "%a %d %b %Y %T %Z";
const char *LC_D_FMT       = "%d.%m.%y";
const char *LC_T_FMT       = "%T";
const char *LC_T_FMT_AMPM  = "";
const char *LC_AM_STR      = "";
const char *LC_PM_STR      = "";
// Дни недели
const char *LC_DAY[7]      = {"Воскресенье", "Понедельник", "Вторник", "Среда",
                              "Четверг", "Пятница", "Суббота"};
// Сокращенные дни недели
const char *LC_ABDAY[7]    = {"Вс", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"};
// Месяцы
const char *LC_MON[12]     = {"Январь", "Февраль", "Март", "Апрель",
                              "Май", "Июнь", "Июль", "Август",
                              "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};
// Сокращенные месяцы
const char *LC_ABMON[12]   = {"Янв", "Фев", "Мар", "Апр", "Май", "Июн",
                              "Июл", "Авг", "Сен", "Окт", "Нов", "Дек"};
const char *LC_ERA         = "";
const char *LC_ERA_D_FMT   = "";
const char *LC_ERA_D_T_FMT = "";
const char *LC_ERA_T_FMT   = "";

// OWM LANGUAGE
const String OWM_LANG = "ru";

// ТЕКУЩИЕ УСЛОВИЯ
const char *TXT_FEELS_LIKE         = "Ощущается как";
const char *TXT_SUNRISE            = "Восход";
const char *TXT_SUNSET             = "Закат";
const char *TXT_MOONRISE           = "Восход луны";
const char *TXT_MOONSET            = "Заход луны";
const char *TXT_WIND               = "Ветер";
const char *TXT_HUMIDITY           = "Влажность";
const char *TXT_UV_INDEX           = "Индекс УФ";
const char *TXT_PRESSURE           = "Давление";
const char *TXT_AIR_QUALITY        = "Качество воздуха";
const char *TXT_AIR_POLLUTION      = "Загрязнение воздуха";
const char *TXT_VISIBILITY         = "Видимость";
const char *TXT_INDOOR_TEMPERATURE = "Темп. дома";
const char *TXT_INDOOR_HUMIDITY    = "Влжн. дома";
const char *TXT_DEWPOINT           = "Точка росы";

// ФАЗЫ ЛУНЫ
const char *TXT_MOONPHASE       = "Фаза луны";
const char *TXT_NEW_MOON        = "Новолуние";
const char *TXT_WAXING_CRESCENT = "Молодая луна";
const char *TXT_FIRST_QUARTER   = "Первая четверть";
const char *TXT_WAXING_GIBBOUS  = "Прибывающая луна";
const char *TXT_FULL_MOON       = "Полнолуние";
const char *TXT_WANING_GIBBOUS  = "Убывающая луна";
const char *TXT_THIRD_QUARTER   = "Последняя четверть";
const char *TXT_WANING_CRESCENT = "Старая луна";

// ИНДЕКС УФ
const char *TXT_UV_LOW       = "Низкий";
const char *TXT_UV_MODERATE  = "Умеренный";
const char *TXT_UV_HIGH      = "Высокий";
const char *TXT_UV_VERY_HIGH = "Очень высокий";
const char *TXT_UV_EXTREME   = "Экстремальный";

// WI-FI
const char *TXT_WIFI_EXCELLENT     = "Отличное";
const char *TXT_WIFI_GOOD          = "Хорошее";
const char *TXT_WIFI_FAIR          = "Среднее";
const char *TXT_WIFI_WEAK          = "Слабое";
const char *TXT_WIFI_NO_CONNECTION = "Нет подключения";

// ЕДИНИЦЫ ИЗМЕРЕНИЯ - ТЕМПЕРАТУРА
const char *TXT_UNITS_TEMP_KELVIN     = "K";
const char *TXT_UNITS_TEMP_CELSIUS    = "\x9F";  //только в RobotoRus
//const char *TXT_UNITS_TEMP_CELSIUS    = "\260C";
const char *TXT_UNITS_TEMP_FAHRENHEIT = "\260F";

// ЕДИНИЦЫ ИЗМЕРЕНИЯ - СКОРОСТЬ ВЕТРА
const char *TXT_UNITS_SPEED_METERSPERSECOND   = "м/с";
const char *TXT_UNITS_SPEED_FEETPERSECOND     = "фут/с";
const char *TXT_UNITS_SPEED_KILOMETERSPERHOUR = "км/ч";
const char *TXT_UNITS_SPEED_MILESPERHOUR      = "миль/ч";
const char *TXT_UNITS_SPEED_KNOTS             = "уз";
const char *TXT_UNITS_SPEED_BEAUFORT          = "Бфт";

// ЕДИНИЦЫ ИЗМЕРЕНИЯ - ДАВЛЕНИЕ
const char *TXT_UNITS_PRES_HECTOPASCALS             = "гПа";
const char *TXT_UNITS_PRES_PASCALS                  = "Па";
//const char *TXT_UNITS_PRES_MILLIMETERSOFMERCURY     = "мм рт. ст.";
const char *TXT_UNITS_PRES_MILLIMETERSOFMERCURY     = "\x9E";   //только в RobotoRus
const char *TXT_UNITS_PRES_INCHESOFMERCURY          = "дюйм рт. ст.";
const char *TXT_UNITS_PRES_MILLIBARS                = "мбар";
const char *TXT_UNITS_PRES_ATMOSPHERES              = "атм";
const char *TXT_UNITS_PRES_GRAMSPERSQUARECENTIMETER = "г/см\262";
const char *TXT_UNITS_PRES_POUNDSPERSQUAREINCH      = "psi";

// ЕДИНИЦЫ ИЗМЕРЕНИЯ - ВИДИМОСТЬ
const char *TXT_UNITS_DIST_KILOMETERS = "км";
const char *TXT_UNITS_DIST_MILES      = "миль";

// ЕДИНИЦЫ ИЗМЕРЕНИЯ - ОСАДКИ
const char *TXT_UNITS_PRECIP_MILLIMETERS = "мм";
const char *TXT_UNITS_PRECIP_CENTIMETERS = "см";
const char *TXT_UNITS_PRECIP_INCHES      = "дюйм";

// ЕДИНИЦЫ ИЗМЕРЕНИЯ - радиация
//const char *TXT_UNITS_RADIATION_SIEVERT     = "\x9CЗв/ч";
//const char *TXT_UNITS_RADIATION_ROENTGEN    = "\x9CР/ч";
const char *TXT_UNITS_RADIATION_SIEVERT     = "\x9B";   //только в RobotoRus
const char *TXT_UNITS_RADIATION_ROENTGEN    = "\x9C";   //только в RobotoRus


// РАЗЛИЧНЫЕ СООБЩЕНИЯ
const char *TXT_LOW_BATTERY = "Низкий заряд батареи";
const char *TXT_NETWORK_NOT_AVAILABLE = "Сеть недоступна";
const char *TXT_TIME_SYNCHRONIZATION_FAILED = "Сбой синхронизации времени";
const char *TXT_WIFI_CONNECTION_FAILED = "Сбой подключения к Wi-Fi";

const char *TXT_ATTEMPTING_HTTP_REQ = "Запрос HTTP...";
const char *TXT_AWAKE_FOR = "Время работы";
const char *TXT_BATTERY_VOLTAGE = "Напряжение батареи";
const char *TXT_CONNECTING_TO = "Подключение к";
const char *TXT_COULD_NOT_CONNECT_TO = "Не удалось подключиться к";
const char *TXT_ENTERING_DEEP_SLEEP_FOR = "Переход в глубокий сон на";
const char *TXT_READING_FROM = "Чтение из";
const char *TXT_FAILED  = "Ошибка";
const char *TXT_SUCCESS = "Успешно";
const char *TXT_UNKNOWN = "Неизвестно";

const char *TXT_NOT_FOUND = "не найдено";
const char *TXT_READ_FAILED = "ошибка чтения";

const char *TXT_FAILED_TO_GET_TIME = "Не удалось получить время!";
const char *TXT_HIBERNATING_INDEFINITELY_NOTICE = "Режим гибернации без времени пробуждения!";
const char *TXT_REFERENCING_OLDER_TIME_NOTICE = "Сбой синхронизации перед сном, используется старое время.";
const char *TXT_WAITING_FOR_SNTP = "Ожидание синхронизации SNTP.";
const char *TXT_LOW_BATTERY_VOLTAGE = "Низкое напряжение батареи!";
const char *TXT_VERY_LOW_BATTERY_VOLTAGE = "Очень низкое напряжение батареи!";
const char *TXT_CRIT_LOW_BATTERY_VOLTAGE = "Критически низкое напряжение батареи!";

// ПРЕДУПРЕЖДЕНИЯ (ALERTS)
const std::vector<String> ALERT_URGENCY = {"незначительный", "умеренный", "сильный", "экстремальный"};
//const std::vector<String> ALERT_URGENCY = {"outlook", "statement", "watch", "advisory", "warning", "emergency"}; // US National Weather Service
// const std::vector<String> ALERT_URGENCY = {"yellow", "amber", "red"};                 // United Kingdom's national weather service (MET Office)
// const std::vector<String> ALERT_URGENCY = {"minor", "moderate", "severe", "extreme"}; // METEO
// const std::vector<String> ALERT_URGENCY = {}; // Disable urgency interpretation (algorithm will fallback to only prefer the most recently issued alerts)


// ALERT TERMINOLOGY
// Weather terminology associated with each alert icon
const std::vector<String> TERM_SMOG =
    {"smog"};
const std::vector<String> TERM_SMOKE =
    {"smoke"};
const std::vector<String> TERM_FOG =
    {"fog", "haar"};
const std::vector<String> TERM_METEOR =
    {"meteor", "asteroid"};
const std::vector<String> TERM_NUCLEAR =
    {"nuclear", "ionizing radiation"};
const std::vector<String> TERM_BIOHAZARD =
    {"biohazard", "biological hazard"};
const std::vector<String> TERM_EARTHQUAKE =
    {"earthquake"};
const std::vector<String> TERM_FIRE =
    {"fire", "red flag"};
const std::vector<String> TERM_HEAT =
    {"heat"};
const std::vector<String> TERM_WINTER =
    {"blizzard", "winter", "ice", "icy", "snow", "sleet", "cold",
     "freezing rain", "wind chill", "freeze", "frost", "hail"};
const std::vector<String> TERM_TSUNAMI =
    {"tsunami", "surf"};
const std::vector<String> TERM_LIGHTNING =
    {"thunderstorm", "storm cell", "pulse storm", "squall line", "supercell",
     "lightning"};
const std::vector<String> TERM_SANDSTORM =
    {"sandstorm", "blowing dust", "dust storm"};
const std::vector<String> TERM_FLOOD =
    {"flood", "storm surge", "seiche", "swell", "high seas", "high tides",
     "tidal surge", "hydrologic"};
const std::vector<String> TERM_VOLCANO =
    {"volcanic", "ash", "volcano", "eruption"};
const std::vector<String> TERM_AIR_QUALITY =
    {"air", "stagnation", "pollution"};
const std::vector<String> TERM_TORNADO =
    {"tornado"};
const std::vector<String> TERM_SMALL_CRAFT_ADVISORY =
    {"small craft", "wind advisory"};
const std::vector<String> TERM_GALE_WARNING =
    {"gale"};
const std::vector<String> TERM_STORM_WARNING =
    {"storm warning"};
const std::vector<String> TERM_HURRICANE_WARNING =
    {"hurricane force wind", "extreme wind", "high wind"};
const std::vector<String> TERM_HURRICANE =
    {"hurricane", "tropical storm", "typhoon", "cyclone"};
const std::vector<String> TERM_DUST =
    {"dust", "sand"};
const std::vector<String> TERM_STRONG_WIND =
    {"wind"};

// КАЧЕСТВО ВОЗДУХА (AQI ТЕКСТ)
extern "C" {
 // Seemingly every country uses a different scale for Air Quality Index (AQI).
// I have written a library to calculate many of the most popular AQI scales.
// Feel free to request the addition of a new AQI scale by opening an Issue.
// https://github.com/lmarzen/pollutant-concentration-to-aqi
// Please choose this region's preferred AQI scale.
//   AUSTRALIA_AQI        CANADA_AQHI          EUROPEAN_UNION_CAQI
//   HONG_KONG_AQHI       INDIA_AQI            CHINA_AQI
//   SINGAPORE_PSI        SOUTH_KOREA_CAI      UNITED_KINGDOM_DAQI
//   UNITED_STATES_AQI  
const aqi_scale_t AQI_SCALE = EUROPEAN_UNION_CAQI;

const char *AUSTRALIA_AQI_TXT[6] = {"Очень хорошее", "Хорошее", "Удовлетворительное", "Плохое", "Очень плохое", "Опасное"};
const char *CANADA_AQHI_TXT[4] = {"Низкий", "Умеренный", "Высокий", "Очень высокий"};
const char *EUROPEAN_UNION_CAQI_TXT[5] = {"Очень низкое", "Низкое", "Среднее", "Высокое", "Очень высокое"};
const char *HONG_KONG_AQHI_TXT[5] = {"Низкий", "Умеренный", "Высокий", "Очень высокий", "Опасный"};
const char *INDIA_AQI_TXT[6] = {"Хорошее", "Удовлетворительное", "Умеренное", "Плохое", "Очень плохое", "Тяжелое"};
const char *CHINA_AQI_TXT[6] = {"Отличное", "Хорошее", "Легкое загрязнение", "Умеренное загрязнение", "Сильное загрязнение", "Опасное загрязнение"};
const char *SINGAPORE_PSI_TXT[5] = {"Хорошее", "Умеренное", "Вредное", "Очень вредное", "Опасное"};
const char *SOUTH_KOREA_CAI_TXT[4] = {"Хорошее", "Среднее", "Вредное", "Очень вредное"};
const char *UNITED_KINGDOM_DAQI_TXT[4] = {"Низкий", "Умеренный", "Высокий", "Очень высокий"};
const char *UNITED_STATES_AQI_TXT[6] = {"Хорошее", "Умеренное", "Вредное для чувствительных групп", "Вредное", "Очень вредное", "Опасное"};
}

// НАПРАВЛЕНИЯ СВЕТА (РУМБЫ КОМПАСА)
const char *COMPASS_POINT_NOTCTION[32] = {
//   0°    11.25°   22.5°    33.75°   45°    56.25°   67.5°    78.75°
   "С",   "СкВ",   "ССВ",   "СВкС",  "СВ",  "СВкВ",  "ВСВ",   "ВкС",
//  90°   101.25°  112.5°   123.75°  135°   146.25°  157.5°   168.75°
   "В",   "ВкЮ",   "ВЮВ",   "ЮВкВ",  "ЮВ",  "ЮВкЮ",  "ЮЮВ",   "ЮкВ",
// 180°   191.25°  202.5°   213.75°  225°   236.25°  247.5°   258.75°
   "Ю",   "ЮкЗ",   "ЮЮЗ",   "ЮЗкЮ",  "ЮЗ",  "ЮЗкЗ",  "ЗЮЗ",   "ЗкЮ",
// 270°   281.25°  292.5°   303.75°  315°   326.25°  337.5°   348.75°
   "З",   "ЗкС",   "ЗСЗ",   "СЗкЗ",  "СЗ",  "СЗкС",  "ССЗ",   "СкЗ",
};

// ОШИБКИ HTTP-КЛИЕНТА
const char *TXT_HTTPC_ERROR_CONNECTION_REFUSED   = "В соединении отказано";
const char *TXT_HTTPC_ERROR_SEND_HEADER_FAILED   = "Ошибка отправки заголовка";
const char *TXT_HTTPC_ERROR_SEND_PAYLOAD_FAILED  = "Ошибка отправки данных";
const char *TXT_HTTPC_ERROR_NOT_CONNECTED        = "Нет подключения";
const char *TXT_HTTPC_ERROR_CONNECTION_LOST      = "Соединение потеряно";
const char *TXT_HTTPC_ERROR_NO_STREAM            = "Нет потока";
const char *TXT_HTTPC_ERROR_NO_HTTP_SERVER       = "Нет HTTP-сервера";
const char *TXT_HTTPC_ERROR_TOO_LESS_RAM         = "Недостаточно ОЗУ";
const char *TXT_HTTPC_ERROR_ENCODING             = "Кодировка Transfer-Encoding не поддерживается";
const char *TXT_HTTPC_ERROR_STREAM_WRITE         = "Ошибка записи потока";
const char *TXT_HTTPC_ERROR_READ_TIMEOUT         = "Тайм-аут чтения";

// СТАТУСЫ ОТВЕТА HTTP
// 1xx
const char *TXT_HTTP_RESPONSE_100 = "Продолжай";
const char *TXT_HTTP_RESPONSE_101 = "Переключение протоколов";
const char *TXT_HTTP_RESPONSE_102 = "Идет обработка";
const char *TXT_HTTP_RESPONSE_103 = "Ранние подсказки";
// 2xx
const char *TXT_HTTP_RESPONSE_200 = "ОК";
const char *TXT_HTTP_RESPONSE_201 = "Создано";
const char *TXT_HTTP_RESPONSE_202 = "Принято";
const char *TXT_HTTP_RESPONSE_203 = "Информация не авторитетна";
const char *TXT_HTTP_RESPONSE_204 = "Нет содержимого";
const char *TXT_HTTP_RESPONSE_205 = "Сбросить содержимое";
const char *TXT_HTTP_RESPONSE_206 = "Частичное содержимое";
const char *TXT_HTTP_RESPONSE_207 = "Многостатусный";
const char *TXT_HTTP_RESPONSE_208 = "Уже зарегистрировано";
const char *TXT_HTTP_RESPONSE_226 = "Использовано IM";
// 3xx
const char *TXT_HTTP_RESPONSE_300 = "Множество вариантов";
const char *TXT_HTTP_RESPONSE_301 = "Перемещено навсегда";
const char *TXT_HTTP_RESPONSE_302 = "Найдено";
const char *TXT_HTTP_RESPONSE_303 = "Смотреть другое";
const char *TXT_HTTP_RESPONSE_304 = "Не изменялось";
const char *TXT_HTTP_RESPONSE_305 = "Использовать прокси";
const char *TXT_HTTP_RESPONSE_307 = "Временное перенаправление";
const char *TXT_HTTP_RESPONSE_308 = "Постоянное перенаправление";
// 4xx
const char *TXT_HTTP_RESPONSE_400 = "Некорректный запрос";
const char *TXT_HTTP_RESPONSE_401 = "Не авторизован";
const char *TXT_HTTP_RESPONSE_402 = "Требуется оплата";
const char *TXT_HTTP_RESPONSE_403 = "Запрещено";
const char *TXT_HTTP_RESPONSE_404 = "Не найдено";
const char *TXT_HTTP_RESPONSE_405 = "Метод не поддерживается";
const char *TXT_HTTP_RESPONSE_406 = "Неприемлемо";
const char *TXT_HTTP_RESPONSE_407 = "Требуется прокси-авторизация";
const char *TXT_HTTP_RESPONSE_408 = "Истекло время ожидания";
const char *TXT_HTTP_RESPONSE_409 = "Конфликт";
const char *TXT_HTTP_RESPONSE_410 = "Удалено";
const char *TXT_HTTP_RESPONSE_411 = "Требуется длина";
const char *TXT_HTTP_RESPONSE_412 = "Условие ложно";
const char *TXT_HTTP_RESPONSE_413 = "Слишком большой запрос";
const char *TXT_HTTP_RESPONSE_414 = "Слишком длинный URI";
const char *TXT_HTTP_RESPONSE_415 = "Неподдерживаемый тип данных";
const char *TXT_HTTP_RESPONSE_416 = "Диапазон не достижим";
const char *TXT_HTTP_RESPONSE_417 = "Ожидание не оправдалось";
const char *TXT_HTTP_RESPONSE_418 = "Я чайник";
const char *TXT_HTTP_RESPONSE_421 = "Неверно направленный запрос";
const char *TXT_HTTP_RESPONSE_422 = "Необрабатываемое содержимое";
const char *TXT_HTTP_RESPONSE_423 = "Заблокировано";
const char *TXT_HTTP_RESPONSE_424 = "Неудачная зависимость";
const char *TXT_HTTP_RESPONSE_425 = "Слишком рано";
const char *TXT_HTTP_RESPONSE_426 = "Требуется обновление";
const char *TXT_HTTP_RESPONSE_428 = "Требуется предусловие";
const char *TXT_HTTP_RESPONSE_429 = "Слишком много запросов";
const char *TXT_HTTP_RESPONSE_431 = "Заголовки запроса слишком большие";
const char *TXT_HTTP_RESPONSE_451 = "Недоступно по юридическим причинам";
// 5xx
const char *TXT_HTTP_RESPONSE_500 = "Внутренняя ошибка сервера";
const char *TXT_HTTP_RESPONSE_501 = "Не реализовано";
const char *TXT_HTTP_RESPONSE_502 = "Плохой шлюз";
const char *TXT_HTTP_RESPONSE_503 = "Сервис недоступен";
const char *TXT_HTTP_RESPONSE_504 = "Время ожидания от шлюза истекло";
const char *TXT_HTTP_RESPONSE_505 = "Версия HTTP не поддерживается";
const char *TXT_HTTP_RESPONSE_506 = "Вариант также ведет переговоры";
const char *TXT_HTTP_RESPONSE_507 = "Недостаточно места";
const char *TXT_HTTP_RESPONSE_508 = "Обнаружено зацикливание";
const char *TXT_HTTP_RESPONSE_510 = "Не расширено";
const char *TXT_HTTP_RESPONSE_511 = "Требуется сетевая авторизация";
// ОШИБКИ ДЕСЕРИАЛИЗАЦИИ ARDUINOJSON
const char *TXT_DESERIALIZATION_ERROR_OK               = "Десериализация: ОК";
const char *TXT_DESERIALIZATION_ERROR_EMPTY_INPUT      = "Десериализация: пустой ввод";
const char *TXT_DESERIALIZATION_ERROR_INCOMPLETE_INPUT = "Десериализация: неполный ввод";
const char *TXT_DESERIALIZATION_ERROR_INVALID_INPUT    = "Десериализация: некорректный ввод";
const char *TXT_DESERIALIZATION_ERROR_NO_MEMORY        = "Десериализация: нет памяти";
const char *TXT_DESERIALIZATION_ERROR_TOO_DEEP         = "Десериализация: слишком глубокая вложенность";
// СТАТУСЫ WI-FI
const char *TXT_WL_NO_SHIELD       = "Плата расширения не найдена";
const char *TXT_WL_IDLE_STATUS     = "В ожидании";
const char *TXT_WL_NO_SSID_AVAIL   = "Сеть SSID не найдена";
const char *TXT_WL_SCAN_COMPLETED  = "Сканирование завершено";
const char *TXT_WL_CONNECTED       = "Подключено";
const char *TXT_WL_CONNECT_FAILED  = "Ошибка подключения";
const char *TXT_WL_CONNECTION_LOST = "Подключение потеряно";
const char *TXT_WL_DISCONNECTED    = "Отключено";