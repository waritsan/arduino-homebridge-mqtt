# Arduino Homebridge MQTT

Arduino library for connecting to Homebridge.

### Dependancies:
* [Mosquitto](https://mosquitto.org)
* [Homebridge](https://github.com/nfarina/homebridge)
* [Homebridge-MQTT](https://github.com/cflurin/homebridge-mqtt)

### Installation Guide
* [PlatformIO](http://platformio.org/lib)

{
	"esp_thermostat": {
		"services": {
			"esp_thermostat": "Thermostat"
		},
		"characteristics": {
			"esp_thermostat": {
				"CurrentHeatingCoolingState": 1,
				"TargetHeatingCoolingState": 1,
				"CurrentTemperature": 29,
				"TargetTemperature": 35,
				"TemperatureDisplayUnits": 0
			}
		}
	}
}
