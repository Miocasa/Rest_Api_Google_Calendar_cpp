#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include "Secrets.h" // api keys and etc
using json = nlohmann::json;

// Get access token using refresh token
std::string getAccessToken() {
    auto r = cpr::Post(
        cpr::Url{"https://oauth2.googleapis.com/token"},
        cpr::Payload{
            {"client_id", CLIENT_ID},
            {"client_secret", CLIENT_SECRET},
            {"refresh_token", REFRESH_TOKEN},
            {"grant_type", "refresh_token"}});
    if (r.status_code != 200)
        throw std::runtime_error("Token error: " + r.text);
    return json::parse(r.text)["access_token"];
}


void listEvents(const std::string& token) {
    auto r = cpr::Get(
        cpr::Url{"https://www.googleapis.com/calendar/v3/calendars/primary/events"},
        cpr::Header{{"Authorization", "Bearer " + token}},
        cpr::Parameters{
            {"maxResults", "10"},
            {"singleEvents", "true"},
            {"orderBy", "startTime"},
            {"timeMin", "2026-10-06T00:00:00Z"}});
    auto j = json::parse(r.text);
    for (auto& e : j["items"])
        std::cout << e.value("summary", "(без названия)") << " — "
                  << e["start"].value("dateTime", e["start"].value("date", ""))
                  << "\n";
}

void createEvent(const std::string& token) {
    json event = {
        {"summary", "Встреча команды"},
        {"description", "Созвон по проекту"},
        {"start", {{"dateTime", "2026-10-08T10:00:00+03:00"},
                   {"timeZone", "Europe/Moscow"}}},
        {"end",   {{"dateTime", "2026-10-08T11:00:00+03:00"},
                   {"timeZone", "Europe/Moscow"}}}
    };
    auto r = cpr::Post(
        cpr::Url{"https://www.googleapis.com/calendar/v3/calendars/primary/events"},
        cpr::Header{{"Authorization", "Bearer " + token},
                    {"Content-Type", "application/json"}},
        cpr::Body{event.dump()});
    std::cout << r.status_code << "\n" << r.text << "\n";
}

int main() {
    try {
        auto token = getAccessToken();
        listEvents(token);
        createEvent(token);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}