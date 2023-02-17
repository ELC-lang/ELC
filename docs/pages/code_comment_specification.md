# 代码注释规范  

## 1. 代码注释的目的  

ELC项目的代码注释主要用于：  

- doxygen生成文档  
- vscode等编辑器的代码提示  

因此代码注释应按照doxygen的语法进行编写，详见[doxygen语法](http://www.doxygen.nl/manual/docblocks.html)。  

## 2. 代码注释的格式  

### 2.1. 单行注释

生成文档目的的单行注释，应该以`///`开头，如：  

```c++
/// 整数类型概念
template<typename T>
constexpr bool is_integer_type=!is_float_type<T>;
```

在未来可能拓展此注释 或 附近有很多以`//`为开头的注释 的情况下，可以以单行的`/*! XX */`注释来生成文档，如：  

```c++
/*! 实例化到此将引发警告 */
#define template_warning(reason) template_warning_helper<T>(reason)
```

对于不以生成文档为目的的单行注释，应该以`//`开头并除非为了对齐或联系上下文使得代码美观，否则不应该在注释符前后加空格，如：  

```c++
#define noexcept_as(...) noexcept(noexcept((__VA_ARGS__)))//dnmd C艹标准就是没有noexcept_as_auto
```

在单行注释连续且多于三行时，应该改用块注释。

### 2.2. 多行注释  

生成文档目的的多行注释，应该以`/*!`开头，如：  

```c++
//end_by_zero_t
/*!
用于标志某一数据串以0结尾
*/
inline constexpr struct end_by_zero_t{}end_by_zero;
```

对于不以生成文档为目的的多行注释，应该以`/*`开头。

不同于单行注释，块注释无论单行与否，注释符前后都应该加空格或者换行以使得代码美观。

### 2.3. 代码注释的参数  

鼓励在代码注释中使用参数，并以`@`而非`\`开头，如：  

```c++
/// @brief 同 die() ，但是有详细的错误信息
/// @param err_msg 错误信息
[[noreturn]]inline void die_with(string_view err_msg)noexcept;
```

## 3. 代码注释的内容  

### 3.1. 代码注释的内容的顺序

代码注释的内容应该按照以下顺序进行编写：

- `@brief`
  用于描述函数或类的功能。
- `@tparam`
  用于描述所需模板参数及其意义。
- `@param`
  用于描述调用所需参数及其意义。
- `@return`
  用于描述函数的返回值及其意义。
- `@note`
- `@warning`
  在函数或类的功能上有特殊说明的地方，如函数对参数的假设。
- `@see`
  参考的其他函数或类。
- `@todo`

## 4. 代码注释的适用范围

代码注释应该尽可能地覆盖所有的函数和类、名称空间、作用范围超出文件的宏定义、全局变量、枚举、类型别名等。

## 5. 代码注释的文档生成测试

ELC使用github actions进行代码注释的文档生成测试，因此可以直接在[github pages](https://elc-lang.github.io/ELC/)上查看文档生成的结果。  
若需要在本地进行测试，文档生成所需的配置文件为`docs/ELC.docgen`。  
