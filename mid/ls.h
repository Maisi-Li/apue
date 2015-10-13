#ifndef LS_H_
#define LS_H_



typedef enum {
	sortByName ,
	sortByATime,
	sortByMTime,
	sortByCTime,
	sortBySize
} SortingMethod;

typedef enum {
	in_1,
	in_C,
	in_l,
	in_n,
	in_x
} DisplayFormat;

typedef enum {
	in_q,
	in_w
} NonPrintableMethod;

typedef enum {
	in_h,
	in_k
} Block;

typedef struct {
	int count;
	int column;
	int l_ino;
	int l_nlink;
	int l_uid;
	int l_gid;
	long unsigned l_size;
	int l_name;
	long unsigned l_blocks;
	int l_major;
	int l_minor;

} Length;



extern SortingMethod flg_sort;
extern int flg_noSort;
extern int flg_R;

extern DisplayFormat flg_display;
extern NonPrintableMethod flg_nonPrintable;

extern Length fileLength;
extern int windowWidth;

extern int flg_h;
extern int flg_s;
extern Block flg_block;
#endif
