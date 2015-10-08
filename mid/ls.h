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

extern SortingMethod flg_sort;
extern int flg_noSort;
extern int flg_R;


