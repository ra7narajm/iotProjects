#ifndef __BLE_SERVICE_CONFIG_H_
#define __BLE_SERVICE_CONFIG_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

//BLE service configuration
/* Service description
   - Navigation Direction Service
     - Navigation Direction Characteristic Rx
   - Date Service
     - Date Characteristic Rx : reading "yyMMddHHmmssZ" (example, 010704120856-0700)
*/
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

//navigation service and characteristics
#define NAVIGATION_DIRECTION_SERVICE_UUID        "b8a71569-4d81-4146-9580-f3a4f2817cfb"
#define NAVIGATION_DIRECTION_CHARACTERISTIC_UUID "b8a71569-4d81-4146-9580-f3a4f2817cfb"

//date service and characteristics
#define NAVIGATION_DATE_SERVICE_UUID        "6ba7b810-9dad-11d1-80b4-00c04fd430c8"
#define NAVIGATION_DATE_CHARACTERISTIC_UUID "6ba7b811-9dad-11d1-80b4-00c04fd430c8"

#define SERVICE_UUID  "a2e12533-acba-46c7-b6cb-1379addd1fdd"
#define CHAR_UUID     "a2e12533-acba-46c7-b6cb-1379addd1fdd"

//#define CHAR_UUID     "c8abbcc7-6d0e-405d-8802-7aff86ea21fe"

void bleSetup(void);
void bleCommunicationTask(void * pvParameters);

#endif //__BLE_SERVICE_CONFIG_H_