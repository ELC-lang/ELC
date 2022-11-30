// ice10215710.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "../../files/elc/stream"
#include "../../files/elc/string"

int main(){
	using namespace elc;//out & endline & string
	using namespace elc::defs;//note

	string a={U'烫',note::size<size_t>(165489453133213)};//my super long string!
	out<<a.substr(542132486521,3)<<endline;//烫*3
	out<<a.memory_cost()<<endline
	   <<a.size()<<endline;
	a[156498321]=U'坤';
	out<<a.substr(156498320,3)<<endline;//烫坤烫
	out<<a.memory_cost()<<endline;
	a.clear();
	a=U"ae"_constexpr_str;//zero memory cost
	out<<a.memory_cost()<<endline;
	//now I guess a[0.5] is the middle of a and e: c
	out<<(elc::char_t)a[0.5]<<endline;
	//a[0.25] is the middle of a and c: b
	out<<(elc::char_t)a[0.25]<<endline;
}
