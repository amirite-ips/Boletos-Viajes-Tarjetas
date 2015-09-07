#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Movilidad.cpp"

TEST_CASE("Las tarjetas pueden ser recargadas", "[recarga]"){
    TarjetaComun t;
    REQUIRE( t.saldo() == tdinero(0) );
    
    t.recargar(100);
    REQUIRE( t.saldo() == tdinero(100) );
    
    t.recargar(200);
    REQUIRE( t.saldo() == tdinero(334) );

    t.recargar(400);
    REQUIRE( t.saldo() == tdinero(826) );
}

