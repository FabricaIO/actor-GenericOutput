/*
* This file and associated .cpp file are licensed under the GPLv3 License Copyright (c) 2024 Sam Groveman
* 
* ArduinoJSON: https://arduinojson.org/
*
* Contributors: Sam Groveman
*/
#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Actor.h>

/// @brief Class describing a generic output on a GPIO pin
class GenericOutput : public Actor {
	protected:
		/// @brief Output configuration
		struct {
			/// @brief The pin number attached to the output
			int Pin;
		} output_config;

		/// @brief Path to configuration file
		String config_path;

		bool configureOutput();

	public:
		GenericOutput(String Name, int Pin, String configFile = "GenericOutput.json");
		bool begin();
		std::tuple<bool, String> receiveAction(int action, String payload = "");
		String getConfig();
		bool setConfig(String config, bool save);
};