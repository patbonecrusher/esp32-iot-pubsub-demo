# ESP32 AWS IoT Thing

## Instructions

### Create an ios IOT thing:
https://github.com/patbonecrusher/pulumi-iotaws

### Create a secrets.h file with the following content:
```c
#include <pgmspace.h>

#define SECRET
#define THINGNAME "thingname_specified_in_policy_iot:Connect"

const char WIFI_SSID[] = "wifiname";
const char WIFI_PASSWORD[] = "password";
const char AWS_IOT_ENDPOINT[] = "yourendpointrt-ats.iot.your-region.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)KEY";
```

### Load and run
Build with platform io in vscode. TBD
Load on board and voila
