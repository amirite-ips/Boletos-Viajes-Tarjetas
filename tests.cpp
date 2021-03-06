#include "Movilidad.cpp"
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

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

TEST_CASE("La tarjeta no se puede usar sin saldo", "[nosaldo]"){
    TarjetaComun t;
    Colectivo           trole   = {"Semtur", "K", 23};
    Fecha               f1      = "7-9-2015 21:17";

    t.recarga(0);
    REQUIRE( t.pagarBoleto(trole, f1) == false );

    REQUIRE( t.recarga(-10) == false);
}

TEST_CASE("Existen fechas", "[datetime]"){
    Fecha f1 = "7-9-2015 21:17";
    Fecha f2 = "7-9-2015 22:20";
    Fecha f3 = "8-9-2015 22:20";

    REQUIRE( f1.hora() == ttiempo(21) );
    REQUIRE( (f2-f1) == ttiempo(63) );
    REQUIRE( (f3-f2) == ttiempo(1440) );
}

TEST_CASE("Costo de los boletos", "[discounting]"){
    TarjetaComun        t1;
    TarjetaMedioBoleto  t2;
    Colectivo           trole   = {"Semtur", "K", 23};
    Fecha               f1      = "7-9-2015 21:17";

    t1.recarga(100);
    t2.recarga(100);

    t1.pagarBoleto(trole, f1);
    t2.pagarBoleto(trole, f1);

    REQUIRE( t1.saldo() == tdinero(100 - 5.75) );
    REQUIRE( t2.saldo() == tdinero(100 - 2.90) );
}

TEST_CASE("Uso del medio boleto en horario correcto", "[medioboleto]"){
    TarjetaMedioBoleto  t2;
    Colectivo           trole   = {"Semtur", "K", 23};
    Fecha               f1      = "7-9-2015 01:54";
    Fecha               f2      = "7-9-2015 17:12";

    t2.recarga(100);

    t2.pagarBoleto(trole, f1);
    REQUIRE( t2.saldo() == tdinero(100 - 5.75) ); // Se paga entero

    t2.pagarBoleto(trole, f2);
    REQUIRE( t2.saldo() == tdinero(100 - 5.75 - 2.90) ); // Se paga medio entre 6hs y 24hs
}

TEST_CASE("Costo del boleto en transbordo", "[transbordo]"){
    TarjetaComun        t1;
    TarjetaMedioBoleto  t2;
    Colectivo           trole   = {"Semtur", "K", 23};
    Colectivo           cole    = {"Semtur", "137", 28};
    Fecha               f1      = "7-9-2015 12:00";
    Fecha               f2      = "7-9-2015 12:18";
    Fecha               f3      = "7-9-2015 12:49";
    Fecha               f4      = "7-9-2015 13:20";

    t1.recarga(100);
    t2.recarga(100);

    t1.pagarBoleto(trole, f1);
    t2.pagarBoleto(trole, f1);

    /* Marcas en otro */
    t1.pagarBoleto(cole, f2);
    t2.pagarBoleto(cole, f2);

    /* Usa transbordo */
    REQUIRE( t1.saldo() == tdinero(100 - 5.75 - 1.9) );
    REQUIRE( t2.saldo() == tdinero(100 - 2.90 - 0.96) );

    /* En horario para otro transbordo, aunque ya acaba de usar uno,
        por lo que no deberia hacer el precio de transbordo */
    t1.pagarBoleto(trole, f3);
    t2.pagarBoleto(trole, f3);

    REQUIRE( t1.saldo() == tdinero(100 - 5.75 - 1.9 - 5.75) );
    REQUIRE( t2.saldo() == tdinero(100 - 2.90 - 0.96 - 2.90) );

    /* Me subo al mismo como para mostrar que el transbordo en el mismo colectivo no se efectúa */
    t1.pagarBoleto(trole, f4);
    t2.pagarBoleto(trole, f4);

    REQUIRE( t1.saldo() == tdinero(100 - 5.75 - 1.90 - 5.75 - 5.75) );
    REQUIRE( t2.saldo() == tdinero(100 - 2.90 - 0.96 - 2.90 - 2.90) );
}

TEST_CASE("Lista de viajes", "[listaviajes]"){
    TarjetaComun        t1;
    Colectivo           trole   = {"Semtur", "K", 23};
    Colectivo           cole    = {"Semtur", "137", 28};
    Colectivo           otrocole    = {"RosBus", "128", 17};
    Fecha               f1      = "7-9-2015 12:00";
    Fecha               f2      = "7-9-2015 13:20";
    Fecha               f3      = "8-10-2015 17:34";

    t1.recarga(100);

    t1.pagarBoleto(trole, f1);
    t1.pagarBoleto(cole, f2);
    t1.pagarBoleto(otrocole, f3);

    tlviajes lista = t1.viajesRealizados();

    REQUIRE((lista[0].colectivo == trole  &&
             lista[0].fecha == f1         &&
             lista[0].monto == tdinero(5.75)));

    // repetir en los otros
    //REQUIRE( lista[1].colectivo == cole && lista[1].fecha == f2 && lista[1].monto == tdinero(5.75));
    //REQUIRE( lista[2].colectivo == otrocole && lista[2].fecha == f3 && lista[2].monto == tdinero(5.75));
}
