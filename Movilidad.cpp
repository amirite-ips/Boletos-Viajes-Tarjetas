#include<iostream>
#include<vector>
#define forn(i, n) for(int i=0; i<(int)(n); ++i)
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
        return a - b;
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
        bool            pagarBoleto(Colectivo, Fecha, tdinero, tdinero);
        constexpr tdinero   BOLETO_COMUN        = 5.75;
        constexpr tdinero   BOLETO_TRANSBORDO   = 1.90;

    private:
        tdinero         credito;
        tlviajes        log_viajes;
};

class TarjetaComun : public TarjetaBase {
    public:
        bool pagarBoleto(Colectivo, Fecha);

    private:




};

class TarjetaMedioBoleto : public TarjetaBase{
    public:
        bool pagarBoleto(Colectivo, Fecha);

    private:
        const tdinero   BOLETO_COMUN        = 2.90;
        const tdinero   BOLETO_TRANSBORDO   = 0.96;



};

/// MAIN

int main(){

    cout<<"Movilidad"<<endl;

    TarjetaComun x;

    x.recarga(368);

    cout<< x.saldo() <<endl;


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
                              const tdinero PCOMUN, const tdinero PTRANS){
    return true;
}

// TarjetaComun

bool TarjetaComun::pagarBoleto(Colectivo colectivo, Fecha fecha){
    return true;
}


// TarjetaMedioBoleto

bool TarjetaMedioBoleto::pagarBoleto(Colectivo colectivo, Fecha fecha){
    return true;
}

