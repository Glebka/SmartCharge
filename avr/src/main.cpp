#include <Arduino.h>

/**
 * Major and minor version of the device
 * Will be packed into a single byte
 **/
const byte VERSION_MAJOR = 1;
const byte VERSION_MINOR = 0;

const unsigned long BAUD_RATE = 9600;

#define MOSFET_PIN 2

/**
 * Defines incoming commands and outgoing response types supported
 * by the device 
 **/
namespace Operation {
  enum OperationType {
    CMD_CHARGE_ON = 1,        // Enable charging, let the current flow thru the device
    CMD_CHARGE_OFF = 2,           // Stop charging
    CMD_SET_CURRENT_LIMIT = 3,    // Limit charging current to the given value
    CMD_UNSET_CURRENT_LIMIT = 4,  // Disable charging current limitation
    CMD_VERSION = 5,              // Get SmartCharger device version
    CMD_GET_STATE = 6,            // Get SmartCharger device state structure
    RESP_OK = 7,                  // Respond with success status
    RESP_FAIL = 8                 // Respond with error status and error code
  };
}

/**
 * Defines error codes
 **/
namespace Error {
  enum ErrorCode {
    ERR_OK,           // Success code
    ERR_WRONG_ARG,    // Wrong argument value
    ERR_UNSUPPORTED,  // Unsupported command
    ERR_FAIL          // Unexpected error
  };
}

/**
 * Data structure that holds command 
 * or response code and optional argument
 **/
struct Packet {
  byte op;          // see Operation::OperationType
  byte arg;         // if op == CMD_SET_CURRENT, arg is a target charing current in mA multiplied by 10^(-2),
                    // Example: in order to set charging current eq to 500 mA, the arg value should be 500 * 10^(-2) => 5
  byte reserved[2]; // struct padding
};

struct DeviceState {
  bool isChargingEnabled;
  bool isCurrentLimitEnabled;
  byte currentLimit;
  byte reserved;                // struct padding
};

static Packet buf;
static DeviceState state;

void send(byte respType, byte code) {
  buf.op = respType;
  buf.arg = code;
  Serial.write((uint8_t*) &buf, sizeof(Packet));
}

void send(DeviceState* state) {
  Serial.write((uint8_t*) state, sizeof(DeviceState));
}

void setup() {
  memset(&state, 0, sizeof(DeviceState));
  memset(&buf, 0, sizeof(Packet));

  pinMode(MOSFET_PIN, OUTPUT);

	Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  send(Operation::RESP_OK, (VERSION_MAJOR << 4) | VERSION_MINOR);
  send(&state);
}

void loop() {

  if (Serial.available() >= sizeof(Packet)) {
    Serial.readBytes((uint8_t*) &buf, sizeof(Packet));
    switch (buf.op)
    {
    case Operation::CMD_VERSION:
      send(Operation::RESP_OK, (VERSION_MAJOR << 4) | VERSION_MINOR);
      break;
    case Operation::CMD_CHARGE_ON:
      state.isChargingEnabled = true;
      send(Operation::RESP_OK, Error::ERR_OK);
      break;
    case Operation::CMD_CHARGE_OFF:
      state.isChargingEnabled = false;
      send(Operation::RESP_OK, Error::ERR_OK);
      break;
    case Operation::CMD_GET_STATE:
      send(Operation::RESP_OK, Error::ERR_OK);
      send(&state);
      break;
    default:
      send(Operation::RESP_FAIL, Error::ERR_UNSUPPORTED);
      break;
    }
  }

  digitalWrite(MOSFET_PIN, state.isChargingEnabled);

}