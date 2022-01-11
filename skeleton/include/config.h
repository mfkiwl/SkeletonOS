#ifndef CONFIG_H_
#define CONFIG_H_

#define MAX_BUFFER_SIZE 8192
#define MEM_SIZE 256

extern int config_init(char *path);

#define CONFIG_INIT(A) \
	config_init(A)

extern int config_read_string(const char *key, char *value);
extern int config_read_double(const char *key, double *value);
extern int config_read_integer(const char *key, int *value);

#define CONFIG_READ_STRING(A, B) \
	config_read_string(A, B)

#define CONFIG_READ_DOUBLE(A, B) \
	config_read_double(A, B)

#define CONFIG_READ_INTEGER(A, B) \
	config_read_integer(A, B)

#endif /* CONFIG_H_ */