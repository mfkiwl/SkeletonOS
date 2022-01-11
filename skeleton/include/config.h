#ifndef CONFIG_H_
#define CONFIG_H_

#define MAX_BUFFER_SIZE 8192
#define MEM_SIZE 256

extern int config_init(char *path);

#define CONFIG_INIT(FILE_NAME) \
	config_init(FILE_NAME)

extern int config_read_string(const char *key, char *value);
extern int config_read_double(const char *key, double *value);
extern int config_read_integer(const char *key, int *value);

#define CONFIG_READ_STRING(KEY, VALUE) \
	config_read_string(KEY, VALUE)

#define CONFIG_READ_DOUBLE(KEY, VALUE) \
	config_read_double(KEY, VALUE)

#define CONFIG_READ_INTEGER(KEY, VALUE) \
	config_read_integer(KEY, VALUE)

#endif /* CONFIG_H_ */