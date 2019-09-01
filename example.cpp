#include "config_system.h"
#include <iostream>

int main( ) {
	try {
		forceinline::config_system config;

		//First we save a file
		if ( config.open_as_write( "example.xml" ) ) {
			//We create a group (note: use a reference)
			auto& my_favourites = config.create_group( "my favourites" );
			
			//Our group contains a subgroup
			auto& consumables = my_favourites.create_group( "consumables" );

			//Add elements to consumables
			consumables.add_element( "food", "pizza" );
			consumables.add_element( "drink", "coke" );

			//Create another group
			auto& my_lucky_numbers = config.create_group( "my lucky numbers" );

			//Add elements to my_lucky_numbers
			my_lucky_numbers.add_element( "most lucky", 3 );
			my_lucky_numbers.add_element( "pretty lucky", 30.45f );

			//Add an element without a group
			config.add_element( "current day", "sunday" );

			//Save our file
			config.save( );
		}

		//Now we read our cool file
		if ( config.open_as_read( "example.xml" ) ) {
			//Note: no reference here!
			auto my_favourites = config.get_group( "my favourites" );

			auto consumables = my_favourites.get_group( "consumables" );
			std::cout << "my favourite food is " << consumables.get_element( "food" ).as_string( ) 
				<< " and my favourite drink is " << consumables.get_element( "drink" ).as_string( ) << std::endl;

			auto my_lucky_numbers = config.get_group( "my lucky numbers" );
			std::cout << "my two lucky numbers are " << my_lucky_numbers.get_element( "most lucky" ).as_int( ) 
				<< " and " << my_lucky_numbers.get_element( "pretty lucky" ).as_float( ) << std::endl;

			std::cout << "the current day is " << config.get_element( "current day" ).as_string( ) << std::endl;

			//Close our file
			config.close( );
		}
	} catch ( const std::exception& e ) {
		//Catch any exceptions
		std::cout << e.what( ) << std::endl;
	}

	std::cin.get( );
	return 0;
}
