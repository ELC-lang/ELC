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
	template<typename iter_t, typename compare_t = default_compare_t>
	void heapify(iter_t begin, iter_t end, iter_t i, compare_t compare = compare_t()) {
		auto largest = i; // 初始化largest为根节点
		auto left_child = begin + 2 * (i - begin) + 1; // 左子节点
		auto right_child = left_child + 1; // 右子节点

		// 如果左子节点比根节点大，则更新largest
		if (left_child < end && compare(*left_child, *largest)==strong_ordering::greater)
			largest = left_child;

		// 如果右子节点比根节点大，则更新largest
		if (right_child < end && compare(*right_child, *largest)==strong_ordering::greater)
			largest = right_child;

		// 如果largest不是根节点，则交换根节点和largest的值，并递归地对largest子树进行heapify
		if (largest != i) {
			swap(*i,*largest);
			heapify(begin, end, largest, compare);
		}
	}

	template<typename iter_t, typename compare_t = default_compare_t>
	void build_max_heap(iter_t begin, iter_t end, compare_t compare = compare_t()) {
		auto n = end - begin;

		// 从最后一个非叶子节点开始进行heapify，直到根节点
		for (auto i = n / 2 - 1; i >= 0; i--)
			heapify(begin, end, begin + i, compare);
	}

	template<typename iter_t, typename compare_t = default_compare_t>
	void heapsort(iter_t begin, iter_t end, compare_t compare = compare_t()) {
		auto n = end - begin;

		// 构建最大堆
		build_max_heap(begin, end, compare);

		// 从最后一个元素开始，依次将其与堆顶元素交换，并对堆顶元素进行heapify
		for (auto i = n - 1; i >= 0; i--) {
			swap(*begin,begin[i]);
			heapify(begin, begin + i, begin, compare);
		}
	}

	template<typename array_t, typename compare_t = default_compare_t>
	void heapify(array_t& array, size_t i, compare_t compare = compare_t()) {
		heapify(array.begin(), array.end(), array.begin() + i, compare);
	}
	template<typename array_t, typename compare_t = default_compare_t>
	void build_max_heap(array_t& array, compare_t compare = compare_t()) {
		build_max_heap(array.begin(), array.end(), compare);
	}
	template<typename array_t, typename compare_t = default_compare_t>
	array_t&& heapsort(array_t&& array, compare_t compare = compare_t()) {
		heapsort(array.begin(), array.end(), compare);
		return forward<array_t>(array);
	}

	//insertion_sort
	template<typename iter_t, typename compare_t = default_compare_t>
	void insertion_sort(iter_t begin, iter_t end, compare_t compare = compare_t()) {
		// 从第二个元素开始，依次将其插入到前面已排序的数组中
		for (auto i = begin + 1; i < end; i++) {
			auto key = std::move(*i);
			auto j = i - 1;
			// 将比key大的元素向后移动
			while (j >= begin && compare(*j, key)==strong_ordering::greater) {
				*(j + 1) = std::move(*j);
				j--;
			}
			// 将key插入到正确的位置
			*(j + 1) = std::move(key);
		}
	}
	template<typename array_t, typename compare_t = default_compare_t>
	array_t&& insertion_sort(array_t&& array, compare_t compare = compare_t()) {
		insertion_sort(array.begin(), array.end(), compare);
		return forward<array_t>(array);
	}

	//quicksort
	// partition函数
	template<typename iter_t, typename compare_t = default_compare_t>
	iter_t partition(iter_t begin, iter_t end, compare_t compare = compare_t()) {
		// 选取最后一个元素作为pivot
		auto&pivot = end[-1];
		auto i = begin;
		for (auto j = begin; j < end - 1; j++) {
			// 如果当前元素小于pivot，将其交换到左边
			if (compare(*j, pivot)==strong_ordering::less) {
				swap(*i,*j);
				i++;
			}
		}
		// 将pivot放到正确的位置
		swap(*i,pivot);
		return i;
	}

	// quicksort函数
	template<typename iter_t, typename compare_t = default_compare_t>
	void quicksort(iter_t begin, iter_t end, compare_t compare = compare_t()) {
		while (end - begin > 1) {
			// 分割数组并递归排序
			const auto pivot = partition(begin, end, compare);
			quicksort(begin, pivot, compare);
			begin = pivot + 1;
		}
	}
	template<typename array_t, typename compare_t = default_compare_t>
	array_t&& quicksort(array_t&& array, compare_t compare = compare_t()) {
		quicksort(array.begin(), array.end(), compare);
		return forward<array_t>(array);
	}

	//introsort
	constexpr size_t size_threshold = 1<<4;

	// introsort循环函数
	template<typename iter_t, typename compare_t = default_compare_t>
	void introsort_loop(iter_t begin, iter_t end, size_t depth_limit, compare_t compare = compare_t()) {
		while (end - begin > size_threshold) {
			// 如果递归深度达到了限制，使用heapsort排序
			if (depth_limit == 0) {
				heapsort(begin, end, compare);
				return;
			}
			depth_limit--;

			// 分割数组并递归排序
			const auto pivot = partition(begin, end, compare);
			introsort_loop(pivot + 1, end, depth_limit, compare);
			end = pivot;
		}
		insertion_sort(begin, end, compare);
	}

	// introsort函数
	template<typename iter_t, typename compare_t = default_compare_t>
	void introsort(iter_t begin, iter_t end, compare_t compare = compare_t()) {
		const auto n = end - begin;
		// 如果数组长度小于等于size_threshold，使用插入排序
		if (n <= size_threshold)
			return insertion_sort(begin, end, compare);
		// 否则使用introsort
		const auto depth_limit = decltype(n)(2 * log(n, 2));
		introsort_loop(begin, end, depth_limit, compare);
	}
	template<typename array_t, typename compare_t = default_compare_t>
	array_t&& introsort(array_t&& array, compare_t compare = compare_t()) {
		introsort(array.begin(), array.end(), compare);
		return forward<array_t>(array);
	}

	// 默认排序函数
	template<typename iter_t, typename compare_t = default_compare_t>
	void sort(iter_t begin, iter_t end, compare_t compare = compare_t()) {
		introsort(begin, end, compare);
	}
	template<typename array_t, typename compare_t = default_compare_t>
	array_t&& sort(array_t&& array, compare_t compare = compare_t()) {
		sort(array.begin(), array.end(), compare);
		return forward<array_t>(array);
	}
}
using sort_n::sort;

//file_end

