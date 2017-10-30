# Arduino Homebridge MQTT

Arduino library for connecting to Homebridge.

### Dependancies:
* [Mosquitto](https://mosquitto.org)
* [Homebridge](https://github.com/nfarina/homebridge)
* [Homebridge-MQTT](https://github.com/cflurin/homebridge-mqtt)

### Installation Guide
* [PlatformIO](http://platformio.org/lib)

homebridge/from/response
{"ack":false,"message":"name 'esp_swi1' undefined."}

topic: homebridge/from/response
payload:
{
	"esp_thermostat": {
		"services": {
			"esp_thermostat": "Thermostat"
		},
		"characteristics": {
			"esp_thermostat": {
				"CurrentHeatingCoolingState": 2,
				"TargetHeatingCoolingState": 0,
				"CurrentTemperature": 20,
				"TargetTemperature": 25,
				"TemperatureDisplayUnits": 0
			}
		}
	}
}
