#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Movilidad.cpp"

TEST_CASE("Las tarjetas pueden ser recargadas", "[recarga]"){
    TarjetaComun t;
    REQUIRE( t.saldo() == tdinero(0) );
    
    t.recarga(100);
    REQUIRE( t.saldo() == tdinero(100) );
    
    t.recarga(200);
    REQUIRE( t.saldo() == tdinero(334) );

    t.recarga(400);
    REQUIRE( t.saldo() == tdinero(826) );
}

TEST_CASE("Costo de los boletos", "[discounting]"){
    TarjetaComun t1;
    TarjetaMedioBoleto t2;
    Colectivo trole = {"Semtur", "K", 23};
    Fecha f1 = {"Sab 5/11", 17, 45};
    
    t1.recarga(100);
    t2.recarga(100);
    
    t1.pagarBoleto(trole, f1);
    t2.pagarBoleto(trole, f1);
    
    REQUIRE( t1.saldo() == tdinero(100 - 5.75) );
    REQUIRE( t2.saldo() == tdinero(100 - 2.90) );
}
