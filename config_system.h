#pragma once
#include <string>
#include <fstream>
#include <vector>

/*
	TODO:
	-Add reading functionality (parsing)
	-Throw exception on error
*/

namespace forceinline {
	class element {
	public:
		element( ) { }
		element( std::string_view element_name, bool value ) : m_element_name( element_name )	{ m_element_string = value ? "1" : "0"; }
		element( std::string_view element_name, double value ) : m_element_name( element_name )	{ m_element_string = std::to_string( value ); }
		element( std::string_view element_name, float value ) : m_element_name( element_name )	{ m_element_string = std::to_string( value ); }
		element( std::string_view element_name, int value ) : m_element_name( element_name )	{ m_element_string = std::to_string( value ); }
		element( std::string_view element_name, const char* value ) : m_element_name( element_name ), m_element_string( value ) { }

		bool as_bool( )				{ return as_int( ) == 1; }
		double as_double( )			{ return std::stod( m_element_string ); }
		float as_float( )			{ return std::stof( m_element_string ); }
		int as_int( )				{ return std::stoi( m_element_string ); }
		std::string as_string( )	{ return m_element_string; }

		std::string get_name( )		{ return m_element_name; }

		std::string make_string( std::uint32_t indent );

	private:
		std::string m_element_name = "", m_element_string = "";
	};

	class element_group {
	public:
		element_group( ) { }
		element_group( std::string_view name ) : m_group_name( name ) { }
		element_group( std::string_view name, std::vector< std::string > content ) : m_group_name( name ) { parse_group( content ); }

		void add_element( std::string_view name, bool value )				{ m_elements.push_back( element( name, value ) ); }
		void add_element( std::string_view name, double value )				{ m_elements.push_back( element( name, value ) ); }
		void add_element( std::string_view name, float value )				{ m_elements.push_back( element( name, value ) ); }
		void add_element( std::string_view name, int value )				{ m_elements.push_back( element( name, value ) ); }
		void add_element( std::string_view name, const char* value )		{ m_elements.push_back( element( name, value ) ); }

		element_group& create_group( std::string_view group_name );
		element_group get_group( std::string_view group_name );

		element get_element( std::string_view element_name );

		std::string get_name( ) { return m_group_name; }

		std::string make_string( std::uint32_t indent );

	private:
		void parse_group( const std::vector< std::string >& content );

		std::string m_group_name = "";

		std::vector< element > m_elements = { };
		std::vector< element_group > m_element_groups = { };
	};

	class config_system {
	public:
		config_system( ) { }
		~config_system( );

		void add_element( std::string_view name, bool value )			{ m_elements.push_back( element( name, value ) ); }
		void add_element( std::string_view name, float value )			{ m_elements.push_back( element( name, value ) ); }
		void add_element( std::string_view name, int value )			{ m_elements.push_back( element( name, value ) ); }
		void add_element( std::string_view name, const char* value )	{ m_elements.push_back( element( name, value ) ); }

		bool open_as_read( std::string_view file );
		bool open_as_write( std::string_view file );

		void save( );
		void close( );

		element_group& create_group( std::string_view group_name );
		element_group get_group( std::string_view group_name );

		element get_element( std::string_view element_name );

	private:
		void parse_config_file( std::string_view file );

		std::fstream m_stream = { };

		std::vector< element > m_elements = { };
		std::vector< element_group > m_element_groups = { };
	};

	std::string get_element_attribute( const std::string& element, const std::string& attribute );
	std::string get_element_value( const std::string& element );
}