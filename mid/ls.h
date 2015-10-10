#ifndef LS_H_
#define LS_H_
#endif



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

typedef struct {
	int count;
	int l_inode;
	int l_nlink;
	int l_uid;
	int l_gid;
	int l_size;
	int l_name;
	int l_block;

} Length;



extern SortingMethod flg_sort;
extern int flg_noSort;
extern int flg_R;

extern DisplayFormat flg_display;
extern NonPrintableMethod flg_nonPrintable;

extern int windowWidth;
