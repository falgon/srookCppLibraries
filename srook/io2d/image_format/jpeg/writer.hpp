// Copyright (C) 2017 roki
#ifndef INCLUDED_SROOK_IO2D_IMAGE_FORMAT_JPEG_WRITER_HPP
#define INCLUDED_SROOK_IO2D_IMAGE_FORMAT_JPEG_WRITER_HPP

#include<srook/io/bofstream.hpp>
#include"core/huffman_table.hpp"
#include<srook/config/noexcept_detection.hpp>
#include<fstream>
#include<string_view>
#include<type_traits>
#include<array>
#include<cstring>

namespace srook{
namespace io2d{
namespace jpeg{
inline namespace v1{

struct jpezy_writer{
	jpezy_writer(const std::size_t buffer_size,const property& pr_,const char* output_name)
		:ofps(buffer_size,output_name,std::ios::out | std::ios::trunc | std::ios::binary),pr(pr_){}

	void write_header()noexcept(false)
	{
		if(!ofps)throw std::runtime_error(__func__);

		// SOI
		(ofps | srook::bofstream::Byte) << MARKER::Marker << MARKER::SOI;

		// JFIF
		(ofps | srook::bofstream::Byte) << MARKER::Marker << MARKER::APP0;
		(ofps | srook::bofstream::Word) << 16;
		(ofps | srook::bofstream::Byte_n(5)) << "JFIF";
		(ofps | srook::bofstream::Word) << 0x0102;
		(ofps | srook::bofstream::Byte) << pr.get<property::At::Units>();
		(ofps | srook::bofstream::Word) << pr.get<property::At::HDensity>();
		(ofps | srook::bofstream::Word) << pr.get<property::At::VDensity>();
		(ofps | srook::bofstream::Byte) << pr.get<property::At::HThumbnail>();
		(ofps | srook::bofstream::Byte) << pr.get<property::At::VThumbnail>();

		// Comment
		if(pr.get<property::At::Comment>()){
			(ofps | srook::bofstream::Byte) << MARKER::Marker << MARKER::COM;
			(ofps | srook::bofstream::Word) << static_cast<unsigned int>(std::strlen(pr.get<property::At::Comment>()) + 3);
			(ofps | srook::bofstream::Byte_n(static_cast<unsigned int>(std::strlen(pr.get<property::At::Comment>())) + 1)) << pr.get<property::At::Comment>();
		}

		// DQT
		(ofps | srook::bofstream::Byte) << MARKER::Marker << MARKER::DQT;
		(ofps | srook::bofstream::Word) << 67;
		(ofps | srook::bofstream::Byte) << 0;
		for(std::size_t i=0; i<YQuantumTb.size(); ++i){
			(ofps | srook::bofstream::Byte) << static_cast<SROOK_BYTE>( YQuantumTb[ ZZ[i] ] );
		}
		(ofps | srook::bofstream::Byte) << MARKER::Marker << MARKER::DQT;
		(ofps | srook::bofstream::Word) << 67;
		(ofps | srook::bofstream::Byte) << 1;
		for(std::size_t i=0; i<CQuantumTb.size(); ++i){
			(ofps | srook::bofstream::Byte) << static_cast<SROOK_BYTE>( CQuantumTb[ ZZ[i] ] );
		}

		// DHT
		(ofps | srook::bofstream::Bytes) << YDcDht;
		(ofps | srook::bofstream::Bytes) << CDcDht;
		(ofps | srook::bofstream::Bytes) << YAcDht;
		(ofps | srook::bofstream::Bytes) << CAcDht;

		// Frame header
		(ofps | srook::bofstream::Byte) << MARKER::Marker << MARKER::SOF0;
		(ofps | srook::bofstream::Word) << (3 * pr.get<property::At::Dimension>() + 8);
		(ofps | srook::bofstream::Byte) << pr.get<property::At::SamplePrecision>();
		(ofps | srook::bofstream::Word) << pr.get<property::At::VSize>();
		(ofps | srook::bofstream::Word) << pr.get<property::At::HSize>();
		(ofps | srook::bofstream::Byte) << pr.get<property::At::Dimension>();

		(ofps | srook::bofstream::Byte) << 0;
		(ofps | srook::bofstream::Byte) << 0x22;
		(ofps | srook::bofstream::Byte) << 0;
		for(std::size_t i=1; i<3; ++i){
			(ofps | srook::bofstream::Byte) << i;
			(ofps | srook::bofstream::Byte) << 0x11;
			(ofps | srook::bofstream::Byte) << 1;
		}

		// Scan Header
		(ofps | srook::bofstream::Byte) << MARKER::Marker << MARKER::SOS;
		(ofps | srook::bofstream::Word) << (2 * pr.get<property::At::Dimension>() + 6);
		(ofps | srook::bofstream::Byte) << pr.get<property::At::Dimension>();
		for(int i=0; i<pr.get<property::At::Dimension>(); ++i){
			(ofps | srook::bofstream::Byte) << i;
			(ofps | srook::bofstream::Byte) << (i == 0 ? 0 : 0x11);
		}
		(ofps | srook::bofstream::Byte) << 0;
		(ofps | srook::bofstream::Byte) << 63;
		(ofps | srook::bofstream::Byte) << 0;
	}

	srook::bofstream& get_stream()noexcept
	{
		return ofps;
	}

	void write_eoi()
	SROOK_NOEXCEPT((ofps | srook::bofstream::Byte) << MARKER::Marker << MARKER::EOI)
	{
		(ofps | srook::bofstream::Byte) << MARKER::Marker << MARKER::EOI;
	}

	void output_file()noexcept(false)
	{
		ofps.output_file();
	}
private:
	srook::bofstream ofps;
	const property& pr;
};

} // inline namespace v1
} // namespace jpeg
} // namespace io2d
} // namespace srook

#endif