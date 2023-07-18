///		Integrantes: Corona Díaz Raúl Omar, Mireles Vega Néstor David, Gutiérrez Esparza Jesús Omar.
///		Estructuras de Datos II. Graciela Lara López.
///		Sección: D07.
///		Proyecto: "Índice Secundario con enlace fuerte".

#include <iostream>
#include <fstream>    //Libreria necesaria para exportar los atributos a un archivo externo
#include <string>     //Libreria necesaria para "to_string"
#include <vector>     //Libreria necesaria para hacer uso de los vectores dentro del programa
#include <algorithm>  //Libreria necesaria para poder convertir a mayusculas los atributos
#include <random>     //Libreria necesaria para la generación de números aleatorios
#include <chrono>     //Libreria necesaria para la generación de números aleatorios
#include <functional> //Libreria necesaria para la generación de números aleatorios
#include <windows.h>  //Libreria necesaria para evitar que se genere el mismo número al solicitarlo

using namespace std;

/// Registro de Contribuyente.
struct Contribuyente {
    int NRR;
    string rfc;
    string name;
    string lastNameFirst;
    string lastNameSecond;
    string phoneNumber;
    string address;
    string city;
    string year;
    string month;
    string day;
    string birthdate;
    string civilStatus;
    int dependents;
};

/// INICIA LA FUNCIÓN DE CONVERTIR A MAYÚSCULAS.
// Toma un contribuyente desde el vector de Contribuyentes y convierte los datos del objeto a mayúsculas.
void toUpper(Contribuyente& myContribuyente) {
    transform(myContribuyente.rfc.begin(), myContribuyente.rfc.end(), myContribuyente.rfc.begin(), ::toupper);
    transform(myContribuyente.name.begin(), myContribuyente.name.end(), myContribuyente.name.begin(), ::toupper);
    transform(myContribuyente.lastNameFirst.begin(), myContribuyente.lastNameFirst.end(), myContribuyente.lastNameFirst.begin(), ::toupper);
    transform(myContribuyente.lastNameSecond.begin(), myContribuyente.lastNameSecond.end(), myContribuyente.lastNameSecond.begin(), ::toupper);
    transform(myContribuyente.address.begin(), myContribuyente.address.end(), myContribuyente.address.begin(), ::toupper);
    transform(myContribuyente.city.begin(), myContribuyente.city.end(), myContribuyente.city.begin(), ::toupper);
    transform(myContribuyente.civilStatus.begin(), myContribuyente.civilStatus.end(), myContribuyente.civilStatus.begin(), ::toupper);
}
/// TERMINA LA FUNCIÓN DE CONVERTIR A MAYÚSCULAS.

/// INICIA LA FUNCION PARA GENERAR UN NÚMERO ALEATORIO.
int randN(unsigned a, unsigned b) {
    int result;

    default_random_engine generator(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(a,b);
    auto dice = bind(distribution, generator);

    Sleep(1);
    result=dice();

    return result;
}
/// TERMINA LA FUNCION PARA GENERAR UN NÚMERO ALEATORIO.

/// INICIA LA FUNCIÓN PARA GENERAR LA LLAVE CANONICA.
string canonicalKey(Contribuyente& myContribuyente) {
    string result;

    //Agrega los primeros 2 caracteres del primer apellido, y los el primer caracter tanto del nombre como del segundo apellido.
    result=myContribuyente.lastNameFirst.substr(0,2)
           +myContribuyente.lastNameSecond.substr(0,1)
           +myContribuyente.name.substr(0,1);

    //Agrega los digitos correspondientes a la fecha de nacimiento
    result+=myContribuyente.year.substr(2,4)
            +myContribuyente.month
            +myContribuyente.day;

    // Se asigna al RFC con digitos random para la homoclave.
    result+=to_string(randN(0,9))+char(randN(65,90))+to_string(randN(0,9));

    return result;
}
/// TERMINA LA FUNCIÓN PARA GENERAR LA LLAVE CANONICA.

/// INICIA LA FUNCIÓN PARA ESTRUCTURAR LOS DATOS DE TAL MANERA QUE SEA LEGIBLE PARA EL USUARIO.
string toString(const Contribuyente&data) {
    string result;

    result=data.rfc+" | "
           +=data.name+' '+data.lastNameFirst+' '+data.lastNameSecond;
    result.resize(50,' ');
    result+=" | "+data.phoneNumber;
    result.resize(63,' ');
    result+=" | "+data.address;
    result.resize(90,' ');
    result+=" | "+data.city;
    result.resize(105,' ');
    result+=" | "+data.year+"/"+data.month+"/"+data.day
            +=" | "+data.civilStatus.substr(0,1);
    result.resize(129,' ');
    result+=" | "+to_string(data.dependents);

    return result;
}
/// TERMINA LA FUNCIÓN PARA ESTRUCTURAR LOS DATOS DE TAL MANERA QUE SEA LEGIBLE PARA EL USUARIO.

/// INICIA LA FUNCIÓN QUE ESCRIBE LOS DATOS AL ARCHIVO EXTERNO.
void writeHaciendaToDisk(const vector<Contribuyente>& data) {
    fstream file;
    Contribuyente aux;
    string result;

    //Abre el archivo para su sobrescritura.
    file.open("hacienda.txt",ios::out);

    //Si el archivo no se pudo crear o abrir lanza error.
    if(!file.is_open()) {
        cout<<"No se pudo crear o abrir archivo para escritura.";
        return;
    }

    //Ciclo en el cual se llena el archivo externo.
    if(!data.empty()) {
        unsigned i(0);
        while(i<data.size()) {
            aux=data.at(i);


            result=to_string(aux.NRR);
            result.resize(3,' ');
            result+="|"+aux.rfc
                  +="|"+aux.name;
            result.resize(38,' ');
            result+="|"+aux.lastNameFirst;
            result.resize(52,' ');
            result+="|"+aux.lastNameSecond;
            result.resize(64,' ');
            result+="|"+aux.phoneNumber;
            result.resize(77,' ');
            result+="|"+aux.address;
            result.resize(105,' ');
            result+="|"+aux.city;
            result.resize(122,' ');
            result+="|"+aux.year+"|"+aux.month+"|"+aux.day
                    +="|"+aux.civilStatus;
            result.resize(146,' ');
            result+="|"+to_string(aux.dependents);

            file<<result<<"\n";
            i++;
        }
    }

    //Cierra el archivo para no consumir más recursos.
    file.close();
}
/// TERMINA LA FUNCIÓN QUE ESCRIBE LOS DATOS AL ARCHIVO EXTERNO.

/// INICIA FUNCIÓN QUE ELIMINA LOS ESPACIOS EN BLANCO AL FINAL DE CADA CADENA
string cleanLastSpaces(const string&data){
    string result(data);
    while (result.size() > 0 && result[result.size()-1] == ' ') { //Eliminar espacios en blanco adicionales al final de la cadena
        result.erase(result.size()-1);
    }

    return result;
}
/// TERMINA FUNCIÓN QUE ELIMINA LOS ESPACIOS EN BLANCO AL FINAL DE CADA CADENA

/// INICIA LA FUNCIÓN QUE LEE LOS DATOS DEL ARCHIVO EXTERNO.
void readFromHacienda(vector<Contribuyente>& data) {
    fstream file;
    string readedData;
    Contribuyente aux;

    //Abre el archivo para su lectura.
    file.open("hacienda.txt",ios::in);

    //Si el archivo no se pudo crear o abrir lanza error.
    if(!file.is_open()) {
        cout<<"No se pudo abrir archivo para lectura.";
        return;
    }

    //Si hay algún dato en el ejecutable, estos datos se ven sobreescritos por los del documento
    if(!data.empty()) {
        data.clear();
    }

    //Ciclo en el cual se llena el ejecutable con los datos del archivo externo.
    while(!file.eof()) {
        getline(file,readedData,'|');

        //Valida que el dato que lee del archivo no sea un salto de linea.
        if(readedData!="") {
            //Entra el NRR
            aux.NRR=stoi(readedData);

            //Entra el RFC
            getline(file,readedData,'|');
            aux.rfc=readedData;

            //Entra el nombre
            getline(file,readedData,'|');
            aux.name=cleanLastSpaces(readedData);

            //Entra el apellido paterno
            getline(file,readedData,'|');
            aux.lastNameFirst=cleanLastSpaces(readedData);

            //Entra el apellido materno
            getline(file,readedData,'|');
            aux.lastNameSecond=cleanLastSpaces(readedData);

            //Entra el telefono
            getline(file,readedData,'|');
            aux.phoneNumber=readedData;

            //Entra el domicilio
            getline(file,readedData,'|');
            aux.address=cleanLastSpaces(readedData);

            //Entra la ciudad
            getline(file,readedData,'|');
            aux.city=cleanLastSpaces(readedData);

            //Entra el año de nacimiento
            getline(file,readedData,'|');
            aux.year=readedData;

            //Entra el mes de nacimiento
            getline(file,readedData,'|');
            aux.month=readedData;

            //Entra el día de nacimiento
            getline(file,readedData,'|');
            aux.day=readedData;

            //Entra el estado civil
            getline(file,readedData,'|');
            aux.civilStatus=cleanLastSpaces(readedData);

            //Entra la cantidad de dependientes
            getline(file,readedData);
            aux.dependents=stoi(readedData);

            //Se agrega a la lista de contribuyentes
            data.push_back(aux);
        }
    }

    //Cierra el archivo para no consumir más recursos.
    file.close();
}
/// TERMINA LA FUNCIÓN QUE LEE LOS DATOS DEL ARCHIVO EXTERNO.

/// INICIA LA FUNCIÓN QUE ESCRIBE LA LLAVE PRIMARIA A UN ARCHIVO EXTERNO.
void writePrimaryIndex(vector<Contribuyente> data) {
    fstream file;
    Contribuyente aux;

    //Abre el archivo para su sobrescritura.
    file.open("INDICE Primario.txt",ios::out);

    //Si el archivo no se pudo crear o abrir lanza error.
    if(!file.is_open()) {
        cout<<"No se pudo crear o abrir archivo para escritura.";
        return;
    }

    //Ordena el vector por su llave primaria (RFC)
    sort(data.begin(), data.end(), [](const Contribuyente& a,const Contribuyente& b) {
        return a.rfc < b.rfc;
    });

    //Ciclo en el cual se llena el archivo externo.
    if(!data.empty()) {
        unsigned i(0);
        while(i<data.size()) {
            aux=data.at(i);
            file<<aux.rfc<<" | "
                <<to_string(aux.NRR)<<"\n";
            i++;
        }
    }

    //Cierra el archivo para no consumir más recursos.
    file.close();
}
/// TERMINA LA FUNCIÓN QUE ESCRIBE LA LLAVE PRIMARIA A UN ARCHIVO EXTERNO.

/// INICIA LA FUNCIÓN QUE ESCRIBE LA LLAVE SECUNDARIA A UN ARCHIVO EXTERNO.
void writeSecondaryIndex(vector<Contribuyente> data){
    fstream file;
    string str;

    //Abre el archivo para su sobrescritura.
    file.open("INDICE Secundario.txt",ios::out);

    //Si el archivo no se pudo crear o abrir lanza error.
    if(!file.is_open()) {
        cout<<"No se pudo crear o abrir archivo para escritura.";
        return;
    }

    //Ordena el vector por su llave primaria (RFC)
    sort(data.begin(), data.end(), [](const Contribuyente& a,const Contribuyente& b) {
        return a.rfc < b.rfc;
    });

    //Una vez ordenado, lo vuelve a ordenar el vector por su llave secundaria (Ciudad)
    sort(data.begin(), data.end(), [](const Contribuyente& a,const Contribuyente& b) {
        return a.city < b.city;
    });

    //Ciclo en el cual se llena el archivo externo.
    if(!data.empty()) {
        unsigned i(0);
        while(i<data.size()) {
            str=data.at(i).city;
            str.resize(14,' ');
            file<<str;

            //Valida que la ciudad de la posicion actual y de la siguiente sean las mismas y no sea el ultimo registro
            while(i<data.size()-1&&data.at(i).city==data.at(i+1).city){
                str=to_string(data.at(i).NRR);
                str.resize(3,' ');
                file<<" | "<<str;
                i++;
            }
            file<<" | "<<to_string(data.at(i).NRR);
            file<<endl;
            i++;
        }
    }

    //Cierra el archivo para no consumir más recursos.
    file.close();
}
/// TERMINA LA FUNCIÓN QUE ESCRIBE LA LLAVE SECUNDARIA A UN ARCHIVO EXTERNO.

/// INICIA LA FUNCIÓN QUE BUSCA LOS DATOS POR LA LLAVE PRIMARIA (RFC).
void findDataPrimaryKey(vector<Contribuyente>& myContribuyente) {
    string looking4rfc; // Variable para buscar por RFC

    cout << "RFC a buscar: ";
    cin >> looking4rfc;

    for (int i = 0; i < myContribuyente.size(); i++) { // Recorrido para encontrar el objeto correcto
        if (looking4rfc == myContribuyente[i].rfc) { // Si nuestra variable (looking4rfc) coincide con el valor de rfc del contribuyente, lo va a mostrar
            cout<<"RFC           | NOMBRE Y APELLIDOS                 | TELEFONO   | DIRECCIÓN                | CIUDAD       | FECHA NAC. | E. CIVIL | DEPENDIENTES"<<endl;
            for(unsigned i=0; i<144; i++) {
                cout<<"-";
            }
            cout<<endl<<toString(myContribuyente[i])<<endl<<endl;

            return;
        }
    }
    cout << "\nNo se encontró ningún contribuyente con el RFC proporcionado." << endl << endl;
}
/// TERMINA LA FUNCIÓN QUE BUSCA LOS DATOS POR LA LLAVE PRIMARIA (RFC).

/// INICIA LA FUNCIÓN QUE BUSCA LOS DATOS POR LA LLAVE SECUNDARIA (CIUDAD).
void findDataByCity(const vector<Contribuyente>& myContribuyentes) {
    string lookin4city; // variable para la ciudad a consultar
    cout << "Ciudad a consultar: ";
    cin.ignore();
    getline(cin,lookin4city);

    for (int i = 0; i < lookin4city.length(); i++) { // Convierte a mayúsculas el valor de la variable
        lookin4city[i] = toupper(lookin4city[i]);
    }

    vector<Contribuyente> contribuyentesCiudad; // Creamos otro vector solo para la ciudad solicitada

    for (const auto& contribuyente : myContribuyentes) {
        if (contribuyente.city == lookin4city) {
            contribuyentesCiudad.push_back(contribuyente); // push_back los manda hasta atrás del vector
        }
    }

    if (contribuyentesCiudad.empty()) { // ¿Está vacío?
        cout << "\nNo se encontraron contribuyentes en la ciudad de " << lookin4city << endl << endl;
        return;
    }

    // Ordena el vector de contribuyentes de la misma ciudad por RFC
    sort(contribuyentesCiudad.begin(), contribuyentesCiudad.end(), [](const Contribuyente& a, const Contribuyente& b) {
        return a.rfc < b.rfc;
    });

    cout<<"RFC           | NOMBRE Y APELLIDOS                 | TELEFONO   | DIRECCIÓN                | CIUDAD       | FECHA NAC. | E. CIVIL | DEPENDIENTES"<<endl;
    for(unsigned i=0; i<144; i++) {
        cout<<"-";
    }
    cout<<endl;
    for (const auto& contribuyente : contribuyentesCiudad) { // Recorrido a través de la lista de contribuyentes de la misma ciudad
        cout<<toString(contribuyente)<<endl;

    }
    cout << endl;
}
/// TERMINA LA FUNCIÓN QUE BUSCA LOS DATOS POR LA LLAVE SECUNDARIA (CIUDAD).

/// INCIA LA FUNCIÓN QUE MUESTRA LA LISTA DE LOS ATRIBUTOS.
void showData(vector<Contribuyente>& listaContribuyentes) {
    // Revisa si la lista no está vacía.
    if(listaContribuyentes.empty()) {
        cout<<"Error: no hay contribuyentes en la lista."<<endl;

        return;
    }
    cout<<"RFC           | NOMBRE Y APELLIDOS                 | TELEFONO   | DIRECCIÓN                | CIUDAD       | FECHA NAC. | E. CIVIL | DEPENDIENTES"<<endl;
    for(unsigned i=0; i<144; i++) {
        cout<<"-";
    }
    cout<<endl;
    // Itera sobre la lista y muestra los datos.
    for(const auto& myContribuyente : listaContribuyentes) {
        cout<<toString(myContribuyente)<<endl;
    }
    cout<<endl;
}
/// TERMINA LA FUNCIÓN QUE MUESTRA LA LISTA DE LOS ATRIBUTOS.

/// INICIA LA FUNCIÓN PARA AGREGAR DATOS.
void insertData(vector<Contribuyente>& listaContribuyentes) {
    Contribuyente myContribuyente;

    // Agrega tildes u otros datos del idioma español.
    setlocale(LC_ALL, "Spanish");
    setlocale(LC_NUMERIC, "C");

    myContribuyente.NRR=listaContribuyentes.size();

    cout<<"Digite su nombre: ";
    cin.ignore();
    getline(cin, myContribuyente.name);

    cout<<"Digite su primer apellido: ";
    getline(cin, myContribuyente.lastNameFirst);

    cout<<"Digite su segundo apellido: ";
    getline(cin, myContribuyente.lastNameSecond);

    cout<<"Digite su número de teléfono: ";
    getline(cin, myContribuyente.phoneNumber);

    cout<<"Digite su dirección: ";
    getline(cin, myContribuyente.address);

    cout<<"Digite su ciudad: ";
    getline(cin, myContribuyente.city);

    cout<<"Digite su año de nacimiento YYYY: ";
    getline(cin, myContribuyente.year);

    cout<<"Digite su mes de nacimiento MM: ";
    getline(cin, myContribuyente.month);

    cout<<"Digite su dia de nacimiento DD: ";
    getline(cin, myContribuyente.day);

    // A la variable birthday se le concatena el año, mes y día.
    myContribuyente.birthdate = myContribuyente.year + myContribuyente.month + myContribuyente.day;

    cout<<"Digite su estado civil: ";
    getline(cin, myContribuyente.civilStatus);

    cout<<"En su hogar, ¿cuántas personas dependen de usted?: ";
    cin>>myContribuyente.dependents;

    // Manda a llamar a la función de la llave canónica antes de convertir a mayúsculas todo el registro.
    myContribuyente.rfc = canonicalKey(myContribuyente);
    // Función de mayúsculas.
    toUpper(myContribuyente);

    // Agrega al final de la lista.
    listaContribuyentes.push_back(myContribuyente);
}
/// TERMINA LA FUNCIÓN PARA AGREGAR DATOS.

/// INICIA LA FUNCIÓN PRINCIPAL.
int main() {
    vector<Contribuyente> listaContribuyentes;
    int myOpc;

    setlocale(LC_ALL, "Spanish");
    setlocale(LC_NUMERIC, "C");

    readFromHacienda(listaContribuyentes);
    writeSecondaryIndex(listaContribuyentes);

    do {
        system("cls");

        cout<<"1) Agregar registros."<<endl
            <<"2) Mostrar todos los registros."<<endl
            <<"3) Buscar registros."<<endl
            <<"4) Salir."<<endl<<endl

            <<"Digite una opción: ";
        cin>>myOpc;

        system("cls");

        switch(myOpc) {
        case 1: {
            insertData(listaContribuyentes);
            writeHaciendaToDisk(listaContribuyentes);
            writePrimaryIndex(listaContribuyentes);
            writeSecondaryIndex(listaContribuyentes);
        }
        break;
        case 2: {
            showData(listaContribuyentes);

            system("pause");
        }
        break;
        case 3:{
            int lmn;

            cout << "\tBUSCAR REGISTRO" << endl << endl
                 << "1) Buscar registro por llave primaria" << endl
                 << "2) Buscar registros por ciudad" << endl << endl

                 << "Selecciona una opción: ";
            cin >> lmn;

            system("cls");

            switch(lmn) {
                case 1:{
                    cout << "\tBUSCAR CONTRIBUYENTE POR RFC" << endl << endl;
                    findDataPrimaryKey(listaContribuyentes);
                    system("pause");
                }break;
                case 2:{
                    cout << "\tBUSCAR CONTRIBUYENTES POR CIUDAD" << endl << endl;
                    findDataByCity(listaContribuyentes);
                    system("pause");
                }break;
            }

            if (lmn < 1 or lmn > 2) {
                cout << "Por favor, selecciona una opción válida." << endl << endl;
                system("pause");
            }
        }break;
        case 4: {
            exit(0);
        }break;
        default:
            cout<<"\n\nOpción invalida.\n";

            system("pause");

            break;
        }
    } while(true);

    return 0;
}
/// TERMINA LA FUNCIÓN PRINCIPAL.
