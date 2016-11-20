
/*
 * 
 *  Based on sample from 
 *  https://github.com/daniele-salvagni/aws-sdk-esp8266
 */

/* This sample uses PutItem on a DynamoDB table to save the state of a virtual
 * temperature sensor.
 *
 * Configuration:
 *  - AWS
 *    AWS_REGION
 *    AWS_ENDPOINT
 *    awsKeyID
 *    awsSecKey
 *  - DynamoDB:
 *    - TABLE_NAME - 
 *      - HASH_KEY_NAME - partition key
 *      - RANGE_KEY_NAME - sort key
 *   - wifi:
 *      pSSID
 *      pPassword
 *      
  */

#include <Wire.h>
#include <AM2320.h>

#include <ESP8266WiFi.h>


#include "Esp8266AWSImplementations.h"
#include "AmazonDynamoDBClient.h"
#include "AWSFoundationalTypes.h"

#include "config.h"



const char* HOST_NAME = "remote-temp-sensor-1";

/* Temperature reading. */
int reading;

Esp8266HttpClient httpClient;
Esp8266DateTimeProvider dateTimeProvider;

AmazonDynamoDBClient ddbClient;

/* Reused objects. */
GetItemInput getItemInput;
PutItemInput putItemInput;
AttributeValue hashKey;
AttributeValue rangeKey;
ActionError actionError;

AM2320 th;

void setup() {

    /* Begin serial communication.
    note: esp8266 only supports this rate
    */
    Serial.begin(115200);
    Serial.println();

    Serial.println("wire.begin()...");
    Wire.begin();
    
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(pSSID);

    WiFi.hostname(HOST_NAME);

    // If pPassword is set the most secure supported mode will be automatically selected
    WiFi.begin(pSSID, pPassword);

    // Wait for WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println();

    /* Initialize ddbClient. */
    ddbClient.setAWSRegion(AWS_REGION);
    ddbClient.setAWSEndpoint(AWS_ENDPOINT);
    ddbClient.setAWSSecretKey(awsSecKey);
    ddbClient.setAWSKeyID(awsKeyID);
    ddbClient.setHttpClient(&httpClient);
    ddbClient.setDateTimeProvider(&dateTimeProvider);
}

/*
 * 
 * Note. printf %f is not supported by arduino
 *    -  http://forum.arduino.cc/index.php?topic=44262.0
 * 
 */
void setItem(PutItemInput& putItemInput, float temp, float humid) {

    /* Create an Item. */
    AttributeValue id;
    id.setS(HASH_KEY_VALUE);
    
    AttributeValue timest;
    timest.setN(dateTimeProvider.getDateTime());

    /* Create an AttributeValue for 'temp', convert the number to a
     * string (AttributeValue object represents numbers as strings), and
     * use setN to apply that value to the AttributeValue. */
    char numberBuffer[20];
    AttributeValue tempAV;
    int cn=snprintf(numberBuffer, sizeof(numberBuffer), "%d.%d", (int)temp, ((int)(temp*100)) % 100 );
    tempAV.setN(numberBuffer);    

    AttributeValue humidAV;
    snprintf(numberBuffer, sizeof(numberBuffer), "%d.%d", (int)humid, ((int)(humid*100)) % 100  );
    humidAV.setN(numberBuffer);

    /* Create the Key-value pairs and make an array of them. */
    MinimalKeyValuePair < MinimalString, AttributeValue> att1(HASH_KEY_NAME, id);
    MinimalKeyValuePair < MinimalString, AttributeValue> att2(RANGE_KEY_NAME, timest);
    MinimalKeyValuePair < MinimalString, AttributeValue> att3("temp", tempAV);
    MinimalKeyValuePair < MinimalString, AttributeValue> att4("humid", humidAV);

    MinimalKeyValuePair<MinimalString, AttributeValue> itemArray[] = 
      { att1, att2, att3, att4 };

    /* Set values for putItemInput. */
    putItemInput.setItem(MinimalMap < AttributeValue > (itemArray, 4));
    putItemInput.setTableName(TABLE_NAME);  
}

void putItem(const PutItemInput& item) {

    /* Perform putItem and check for errors. */
    PutItemOutput putItemOutput = ddbClient.putItem(item, actionError);
    switch (actionError) {
    case NONE_ACTIONERROR:
        Serial.println("PutItem succeeded!");
        break;
    case INVALID_REQUEST_ACTIONERROR:
        Serial.print("ERROR: Invalid request");
        Serial.println(putItemOutput.getErrorMessage().getCStr());
        break;
    case MISSING_REQUIRED_ARGS_ACTIONERROR:
        Serial.println(
                "ERROR: Required arguments were not set for PutItemInput");
        break;
    case RESPONSE_PARSING_ACTIONERROR:
        Serial.println("ERROR: Problem parsing http response of PutItem");
        break;
    case CONNECTION_ACTIONERROR:
        Serial.println("ERROR: Connection problem");
        break;
    }
     /* wait to not double-record */
    delay(750);
}

void loop() {
    
    Serial.print(" Reading sensor...");
    
    
    switch(th.Read()) {
      case 2:
        Serial.println("CRC failed");
        break;
      case 1:
        Serial.println("Sensor offline");
        break;
      case 0:
        Serial.print("humidity: ");
        Serial.print(th.h);
        Serial.print("%, temperature: ");
        Serial.print(th.t);
        Serial.println("*C");
        break;
    }

    setItem(putItemInput, th.t, th.h );
    
    putItem(putItemInput);

    delay(5 * 1000);
}

