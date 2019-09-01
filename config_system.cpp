#include "config_system.h"
#include <algorithm>
#include <string>

namespace forceinline {
	//Returns a string which will be written to the config file
	std::string element::make_string( std::uint32_t indent ) {
		std::string config_string( indent, '\t' );
		
		config_string += "<element name=\"";
		config_string += m_element_name;
		config_string += "\">";
		config_string += m_element_string;
		config_string += "</element>";

		return config_string;
	}
	
	/*
		Returns a reference to the created group which you can use to add elements/groups
		directly to.
	*/
	element_group& element_group::create_group( std::string_view group_name ) {
		element_group new_group( group_name );
		m_element_groups.push_back( new_group );

		auto it = std::find_if( m_element_groups.begin( ), m_element_groups.end( ), [ & ]( element_group e_g ) {
			return e_g.get_name( ).compare( group_name ) == 0;
		} );

		if ( it == m_element_groups.end( ) )
			throw std::runtime_error( "element_group::create_group: created group could not be found in m_element_groups" );

		return *it;
	}

	element_group element_group::get_group( std::string_view group_name ) {
		auto it = std::find_if( m_element_groups.begin( ), m_element_groups.end( ), [ & ]( element_group e_g ) {
			return e_g.get_name( ).compare( group_name ) == 0;
		} );

		if ( it == m_element_groups.end( ) ) {
			std::string exception_str = "element_group::get_group: group ";

			exception_str += group_name;
			exception_str += " not found";

			throw std::exception( exception_str.data( ) );
		}

		return *it;
	}

	element element_group::get_element( std::string_view element_name ) {
		auto it = std::find_if( m_elements.begin( ), m_elements.end( ), [ & ]( element e ) {
			return e.get_name( ).compare( element_name ) == 0;
		} );

		if ( it == m_elements.end( ) ) {
			std::string exception_str = "element_group::get_element: element ";

			exception_str += element_name;
			exception_str += " not found";

			throw std::exception( exception_str.data( ) );
		}

		return *it;
	}

	//Returns a string which will be written to the config file
	std::string element_group::make_string( std::uint32_t indent ) {
		std::string config_string( indent, '\t' );

		config_string += "<element_group name=\"";
		config_string += m_group_name;
		config_string += "\">\n";

		for ( auto& element_group : m_element_groups )
			config_string += element_group.make_string( indent + 1 ) + '\n';

		for ( auto& element : m_elements )
			config_string += element.make_string( indent + 1 ) + '\n';

		config_string += std::string( indent, '\t' ) + "</element_group>";

		return config_string;
	}

	void element_group::parse_group( const std::vector< std::string >& content ) {
		auto indent = content[ 0 ].find( "<" );

		std::vector< std::string > group_elements = { };
		for ( std::size_t i = 0; i < content.size( ); i++ ) {
			if ( content[ i ].find( "<element " ) == indent ) {
				auto element_name = get_element_attribute( content[ i ], "name" );
				auto element_value = get_element_value( content[ i ] );

				m_elements.push_back( element( element_name, element_value.data( ) ) );
			} else if ( content[ i ].find( "<element_group" ) == indent ) {
				auto group_name = get_element_attribute( content[ i ], "name" );

				do {
					group_elements.push_back( content[ ++i ] );
				} while ( content[ i ].find( "</element_group>" ) != indent );

				m_element_groups.push_back( element_group( group_name, group_elements ) );
				group_elements.clear( );
			}
		}
	}

	config_system::~config_system( ) {
		close( );
	}

	bool config_system::open_as_read( std::string_view file ) {
		//Close any previous stream
		close( );

		//Open a new stream for reading
		m_stream.open( file.data( ), std::ios::in );

		if ( !m_stream.is_open( ) )
			throw std::exception( "config_system::open_as_read: could not open file in read mode" );

		//Parse the file
		parse_config_file( file );
		return true;
	}

	bool config_system::open_as_write( std::string_view file ) {
		//Close any previous stream
		close( );

		//Open a new stream for writing
		m_stream.open( file.data( ), std::ios::out );

		if ( !m_stream.is_open( ) )
			throw std::exception( "config_system::open_as_write: could not open file in write mode" );

		return true;
	}

	//This should be used when writing to a file
	void config_system::save( ) {
		if ( !m_stream.is_open( ) )
			throw std::runtime_error( "config_system::save: file stream wasn't open when attempting to save" );

		for ( auto& element_group : m_element_groups )
			m_stream << element_group.make_string( 0 ) + '\n';

		for ( auto& element : m_elements )
			m_stream << element.make_string( 0 ) + '\n';

		close( );
	}

	//This should be used when reading from a file
	void config_system::close( ) {
		m_elements.clear( );
		m_element_groups.clear( );

		if ( m_stream.is_open( ) )
			m_stream.close( );
	}

	/*
		Returns a reference to the created group which you can use to add elements/groups
		directly to.
	*/
	element_group& config_system::create_group( std::string_view group_name ) {
		element_group new_group( group_name );
		m_element_groups.push_back( new_group );

		auto it = std::find_if( m_element_groups.begin( ), m_element_groups.end( ), [ & ]( element_group e_g ) {
			return e_g.get_name( ).compare( group_name ) == 0;
		} );

		if ( it == m_element_groups.end( ) )
			throw std::runtime_error( "config_system::create_group: created group could not be found in m_element_groups" );

		return *it;
	}
	
	element_group config_system::get_group( std::string_view group_name ) {
		auto it = std::find_if( m_element_groups.begin( ), m_element_groups.end( ), [ & ]( element_group e_g ) {
			return e_g.get_name( ).compare( group_name ) == 0;
		} );

		if ( it == m_element_groups.end( ) ) {
			std::string exception_str = "config_system::get_group: group ";
			
			exception_str += group_name;
			exception_str += " not found";

			throw std::exception( exception_str.data( ) );
		}		

		return *it;
	}

	element config_system::get_element( std::string_view element_name ) {
		auto it = std::find_if( m_elements.begin( ), m_elements.end( ), [ & ]( element e ) {
			return e.get_name( ).compare( element_name ) == 0;
		} );

		if ( it == m_elements.end( ) ) {
			std::string exception_str = "config_system::get_element: element ";

			exception_str += element_name;
			exception_str += " not found";

			throw std::exception( exception_str.data( ) );
		}

		return *it;
	}

	void config_system::parse_config_file( std::string_view file ) {
		std::vector< std::string > elements = { };

		std::string temp = "";
		while ( std::getline( m_stream, temp ) )
			elements.push_back( temp );
		
		std::vector< std::string > group_elements = { };
		for ( std::size_t i = 0; i < elements.size( ); i++ ) {
			if ( elements[ i ].find( "<element " ) == 0 ) {
				auto element_name = get_element_attribute( elements[ i ], "name" );
				auto element_value = get_element_value( elements[ i ] );

				m_elements.push_back( element( element_name, element_value.data( ) ) );
			} else if ( elements[ i ].find( "<element_group" ) == 0 ) {
				auto group_name = get_element_attribute( elements[ i ], "name" );

				do {
					group_elements.push_back( elements[ ++i ] );
				} while ( elements[ i ].compare( "</element_group>" ) != 0 );

				m_element_groups.push_back( element_group( group_name, group_elements ) );
				group_elements.clear( );
			}
		}
	}

	std::string get_element_attribute( const std::string& element, const std::string& attribute ) {
		auto attrib_pos = element.find( attribute + "=" );

		if ( attrib_pos == std::string::npos )
			return "";

		auto attrib_start = element.find( "\"", attrib_pos );
		
		if ( attrib_start == std::string::npos )
			return "";

		auto attrib_end = element.find( "\"", attrib_start + 1 );

		if ( attrib_end == std::string::npos )
			return "";

		return element.substr( attrib_start + 1, attrib_end - attrib_start - 1 );
	}

	std::string get_element_value( const std::string& element ) {
		auto value_start = element.find_first_of( ">" );

		if ( value_start == std::string::npos )
			return "";

		auto value_end = element.find( "</element>", value_start );

		if ( value_end == std::string::npos )
			return "";
		
		return element.substr( value_start + 1, value_end - value_start - 1 );
	}
}