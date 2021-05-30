#ifndef FS_H_
#define FS_H_

//extern void fs_init(const char*);
extern int fs_write(const char *, void *, int);
extern int fs_read(const char *, void *, int);

#define FS_WRITE(A, B, C) \
	fs_write(A, B, C);

#define FS_READ(A, B, C) \
	fs_read(A, B, C)

#endif // !FS_H_