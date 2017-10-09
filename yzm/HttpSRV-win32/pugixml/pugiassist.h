/*
 * 增加该辅助类;实现char数组输出,实现取自官方文档;
 * 增加常用的命名重定义;       
 * 2011-4-21    yulu
 *   
 * Copyright (c) 2011 by <yulu / organization here>
 */

#ifndef INCLUDED_PUGIASSIST_H
#define INCLUDED_PUGIASSIST_H

#include "pugixml.hpp"
#include <string>
#include <stdio.h>
#if defined(WIN32)||defined(WIN64)
#else
#include <string.h>
#endif

typedef pugi::xml_node			CPugiXmlNode;
typedef pugi::xml_document		CPugiXmlDocument;
typedef pugi::xml_attribute		CPugiXmlAttribute;

#define PUGI_XML_DECLARATION	pugi::node_declaration
#define PUGI_XML_ELEMENT		pugi::node_element
#define PUGI_XML_PCDATA			pugi::node_pcdata

namespace pugi
{

	//////////////////////////////////////////////////////////////////////////
	// custom writer std::string
	class xml_string_writer : public xml_writer
	{
	public:
		std::string & result() { return m_result; }
		const std::string & result() const { return m_result; }
		void reset() { m_result.clear(); }

		virtual void write(const void* data, size_t size) { m_result += std::string(static_cast<const char*>(data), size); }

	protected:
		std::string m_result;
	};

	//sample
	/*std::string node_to_string(pugi::xml_node node)
	{
		xml_string_writer writer;
		node.print(writer);

		return writer.result();
	}*/

	//////////////////////////////////////////////////////////////////////////
	//custom writer, memory buffer ( just use, not own )
	class xml_memory_writer : public xml_writer
	{
	public:
		xml_memory_writer() : m_buffer(0), m_capacity(0), m_need(0) {}
		xml_memory_writer( char * buffer, size_t capacity ) : m_buffer(buffer), m_capacity(capacity), m_need(0) {}

		size_t written_size() const 
		{
			if( m_capacity == 0 )
				return 0;
			return m_need < m_capacity - 1 ? m_need : m_capacity - 1; 
		}
		size_t need_size() const { return m_need; }
		size_t capacity() const { return m_capacity; }
		char * result() { return m_buffer; }
		const char * result() const { return m_buffer; }

		void reset() { m_need = 0; }
		void reset( char * buffer, size_t capacity ) { m_buffer = buffer; m_capacity = capacity; m_need = 0; }

		void write_finish() 
		{ 
			if( m_capacity == 0 )
				return;
			if( m_need < m_capacity - 1 )
				m_buffer[ m_need ] = '\0';
			else
				m_buffer[ m_capacity - 1 ] = '\0';
		}

		virtual void write(const void* data, size_t size)
		{
			if( m_need < m_capacity - 1 )
			{
				size_t chunk = (m_capacity - 1 - m_need < size) ? m_capacity - 1 - m_need : size;
				memcpy( m_buffer + m_need, data, chunk);
			}

			m_need += size;
		}

	protected:
		char * m_buffer;
		size_t m_capacity;
		size_t m_need;
	};

	//sample
	//char* node_to_buffer(pugi::xml_node node, char* buffer, size_t size)
	//{
	//	if (size == 0) return buffer;

	//	// leave one character for null terminator
	//	xml_memory_writer writer(buffer, size);
	//	node.print(writer);

	//	// null terminate
	//	buffer[writer.written_size()] = 0; // or : writter.write_finish();

	//	return buffer;
	//}

	//char* node_to_buffer_heap(pugi::xml_node node)
	//{
	//	// first pass: get required memory size
	//	xml_memory_writer counter;
	//	node.print(counter);

	//	// allocate necessary size (+1 for null termination)
	//	char* buffer = new char[counter.need_size() + 1];

	//	// second pass: actual printing
	//	xml_memory_writer writer(buffer, counter.need_size());
	//	node.print(writer);

	//	// null terminate
	//	buffer[writer.written_size()] = 0; // or : writter.write_finish();

	//	return buffer;
	//}
}

//////////////////////////////////////////////////////////////////////////

//sample:

//int main()
//{
//	// get a test document
//	pugi::xml_document doc;
//	doc.load("<foo bar='baz'>hey</foo>");
//
//	// get contents as std::string (single pass)
//	printf("contents: [%s]\n", node_to_string(doc).c_str());
//
//	// get contents into fixed-size buffer (single pass)
//	char large_buf[128];
//	printf("contents: [%s]\n", node_to_buffer(doc, large_buf, sizeof(large_buf)));
//
//	// get contents into fixed-size buffer (single pass, shows truncating behavior)
//	char small_buf[22];
//	printf("contents: [%s]\n", node_to_buffer(doc, small_buf, sizeof(small_buf)));
//
//	// get contents into heap-allocated buffer (two passes)
//	char* heap_buf = node_to_buffer_heap(doc);
//	printf("contents: [%s]\n", heap_buf);
//	delete[] heap_buf;
//}

//////////////////////////////////////////////////////////////////////////

#endif // INCLUDED_PUGIASSIST_H
