// Copyright (C) 2017 roki
#ifndef INCLUDED_SROOK_IO2D_COMPRESSION_JPEG_PNM_STREAM_HPP
#define INCLUDED_SROOK_IO2D_COMPRESSION_JPEG_PNM_STREAM_HPP

#include<fstream>
#include<stdexcept>
#include<string>
#include<algorithm>
#include<optional>
#include<vector>
#include<list>
#include<iostream>
#include<experimental/array>
#include<experimental/iterator>
#include<boost/algorithm/string/split.hpp>
#include<boost/range/algorithm/copy.hpp>
#include<boost/algorithm/string/classification.hpp>
#include<boost/range/algorithm/transform.hpp>
#include<srook/algorithm/for_each.hpp>
#include"../jpeg/core/jpeg.hpp"
#include"../jpeg/writer.hpp"
#include"../jpeg/encoder.hpp"

namespace srook{
namespace io2d{
namespace pnm{
inline namespace v1{

struct to_jpeg{
	explicit constexpr to_jpeg(const char* file_):file(file_){}
	const char* file;
};


const auto gray_scale = []{};

struct pnm_stream{
	pnm_stream(const char* file_name)
		:initializing_succeed(true),width(0),height(0),max_color(0)
	{
		using namespace std::string_literals;

		const auto jump_comment = 
			[](std::ifstream& ifs){
				std::string str;
				while(std::getline(ifs,str) and str.find("#")!=std::string::npos);
				return str;
			};

		std::ifstream ifs(file_name);
		
		if(!ifs){
		   	initializing_succeed = false;
		}else{
			std::string format = jump_comment(ifs);
			if(format != "P3"s){
				initializing_succeed = false;
			}else{
				format = jump_comment(ifs);
				std::list<std::string> wh;
				boost::split(wh,format,boost::is_space());
				if(wh.size() != 2){
					initializing_succeed = false;
				}else{
					width = std::stoi(*std::begin(wh));
					height = std::stoi(*std::next(std::begin(wh),1));

					format = jump_comment(ifs);
					max_color = std::stoi(format);
	
					std::list<value_type> img;
					for(std::string line = jump_comment(ifs); !ifs.eof(); line = jump_comment(ifs)){
						std::list<std::string> spl;
						boost::split(spl,line,boost::is_space());
						if(spl.back()==""s)spl.pop_back();
						boost::transform(spl,std::back_inserter(img),[](const std::string& str){return srook::byte(std::stoi(str));});
					}

					rgb_img.resize(img.size()/3);
					typename decltype(rgb_img)::value_type::value_type r,g,b;
					for(auto& v:rgb_img){
						for(typename decltype(rgb_img)::value_type::value_type& cl:{std::ref(r),std::ref(g),std::ref(b)}){
							cl = img.front();
							img.pop_front();
						}
						v = std::experimental::make_array(r,g,b);
					}
				}
				std::cout << "width: " << width << " height: " << height << std::endl;
			}
		}
	}

	explicit operator bool()const noexcept
	{
		return initializing_succeed;
	}
private:
	using value_type = srook::byte;
	using rgb_type = srook::byte;

	bool initializing_succeed;
	std::size_t width,height,max_color;
	std::vector<std::array<rgb_type,3>> rgb_img;

	inline void report_error(const char* funcName)const noexcept(false)
	{
		if(initializing_succeed)return;

		std::string str = "Initializing was failed: ";
		str += funcName;
		throw std::runtime_error(str.c_str());
	}

	template<class T,std::enable_if_t<std::is_same_v<T,std::ostream> or std::is_same_v<T,std::ofstream>,std::nullptr_t> = nullptr>
	friend T& operator<<(T& os,const pnm_stream& pnm)
	noexcept(false)
	{
		using p3_ascii_type = std::size_t;

		pnm.report_error(__func__);
		os << "P3\n" << static_cast<p3_ascii_type>(pnm.width) << " " << static_cast<p3_ascii_type>(pnm.height) << "\n";
		os << static_cast<p3_ascii_type>(pnm.max_color) << "\n";

		for(const auto& rgb:pnm.rgb_img){
			boost::transform(rgb,std::experimental::make_ostream_joiner(os," "),[](const auto& v){return static_cast<p3_ascii_type>(v);});
			os << '\n';
		}
		return os;
	}

	std::tuple<std::vector<rgb_type>,std::vector<rgb_type>,std::vector<rgb_type>>
	split_rgb()const noexcept
	{
		std::vector<rgb_type> r(rgb_img.size()),g(rgb_img.size()),b(rgb_img.size());

		srook::for_each(
				srook::make_counter({std::ref(r),std::ref(g),std::ref(b)}),
				[this](std::vector<rgb_type>& element,std::size_t i)
				{
					boost::transform(rgb_img,std::begin(element),[&i](const std::array<rgb_type,3>& ar){return srook::byte(ar[i]);});
				}
		);

		return std::make_tuple(r,g,b);
	}

	friend std::ofstream& 
	operator<<(std::ofstream& ofs,const std::pair<const to_jpeg,const pnm_stream&>& pnm)
	noexcept(false)
	{
		ofs.close();
		pnm.second.report_error(__func__);

		srook::io2d::jpeg::property pr{
				pnm.second.width,
				pnm.second.height,
				3,8,
				"Encoded by srook io2d",
				srook::io2d::jpeg::property::Format::JFIF,
				srook::byte(1),srook::byte(2),
				srook::io2d::jpeg::property::Units::dots_inch,
				96,96,
				0,0,
				srook::io2d::jpeg::property::ExtensionCodes::undefined
		};

		auto&& [r,g,b] = pnm.second.split_rgb();
		srook::io2d::jpeg::encoder enc(std::move(pr),std::move(r),std::move(g),std::move(b));
		const std::size_t size = enc.encode<decltype(enc)::COLOR_MODE>(pnm.first.file);
		std::cout << "Output size: " << size << " srook::byte" << std::endl;

		return ofs;
	}

	friend std::ofstream&
	operator<<(std::ofstream& ofs,const std::pair<decltype(gray_scale),std::pair<const to_jpeg,const pnm_stream&>>& pnm)
	{
		ofs.close();
		pnm.second.second.report_error(__func__);

		srook::io2d::jpeg::property pr{
			pnm.second.second.width,
			pnm.second.second.height,
			3,8,
			"Encoded by srook io2d",
			srook::io2d::jpeg::property::Format::JFIF,
			srook::byte(1),srook::byte(2),
			srook::io2d::jpeg::property::Units::dots_inch,
			96,96,
			0,0,
			srook::io2d::jpeg::property::ExtensionCodes::undefined
		};

		auto&& [r,g,b] = pnm.second.second.split_rgb();
		srook::io2d::jpeg::encoder enc(std::move(pr),std::move(r),std::move(g),std::move(b));
		const std::size_t size = enc.encode<decltype(enc)::GRAY_MODE>(pnm.second.first.file);
		std::cout << "Output size: " << size << " srook::byte" << std::endl;

		return ofs;
	}

	friend std::pair<const decltype(gray_scale),std::pair<const to_jpeg,const pnm_stream&>>
	operator|(const std::pair<const to_jpeg,const pnm_stream&>& pnm,const decltype(gray_scale)& gr)
	{
		return std::make_pair(gr,pnm);
	}

	friend std::pair<const to_jpeg,const pnm_stream&> 
	operator|(const pnm_stream& pnm,const to_jpeg& jpeg_tag)noexcept
	{
		return std::make_pair(jpeg_tag,std::cref(pnm));
	}
};

} // inline namespace v1
} // namespace pnm
} // namespace io2d
} // namespace srook

#endif