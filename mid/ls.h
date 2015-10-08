#ifndef LS_H_
#define LS_H_
#endif

typedef enum {
	sortByName = 1 << 0,
	sortByATime = 1 << 1,
	sortByMTime = 1 << 2,
	sortByCTime = 1 << 3,
	sortBYSize  = 1 << 4,
	reverseSort = 1 << 5,
	noSort = 1 << 6
} SortingMethod;

// Default sorting method is by name
//extern enum  SortingMethod flg_sort;




