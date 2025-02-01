#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

// Function to simulate microplastic pollution levels based on weather conditions
void simulate_microplastic_pollution(double temperature, double windSpeed, double humidity) {
    double waterPollution = temperature * 0.5 + humidity * 0.3;
    double airPollution = windSpeed * 0.7 + humidity * 0.2;

    printf("\n=== Simulated Microplastic Pollution Data ===\n");
    printf("Water Pollution Level (µg/L): %.2f\n", waterPollution);
    printf("Air Pollution Level (particles/m³): %.2f\n", airPollution);
    printf("============================================\n");
}

int main() {
    char city[100];
    char apiKey[100];
    char command[512];
    char buffer[4096];  // Buffer to hold weather data

    // Ask for city and API key
    printf("Enter city name: ");
    scanf("%s", city);
    printf("Enter your OpenWeather API key: ");
    scanf("%s", apiKey);

    // Create the curl command to fetch the weather data
    snprintf(command, sizeof(command), 
                "curl -s \"http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric\" > weather.json", 
                city, apiKey);

    // Execute the curl command
    system(command);

    // Open the weather.json file and read the data
    FILE *file = fopen("weather.json", "r");
    if (!file) {
        printf("Error opening file\n");
        return 1;
    }

    // Read the content of the file into the buffer
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[bytesRead] = '\0';  // Null-terminate the buffer
    fclose(file);

    // Parse the JSON data using cJSON
    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        printf("Error parsing JSON\n");
        return 1;
    }

    // Extract weather data
    cJSON *main = cJSON_GetObjectItemCaseSensitive(json, "main");
    cJSON *weather = cJSON_GetArrayItem(cJSON_GetObjectItemCaseSensitive(json, "weather"), 0);
    cJSON *wind = cJSON_GetObjectItemCaseSensitive(json, "wind");

    if (main && weather && wind) {
        double temp = cJSON_GetObjectItemCaseSensitive(main, "temp")->valuedouble;
        double humidity = cJSON_GetObjectItemCaseSensitive(main, "humidity")->valuedouble;
        double windSpeed = cJSON_GetObjectItemCaseSensitive(wind, "speed")->valuedouble;

        // Print weather data
        printf("\n=== Current Weather Data ===\n");
        printf("City: %s\n", city);
        printf("Temperature: %.2f°C\n", temp);
        printf("Weather Description: %s\n", cJSON_GetObjectItemCaseSensitive(weather, "description")->valuestring);
        printf("Humidity: %.2f%%\n", humidity);
        printf("Wind Speed: %.2f m/s\n", windSpeed);
        printf("===========================\n");

        // Simulate microplastic pollution based on weather data
        simulate_microplastic_pollution(temp, windSpeed, humidity);
    } else {
        printf("Error extracting weather data\n");
    }

    // Clean up the JSON object
    cJSON_Delete(json);
    return 0;
}
