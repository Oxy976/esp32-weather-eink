/* Client side utilities for esp32-weather-epd.
 * Copyright (C) 2022-2024  Luke Marzen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

// built-in C++ libraries
#include <cstring>
#include <vector>

// arduino/esp32 libraries
#include <Arduino.h>
#include <esp_sntp.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <time.h>
#include <WiFi.h>

// additional libraries
#include <Adafruit_BusIO_Register.h>
#include <ArduinoJson.h>

// header files
#include "_locale.h"
#include "api_response.h"
#include "aqi.h"
#include "client_utils.h"
#include "config.h"
#include "display_utils.h"
#include "renderer.h"
#ifndef USE_HTTP
#include <WiFiClientSecure.h>
#endif

#ifdef USE_HTTP
static const uint16_t OWM_PORT = 80;
#else
static const uint16_t OWM_PORT = 443;
#endif

/* Power-on and connect WiFi. */
wl_status_t startWiFi(int &wifiRSSI)
{
  WiFi.mode(WIFI_STA);
  Serial.printf("%s '%s'", TXT_CONNECTING_TO, WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long timeout = millis() + WIFI_TIMEOUT;
  wl_status_t connection_status = WiFi.status();

  while ((connection_status != WL_CONNECTED) && (millis() < timeout))
  {
    Serial.print(".");
    delay(50);
    connection_status = WiFi.status();
  }
  Serial.println();

  if (connection_status == WL_CONNECTED)
  {
    wifiRSSI = WiFi.RSSI();
    Serial.println("IP: " + WiFi.localIP().toString());
  }
  else
  {
    Serial.printf("%s '%s'\n", TXT_COULD_NOT_CONNECT_TO, WIFI_SSID);
  }
  return connection_status;
}

/* Disconnect and power-off WiFi. */
void killWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

/* Prints the local time to serial monitor. */
bool printLocalTime(tm *timeInfo)
{
  int attempts = 0;
  while (!getLocalTime(timeInfo) && attempts++ < 3)
  {
    Serial.println(TXT_FAILED_TO_GET_TIME);
    return false;
  }
  Serial.println(timeInfo, "%A, %B %d, %Y %H:%M:%S");
  return true;
}

/* Waits for NTP server time sync. */
bool waitForSNTPSync(tm *timeInfo)
{
  unsigned long timeout = millis() + NTP_TIMEOUT;
  if ((sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET) && (millis() < timeout))
  {
    Serial.print(TXT_WAITING_FOR_SNTP);
    delay(100);
    while ((sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET) && (millis() < timeout))
    {
      Serial.print(".");
      delay(100);
    }
    Serial.println();
  }
  return printLocalTime(timeInfo);
}

/* Perform an HTTP GET request to OpenWeatherMap's "One Call" API */
#ifdef USE_HTTP
int getOWMonecall_OWM(WiFiClient &client, owm_resp_onecall_t &r)
#else
int getOWMonecall_OWM(WiFiClientSecure &client, owm_resp_onecall_t &r)
#endif
{
  int attempts = 0;
  bool rxSuccess = false;
  DeserializationError jsonErr = {};
  String uri = "/data/" + OWM_ONECALL_VERSION + "/onecall?lat=" + LAT + "&lon=" + LON + "&lang=" + OWM_LANG + "&units=standard&exclude=minutely";
#if !DISPLAY_ALERTS
  uri += ",alerts";
#endif

  String sanitizedUri = OWM_ENDPOINT + uri + "&appid={API key}";
  uri += "&appid=" + OWM_APIKEY;

  Serial.print(TXT_ATTEMPTING_HTTP_REQ);
  Serial.println(": " + sanitizedUri);
  int httpResponse = 0;
  while (!rxSuccess && attempts < 3)
  {
    wl_status_t connection_status = WiFi.status();
    if (connection_status != WL_CONNECTED)
    {
      return -512 - static_cast<int>(connection_status);
    }

    HTTPClient http;
    http.setConnectTimeout(HTTP_CLIENT_TCP_TIMEOUT);
    http.setTimeout(HTTP_CLIENT_TCP_TIMEOUT);
    http.begin(client, OWM_ENDPOINT, OWM_PORT, uri);
    httpResponse = http.GET();
    if (httpResponse == HTTP_CODE_OK)
    {
      String jsonPayload = http.getString();
      jsonErr = deserializeOneCall(jsonPayload, r);
      if (jsonErr)
      {
        httpResponse = -256 - static_cast<int>(jsonErr.code());
      }
      rxSuccess = !jsonErr;
    }
    client.stop();
    http.end();
    Serial.println("  " + String(httpResponse, DEC) + " " + getHttpResponsePhrase(httpResponse));
    ++attempts;
  }

  return httpResponse;
}

/* Perform an HTTP GET request to OpenWeatherMap's "Air Pollution" API */
#ifdef USE_HTTP
int getOWMairpollution(WiFiClient &client, owm_resp_air_pollution_t &r)
#else
int getOWMairpollution(WiFiClientSecure &client, owm_resp_air_pollution_t &r)
#endif
{
  if (WEATHER_PROVIDER != "OPEN_WEATHER" && CURRENT_PROVIDER != "OPEN_WEATHER")
  {
    return HTTP_CODE_OK;
  }

  int attempts = 0;
  bool rxSuccess = false;
  DeserializationError jsonErr = {};

  time_t now;
  int64_t end = time(&now);
  int64_t start = end - ((3600 * OWM_NUM_AIR_POLLUTION) - 1);
  char endStr[22];
  char startStr[22];
  sprintf(endStr, "%lld", end);
  sprintf(startStr, "%lld", start);
  String uri = "/data/2.5/air_pollution/history?lat=" + LAT + "&lon=" + LON + "&start=" + startStr + "&end=" + endStr + "&appid=" + OWM_APIKEY;
  String sanitizedUri = OWM_ENDPOINT + "/data/2.5/air_pollution/history?lat=" + LAT + "&lon=" + LON + "&start=" + startStr + "&end=" + endStr + "&appid={API key}";

  Serial.print(TXT_ATTEMPTING_HTTP_REQ);
  Serial.println(": " + sanitizedUri);
  int httpResponse = 0;
  while (!rxSuccess && attempts < 3)
  {
    wl_status_t connection_status = WiFi.status();
    if (connection_status != WL_CONNECTED)
    {
      return -512 - static_cast<int>(connection_status);
    }

    HTTPClient http;
    http.setConnectTimeout(HTTP_CLIENT_TCP_TIMEOUT);
    http.setTimeout(HTTP_CLIENT_TCP_TIMEOUT);
    http.begin(client, OWM_ENDPOINT, OWM_PORT, uri);
    httpResponse = http.GET();
    if (httpResponse == HTTP_CODE_OK)
    {
      jsonErr = deserializeAirQuality(http.getStream(), r);
      if (jsonErr)
      {
        httpResponse = -256 - static_cast<int>(jsonErr.code());
      }
      rxSuccess = !jsonErr;
    }
    client.stop();
    http.end();
    Serial.println("  " + String(httpResponse, DEC) + " " + getHttpResponsePhrase(httpResponse));
    ++attempts;
  }

  return httpResponse;
}

// -------------------------------------------------------------------------
// ВНУТРЕННЯЯ ФУНКЦИЯ: Получение прогноза погоды через макрос Google Script
// -------------------------------------------------------------------------
#ifdef USE_HTTP
static int getOWMonecall_Google(WiFiClient &client, owm_resp_onecall_t &r)
#else
static int getOWMonecall_Google(WiFiClientSecure &client, owm_resp_onecall_t &r)
#endif
{
  int attempts = 0;
  bool rxSuccess = false;
  DeserializationError jsonErr = {};

  String fullUrl = GOOGLE_SCRIPT_URL + "?lat=" + LAT + "&lon=" + LON;

  Serial.print(TXT_ATTEMPTING_HTTP_REQ);
  Serial.println(": " + fullUrl);
  int httpResponse = 0;

  while (!rxSuccess && attempts < 3)
  {
    wl_status_t connection_status = WiFi.status();
    if (connection_status != WL_CONNECTED)
    {
      return -512 - static_cast<int>(connection_status);
    }

    HTTPClient http;
    // Задаем стабильный тайм-аут для выкачивания тяжелого скрипта
    http.setConnectTimeout(15000);
    http.setTimeout(15000);

    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.begin(client, fullUrl);

    httpResponse = http.GET();
    if (httpResponse == HTTP_CODE_OK)
    {
      // Считываем весь пакет одной строкой в ОЗУ
      String jsonPayload = http.getString();

#if DEBUG_LEVEL >= 1
      Serial.printf("[DEBUG NET] Downloaded %d bytes from Google Script.\n", jsonPayload.length());
#endif

      // Передаем строку в исправленный v7 десериализатор
      jsonErr = deserializeOneCall(jsonPayload, r);
      if (jsonErr)
      {
        httpResponse = -256 - static_cast<int>(jsonErr.code());
      }
      rxSuccess = !jsonErr;
    }
    client.stop();
    http.end();
    Serial.println("  " + String(httpResponse, DEC) + " " + getHttpResponsePhrase(httpResponse));
    ++attempts;
  }

  return httpResponse;
}

// -------------------------------------------------------------------------
// ФУНКЦИЯ: Опрос локальной домашней метеостанции
// -------------------------------------------------------------------------
#ifdef USE_HTTP
int getLocalMeteoData(WiFiClient &client, owm_resp_onecall_t &r)
#else
int getLocalMeteoData(WiFiClientSecure &client, owm_resp_onecall_t &r)
#endif
{
  HTTPClient http;
  http.setConnectTimeout(3000);
  http.setTimeout(3000);

  Serial.println("Attempting LOCAL METEO REQ: " + LOCAL_METEO_URL);

  WiFiClient localPlainClient;
  http.begin(localPlainClient, LOCAL_METEO_URL);

  int httpResponse = http.GET();
  if (httpResponse == HTTP_CODE_OK)
  {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, http.getStream());

    if (!error && doc.is<JsonArray>())
    {
      float extTemp = NAN;
      int extHum = -1;
      int extPress = 0;

      float temp_sht = NAN;
      float temp_htu = NAN;
      float temp_ds = NAN;

      JsonArray arr = doc.as<JsonArray>();
      for (JsonObject sensor : arr)
      {
        if (!sensor["act"].as<bool>())
          continue;

        String id_name = sensor["id_name"].as<String>();
        float val = sensor["val"].as<float>();

        if (id_name == "temp_ext_sht")
          temp_sht = val;
        else if (id_name == "temp_ext_htu")
          temp_htu = val;
        else if (id_name == "temp_ext_ds")
          temp_ds = val;

        else if (id_name == "hum_ext_sht")
          extHum = (int)val;
        else if (id_name == "hum_ext_htu" && extHum == -1)
          extHum = (int)val;

        else if (id_name == "press_int_bme")
          extPress = (int)val;
      }

      if (!std::isnan(temp_sht))
      {
        extTemp = temp_sht;
      }
      else if (!std::isnan(temp_htu))
      {
        extTemp = temp_htu;
      }
      else if (!std::isnan(temp_ds))
      {
        extTemp = temp_ds;
      }

      r.current.temp = extTemp;
      r.current.humidity = extHum;
      r.current.pressure = extPress;
      r.current.weather.id = 800;

      Serial.printf("[Local API Success] Selected -> T: %.2f, H: %d, P: %d\n", extTemp, extHum, extPress);
    }
    else
    {
      Serial.println("Failed to parse dynamic local JSON array");
      httpResponse = -256;
    }
  }
  http.end();
  return httpResponse;
}
// -------------------------------------------------------------------------
// ГЛАВНАЯ ДИСПЕТЧЕРСКАЯ ФУНКЦИЯ (Имя и сигнатура сохранены для main.cpp)
// -------------------------------------------------------------------------
#ifdef USE_HTTP
int getOWMonecall(WiFiClient &client, owm_resp_onecall_t &r)
#else
int getOWMonecall(WiFiClientSecure &client, owm_resp_onecall_t &r)
#endif
{
  if (WEATHER_PROVIDER == "GOOGLE_SCRIPT")
  {
    return getOWMonecall_Google(client, r);
  }
  else
  {
    return getOWMonecall_OWM(client, r);
  }
}
/* Prints debug information about heap usage. */
void printHeapUsage()
{
  Serial.println("[debug] Heap Size : " + String(ESP.getHeapSize()) + " B");
  Serial.println("[debug] Available Heap : " + String(ESP.getFreeHeap()) + " B");
  Serial.println("[debug] Min Free Heap : " + String(ESP.getMinFreeHeap()) + " B");
  Serial.println("[debug] Max Allocatable : " + String(ESP.getMaxAllocHeap()) + " B");
  return;
}
