function doGet(e) {
  try {
    var lat = e.parameter.lat;
    var lon = e.parameter.lon;
    
    // =========================================================================
    // НАСТРОЙКА: Вставьте сюда ваш личный API-ключ от OpenWeatherMap
    // =========================================================================
    var OWM_API_KEY = "ВАШ_API_КЛЮЧ_ОТ_OPENWEATHERMAP"; 
    // =========================================================================

    if (!lat || !lon) {
      return ContentService.createTextOutput(JSON.stringify({ error: "Missing lat or lon parameters" }))
                           .setMimeType(ContentService.MimeType.TEXT);
    }

    if (OWM_API_KEY === "ВАШ_API_КЛЮЧ_ОТ_OPENWEATHERMAP") {
      return ContentService.createTextOutput(JSON.stringify({ error: "Please configure OWM_API_KEY inside the script!" }))
                           .setMimeType(ContentService.MimeType.TEXT);
    }

    // Формируем запрос к оригинальному API OWM One Call (запрашиваем стандартные Кельвины)
    // Исключаем ненужный минутный прогноз (minutely), чтобы облегчить пакет
    var owmUrl = "https://openweathermap.org?" +
                 "lat=" + lat +
                 "&lon=" + lon +
                 "&exclude=minutely" +
                 "&units=standard" + // standard возвращает температуру строго в Кельвинах
                 "&appid=" + OWM_API_KEY;

    // Выкачиваем данные силами серверов Google
    var response = UrlFetchApp.fetch(owmUrl, {muteHttpExceptions: true});
    var responseCode = response.getResponseCode();
    
    if (responseCode !== 200) {
      return ContentService.createTextOutput(JSON.stringify({ error: "OpenWeatherMap returned HTTP Error " + responseCode }))
                           .setMimeType(ContentService.MimeType.TEXT);
    }

    // Считываем оригинальный JSON от OpenWeatherMap
    var owmData = JSON.parse(response.getContentText());

    // Так как OWM отдает огромный массив hourly (48 точек) и daily (8 дней), 
    // мы точечно обрежем их прямо в облаке, чтобы ESP32 не тратил ОЗУ и не падал по тайм-ауту.
    
    // 1. Ограничиваем почасовой прогноз до 25 точек (для суточного графика)
    if (owmData.hourly && owmData.hourly.length > 25) {
      owmData.hourly = owmData.hourly.slice(0, 25);
    }

    // 2. Ограничиваем посуточный прогноз до 5 дней (для нижней панели дисплея)
    if (owmData.daily && owmData.daily.length > 5) {
      owmData.daily = owmData.daily.slice(0, 5);
    }

    // Отдаем ESP32 чистый, облегченный оригинальный JSON-поток в формате TEXT
    return ContentService.createTextOutput(JSON.stringify(owmData))
                         .setMimeType(ContentService.MimeType.TEXT);

  } catch (error) {
    // В случае сбоя возвращаем ошибку текстом (без ломающих сокет HTML-тегов)
    return ContentService.createTextOutput(JSON.stringify({ error: error.toString() }))
                         .setMimeType(ContentService.MimeType.TEXT);
  }
}
