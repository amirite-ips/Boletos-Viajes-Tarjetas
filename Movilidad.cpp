#include <iostream>
#include <vector>
#include "include/catch.hpp"
#define forn(i, n) for(int i=0; i<(int)(n); ++i)
#define mod(a, b) (((a)%(b)+(b))%(b))
#define debug(x) cout<<#x<<" = "<<(x)<<endl;
using namespace std;

typedef float tdinero;
typedef int tinterno;
typedef int ttiempo;

///CLASSES AND INTERFACES

struct Colectivo{
    string      empresa;
    string      linea;
    tinterno    numero_interno;
};

struct Fecha{
    string  fecha;
    ttiempo hora;
    ttiempo minutos;
    ttiempo operator-(Fecha origen) const {
        auto aTiempoAbsoluto = [](ttiempo h, ttiempo m){ return 60 * h + m; };
        ttiempo a = aTiempoAbsoluto(hora, minutos);
        ttiempo b = aTiempoAbsoluto(origen.hora, origen.minutos);
        return mod(a - b, 1440);
    }
};

struct Viaje{
    Colectivo   colectivo;
    Fecha       fecha;
    tdinero     monto;
};

typedef vector<Viaje> tlviajes;

class TarjetaBase{
    public:
        TarjetaBase();
        virtual bool    pagarBoleto(Colectivo, Fecha) = 0;
        void            recarga(tdinero);
        tdinero         saldo();
        tlviajes        viajesRealizados();

    protected:
        bool            pagarBoleto(Colectivo, Fecha, const tdinero, const tdinero);
        static constexpr tdinero   BOLETO_COMUN        = 5.75;
        static constexpr tdinero   BOLETO_TRANSBORDO   = 1.90;

    private:
        tdinero         credito;
        tlviajes        log_viajes;
};

class TarjetaComun : public TarjetaBase {
    public:
        bool pagarBoleto(Colectivo, Fecha);
};

class TarjetaMedioBoleto : public TarjetaBase{
    public:
        bool pagarBoleto(Colectivo, Fecha);

    private:
        static constexpr tdinero   BOLETO_COMUN        = 2.90;
        static constexpr tdinero   BOLETO_TRANSBORDO   = 0.96;
};

/// MAIN

int main(){
    int a = 2;
    int b = 3;
    int c = 2;
    REQUIRE(a == b);
    REQUIRE(b == c);
    REQUIRE(a == c);

    return 0;
}

/// IMPLEMENTATIONS

// TarjetaBase

TarjetaBase::TarjetaBase() : credito(0) {}

void TarjetaBase::recarga(tdinero nuevo_monto){
    const pair<tdinero, tdinero> BONIFICACION [] = {{368,92}, {196,34}, {0,0}};

    credito += nuevo_monto;
    for(auto &p : BONIFICACION)
        if( nuevo_monto >= p.first ){
            credito += p.second;
            return;
        }
}

tdinero TarjetaBase::saldo(){
    return credito;
}

tlviajes TarjetaBase::viajesRealizados(){
    return log_viajes;
}

bool TarjetaBase::pagarBoleto(Colectivo colectivo, Fecha fecha,
                              const tdinero P_COMUN, const tdinero P_TRANS){
    tdinero descuento = P_COMUN;
    int n_viajes = log_viajes.size();

    if( ( n_viajes >= 1 )
    &&  ( colectivo.linea != log_viajes[n_viajes-1].colectivo.linea )
    &&  ( (fecha - log_viajes[n_viajes-1].fecha) < 60 )
    &&  ( n_viajes < 2 || (fecha - log_viajes[n_viajes-2].fecha >= 60) ) )
            descuento = P_TRANS;

    if( credito < descuento ) return false;

    credito -= descuento;
    log_viajes.push_back({colectivo, fecha, descuento});
    return true;
}

// TarjetaComun

bool TarjetaComun::pagarBoleto(Colectivo colectivo, Fecha fecha){
    return TarjetaBase::pagarBoleto(colectivo, fecha, TarjetaBase::BOLETO_COMUN,
                                    TarjetaBase::BOLETO_TRANSBORDO);
}

// TarjetaMedioBoleto

bool TarjetaMedioBoleto::pagarBoleto(Colectivo colectivo, Fecha fecha){
    if( fecha.hora < 6 )
         return TarjetaBase::pagarBoleto(colectivo, fecha, TarjetaBase::BOLETO_COMUN,
                                         TarjetaBase::BOLETO_TRANSBORDO);
    else return TarjetaBase::pagarBoleto(colectivo, fecha, BOLETO_COMUN, BOLETO_TRANSBORDO);
}

