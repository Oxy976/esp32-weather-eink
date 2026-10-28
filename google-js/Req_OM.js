function doGet(e) {
  try {
    var lat = e.parameter.lat;
    var lon = e.parameter.lon;
    
    if (!lat || !lon) {
      return ContentService.createTextOutput(JSON.stringify({ error: "Missing lat or lon parameters" }))
                           .setMimeType(ContentService.MimeType.TEXT);
    }

    // Облегченный и стабильный URL к Open-Meteo (без ломающих сервер параметров sunrise/sunset)
    var openMeteoUrl = "https://api.open-meteo.com/v1/forecast?" +
                       "latitude=" + lat +
                       "&longitude=" + lon +
                       "&current=temperature_2m,relative_humidity_2m,apparent_temperature,surface_pressure,weather_code,wind_speed_10m,cloud_cover,visibility" +
                       "&hourly=temperature_2m,weather_code,surface_pressure,relative_humidity_2m,cloud_cover,visibility" +
                       "&daily=weather_code,temperature_2m_max,temperature_2m_min,sunrise,sunset" +
                       "&timeformat=unixtime&timezone=auto"; 

    var response = UrlFetchApp.fetch(openMeteoUrl, {muteHttpExceptions: true});
    var responseCode = response.getResponseCode();
    
    if (responseCode !== 200) {
      return ContentService.createTextOutput(JSON.stringify({ error: "OpenMeteo returned HTTP " + responseCode }))
                           .setMimeType(ContentService.MimeType.TEXT);
    }

    var meteoData = JSON.parse(response.getContentText());

    // Переводим текущие показатели в Кельвины
    var curTempK = meteoData.current.temperature_2m + 273.15;
    var curFeelsK = meteoData.current.apparent_temperature ? (meteoData.current.apparent_temperature + 273.15) : curTempK;
    var curDewK = curTempK - ((100 - meteoData.current.relative_humidity_2m) / 5);

    // ПРАВИЛЬНОЕ И ЧИСТОЕ ЧТЕНИЕ UNIX-ВРЕМЕНИ ДЛЯ CURRENT (БЕЗ ДУБЛИРОВАНИЯ И КОНВЕРТАЦИЙ)
    var currentSunrise = Math.floor(Date.now() / 1000) - (6 * 3600);
    var currentSunset = Math.floor(Date.now() / 1000) + (6 * 3600);

    if (meteoData.daily && meteoData.daily.sunrise && meteoData.daily.sunrise[0]) {
      currentSunrise = meteoData.daily.sunrise[0]; // Прямое присвоение числа
    }
    if (meteoData.daily && meteoData.daily.sunset && meteoData.daily.sunset[0]) {
      currentSunset = meteoData.daily.sunset[0];   // Прямое присвоение числа
    }

    var owmMock = {
      lat: parseFloat(lat),
      lon: parseFloat(lon),
      timezone: meteoData.timezone || "Europe/Moscow",
      timezone_offset: meteoData.utc_offset_seconds || 10800,
      
      current: {
        dt: Math.floor(Date.now() / 1000),
        sunrise: currentSunrise,
        sunset: currentSunset,
        temp: parseFloat(curTempK.toFixed(2)),
        feels_like: parseFloat(curFeelsK.toFixed(2)),
        pressure: Math.floor(meteoData.current.surface_pressure) || 1013,
        humidity: Math.floor(meteoData.current.relative_humidity_2m) || 60,
        dew_point: parseFloat(curDewK.toFixed(2)), 
        uvi: 1.0,
        clouds: Math.floor(meteoData.current.cloud_cover) || 0,
        visibility: Math.floor(meteoData.current.visibility) || 10000,
        wind_speed: parseFloat(meteoData.current.wind_speed_10m.toFixed(2)) || 0.0,
        wind_gust: parseFloat(meteoData.current.wind_speed_10m.toFixed(2)) || 0.0,
        wind_deg: 0,
        rain: { "1h": 0.0 },
        snow: { "1h": 0.0 },
        weather: [{
          id: mapWmoToOwm(meteoData.current.weather_code),
          main: "Weather",
          description: "Meteo Code " + meteoData.current.weather_code,
          icon: "01d"
        }]
      },
      hourly: [],
      daily: [],
      alerts: []
    };

    // НАПОЛНЕНИЕ ПОЧАСОВОГО ПРОГНОЗА (25 точек, Кельвины)
    if (meteoData.hourly && meteoData.hourly.time) {
      var utcOffsetSec = meteoData.utc_offset_seconds || 0;
      var localDate = new Date(Date.now() + (utcOffsetSec * 1000));
      localDate.setUTCHours(0, 0, 0, 0);
      
      var startOfDayUnix = Math.floor((localDate.getTime() - (utcOffsetSec * 1000)) / 1000);

      var startIndex = 0;
      for (var i = 0; i < meteoData.hourly.time.length; i++) {
        var hrTime = Math.floor(new Date(meteoData.hourly.time[i]).getTime() / 1000);
        if (Math.abs(hrTime - startOfDayUnix) < 1800) {
          startIndex = i;
          break;
        }
      }

      for (var k = 0; k <= 24; k++) {
        var targetIndex = startIndex + k;
        if (targetIndex >= meteoData.hourly.time.length) break;

        var pointDt = startOfDayUnix + (k * 3600); 
        var hrTempC = meteoData.hourly.temperature_2m[targetIndex];
        
        var hrHumidity = meteoData.hourly.relative_humidity_2m ? (meteoData.hourly.relative_humidity_2m[targetIndex] || 60) : 60;
        var hrClouds = meteoData.hourly.cloud_cover ? (meteoData.hourly.cloud_cover[targetIndex] || 0) : 0;
        var hrVisibility = meteoData.hourly.visibility ? (meteoData.hourly.visibility[targetIndex] || 10000) : 10000;
        var hrPressure = meteoData.hourly.surface_pressure ? (meteoData.hourly.surface_pressure[targetIndex] || 1013) : 1013;

        var hrTempK = hrTempC + 273.15;
        var hrDewK = hrTempK - ((100 - hrHumidity) / 5);

        owmMock.hourly.push({
          dt: pointDt,
          temp: Math.round(hrTempK * 100) / 100,
          feels_like: Math.round(hrTempK * 100) / 100,
          pressure: Math.floor(hrPressure),
          humidity: Math.floor(hrHumidity),
          dew_point: Math.round(hrDewK * 100) / 100,
          clouds: Math.floor(hrClouds),
          uvi: 0.0,
          visibility: Math.floor(hrVisibility),
          wind_speed: 0.0,
          wind_gust: 0.0,
          wind_deg: 0,
          pop: 0.0,
          rain: { "1h": 0.0 },
          snow: { "1h": 0.0 },
          weather: [{
            id: mapWmoToOwm(meteoData.hourly.weather_code[targetIndex]),
            main: "Weather",
            description: "Hourly",
            icon: "01d"
          }]
        });
      }
    }
    
    // НАПОЛНЕНИЕ ПОСУТОЧНОГО ПРОГНОЗА (Строго 5 дней, Кельвины)
    if (meteoData.daily && meteoData.daily.time) {
      var maxDays = Math.min(5, meteoData.daily.time.length);

      for (var j = 0; j < maxDays; j++) {
        // Базовый полдень для текущего дня
        var dayMiddayUnix = Number(meteoData.daily.time[j]) + (12 * 3600);
        
        var minT = meteoData.daily.temperature_2m_min[j] + 273.15;
        var maxT = meteoData.daily.temperature_2m_max[j] + 273.15;
        var dailyDewK = minT - 2;

        // ПРАВИЛЬНОЕ И ЧИСТОЕ ЧТЕНИЕ UNIX-ВРЕМЕНИ ДЛЯ DAILY
        var dailySunrise = dayMiddayUnix - (6 * 3600); 
        var dailySunset = dayMiddayUnix + (6 * 3600);  

        if (meteoData.daily.sunrise && meteoData.daily.sunrise[j]) {
          dailySunrise = meteoData.daily.sunrise[j]; // Прямое присвоение числа
        }
        if (meteoData.daily.sunset && meteoData.daily.sunset[j]) {
          dailySunset = meteoData.daily.sunset[j];   // Прямое присвоение числа
        }

        owmMock.daily.push({
          dt: dayMiddayUnix,
          sunrise: dailySunrise, 
          sunset: dailySunset,  
          moonrise: dayMiddayUnix,
          moonset: dayMiddayUnix,
          moon_phase: 0.5,
          pressure: 1013,
          humidity: 60,
          dew_point: parseFloat(dailyDewK.toFixed(2)),
          wind_speed: 0.0,
          wind_gust: 0.0,
          wind_deg: 0,
          clouds: 0,
          pop: 0.0,
          rain: 0.0,
          snow: 0.0,
          uvi: 1.0,
          temp: {
            min: parseFloat(minT.toFixed(2)),
            max: parseFloat(maxT.toFixed(2)),
            day: parseFloat(maxT.toFixed(2)),
            morn: parseFloat(minT.toFixed(2)),
            eve: parseFloat(maxT.toFixed(2)),
            night: parseFloat(minT.toFixed(2))
          },
          feels_like: {
            day: parseFloat(maxT.toFixed(2)),
            night: parseFloat(minT.toFixed(2)),
            eve: parseFloat(maxT.toFixed(2)),
            morn: parseFloat(minT.toFixed(2))
          },
          weather: [{
            id: mapWmoToOwm(meteoData.daily.weather_code[j]),
            main: "Weather",
            description: "Daily",
            icon: "01d"
          }]
        });
      }
    }

    return ContentService.createTextOutput(JSON.stringify(owmMock))
                         .setMimeType(ContentService.MimeType.TEXT);

  } catch (error) {
    return ContentService.createTextOutput(JSON.stringify({ error: error.toString() }))
                         .setMimeType(ContentService.MimeType.TEXT);
  }
}

function mapWmoToOwm(wmoCode) {
  if (wmoCode === 0) return 800;
  if (wmoCode === 1 || wmoCode === 2 || wmoCode === 3) return 803;
  if (wmoCode === 45 || wmoCode === 48) return 741;
  if (wmoCode >= 51 && wmoCode <= 55) return 500;
  if (wmoCode >= 56 && wmoCode <= 57) return 511;
  if (wmoCode >= 61 && wmoCode <= 65) return 502;
  if (wmoCode >= 66 && wmoCode <= 67) return 511;
  if (wmoCode >= 71 && wmoCode <= 75) return 601;
  if (wmoCode === 77) return 600;
  if (wmoCode >= 80 && wmoCode <= 82) return 521;
  if (wmoCode >= 85 && wmoCode <= 86) return 621;
  if (wmoCode >= 95 && wmoCode <= 99) return 211;
  return 800;
}
