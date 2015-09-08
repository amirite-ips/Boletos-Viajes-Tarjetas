#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>

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

class Fecha{
    public:
        Fecha(const char *);
        int hora() const;
        ttiempo operator-(Fecha) const;
    private:
        tm in;
        time_t datetime;
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

/// IMPLEMENTATIONS

//Fecha

Fecha::Fecha(const char *date) : in{} {
    sscanf(date,"%i-%i-%i %i:%i",&in.tm_mday, &in.tm_mon, &in.tm_year, &in.tm_hour, &in.tm_min);
    in.tm_year -= 1900;
    in.tm_mon -= 1;
    datetime = mktime( &in );
}

inline int Fecha::hora() const {
    return in.tm_hour;
}

ttiempo Fecha::operator-(Fecha origen) const {
    return (datetime - origen.datetime) / 60;
}
// TarjetaBase

TarjetaBase::TarjetaBase() : credito(0) {}

bool TarjetaBase::recarga(tdinero nuevo_monto){
    if (nuevo_monto <= 0) return false;
    const pair<tdinero, tdinero> BONIFICACION [] = {{368,92}, {196,34}, {0,0}};

    credito += nuevo_monto;
    for(auto &p : BONIFICACION)
        if( nuevo_monto >= p.first ){
            credito += p.second;
            return true;
        }
    return true;
}

tdinero TarjetaBase::saldo(){
    return credito;
}

tlviajes TarjetaBase::viajesRealizados(){
    return log_viajes;
}

bool TarjetaBase::pagarBoleto(Colectivo colectivo, Fecha fecha, const tdinero P_COMUN, const tdinero P_TRANS){
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
    if( fecha.hora() < 6 )
         return TarjetaBase::pagarBoleto(colectivo, fecha, TarjetaBase::BOLETO_COMUN,
                                         TarjetaBase::BOLETO_TRANSBORDO);
    else return TarjetaBase::pagarBoleto(colectivo, fecha, BOLETO_COMUN, BOLETO_TRANSBORDO);
}

