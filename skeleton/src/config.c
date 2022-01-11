#include <stdio.h>
#include <stdlib.h>

#include "arch.h"
#include "json.h"
#include "config.h"
#include "logger.h"

json_t const *jsonConfig = NULL;

int config_init(char *path)
{
	char buffer[MAX_BUFFER_SIZE];
	if (textFileRead(path, buffer) == 1)
	{
		while(true);
		return 1;
	}



	json_t mem[MEM_SIZE];
	jsonConfig = JSON_PARSE(buffer, mem, sizeof mem / sizeof *mem);

	if (!jsonConfig)
	{
		LOGGER_ERROR("Error json create.");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int config_read_string(const char *key, char *value)
{
	json_t const *element = JSON_GET_PROPERTY(jsonConfig, key);

	if (!element || JSON_TEXT != JSON_GET_PROPERTY_TYPE(element))
	{
		LOGGER_ERROR("Error, the element \"%s\" is not found.\n", key);
		return EXIT_FAILURE;
	}
	else
	{
		const char *value_tmp = JSON_GET_PROPERTY_VALUE(element);
		memcpy(value, value_tmp, strlen(value_tmp) + 1);

		return EXIT_SUCCESS;
	}
}

int config_read_double(const char *key, double *value)
{
	json_t const *element = JSON_GET_PROPERTY(jsonConfig, key);

	// the element can be also an integer, but we want to read it as a double
	if (!element || (JSON_REAL != JSON_GET_PROPERTY_TYPE(element) && JSON_INTEGER != JSON_GET_PROPERTY_TYPE(element)))
	{
		LOGGER_ERROR("Error, the element \"%s\" is not found.\n", key);
		return EXIT_FAILURE;
	}
	else
	{
		double value_tmp = JSON_GET_PROPERTY_VALUE_AS_DOUBLE(element);
		*value = value_tmp;

		return EXIT_SUCCESS;
	}
}

int config_read_integer(const char *key, int *value)
{
	json_t const *element = JSON_GET_PROPERTY(jsonConfig, key);

	if (!element || JSON_INTEGER != JSON_GET_PROPERTY_TYPE(element))
	{
		LOGGER_ERROR("Error, the element \"%s\" is not found.\n", key);
		return EXIT_FAILURE;
	}
	else
	{
		int value_tmp = JSON_GET_PROPERTY_VALUE_AS_INT(element);
		*value = value_tmp;

		return EXIT_SUCCESS;
	}
}
