#include <elc/base_defs>
#include <iostream>
//opencv to write png
#include <opencv2/opencv.hpp>

int main() {
	using namespace elc::defs;
	//以下名称和全局名称重复所以需要反复声明
	using elc::defs::rand;
	using elc::defs::type_info;

	typedef uint16_t test_type;
	//let's output elc::defs::rand<test_type>'s output to a png file
	typedef unsigned_specific_size_t<sizeof(test_type)/2> half_type;//用这个当作图片坐标
	auto half_max = max(type_info<half_type>);
	{
		//灰度类型
		typedef unsigned char gray_type;
		//创建一个灰度图像，大小为half_max+1*half_max+1
		cv::Mat_<gray_type> img(half_max + 1, half_max + 1, gray_type(0));
		//填充
		auto rand_times = half_max * half_max*128;
		for (size_t i = 0; i < rand_times; ++i) {
			auto rand_v = rand<test_type>();
			auto x		= half_type(rand_v >> (CHAR_BIT * sizeof(half_type)));
			auto y		= half_type(rand_v);
			//gray++
			++img(x, y);
		}
		//亮度映射！
		//取矩阵中max
		auto max_gray = *std::max_element(img.begin(), img.end());
		//映射到0-255
		for(auto& i: img) {
			i = i * 255 / max_gray;
		}
		//写入文件
		cv::imwrite("output.png", img);
	}
	{
		//均匀性测试，需要彩色图像
		cv::Mat_<cv::Vec3b> img_color(half_max + 1, half_max + 1, cv::Vec3b(0, 0, 0));
		//填充
		//对于每个像素，随机填充rgb
		double rad_total = 0, green_total = 0, blue_total = 0;
		for(auto& i: img_color) {
			auto rad = rand<unsigned char>();
			auto green = rand<unsigned char>();
			auto blue  = rand<unsigned char>();
			rad_total += rad;
			green_total += green;
			blue_total += blue;
			i = cv::Vec3b(rad, green, blue);
		}
		//映射起来挺麻烦就算了
		//写入文件
		cv::imwrite("output_color.png", img_color);
		//输出三个total
		std::cout << "rad_total:" << rad_total << std::endl;
		std::cout << "green_total:" << green_total << std::endl;
		std::cout << "blue_total:" << blue_total << std::endl;
	}

	return 0;
}
