#include "esp32/config.h"
#include "nvs.h" // Have this to do low level storage debug output
#include "esp_partition.h"  // Have this to access flash partition table

void printNvsStats() {
  nvs_stats_t nvs_stats;
  if (!nvs_get_stats(NULL, &nvs_stats)) {
    Serial.print("\n\nNon-Volatile Storage Stats");
    Serial.print("\n used_entries: ");
    Serial.print(nvs_stats.used_entries);
    Serial.print("\n free_entries: ");
    Serial.print(nvs_stats.free_entries);
    Serial.print("\n total_entries: ");
    Serial.print(nvs_stats.total_entries);
    Serial.print("\n namespace_count: ");
    Serial.print(nvs_stats.namespace_count);
  } else {
    Serial.println("Error getting NVS Stats!");
  }
}

void printRamStats() {
  Serial.print("\n\nInternal Ram Stats");
  Serial.print("\n Heap Size: ");
  Serial.print(ESP.getHeapSize()/1024);
  Serial.print("Kb");
  Serial.print("\n Free Heap: ");
  Serial.print(ESP.getFreeHeap()/1024);
  Serial.print("Kb");

  Serial.print("\n\nSPI RAM stats");
  Serial.print("\n PSRAM Size: ");
  Serial.print(ESP.getPsramSize()/1024);
  Serial.print("Kb");
  Serial.print("\n Free PSRAM: ");
  Serial.print(ESP.getFreePsram()/1024);
  Serial.print("Kb");
}

// How Partitions are configured:
// - in esp32-arduino tools/partitions there are csv files defining partions
//   Can define there how big spiffs partition is vs apps, nvs, etc. 
//   Define own crazy partitions, whatever.
// - esp32-arduino boards.text for ttgo t-watch can get the partition into arduino menu
//   This is where we set the max size of the app executable.
// More deets: https://robotzero.one/arduino-ide-partitions/

void printFlashStats() {
  Serial.print("\n\nFlash Stats");
  Serial.print("\n Flash Chip Size: ");
  Serial.print(ESP.getFlashChipSize()/1024);
  Serial.print("Kb");
  
  Serial.print("\n Flash Chip Speed: ");
  Serial.print(ESP.getFlashChipSpeed());
  Serial.print("\n Flash Chip Mode: ");
  Serial.print(ESP.getFlashChipMode(), HEX);

  // App partitions
  Serial.print("\n\nApp Partitions");
  for (esp_partition_iterator_t i = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL); 
       i != NULL; i = esp_partition_next(i)) {
    const esp_partition_t * partition_info = esp_partition_get(i);
    Serial.print("\n Label: ");
    Serial.print(partition_info->label);
    Serial.print("\n Type: ");
    Serial.print(partition_info->type, HEX);
    Serial.print("\n Subtype: ");
    Serial.print(partition_info->subtype, HEX);
    Serial.print("\n Size: ");
    Serial.print(partition_info->size);
    Serial.print("\n Address: ");
    Serial.print(partition_info->address);
    Serial.print("\n");
  }

  // Data partitions
  Serial.print("\nData Partitions");
  for (esp_partition_iterator_t i = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL); 
       i != NULL; i = esp_partition_next(i)) {
    const esp_partition_t * partition_info = esp_partition_get(i);
    Serial.print("\n Label: ");
    Serial.print(partition_info->label);
    Serial.print("\n Type: ");
    Serial.print(partition_info->type, HEX);
    Serial.print("\n Subtype: ");
    Serial.print(partition_info->subtype, HEX);
    Serial.print("\n Size: ");
    Serial.print(partition_info->size);
    Serial.print("\n Address: ");
    Serial.print(partition_info->address);
    Serial.print("\n");
  }

}
