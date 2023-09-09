//sort.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/ELC-lang/ELC
*/
/*
			   ,??????????????????????????
			   ????????????????????????????
			  ?????????????????????????????+
			  ?????+++++++++++++++++++++++++
			 ?????+++++++++++++++++++++++++
			??????++++++++++++++++++++++++
			?????++
		   ?????+++
		  ??????+++IIIIIIIIIIIIIIIIII
		  ?????+++++IIIIIIIIIIIIIIIIII
		 ?????++++++++++++++++++++++++
		??????+++++++++++++++++++++++:
		?????++
	   ?????+++
	  ??????++
	  ??????????????????????????????II
	 ??????????????????????????????IIII
	,?????????????????????????????IIIII+
	 =???++++++++++++++++++++++++++III?
	   ?++++++++++++++++++++++++++++I+
*/

namespace sort_n{
	typedef compare_t default_compare_t;

	//heapsort
	template<typename array_t, typename compare_t = default_compare_t>
	void heapify(array_t& array, size_t begin, size_t end, size_t i, compare_t compare = compare_t()) {
		size_t largest = i + begin; // 初始化largest为根节点
		size_t left_child = 2 * i + 1 + begin; // 左子节点
		size_t right_child = left_child + 1; // 右子节点

		// 如果左子节点比根节点大，则更新largest
		if (left_child < end && compare(array[left_child], array[largest])==strong_ordering::greater)
			largest = left_child;

		// 如果右子节点比根节点大，则更新largest
		if (right_child < end && compare(array[right_child], array[largest])==strong_ordering::greater)
			largest = right_child;

		// 如果largest不是根节点，则交换根节点和largest的值，并递归地对largest子树进行heapify
		if (largest != i + begin) {
			swap(array[i + begin], array[largest]);
			heapify(array, begin, end, largest - begin, compare);
		}
	}

	template<typename array_t, typename compare_t = default_compare_t>
	void build_max_heap(array_t& array, size_t begin, size_t end, compare_t compare = compare_t()) {
		size_t n = end - begin;

		// 从最后一个非叶子节点开始进行heapify，直到根节点
		for (ptrdiff_t i = n / 2 - 1; i >= 0; i--)
			heapify(array, begin, end, i, compare);
	}

	template<typename array_t, typename compare_t = default_compare_t>
	array_t& heapsort(array_t& array, size_t begin, size_t end, compare_t compare = compare_t()) {
		size_t n = end - begin;

		// 构建最大堆
		build_max_heap(array, begin, end, compare);

		// 从最后一个元素开始，依次将其与堆顶元素交换，并对堆顶元素进行heapify
		for (ptrdiff_t i = n - 1; i >= 0; i--) {
			swap(array[begin], array[begin + i]);
			heapify(array, begin, begin + i, 0, compare);
		}
		return array;
	}

	template<typename array_t, typename compare_t = default_compare_t>
	void heapify(array_t& array, size_t n, size_t i, compare_t compare = compare_t()) {
		heapify(array, 0, n, i, compare);
	}
	template<typename array_t, typename compare_t = default_compare_t>
	void build_max_heap(array_t& array, compare_t compare = compare_t()) {
		build_max_heap(array, 0, array.size(), compare);
	}
	template<typename array_t, typename compare_t = default_compare_t>
	array_t& heapsort(array_t& array, compare_t compare = compare_t()) {
		return heapsort(array, 0, array.size(), compare);
	}

	//insertion_sort
	template<typename array_t, typename compare_t = default_compare_t>
	array_t& insertion_sort(array_t& array, size_t begin, size_t end, compare_t compare = compare_t()) {
		// 从第二个元素开始，依次将其插入到前面已排序的数组中
		for (size_t i = begin + 1; i < end; i++) {
			auto key = move(array[i]);
			ptrdiff_t j=i-1;
			// 将比key大的元素向后移动
			while (j >= ptrdiff_t(begin) && compare(array[j], key)==strong_ordering::greater) {
				array[j + 1] = move(array[j]);
				j--;
			}
			// 将key插入到正确的位置
			array[j + 1] = move(key);
		}
		return array;
	}
	template<typename array_t, typename compare_t = default_compare_t>
	array_t& insertion_sort(array_t& array, compare_t compare = compare_t()) {
		return insertion_sort(array, 0, array.size(), compare);
	}

	//quicksort
	// partition函数
	template<typename array_t, typename compare_t = default_compare_t>
	size_t partition(array_t& array, size_t begin, size_t end, compare_t compare = compare_t()) {
		// 选取最后一个元素作为pivot
		auto pivot = array[end - 1];
		size_t i = begin;
		for (size_t j = begin; j < end - 1; j++) {
			// 如果当前元素小于pivot，将其交换到左边
			if (compare(array[j], pivot)==strong_ordering::less) {
				swap(array[i], array[j]);
				i++;
			}
		}
		// 将pivot放到正确的位置
		swap(array[i], array[end - 1]);
		return i;
	}

	// quicksort循环函数
	template<typename array_t, typename compare_t = default_compare_t>
	void quicksort_loop(array_t& array, size_t begin, size_t end, compare_t compare = compare_t()) {
		while (end - begin > 1) {
			// 分割数组并递归排序
			const size_t pivot = partition(array, begin, end, compare);
			quicksort_loop(array, begin, pivot, compare);
			begin = pivot + 1;
		}
	}

	// quicksort函数
	template<typename array_t, typename compare_t = default_compare_t>
	array_t& quicksort(array_t& array, compare_t compare = compare_t()) {
		quicksort_loop(array, 0, array.size(), compare);
		return array;
	}

	//introsort
	constexpr size_t size_threshold = 1<<4;

	// introsort循环函数
	template<typename array_t, typename compare_t = default_compare_t>
	void introsort_loop(array_t& array, size_t begin, size_t end, size_t depth_limit, compare_t compare = compare_t()) {
		while (end - begin > size_threshold) {
			// 如果递归深度达到了限制，使用heapsort排序
			if (depth_limit == 0) {
				heapsort(array, begin, end, compare);
				return;
			}
			depth_limit--;

			// 分割数组并递归排序
			const size_t pivot = partition(array, begin, end, compare);
			introsort_loop(array, pivot + 1, end, depth_limit, compare);
			end = pivot;
		}
		// 如果数组长度小于等于size_threshold，使用插入排序
		if(begin < end)
			insertion_sort(array, begin, end, compare);
	}

	// introsort函数
	template<typename array_t, typename compare_t = default_compare_t>
	array_t& introsort(array_t& array, compare_t compare = compare_t()) {
		const size_t n = array.size();
		// 如果数组长度小于等于size_threshold，使用插入排序
		if (n <= size_threshold)
			return insertion_sort(array, compare);
		// 否则使用introsort
		const size_t depth_limit = size_t(2 * log(n, 2));
		introsort_loop(array, 0, n, depth_limit, compare);
		return array;
	}

	// 默认排序函数
	template<typename array_t, typename compare_t = default_compare_t>
	array_t& sort(array_t& array, compare_t compare = compare_t()) {
		return introsort(array, compare);
	}
}
using sort_n::sort;

//file_end

