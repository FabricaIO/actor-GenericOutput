#include"GenericOutput.h"

/// @brief Creates a generic output
/// @param Name The device name
/// @param Pin Pin to use
/// @param configFile Name of the config file to use
GenericOutput::GenericOutput(String Name, int Pin, String configFile) : Actor(Name) {
	config_path = "/settings/act/" + configFile;
	output_config.Pin = Pin;
}

/// @brief Starts an output 
/// @return True on success
bool GenericOutput::begin() {
	// Set description
	Description.actionQuantity = 1;
	Description.type = "output";
	Description.actions = {{"state", 0}};
	// Create settings directory if necessary
	if (!checkConfig(config_path)) {
		// Set defaults
		return setConfig(getConfig(), true);
	} else {
		// Load settings
		return setConfig(Storage::readFile(config_path), false);
	}
}

/// @brief Receives an action
/// @param action The action to process (only option is 0 for set output)
/// @param payload A 0 or 1 to set the pin low or high
/// @return JSON response with OK
std::tuple<bool, String> GenericOutput::receiveAction(int action, String payload) {
	if (action == 0) {
		digitalWrite(output_config.Pin, payload.toInt());
	}	
	return { true, R"({"success": true})" };
}

/// @brief Gets the current config
/// @return A JSON string of the config
String GenericOutput::getConfig() {
	// Allocate the JSON document
	JsonDocument doc;
	// Assign current values
	doc["Name"] = Description.name;
	doc["Pin"] = output_config.Pin;
	// Create string to hold output
	String output;
	// Serialize to string
	serializeJson(doc, output);
	return output;
}

/// @brief Sets the configuration for this device
/// @param config A JSON string of the configuration settings
/// @param save If the configuration should be saved to a file
/// @return True on success
bool GenericOutput::setConfig(String config, bool save) {
	// Allocate the JSON document
  	JsonDocument doc;
	// Deserialize file contents
	DeserializationError error = deserializeJson(doc, config);
	// Test if parsing succeeds.
	if (error) {
		Logger.print(F("Deserialization failed: "));
		Logger.println(error.f_str());
		return false;
	}
	// Assign loaded values
	Description.name = doc["Name"].as<String>();
	output_config.Pin = doc["Pin"].as<int>();
	if (save) {
		if (!saveConfig(config_path, config)) {
			return false;
		}
	}
	return configureOutput();
}

/// @brief Configures the pin for use
/// @return True on success
bool GenericOutput::configureOutput() {
	pinMode(output_config.Pin, OUTPUT);
	return true;
}