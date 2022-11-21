//magic_number.hpp
//at namespace elc::defs::base
/*
未完成的elc解释器base_defs文件
由steve02081504与Alex0125设计、编写
转载时请在不对此文件做任何修改的同时注明出处
项目地址：https://github.com/steve02081504/ELC
*/
namespace magic_number{
	/*! 黄金分割比
		数理逻辑不应通过强奸感性来定义审美.
		我们应当遵从物体的自身结构,辅助线仅能在这基础上起到梳理作用,并不能以此作为创作起点来表达情感,它仅存于外部世界,无法到达内心.
		黄金分割并不万能.
	*/
	constexpr auto gold=0.61803398874989484820458683436563811772030917980576286213544862270526046281890244970720720418939113748475408807538689175212663386222353693179318006076672635443338908659593958290563832266131992829026788067520876689250171169620703222104321626954862629631361443814975870122034080588795445474924618569536486444924104432077134494704956584678850987433944221254487706647809158846074998871240076521705751797883416625624940758906970400028121042762177111777805315317141011704666599146697987317613560067087480710131795236894275219484353056783002287856997829778347845878228911097625003026961561700250464338243776486102838312683303724292675263116533924731671112115881863851331620384005222165791286675294654906811317159934323597349498509040947621322298101726107059611645629909816290555208524790352406020172799747175342777592778625619432082750513121815628551222480939471234145170223735805772786160086883829523045926478780178899219902707769038953219681986151437803149974110692608867429622675756052317277752035361393621076738937645560606059216589466759551900400555908950229530942312482355212212415444006470340565734797663972394949946584578873039623090375033993856210242369025138680414577995698122445747178034173126453220416397232134044449487302315417676893752103068737880344170093954409627955898678723209512426893557309704509595684401755519881921802064052905518934947592600734852282101088194644544222318891319294689622002301443770269923007803085261180754519288770502109684249362713592518760777884665836150238913493333122310533923213624319263728910670503399282265263556209029798642472759772565508615487543574826471814145127000602389016207773224499435308899909501680328112194320481964387675863314798571911397815397807476150772211750826945863932045652098969855567814106968372884058746103378105444390943683583581381131168993855576975484149144534150912954070050194775486163075422641729394680367319805861833918328599130396072014455950449779212076124785645916160837059498786006970189409886400764436170933417270919143365013715;//精度2000(然而只是double而已.)

	/*! 每次扩容后的空间与原空间为这样的比值可以最小化时空负担
		是的,以一种较为艺术的比值.
	*/
	constexpr auto gold_of_resize=gold+1;//精度2000——大概

	/*! 圆周率. */
	constexpr auto pi = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700660631558817488152092096282925409171536436789259036001133053054882046652138414695194151160943305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912983367336244065664308602139494639522473719070217986094370277053921717629317675238467481846766940513200056812714526356082778577134275778960917363717872146844090122495343014654958537105079227968925892354201995611212902196086403441815981362977477130996051870721134999999837297804995105973173281609631859502445945534690830264252230825334468503526193118817101000313783875288658753320838142061717766914730359825349042875546873115956286388235378759375195778185778053217122680661300192787661119590921642019893809525720106548586327886593615338182796823030195203530185296899577362259941389124972177528347913151557485724245415069595082953311686172785588907509838175463746493931925506040092770167113900984882401285836160356370766010471018194295559619894676783744944825537977472684710404753464620804668425906949129331367702898915210475216205696602405803815019351125338243003558764024749647326391419927260426992279678235478163600934172164121992458631503028618297455570674983850549458858692699569092721079750930295532116534498720275596023648066549911988183479775356636980742654252786255181841757467289097777279380008164706001614524919217321721477235014144197356854816136115735255213347574184946843852332390739414333454776241686251898356948556209921922218427255025425688767179049460165346680498862723279178608578438382796797668145410095388378636095068006422512520511739298489608412848862694560424196528502221066118630674427862203919494504712371378696095636437191728746776465757396241389086583264599581339047802759009;//精度2000

	/*! 自然对数的底数. */
	constexpr auto e =	2.71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642742746639193200305992181741359662904357290033429526059563073813232862794349076323382988075319525101901157383418793070215408914993488416750924476146066808226480016847741185374234544243710753907774499206955170276183860626133138458300075204493382656029760673711320070932870912744374704723069697720931014169283681902551510865746377211125238978442505695369677078544996996794686445490598793163688923009879312773617821542499922957635148220826989519366803318252886939849646510582093923982948879332036250944311730123819706841614039701983767932068328237646480429531180232878250981945581530175671736133206981125099618188159304169035159888851934580727386673858942287922849989208680582574927961048419844436346324496848756023362482704197862320900216099023530436994184914631409343173814364054625315209618369088870701676839642437814059271456354906130310720851038375051011574770417189861068739696552126715468895703503540212340784981933432106817012100562788023519303322474501585390473041995777709350366041699732972508868769664035557071622684471625607988265178713419512466520198918006308407507245458379639351364421067750539471789004518357551541252235905906872648786357525419112888773717663748602766063496035367947026923229718683277173932361920077745221262475186983349515101986426988784717193966497690708252174233656627259284406204302141137199227852699846988477023238238400556555178890876613601304770984386116870523105531491625172837327286760072481729876375698163354150746088386636406934704372066886512756882661497307886570156850169186474885416791545965072342877306998537139043002665307839877638503238182155355973235306860430106757608389086270498418885951380910304235957824951439885901131858358406674723702971497850841458530857813391562707603563907639473114554958322669457024941398316343323789759556808568362972538679132750555425244919435891284050452269538121791319145135009938463117740179715122837854601160359554028644059;//精度2000

	constexpr auto god=72;/// 神已死,神万岁.

	/*! 无符号位的对应类型 */
	template<typename T>
	using to_unsigned_t = decltype(lambda{
		if constexpr(::std::is_unsigned_v<T>||::std::is_floating_point_v<T>)
			return T();
		else
			return::std::make_unsigned_t<T>();
	}());
	/*! 符号位查询 */
	template<typename T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]force_inline constexpr bool is_negative(T x)noexcept{
		if constexpr(::std::is_signed_v<T>){
			if constexpr(::std::is_floating_point_v<T>)
				return ::std::signbit(x);
			else
				return x<0;
		}
		else
			return false;
	}
	/*!符号位设置 */
	template<typename T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]force_inline constexpr T copy_as_negative(auto x,bool negative=1)noexcept{
		if constexpr(::std::is_signed_v<decltype(x)>){
			if constexpr(::std::is_floating_point_v<decltype(x)>)
				return(T)::std::copysign(x,negative?-1:1);
			else
				return(T)negative?T{}-x:x;
		}
		else
			return x;
	}
	[[nodiscard]]force_inline constexpr auto copy_as_negative(auto x,bool negative=1)noexcept{
		return copy_as_negative<decltype(x)>(x,negative);
	}
	[[nodiscard]]force_inline constexpr auto copy_as_not_negative(auto x)noexcept{
		return copy_as_negative(x,false);
	}
	/*! 任意算数类型安全转型(ub避免.). */
	template<typename T,typename U> requires ::std::is_arithmetic_v<U>
	[[nodiscard]]force_inline constexpr T safe_arithmetic_cast(U x)noexcept{
		if constexpr(::std::is_floating_point_v<U> && ::std::is_unsigned_v<T>)
			return (T)(intmax_t)x;
		else
			return (T)x;
	}
	/*! 任意算数类型转size_t */
	template<typename T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]force_inline constexpr size_t to_size_t(T x)noexcept{
		return safe_arithmetic_cast<size_t>(x);
	}
	/*! 任意算数类型转uintmax_t */
	template<typename T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]force_inline constexpr uintmax_t to_uintmax_t(T x)noexcept{
		return safe_arithmetic_cast<uintmax_t>(x);
	}
	/*! 求余 */
	template<typename T1,typename T2> requires ::std::is_arithmetic_v<T1> and ::std::is_arithmetic_v<T2>
	[[nodiscard]]force_inline constexpr auto mod(T1 a,T2 b){
		if constexpr(::std::is_floating_point_v<T1>||::std::is_floating_point_v<T2>)
			return ::std::fmod(a,b);
		else
			return a%b;
	}
	/*! 设置浮点舍入 */
	template<typename T>
	force_inline constexpr void set_rounding(int mode){
		if constexpr(::std::is_floating_point_v<T>)
			::std::fesetround(mode);
	}
	/*!获取浮点舍入 */
	template<typename T>
	[[nodiscard]]force_inline constexpr int get_rounding(){
		if constexpr(::std::is_floating_point_v<T>)
			return ::std::fegetround();
		else
			return 0;
	}
	/*! 设置并自动恢复浮点舍入 */
	template<typename T>
	struct rounding_auto_setter{
		int old_rounding;
		rounding_auto_setter(int new_rounding){
			old_rounding=get_rounding<T>();
			set_rounding<T>(new_rounding);
		}
		~rounding_auto_setter(){
			set_rounding<T>(old_rounding);
		}
	};
	/// 位操作：循环左移（无mod）
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<class T> requires ::std::is_unsigned_v<T>
	[[nodiscard]]force_inline constexpr auto rotl_nomod(const T v,const auto R)noexcept;
	#define rot_base(opt,antiopt) static_cast<T>(static_cast<T>(v opt r) | static_cast<T>(v antiopt (d - r)))
	/// 位操作：循环右移（无mod）
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<class T> requires ::std::is_unsigned_v<T>
	[[nodiscard]]force_inline constexpr auto rotr_nomod(const T v,const auto r)noexcept{
		constexpr auto d = ::std::numeric_limits<T>::digits;
		if constexpr(::std::is_unsigned_v<decltype(r)>){
			if(r)
				return rot_base(>>,<<);
			else
				return v;
		}
		else{
			if(r>0)
				return rot_base(>>,<<);
			elseif(r==0)
				return v;
			else//r<0
				return rotl_nomod(v,0-r);
		}
	}
	/// 位操作：循环左移（无mod）
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<class T> requires ::std::is_unsigned_v<T>
	[[nodiscard]]force_inline constexpr auto rotl_nomod(const T v,const auto r)noexcept{
		constexpr auto d = ::std::numeric_limits<T>::digits;
		if constexpr(::std::is_unsigned_v<decltype(r)>){
			if(r)
				return rot_base(<<,>>);
			else
				return v;
		}
		else{
			if(r>0)
				return rot_base(<<,>>);
			elseif(r==0)
				return v;
			else//r<0
				return rotr_nomod(v,0-r);
		}
	}
	#undef rot_base
	/// 位操作：循环右移
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<class T> requires ::std::is_unsigned_v<T>
	[[nodiscard]]force_inline constexpr auto rotr(const T v,const auto R)noexcept{
		constexpr auto d = ::std::numeric_limits<T>::digits;
		const auto r = mod(R,d);
		return rotr_nomod(v,r);
	}
	/// 位操作：循环左移
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<class T> requires ::std::is_unsigned_v<T>
	[[nodiscard]]force_inline constexpr auto rotl(const T v,const auto R)noexcept{
		constexpr auto d = ::std::numeric_limits<T>::digits;
		const auto r = mod(R,d);
		return rotl_nomod(v,r);
	}
	template<class T> requires ::std::is_arithmetic_v<T>
	class rot_iterator{
		static constexpr auto rot_offset_npos = ::std::numeric_limits<T>::digits;//d
		size_t _offset;
	public:
		force_inline constexpr rot_iterator(size_t offset)noexcept:_offset(offset%rot_offset_npos){}
		force_inline constexpr rot_iterator&operator++()noexcept{
			_offset++;
			if(_offset==rot_offset_npos)
				_offset=0;
			return*this;
		}
		force_inline constexpr rot_iterator&operator--()noexcept{
			if(_offset==0)
				_offset=rot_offset_npos;
			_offset--;
			return*this;
		}
		force_inline constexpr rot_iterator operator++(int)noexcept{
			rot_iterator tmp(*this);
			operator++();
			return tmp;
		}
		force_inline constexpr rot_iterator operator--(int)noexcept{
			rot_iterator tmp(*this);
			operator--();
			return tmp;
		}
		[[nodiscard]]force_inline constexpr size_t value()const noexcept{
			return _offset;
		}
	};
	/// 位操作：循环左移（无mod）
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<class T> requires ::std::is_unsigned_v<T>
	[[nodiscard]]force_inline constexpr auto rotl_nomod(const T v,const rot_iterator<T>&r)noexcept{
		return rotl_nomod(v,r.value());
	}
	/// 位操作：循环右移（无mod）
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<class T> requires ::std::is_unsigned_v<T>
	[[nodiscard]]force_inline constexpr auto rotr_nomod(const T v,const rot_iterator<T>&r)noexcept{
		return rotr_nomod(v,r.value());
	}
	/// 位操作：循环左移
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<class T> requires ::std::is_unsigned_v<T>
	[[nodiscard]]force_inline constexpr auto rotl(const T v,const rot_iterator<T>&r)noexcept{
		return rotl_nomod(v,r);
	}
	/// 位操作：循环右移
	/// 不使用std版本而是自己写的原因：std版本右操作数只能是int而不能是size_t或别的，标准会傻逼
	template<class T> requires ::std::is_unsigned_v<T>
	[[nodiscard]]force_inline constexpr auto rotr(const T v,const rot_iterator<T>&r)noexcept{
		return rotr_nomod(v,r);
	}
	//abs
	template<class T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]force_inline constexpr auto abs(const T v)noexcept{
		if constexpr(::std::is_signed_v<T>)
			return v>=0?v:-v;
		else
			return v;
	}
	//feq
	template<class T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]force_inline constexpr bool feq(const T a,const T b)noexcept{
		return abs(a-b)<=::std::numeric_limits<T>::epsilon();
	}
	//isNaN
	template<class T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]force_inline constexpr bool isNaN(const T v)noexcept{
		if constexpr(::std::is_floating_point_v<T>)
			return v!=v;
		else
			return false;
	}
	//sub
	template<class T1,class T2> requires was_not_an_ill_form(declvalue(T1)-declvalue(T2))
	[[nodiscard]]force_inline constexpr auto sub(const T1 a,const T2 b)noexcept{
		if constexpr(::std::is_arithmetic_v<T1>&&::std::is_arithmetic_v<T2>){
			if constexpr(::std::is_floating_point_v<T1>||::std::is_floating_point_v<T2>)
				return a-b;
			else{
				using signedT = ::std::make_signed_t<::std::common_type_t<T1,T2,signed char>>;
				return static_cast<signedT>(a)-static_cast<signedT>(b);
			}
		}
		else
			return a-b;
	}
	//exp
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<class T> requires ::std::is_floating_point_v<T>
	[[nodiscard]]force_inline constexpr T exp(const T v)noexcept{
		if in_consteval{
			typedef decltype(::std::exp(v)) RT;
			auto exp_impl = recursive_lambda(RT x,RT sum,RT n,size_t i,RT t)noexcept -> RT{
				const auto epsilon=sum+t/n;
				if(feq(sum,epsilon))
					return sum;
				else
					return self_recursion(x,epsilon,n*i,i+1,t*x);
			};
			auto exp_impl_caller = get_recursive_lambda_caller(exp_impl);
			return exp_impl_caller(RT{v},RT{1},RT{1},2,RT{v});
		}
		else
			return ::std::exp(v);
	}
	//log
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<class T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]force_inline constexpr auto log(const T a)noexcept{
		if in_consteval{
			typedef decltype(::std::log(a)) RT;
			auto log_impl = recursive_lambda(const RT x,const RT y)noexcept -> RT{
				auto log_iter = lambda(RT x,RT y)noexcept{
					const auto exp_y = exp(y);
					return y + T{2}*(x-exp_y)/(x+exp_y);
				};
				return RT(feq(y,log_iter(x,y)) ? y : self_recursion(x,log_iter(x,y)));
			};
			auto log_impl_caller = get_recursive_lambda_caller(log_impl);
			return log_impl_caller((RT)a,RT{0});
		}
		else
			return ::std::log(a);
	}
	template<class T,class U> requires ::std::is_arithmetic_v<T> && ::std::is_arithmetic_v<U>
	[[nodiscard]]force_inline constexpr auto log(const T a,const U b)noexcept{
		return log(a)/log(b);
	}
	//pow
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<class T,class U> requires (::std::is_arithmetic_v<T> && ::std::is_arithmetic_v<U>)
	[[nodiscard]]force_inline constexpr auto pow(const T a,const U b)noexcept{
		if in_consteval{
			typedef decltype(::std::pow(a,b)) RT;
			return exp(RT(b)*log(RT(a)));
		}
		else
			return ::std::pow(a,b);
	}
	//trunc
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<class T> requires ::std::is_floating_point_v<T>
	[[nodiscard]]force_inline constexpr auto trunc(const T v)noexcept{
		if in_consteval{
			typedef decltype(::std::trunc(v)) RT;
			return static_cast<RT>((intmax_t)v);
		}
		else
			return ::std::trunc(v);
	}
	//ceil
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<class T> requires ::std::is_floating_point_v<T>
	[[nodiscard]]force_inline constexpr auto ceil(const T v)noexcept{
		if in_consteval{
			typedef decltype(::std::ceil(v)) RT;
			auto ceil_impl = lambda(T x, T y)noexcept{
				return feq(x,y) ? y : y+T{1};
			};
			return v<0 ? -static_cast<T>(to_uintmax_t(-v)) : ceil_impl(v,static_cast<T>(to_uintmax_t(v)));
		}
		else
			return ::std::ceil(v);
	}
	//sqrt
	//不使用std版本而是自己写的原因：std版本不是constexpr，标准会傻逼
	template<class T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]force_inline constexpr auto sqrt(const T v)noexcept{
		if in_consteval{
			typedef decltype(::std::sqrt(v)) RT;
			auto sqrt_impl = recursive_lambda(const RT x,const RT curr,const RT prev)noexcept -> RT{
				return feq(curr,prev) ? curr : self_recursion(x,(curr+x/curr)/RT{2},curr);
			};
			auto sqrt_impl_caller = get_recursive_lambda_caller(sqrt_impl);
			if(v >= 0 && v < ::std::numeric_limits<RT>::infinity())
				return sqrt_impl_caller((RT)v,(RT)v,(RT)0);
			else
				return ::std::numeric_limits<RT>::quiet_NaN();
		}
		else
			return ::std::sqrt(v);
	}

	/*! 判断某数是否是素数,无预先检查以供其他素数相关函数快速调用. */
	template<class T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]inline constexpr bool is_prime_num_no_pre_check(T a)noexcept{
		/*
		应某人的要求补注释(都是主人的任务罢了.).
		建议先看有预先检查的部分以便理解思路.
		*/
		T b=safe_arithmetic_cast<T>(sqrt(a));//若一个数可以分解为两因数之积,其中一个因数必定≤其开方:反指数式减少遍历范围.
		/*
		接下来:
		设要判定的数n(6x±1的缩写).
		测试数可以看为6i-1 6i 6i+1 6i+2 6i+3 6i+4的其中之一,同样以6为周期.
		对于测试数的整个周期,其中:
			如果n能被6i 6i+2 6i+4整除:则n要是偶数,但±1,排除.
			如果n能被6i+3整除:则n要是3倍数,但±1,排除.
		综上,循环中只要考虑6i±1的情况.
		所以以5起始,前进6(自然选择号,前进4!),逐次判断0与+2.
		虽然很想写成以6为起始逐次判断±1的对称格式但是这样会加重时空负担.(不甘心.....)
		*/
		for(T c=5;c<=b;c+=6)//遍历判断是否能被因数分解——不会有人看不懂吧?
			if((!mod(a,c))||(!mod(a,(c+2))))
				return false;
		/*
		最后,为什么是6?
		就结论来说,此数值选择最常出现的两因数(除0或1外.)之积可以最大程度减少时间占用.
		所以虽然更喜欢7/8/9之类的数不过使用6收益最大.
		要不是这样早就写成7了.
		*/
		return true;
	}
	/*! 判断某数是否是素数 */
	template<class T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]inline constexpr bool is_prime_num(T a)noexcept{
		if constexpr(::std::is_floating_point_v<T>)
			if(a != ceil(a))
				return false;
		a=abs(a);

		if(a<4)
			return true;//1和0也是prime,我不管.
		/*
		当x≥1,那么≥5的自然数如下:
		6x-1 6x 6x+1 6x+2 6x+3 6x+4
		6(x+1)-1 6(x+1) 6(x+1)+1 ... //这已经是下一周期了.

		考虑单个周期:
		6x+2 6x+3 6x+4 是 2(3x+1) 3(2x+1) 2(3x+2),排除.
		6x,排除.
		那么,只用考虑6x±1是否是prime.
		*/
		if(mod(mod(a,6)-1,4))
			return false;
		/*
		因为no_pre_check判定没有考虑到≤5的数,所以本函数第一个if进行判定补全.
		*/
		return is_prime_num_no_pre_check(a);
	}
	/// 求大于或等于某数的素数
	template<class T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]inline constexpr T get_prime_num_big_or_eq_than(T a)noexcept{
		if constexpr(::std::is_floating_point_v<T>)
			a=ceil(a);
		a=abs(a);
		if(a<4)return a;
		//将a转换为6x-1的形式.
		{
			T b=mod(a,6);
			if(b==1 && is_prime_num_no_pre_check(a))return a;
			a+=b?5-b:1;
		}
		//循环判断.
		for(;;a+=6)
			if(is_prime_num_no_pre_check(a))return a;
			elseif(is_prime_num_no_pre_check(a+2))return a+2;
	}
	/// 求大于某数的素数
	template<class T> requires ::std::is_arithmetic_v<T>
	[[nodiscard]]inline constexpr T get_prime_num_big_than(T a)noexcept{
		return get_prime_num_big_or_eq_than(a+1);
	}
	push_and_disable_msvc_warning(26467);//gold_of_resize永远为正数
	/// 已知当前array的size，求下一个合适的提前分配大小
	[[nodiscard]]inline constexpr size_t get_next_gold_size_to_resize_for_array(size_t size)noexcept{
		/*
		每次扩容后的空间与原空间比大致为gold of resize可以最小化时空负担.
		*/
		return size_t(size*gold_of_resize);
	}
	/// 已知当前hash table的size，求下一个合适的桶大小
	[[nodiscard]]inline constexpr size_t get_next_gold_size_to_resize_for_hash(size_t size)noexcept{
		/*
		素数大小的桶数可以使hash table中的每个桶尽可能活跃.
		每次扩容后的空间与原空间比大致为gold of resize可以最小化时空负担.
		*/
		return size_t(get_prime_num_big_or_eq_than(size*gold_of_resize));
	}
	pop_msvc_warning();

	/// 线性插值.
	namespace linear_interpolation{
		[[nodiscard]]inline constexpr auto get_k(auto y1,auto y2,auto δx)noexcept{
			auto δy=sub(y2,y1);
			auto k=δy/δx;
			return k;
		}
		[[nodiscard]]inline constexpr auto get_k(auto y1,auto y2)noexcept{
			return get_k(y1,y2,1.0);
		}
		[[nodiscard]]inline constexpr auto get_result(auto y1,auto k,auto δx)noexcept{
			auto diff=k*δx;
			return y1+diff;
		}
		[[nodiscard]]inline constexpr auto get_reverse_result(auto k,auto δx,auto y)noexcept{
			auto diff=k*δx;
			return y-diff;
		}
	}
}
using magic_number::to_unsigned_t;
using magic_number::is_negative;
using magic_number::copy_as_negative;
using magic_number::copy_as_not_negative;
using magic_number::to_size_t;
using magic_number::mod;
using magic_number::set_rounding;
using magic_number::get_rounding;
using magic_number::rounding_auto_setter;
using magic_number::rotl;
using magic_number::rotr;
using magic_number::rotl_nomod;
using magic_number::rotr_nomod;
using magic_number::rot_iterator;
using magic_number::isNaN;
using magic_number::abs;
using magic_number::exp;
using magic_number::log;
using magic_number::pow;
using magic_number::sqrt;
using magic_number::ceil;
using magic_number::trunc;
using magic_number::is_prime_num;
using magic_number::get_prime_num_big_or_eq_than;
using magic_number::get_prime_num_big_than;
using magic_number::get_next_gold_size_to_resize_for_array;
using magic_number::get_next_gold_size_to_resize_for_hash;

//file_end

