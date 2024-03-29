#ifndef _rbt_main_HEADER
#define _rbt_main_HEADER
#include	"RBT_defs.h"
#include	<iostream>
#include	<string>

/*        CUSTOM DATA TYPES        */
template <typename K = int, typename D = int>
class rbTree;

/*    Basic Node of Tree    */
template <typename K = int, typename D = int>
class rbtNode {
	template <typename T, typename U> friend class rbTree;

		// Base Variables
	bool color;
	K key;
	D data;
	size_t descendants;

		// Node Structure
	rbtNode<K,D> *left, *right, *parent;

	public:
			// Public Node Functions
		rbtNode(K, D);
		rbtNode(const rbtNode<K,D> *);
		D rbt_getData() const;
		K rbt_getKey() const;

		bool equal(const rbtNode<K,D> *);
};

/*            Tree Class           */
template <typename K, typename D>
class rbTree {
	// Base Variables
	size_t size;
	rbtNode<K,D> *root;

	// Private Functions
		// RBT_main
	void dummy() {}
	void leftRotate(rbtNode<K,D> *);
	void rightRotate(rbtNode<K,D> *);
	void deleteBranch(rbtNode<K,D> *);
	void copyTree(rbtNode<K,D> *, rbtNode<K,D> **);
	void rbt_free();

		// RBT_traversal
	template <typename F>
		inline size_t nodesSpecifiedLevelBase(size_t, rbtNode<K,D>, F) const;

	void nodesPre(rbtNode<K,D> *) const;
	void nodesPreWithKey(rbtNode<K,D> *, K) const;
	void nodesPreWithData(rbtNode<K,D> *, D) const;
	void nodesPreWithDataKey(rbtNode<K,D> *, D, K) const;
	void nodesIn(rbtNode<K,D> *) const;
	void nodesInWithKey(rbtNode<K,D> *, K) const;
	void nodesInWithData(rbtNode<K,D> *, D) const;
	void nodesInWithDataKey(rbtNode<K,D> *, D, K) const;
	void nodesPost(rbtNode<K,D> *) const;
	void nodesPostWithKey(rbtNode<K,D> *, K) const;
	void nodesPostWithData(rbtNode<K,D> *, D) const;
	void nodesPostWithDataKey(rbtNode<K,D> *, D, K) const;
	void nodesLevel() const;
	void nodesLevelWithKey(K) const;
	void nodesLevelWithData(D) const;
	void nodesLevelWithDataKey(D, K) const;
	size_t nodesSpecifiedLevel(size_t) const;
	size_t nodesSpecifiedLevelWithKey(size_t, K) const;
	size_t nodesSpecifiedLevelWithData(size_t, D) const;
	size_t nodesSpecifiedLevelWithDataKey(size_t, D, K) const;
	void getAllKeys(rbtNode<K,D> *) const;
	void getAllData(rbtNode<K,D> *) const;
	void countKey(rbtNode<K,D> *, K) const;
	void countData(rbtNode<K,D> *, D) const;
	void countKeyData(rbtNode<K,D> *, K, D) const;

		// RBT_access
	inline size_t findIdxBase(rbtNode<K,D> *) const;
	rbtNode<K,D> *key_maxData(rbtNode<K,D> *, K) const;
	rbtNode<K,D> *key_minData(rbtNode<K,D> *, K) const;

	size_t getHeight(rbtNode<K,D> *) const;
	void getWidth(rbtNode<K,D> *, size_t) const;
	size_t getLeafNodes(rbtNode<K,D> *) const;
	rbtNode<K,D> *searchData(rbtNode<K,D> *, D ) const;
	rbtNode<K,D> *dataMax(rbtNode<K,D> *) const;
	rbtNode<K,D> *dataMin(rbtNode<K,D> *) const;
	rbtNode<K,D> *search(rbtNode<K,D> *, K, D) const;

		// RBT_sort
	size_t treeify(size_t, size_t, rbtNode<K,D> **, rbtNode<K,D> **);
	size_t treeifyShallow(size_t, size_t, rbtNode<K,D> **, rbtNode<K,D> **);
	template <K, D>
		inline friend void mergeSortBase(rbtNode<K,D> **, size_t, size_t, size_t);
	template <K, D>
		inline friend void mergeSortCallerBase(rbtNode<K,D> **, size_t, size_t);

		// RBT_indel
	template <typename F>
		void rbt_repInsertBase(size_t, F, bool);
	template <typename F>
		inline void repDeleteBase(size_t, rbtNode<K,D> **, F, bool);
	template <typename F, typename T>
		inline void repDeleteBase(size_t, T *, F, bool);
	template <typename F1, typename F2, typename T>
		inline void removeBase(size_t, rbtNode<K,D> **, T *, F1, F2, bool);
	template <typename F, typename T>
		inline void removeRawBase(size_t, T *, F, bool);
	template <typename F, typename T>
		inline void removeBase(T, F);
	template <typename F>
		inline void removeDuplicatesBase(F);

		// RBT_ops
	inline rbTree<K,D> subBase(const rbTree<K,D> &, void (*foo)(rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&)) const;
	inline rbTree<K,D> *divideBase(
		size_t div,
		enum rbtraversal traversal,
		rbTree<K,D> *(rbTree<K, D>::*foo)(),
		void (rbTree<K,D>::*bar)(size_t, rbtNode<K,D> **, rbTree<K,D> *, enum rbtraversal, size_t (rbTree<K, D>::*)(size_t, size_t, rbtNode<K,D> **, rbtNode<K,D> **)),
		size_t (rbTree<K,D>::*nar)(size_t, size_t, rbtNode<K,D> **, rbtNode<K,D> **));
	inline rbTree<K,D> *deepDivideHelper();
	inline rbTree<K,D> *shallowDivideHelper();
	inline void distributeDivideHelper(
		size_t,
		rbtNode<K,D> **,
		rbTree<K,D> *,
		enum rbtraversal,
		size_t (rbTree<K, D>::*)(size_t, size_t, rbtNode<K,D> **, rbtNode<K,D> **));
	inline void scatterDivideHelper(
		size_t,
		rbtNode<K,D> **,
		rbTree<K,D> *,
		enum rbtraversal,
		size_t (rbTree<K, D>::*)(size_t, size_t, rbtNode<K,D> **, rbtNode<K,D> **));

		// RBT_setops
	template <typename F>
		inline rbTree<K,D> orBase(
								 const rbTree<K,D> &,
								 void (*)(rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&),
								 F,
								 bool = false,
								 bool = false
								 ) const;
	template <typename F>
		inline rbTree<K,D> orBaseI(
								  const rbTree<K,D> &,
								  void (*)(rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&),
								  F,
								  bool = false,
								  bool = false
								  );
	template <typename F>
		inline rbTree<K,D> andBase(
								  const rbTree<K,D> &,
								  void (*)(rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&),
								  F,
								  bool = false
								  ) const;
	template <typename F>
		inline rbTree<K,D> andBaseI(
								   const rbTree<K,D> &,
								   void (*)(rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&, rbtNode<K,D> **&),
								   F,
								   bool = false
								   );

		// RBT_limit
	template <typename F>
		void rbt_clampBase(F);

		// RBT_value
	void nodeReposition(rbtNode<K,D> **, rbtNode<K,D> **, rbtNode<K,D> **, rbtNode<K,D> *);
	template <typename F>
		inline void rbt_setBase(rbtNode<K,D> *, F);
	template <typename F>
		inline void rbt_funcAllBase(F);

		// RBT_string
	void testOrder(rbtNode<K,D> *) const;
	void preOrder(rbtNode<K,D> *) const;
	void inOrder(rbtNode<K,D> *) const;
	void postOrder(rbtNode<K,D> *) const;
	void keyAndData(rbtNode<K,D> *) const;
	void printLevel(rbtNode<K,D> *, size_t) const;
	void levelOrder() const;

	public:
		// Constructors
			// RBT_main
		rbTree();
		rbTree(const rbTree<K,D> &);
		~rbTree();
			// RBT_sort
		rbTree(size_t, rbtNode<K,D> **&);


		// Public Tree Functions
			// RBT_main
		bool rbt_isEmpty() const;
		size_t rbt_getSize() const;
		rbtNode<K,D> *rbt_getRoot() const;

		void rbt_insert(K, D);
		D rbt_delete(rbtNode<K,D> *);
		D rbt_pop();

		rbTree<K,D> rbt_copy() const;
		void operator=(const rbTree<K,D> &);

		void rbt_clear();

			// RBT_traversal
		rbtNode<K,D> **resize_helper(rbtNode<K,D> **, size_t) const;

		rbtNode<K,D> **rbt_getAllNodes(enum rbtraversal = IN_ORDER) const;
		rbtNode<K,D> **rbt_getAllNodes(rbtNode<K,D> **, enum rbtraversal = IN_ORDER) const;

		rbtNode<K,D> **rbt_getAllNodesAtLevel(size_t &, size_t) const;
		rbtNode<K,D> **rbt_getAllNodesAtLevel(rbtNode<K,D> **, size_t &, size_t) const;

		rbtNode<K,D> **rbt_getAllNodesWithKey(K, size_t &, enum rbtraversal = IN_ORDER, size_t = 0) const;
		rbtNode<K,D> **rbt_getAllNodesWithKey(rbtNode<K,D> **, K, size_t &, enum rbtraversal = IN_ORDER, size_t = 0) const;

		rbtNode<K,D> **rbt_getAllNodesWithData(D, size_t &, enum rbtraversal = IN_ORDER, size_t = 0) const;
		rbtNode<K,D> **rbt_getAllNodesWithData(rbtNode<K,D> **, D, size_t &, enum rbtraversal = IN_ORDER, size_t = 0) const;

		rbtNode<K,D> **rbt_getAllNodesWithDataKey(D, K, size_t &, enum rbtraversal = IN_ORDER, size_t = 0) const;
		rbtNode<K,D> **rbt_getAllNodesWithDataKey(rbtNode<K,D> **, D, K, size_t &, enum rbtraversal = IN_ORDER, size_t = 0) const;

		K *rbt_getAllKeys(K * = 0) const;
		D *rbt_getAllData(D * = 0) const;

		rbtNode<K,D> *rbt_findKey(K) const;
		rbtNode<K,D> *rbt_findData(D) const;
		rbtNode<K,D> *rbt_findDataKey(D, K) const;

		size_t rbt_countKey(K) const;
		size_t rbt_countData(D) const;
		size_t rbt_countKeyData(K, D) const;

			// RBT_access
		size_t rbt_getHeightExact() const;
		size_t rbt_getHeight() const;

		size_t rbt_getWidthExact() const;
		size_t *rbt_getWidths(size_t &) const;
		size_t rbt_getWidth() const;

		size_t rbt_getLeafNodes() const;
		size_t rbt_getInnerNodes() const;

		inline size_t rbt_findIdxKey(K) const;
		inline size_t rbt_findIdxData(D) const;
		inline size_t rbt_findIdx(K, D) const;
		inline size_t rbt_findIdx(rbtNode<K,D> *) const;
		rbtNode<K,D> *rbt_nodeAt(size_t = 0) const;

		rbtNode<K,D> *rbt_searchKey(K) const;
		rbtNode<K,D> *rbt_searchKey(rbtNode<K,D> *) const;
		rbtNode<K,D> *rbt_maxKey() const;
		rbtNode<K,D> *rbt_minKey() const;

		rbtNode<K,D> *rbt_searchData(D) const;
		rbtNode<K,D> *rbt_searchData(rbtNode<K,D> *) const;
		rbtNode<K,D> *rbt_maxData() const;
		rbtNode<K,D> *rbt_minData() const;

		rbtNode<K,D> *rbt_search(K, D) const;
		rbtNode<K,D> *rbt_search(rbtNode<K,D> *) const;

		rbtNode<K,D> *rbt_maxKey_maxData() const;
		rbtNode<K,D> *rbt_maxKey_minData() const;
		rbtNode<K,D> *rbt_minKey_maxData() const;
		rbtNode<K,D> *rbt_minKey_minData() const;

		rbtNode<K,D> *rbt_maxData_maxKey() const;
		rbtNode<K,D> *rbt_minData_maxKey() const;
		rbtNode<K,D> *rbt_maxData_minKey() const;
		rbtNode<K,D> *rbt_minData_minKey() const;

			// RBT_sort
		inline static bool keyCompair(rbtNode<K,D> *, rbtNode<K,D> *);
		inline static bool keyCompairR(rbtNode<K,D> *, rbtNode<K,D> *);
		inline static bool dataCompair(rbtNode<K,D> *, rbtNode<K,D> *);
		inline static bool dataCompairR(rbtNode<K,D> *, rbtNode<K,D> *);
		inline static bool keyDataCompair(rbtNode<K,D> *, rbtNode<K,D> *);
		inline static bool keyDataCompairR(rbtNode<K,D> *, rbtNode<K,D> *);
		inline static bool dataKeyCompair(rbtNode<K,D> *, rbtNode<K,D> *);
		inline static bool dataKeyCompairR(rbtNode<K,D> *, rbtNode<K,D> *);

		void rbt_treeify(size_t, rbtNode<K,D> **);
		template <K, D>
			friend void rbt_nodeSort(rbtNode<K,D> **, size_t, size_t, enum rbtsort);
		template <K, D>
			friend void rbt_nodeSortCustom(rbtNode<K,D> **, void (*foo)(rbtNode<K,D> *, rbtNode<K,D> *), size_t, size_t);

			// RBT_indel
		void rbt_repInsert(size_t, K * = NULL, D * = NULL);
		void rbt_repInsert(size_t, rbtNode<K,D> **);
		void rbt_repInsertFunc(size_t,
							   K = K(),
							   K (*)(K, D) = [] (K st, D) -> K {return st + 1;},
							   D * = NULL);
		void rbt_repInsertInc(size_t, K = K(), K = K(1), D * = NULL);
		void rbt_repInsertDec(size_t, K = K(), K = K(1), D * = NULL);
		void rbt_repInsertFor(
							 void (*initialize)(K &, D &) = [] (K &key, D &data) -> void {key = 0; data = 0;},
							 bool (*condition)(K &, D &) = [] (K &key, D &) -> bool {return key < 10;},
							 void (*step)(K &, D &) = [] (K &key, D &data) -> void {key += 1; data += 1;}
							 );

		void rbt_repDelete(size_t, rbtNode<K,D> **);
		void rbt_repDelete(size_t, K *, D *);
		void rbt_repDeleteKey(size_t, K *);
		void rbt_repDeleteKey(size_t arrayLength, rbtNode<K,D> **);
		void rbt_repDeleteData(size_t, D *);
		void rbt_repDeleteData(size_t arrayLength, rbtNode<K,D> **);

		void rbt_remove(rbtNode<K,D> *);
		void rbt_remove(size_t, rbtNode<K,D> **);
		void rbt_remove(K, D);
		void rbt_remove(size_t, K *, D *);
		void rbt_removeKey(K);
		void rbt_removeKey(rbtNode<K,D> *);
		void rbt_removeKey(size_t, K *);
		void rbt_removeKey(size_t, rbtNode<K,D> **);
		void rbt_removeData(D);
		void rbt_removeData(rbtNode<K,D> *);
		void rbt_removeData(size_t, D *);
		void rbt_removeData(size_t, rbtNode<K,D> **);

		void rbt_removeDuplicates();
		void rbt_removeDuplicateKeys();
		void rbt_removeDuplicateData();

		D rbt_delete(K, D);
		D rbt_deleteKey(K);
		D rbt_deleteData(D);

			// RBT_operation
				// TREE UNION AND INTERSECTION
		rbTree<K,D> operator|(const rbTree<K,D> &) const;
		rbTree<K,D> operator|=(const rbTree<K,D> &);
		rbTree<K,D> rbt_or(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_orI(const rbTree<K,D> &);
		rbTree<K,D> rbt_orLeast(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_orLeastI(const rbTree<K,D> &);
		rbTree<K,D> rbt_orMost(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_orMostI(const rbTree<K,D> &);

		rbTree<K,D> rbt_orKey(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_orKeyI(const rbTree<K,D> &);
		rbTree<K,D> rbt_orKeyLeast(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_orKeyLeastI(const rbTree<K,D> &);
		rbTree<K,D> rbt_orKeyMost(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_orKeyMostI(const rbTree<K,D> &);

		rbTree<K,D> rbt_orData(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_orDataI(const rbTree<K,D> &);
		rbTree<K,D> rbt_orDataLeast(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_orDataLeastI(const rbTree<K,D> &);
		rbTree<K,D> rbt_orDataMost(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_orDataMostI(const rbTree<K,D> &);

		rbTree<K,D> operator^(const rbTree<K,D> &) const;
		rbTree<K,D> operator^=(const rbTree<K,D> &);
		rbTree<K,D> rbt_xor(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_xorI(const rbTree<K,D> &);
		rbTree<K,D> rbt_xorLeast(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_xorLeastI(const rbTree<K,D> &);
		rbTree<K,D> rbt_xorMost(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_xorMostI(const rbTree<K,D> &);

		rbTree<K,D> rbt_xorKey(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_xorKeyI(const rbTree<K,D> &);
		rbTree<K,D> rbt_xorKeyLeast(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_xorKeyLeastI(const rbTree<K,D> &);
		rbTree<K,D> rbt_xorKeyMost(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_xorKeyMostI(const rbTree<K,D> &);

		rbTree<K,D> rbt_xorData(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_xorDataI(const rbTree<K,D> &);
		rbTree<K,D> rbt_xorDataLeast(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_xorDataLeastI(const rbTree<K,D> &);
		rbTree<K,D> rbt_xorDataMost(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_xorDataMostI(const rbTree<K,D> &);

		rbTree<K,D> operator&(const rbTree<K,D> &) const;
		rbTree<K,D> operator&=(const rbTree<K,D> &);
		rbTree<K,D> rbt_and(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_andI(const rbTree<K,D> &);
		rbTree<K,D> rbt_andMost(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_andMostI(const rbTree<K,D> &);
		rbTree<K,D> rbt_andLeast(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_andLeastI(const rbTree<K,D> &);

		rbTree<K,D> rbt_andKey(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_andKeyI(const rbTree<K,D> &);
		rbTree<K,D> rbt_andKeyMost(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_andKeyMostI(const rbTree<K,D> &);
		rbTree<K,D> rbt_andKeyLeast(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_andKeyLeastI(const rbTree<K,D> &);

		rbTree<K,D> rbt_andData(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_andDataI(const rbTree<K,D> &);
		rbTree<K,D> rbt_andDataMost(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_andDataMostI(const rbTree<K,D> &);
		rbTree<K,D> rbt_andDataLeast(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_andDataLeastI(const rbTree<K,D> &);

				// TREE BASIC OPERATIONS
		rbTree<K,D> operator+(const rbTree<K,D> &) const;
		rbTree<K,D> operator+=(const rbTree<K,D> &);
		rbTree<K,D> rbt_add(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_addI(const rbTree<K,D> &);

		rbTree<K,D> operator-(const rbTree<K,D> &) const;
		rbTree<K,D> operator-=(const rbTree<K,D> &);
		rbTree<K,D> rbt_sub(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_subI(const rbTree<K,D> &);
		rbTree<K,D> rbt_subKeys(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_subKeysI(const rbTree<K,D> &);
		rbTree<K,D> rbt_subData(const rbTree<K,D> &) const;
		rbTree<K,D> rbt_subDataI(const rbTree<K,D> &);

		rbTree<K,D> operator*(const size_t &val) const;
		rbTree<K,D> operator*=(const size_t &val);
		rbTree<K,D> rbt_mul(const size_t &val) const;
		rbTree<K,D> rbt_mulI(const size_t &val);

				// TREE ADVANCED OPERATIONS 
		rbTree<K,D> rbt_invert();
		rbTree<K,D> rbt_invertI();
		rbTree<K,D> *rbt_distribute(size_t, enum rbtraversal = LEVEL_ORDER);
		rbTree<K,D> *rbt_distribute_shallow(size_t, enum rbtraversal = LEVEL_ORDER);
		rbTree<K,D> *rbt_scatter(size_t, enum rbtraversal = LEVEL_ORDER);
		rbTree<K,D> *rbt_scatter_shallow(size_t, enum rbtraversal = LEVEL_ORDER);

			// RBT_limit
		void rbt_clamp(K, K, D, D);
		void rbt_clamp(size_t, K *, K *, size_t, D *, D *);
		void rbt_exclude(K, K, D, D);
		void rbt_exclude(size_t, K *, K *, size_t, D *, D *);

		void rbt_clampKey(K, K);
		void rbt_clampKey(size_t, K *, K *);
		void rbt_excludeKey(K, K);
		void rbt_excludeKey(size_t, K *, K *);
		void rbt_upperLimitKey(K);
		void rbt_lowerLimitKey(K);
		void rbt_deleteHighestKey(const size_t);
		void rbt_removeHighestKey(const size_t);
		void rbt_deleteLowestKey(const size_t);
		void rbt_removeLowestKey(const size_t);

		void rbt_clampData(D, D);
		void rbt_clampData(size_t, D *, D *);
		void rbt_excludeData(D, D);
		void rbt_excludeData(size_t, D *, D *);
		void rbt_upperLimitData(D);
		void rbt_lowerLimitData(D);
		void rbt_deleteHighestData(const size_t);
		void rbt_removeHighestData(const size_t);
		void rbt_deleteLowestData(const size_t);
		void rbt_removeLowestData(const size_t);

			// RBT_value
		void rbt_set(rbtNode<K,D> *, K, D);
		void rbt_func(rbtNode<K,D> *, void (*)(K &, D &));

		void rbt_setAll(K, D);
		void rbt_funcAll(void (*)(K &, D &));

		void rbt_setKey(rbtNode<K,D> *, K);
		void rbt_funcKey(rbtNode<K,D> *, K (*)());
		void rbt_funcKey(rbtNode<K,D> *, K (*)(K, D));
		void rbt_funcKeyK(rbtNode<K,D> *, K (*)(K));
		void rbt_funcKeyD(rbtNode<K,D> *, K (*)(D));

		void rbt_setAllKeys(K);
		void rbt_funcAllKeys(K (*)());
		void rbt_funcAllKeys(K (*)(K, D));
		void rbt_funcAllKeysK(K (*)(K));
		void rbt_funcAllKeysD(K (*)(D));

		void rbt_setData(rbtNode<K,D> *, D);
		void rbt_funcData(rbtNode<K,D> *, D (*)());
		void rbt_funcData(rbtNode<K,D> *, D (*)(K, D));
		void rbt_funcDataK(rbtNode<K,D> *, D (*)(K));
		void rbt_funcDataD(rbtNode<K,D> *, D (*)(D));

		void rbt_setAllData(D);
		void rbt_funcAllData(D (*)());
		void rbt_funcAllData(D (*)(K, D));
		void rbt_funcAllDataK(D (*)(K));
		void rbt_funcAllDataD(D (*)(D));

			// RBT_string
		std::string rbt_string(enum rbtraversal = IN_ORDER, size_t = 0) const;
		template <K, D>
			friend std::ostream& operator<<(std::ostream &, rbTree<K,D> &);
};


/*			Modules			*/
#include	"RBT_traversal.h"
#include	"RBT_access.h"
#include	"RBT_string.h"
#include	"RBT_sort.h"
#include	"RBT_indel.h"
#include	"RBT_ops.h"
#include	"RBT_setops.h"
#include	"RBT_limit.h"
#include	"RBT_value.h"

#include	"../src/RBT_main.cpp"
#endif /* _rbt_main_HEADER */