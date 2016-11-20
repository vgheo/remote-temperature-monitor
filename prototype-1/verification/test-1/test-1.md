*Test 1*


SuT:
HW - as described
SW:
- esp8266/Arduino : 2.3.0
- AM2320 : vgheo/AM2320 	b42df3
- aws-sdk-esp8266 : 1be08402ac24
- remote-temp-sensor: 93a024a52bac3e9ead74195bfaffacfac4b7571b

- DynamoDB table:
https://eu-central-1.console.aws.amazon.com/dynamodb/home?region=eu-central-1#tables:selected=aws-esp8266-sample

#Test 1
1. load application onto d1m

OBSERVED
- serial console:
```
[...]
Reading sensor...humidity: 68.20%, temperature: 25.90*C
PutItem succeeded!
```
- dynamodb table : ./aws-esp8266-sample.csv

Verdict: PASS

