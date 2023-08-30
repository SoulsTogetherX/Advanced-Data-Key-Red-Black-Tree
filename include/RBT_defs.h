#ifndef _rbt_macro_HEADER
#define _rbt_macro_HEADER
#include	<stddef.h>
	/*	 GLOBAL VARIABLES	*/
		/*		  CUSTOM DATA TYPES		  */
enum rbtraversal	{TEST_ORDER, PRE_ORDER, IN_ORDER, POST_ORDER, KEY_DATA, LEVEL_ORDER, PRINT_LEVEL};
enum rbtsort		{KEY, KEY_R, DATA, DATA_R, KEYDATA, KEYDATA_R, DATAKEY, DATAKEY_R, NONE};

		/*		  OTHER DATA TYPES		  */
namespace {
	inline void *rbt__otherData__;
}

#endif /* _rbt_macro_HEADER */