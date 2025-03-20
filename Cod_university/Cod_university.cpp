#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <memory>
#include <algorithm> // Para usar sort
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

using namespace std;

// Declaraciones de funciones
void limpiarPantalla();
int calcularEdad(const string& fechaNacimiento);
string seleccionarFacultad(); // Declaraci�n de seleccionarFacultad
string seleccionarCarrera(const string& facultad); // Declaraci�n de seleccionarCarrera
bool preguntarContinuar();

// Funci�n para limpiar la pantalla
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Funci�n para calcular la edad
int calcularEdad(const string& fechaNacimiento) {
    time_t ahora = time(0);
    tm fechaActual;
    localtime_s(&fechaActual, &ahora); // Usar localtime_s en lugar de localtime

    int anioActual = fechaActual.tm_year + 1900;
    int mesActual = fechaActual.tm_mon + 1;
    int diaActual = fechaActual.tm_mday;

    int diaNacimiento = stoi(fechaNacimiento.substr(0, 2));
    int mesNacimiento = stoi(fechaNacimiento.substr(3, 2));
    int anioNacimiento = stoi(fechaNacimiento.substr(6, 4));

    int edad = anioActual - anioNacimiento;

    if (mesActual < mesNacimiento || (mesActual == mesNacimiento && diaActual < diaNacimiento)) {
        edad--;
    }

    return edad;
}

// Funci�n para seleccionar facultad
string seleccionarFacultad() {
    int opcion;
    cout << "----------\n";
    cout << "   Menu\n";
    cout << "----------\n";
    cout << "1. Finanzas Economicas\n";
    cout << "2. Sistemas de la Comunicacion\n";
    cout << "3. Medicina y Salud\n";
    cout << "Seleccione una opcion: ";
    cin >> opcion;
    cin.ignore();

    switch (opcion) {
    case 1:
        return "Finanzas Economicas";
    case 2:
        return "Sistemas de la Comunicacion";
    case 3:
        return "Medicina y Salud";
    default:
        cout << "Opci�n no v�lida. Seleccionando Finanzas Economicas por defecto.\n";
        return "Finanzas Economicas";
    }
}

// Funci�n para seleccionar carrera
string seleccionarCarrera(const string& facultad) {
    int opcion;
    cout << "----------\n";
    cout << "   Menu\n";
    cout << "----------\n";
    cout << "Facultad: " << facultad << "\n";
    if (facultad == "Finanzas Economicas") {
        cout << "1. Administracion de Empresas\n";
        cout << "2. Auditoria\n";
    }
    else if (facultad == "Sistemas de la Comunicacion") {
        cout << "1. Ingenieria en Sistemas\n";
        cout << "2. Arquitecto de Redes\n";
    }
    else if (facultad == "Medicina y Salud") {
        cout << "1. Medicina General\n";
        cout << "2. Traumatologia\n";
        cout << "3. Pediatria\n";
    }
    cout << "Seleccione una opcion: ";
    cin >> opcion;
    cin.ignore();

    if (facultad == "Finanzas Economicas") {
        switch (opcion) {
        case 1:
            return "Administracion de Empresas";
        case 2:
            return "Auditoria";
        default:
            cout << "Opci�n no v�lida. Seleccionando Administracion de Empresas por defecto.\n";
            return "Administracion de Empresas";
        }
    }
    else if (facultad == "Sistemas de la Comunicacion") {
        switch (opcion) {
        case 1:
            return "Ingenieria en Sistemas";
        case 2:
            return "Arquitecto de Redes";
        default:
            cout << "Opci�n no v�lida. Seleccionando Ingenieria en Sistemas por defecto.\n";
            return "Ingenieria en Sistemas";
        }
    }
    else if (facultad == "Medicina y Salud") {
        switch (opcion) {
        case 1:
            return "Medicina General";
        case 2:
            return "Traumatologia";
        case 3:
            return "Pediatria";
        default:
            cout << "Opci�n no v�lida. Seleccionando Medicina General por defecto.\n";
            return "Medicina General";
        }
    }
    return "";
}

// Clase Estudiante
class Estudiante {
protected:
    string carnet;
    string nombre1;
    string nombre2;
    string apellido1;
    string apellido2;
    string fechaNacimiento;
    string seccion;
    string carrera;
    int semestre;
    vector<string> cursos;

public:
    Estudiante(string carnet, string n1, string n2, string a1, string a2, string fecha, string sec, string carr, int sem)
        : carnet(carnet), nombre1(n1), nombre2(n2), apellido1(a1), apellido2(a2), fechaNacimiento(fecha), seccion(sec), carrera(carr), semestre(sem) {
    }

    void agregarCurso(const string& curso) {
        cursos.push_back(curso);
    }

    virtual void mostrarInfo() const {
        int edad = calcularEdad(fechaNacimiento);
        cout << "--------------------------\n";
        cout << "Informaci�n del Estudiante\n";
        cout << "--------------------------\n";
        cout << "Carnet: " << carnet << endl;
        cout << "Nombre: " << nombre1 << " " << nombre2 << " " << apellido1 << " " << apellido2 << endl;
        cout << "Fecha de Nacimiento: " << fechaNacimiento << endl;
        cout << "Edad: " << edad << " a�os" << endl;
        cout << "Carrera: " << carrera << endl;
        cout << "Semestre: " << semestre << endl;
        cout << "Secci�n: " << seccion << endl;
        cout << "--------------------------\n";
        cout << "Cursos:\n";
        for (size_t i = 0; i < cursos.size(); ++i) {
            cout << "  " << i + 1 << ". " << cursos[i] << endl;
        }
        cout << "----------------------\n";
    }

    string getCarnet() const {
        return carnet;
    }

    string getSeccion() const {
        return seccion;
    }

    string getCarrera() const {
        return carrera;
    }

    int getSemestre() const {
        return semestre;
    }

    const vector<string>& getCursos() const {
        return cursos;
    }

    virtual ~Estudiante() = default;
};

// Funci�n para buscar un estudiante por carnet en todas las carreras
void buscarPorCarnet(const vector<unique_ptr<Estudiante>>& estudiantes, const string& carnet) {
    bool encontrado = false;
    for (const auto& est : estudiantes) {
        if (est->getCarnet() == carnet) {
            est->mostrarInfo();
            encontrado = true;
            break;
        }
    }
    if (!encontrado) {
        cout << "No existe un estudiante con el carnet " << carnet << ".\n";
    }
}

// Funci�n para mostrar estudiantes por semestre y secci�n
void mostrarEstudiantesPorSemestre(const vector<unique_ptr<Estudiante>>& estudiantes, int semestre, const string& seccion) {
    cout << "---------------------------------\n";
    cout << "Estudiantes del Semestre " << semestre << ", Secci�n " << seccion << ":\n";
    cout << "---------------------------------\n";
    for (const auto& est : estudiantes) {
        if (est->getSeccion() == seccion && est->getSemestre() == semestre) {
            est->mostrarInfo();
        }
    }
    cout << "----------------------\n";
}

// Funci�n para mostrar todos los estudiantes agrupados por facultad, carrera y secci�n
void mostrarTodosEstudiantesAgrupados(const vector<unique_ptr<Estudiante>>& estudiantes) {
    map<string, map<string, map<string, vector<Estudiante*>>>> agrupados;

    // Agrupar estudiantes por facultad, carrera y secci�n
    for (const auto& est : estudiantes) {
        string facultad;
        if (est->getCarrera() == "Administracion de Empresas" || est->getCarrera() == "Auditoria") {
            facultad = "Finanzas Economicas";
        }
        else if (est->getCarrera() == "Ingenieria en Sistemas" || est->getCarrera() == "Arquitectura de Redes") {
            facultad = "Sistemas de la Comunicacion";
        }
        else if (est->getCarrera() == "Medicina General" || est->getCarrera() == "Traumatologia" || est->getCarrera() == "Pediatria") {
            facultad = "Medicina y Salud";
        }
        agrupados[facultad][est->getCarrera()][est->getSeccion()].push_back(est.get());
    }

    // Mostrar estudiantes agrupados
    for (const auto& facultad : agrupados) {
        cout << "Facultad: " << facultad.first << "\n";
        for (const auto& carrera : facultad.second) {
            cout << "  Carrera: " << carrera.first << "\n";
            for (const auto& seccion : carrera.second) {
                cout << "    Secci�n: " << seccion.first << "\n";
                for (const auto& est : seccion.second) {
                    est->mostrarInfo();
                }
            }
        }
    }
}

// Funci�n para mostrar estudiantes por carrera y secci�n en orden alfab�tico
void mostrarEstudiantesPorCarreraSeccion(const vector<unique_ptr<Estudiante>>& estudiantes) {
    limpiarPantalla();
    string facultad = seleccionarFacultad();
    limpiarPantalla();
    string carrera = seleccionarCarrera(facultad);
    limpiarPantalla();
    string seccion;
    cout << "Seleccione la secci�n (A/B): ";
    getline(cin, seccion);
    limpiarPantalla();

    // Filtrar estudiantes por carrera y secci�n
    vector<Estudiante*> estudiantesFiltrados;
    for (const auto& est : estudiantes) {
        if (est->getCarrera() == carrera && est->getSeccion() == seccion) {
            estudiantesFiltrados.push_back(est.get());
        }
    }

    // Ordenar estudiantes por nombre
    sort(estudiantesFiltrados.begin(), estudiantesFiltrados.end(), [](Estudiante* a, Estudiante* b) {
        return a->getCarnet() < b->getCarnet(); // Ordenar por carnet (puedes cambiar a nombre si lo prefieres)
        });

    // Mostrar estudiantes
    cout << "---------------------------------\n";
    cout << "Estudiantes de la Carrera: " << carrera << ", Secci�n " << seccion << ":\n";
    cout << "---------------------------------\n";
    for (const auto& est : estudiantesFiltrados) {
        est->mostrarInfo();
    }
    cout << "----------------------\n";
}

map<string, map<int, vector<string>>> cursosPorCarrera = {
    {"Administracion de Empresas", {
        {1, {"Introducci�n a la Administraci�n", "Matem�ticas B�sicas", "Contabilidad I", "Econom�a General", "Inform�tica I"}},
        {2, {"Estad�stica I", "Contabilidad II", "Microeconom�a", "Derecho Empresarial", "Inform�tica II"}},
        {3, {"Administraci�n Financiera", "Macroeconom�a", "Gesti�n de Recursos Humanos", "Marketing I", "Inform�tica III"}},
        {4, {"Estad�stica II", "Contabilidad III", "Econom�a Internacional", "Derecho Laboral", "Inform�tica IV"}},
        {5, {"Gesti�n de Proyectos", "Finanzas Corporativas", "Comportamiento Organizacional", "Marketing II", "Inform�tica V"}},
        {6, {"Auditor�a I", "Gesti�n de Calidad", "Log�stica y Operaciones", "Investigaci�n de Mercados", "Inform�tica VI"}},
        {7, {"Auditor�a II", "Gesti�n Estrat�gica", "Comercio Internacional", "�tica Empresarial", "Inform�tica VII"}},
        {8, {"Sistemas de Informaci�n Gerencial", "Gesti�n de Riesgos", "Innovaci�n Empresarial", "Responsabilidad Social", "Inform�tica VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Administraci�n", "Emprendimiento", "Liderazgo", "Inform�tica IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigaci�n", "Consultor�a Empresarial", "Negocios Internacionales", "Inform�tica X"}}
    }},
    {"Auditoria", {
        {1, {"Introducci�n a la Auditor�a", "Matem�ticas B�sicas", "Contabilidad I", "Econom�a General", "Inform�tica I"}},
        {2, {"Estad�stica I", "Contabilidad II", "Microeconom�a", "Derecho Empresarial", "Inform�tica II"}},
        {3, {"Auditor�a I", "Macroeconom�a", "Gesti�n de Recursos Humanos", "Marketing I", "Inform�tica III"}},
        {4, {"Auditor�a II", "Contabilidad III", "Econom�a Internacional", "Derecho Laboral", "Inform�tica IV"}},
        {5, {"Auditor�a III", "Finanzas Corporativas", "Comportamiento Organizacional", "Marketing II", "Inform�tica V"}},
        {6, {"Auditor�a IV", "Gesti�n de Calidad", "Log�stica y Operaciones", "Investigaci�n de Mercados", "Inform�tica VI"}},
        {7, {"Auditor�a V", "Gesti�n Estrat�gica", "Comercio Internacional", "�tica Empresarial", "Inform�tica VII"}},
        {8, {"Auditor�a VI", "Gesti�n de Riesgos", "Innovaci�n Empresarial", "Responsabilidad Social", "Inform�tica VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Auditor�a", "Emprendimiento", "Liderazgo", "Inform�tica IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigaci�n", "Consultor�a Empresarial", "Negocios Internacionales", "Inform�tica X"}}
    }},
    {"Ingenieria en Sistemas", {
        {1, {"Introducci�n a la Programaci�n", "Matem�ticas B�sicas", "F�sica I", "L�gica de Sistemas", "Inform�tica I"}},
        {2, {"Programaci�n I", "Matem�ticas Discretas", "F�sica II", "Algoritmos", "Inform�tica II"}},
        {3, {"Programaci�n II", "Estructuras de Datos", "C�lculo I", "Electr�nica Digital", "Inform�tica III"}},
        {4, {"Programaci�n III", "Bases de Datos I", "C�lculo II", "Circuitos El�ctricos", "Inform�tica IV"}},
        {5, {"Sistemas Operativos", "Bases de Datos II", "Redes de Computadoras I", "Ingenier�a de Software I", "Inform�tica V"}},
        {6, {"Arquitectura de Computadoras", "Redes de Computadoras II", "Ingenier�a de Software II", "Inteligencia Artificial", "Inform�tica VI"}},
        {7, {"Compiladores", "Sistemas Distribuidos", "Seguridad Inform�tica", "Proyecto de Software I", "Inform�tica VII"}},
        {8, {"Administraci�n de Proyectos", "Sistemas Embebidos", "Cloud Computing", "Proyecto de Software II", "Inform�tica VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Sistemas", "Big Data", "Machine Learning", "Inform�tica IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigaci�n", "Blockchain", "Ciberseguridad", "Inform�tica X"}}
    }},
    {"Arquitectura de Redes", {
        {1, {"Introducci�n a las Redes", "Matem�ticas B�sicas", "F�sica I", "L�gica de Sistemas", "Inform�tica I"}},
        {2, {"Redes I", "Matem�ticas Discretas", "F�sica II", "Algoritmos", "Inform�tica II"}},
        {3, {"Redes II", "Estructuras de Datos", "C�lculo I", "Electr�nica Digital", "Inform�tica III"}},
        {4, {"Redes III", "Bases de Datos I", "C�lculo II", "Circuitos El�ctricos", "Inform�tica IV"}},
        {5, {"Sistemas Operativos", "Bases de Datos II", "Redes de Computadoras I", "Ingenier�a de Software I", "Inform�tica V"}},
        {6, {"Arquitectura de Computadoras", "Redes de Computadoras II", "Ingenier�a de Software II", "Inteligencia Artificial", "Inform�tica VI"}},
        {7, {"Compiladores", "Sistemas Distribuidos", "Seguridad Inform�tica", "Proyecto de Redes I", "Inform�tica VII"}},
        {8, {"Administraci�n de Proyectos", "Sistemas Embebidos", "Cloud Computing", "Proyecto de Redes II", "Inform�tica VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Redes", "Big Data", "Machine Learning", "Inform�tica IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigaci�n", "Blockchain", "Ciberseguridad", "Inform�tica X"}}
    }},
    {"Medicina General", {
        {1, {"Anatom�a Humana", "Biolog�a Celular", "Qu�mica General", "Histolog�a", "Inform�tica M�dica I"}},
        {2, {"Fisiolog�a Humana", "Bioqu�mica", "Gen�tica", "Embriolog�a", "Inform�tica M�dica II"}},
        {3, {"Microbiolog�a", "Patolog�a General", "Farmacolog�a I", "Semiolog�a", "Inform�tica M�dica III"}},
        {4, {"Farmacolog�a II", "Inmunolog�a", "Parasitolog�a", "Epidemiolog�a", "Inform�tica M�dica IV"}},
        {5, {"Medicina Interna I", "Cirug�a General", "Pediatr�a I", "Ginecolog�a y Obstetricia I", "Inform�tica M�dica V"}},
        {6, {"Medicina Interna II", "Cirug�a Especial", "Pediatr�a II", "Ginecolog�a y Obstetricia II", "Inform�tica M�dica VI"}},
        {7, {"Medicina Familiar", "Psiquiatr�a", "Dermatolog�a", "Oftalmolog�a", "Inform�tica M�dica VII"}},
        {8, {"Medicina Legal", "Toxicolog�a", "Radiolog�a", "Otorrinolaringolog�a", "Inform�tica M�dica VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Medicina", "Medicina Preventiva", "Medicina Alternativa", "Inform�tica M�dica IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigaci�n", "Medicina de Emergencia", "Medicina Comunitaria", "Inform�tica M�dica X"}}
    }},
    {"Traumatologia", {
        {1, {"Anatom�a Humana", "Biolog�a Celular", "Qu�mica General", "Histolog�a", "Inform�tica M�dica I"}},
        {2, {"Fisiolog�a Humana", "Bioqu�mica", "Gen�tica", "Embriolog�a", "Inform�tica M�dica II"}},
        {3, {"Microbiolog�a", "Patolog�a General", "Farmacolog�a I", "Semiolog�a", "Inform�tica M�dica III"}},
        {4, {"Farmacolog�a II", "Inmunolog�a", "Parasitolog�a", "Epidemiolog�a", "Inform�tica M�dica IV"}},
        {5, {"Traumatolog�a I", "Cirug�a General", "Pediatr�a I", "Ginecolog�a y Obstetricia I", "Inform�tica M�dica V"}},
        {6, {"Traumatolog�a II", "Cirug�a Especial", "Pediatr�a II", "Ginecolog�a y Obstetricia II", "Inform�tica M�dica VI"}},
        {7, {"Traumatolog�a III", "Psiquiatr�a", "Dermatolog�a", "Oftalmolog�a", "Inform�tica M�dica VII"}},
        {8, {"Traumatolog�a IV", "Toxicolog�a", "Radiolog�a", "Otorrinolaringolog�a", "Inform�tica M�dica VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Traumatolog�a", "Medicina Preventiva", "Medicina Alternativa", "Inform�tica M�dica IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigaci�n", "Medicina de Emergencia", "Medicina Comunitaria", "Inform�tica M�dica X"}}
    }},
    {"Pediatria", {
        {1, {"Anatom�a Humana", "Biolog�a Celular", "Qu�mica General", "Histolog�a", "Inform�tica M�dica I"}},
        {2, {"Fisiolog�a Humana", "Bioqu�mica", "Gen�tica", "Embriolog�a", "Inform�tica M�dica II"}},
        {3, {"Microbiolog�a", "Patolog�a General", "Farmacolog�a I", "Semiolog�a", "Inform�tica M�dica III"}},
        {4, {"Farmacolog�a II", "Inmunolog�a", "Parasitolog�a", "Epidemiolog�a", "Inform�tica M�dica IV"}},
        {5, {"Pediatr�a I", "Cirug�a General", "Ginecolog�a y Obstetricia I", "Nutrici�n Infantil", "Inform�tica M�dica V"}},
        {6, {"Pediatr�a II", "Cirug�a Especial", "Ginecolog�a y Obstetricia II", "Psicolog�a Infantil", "Inform�tica M�dica VI"}},
        {7, {"Pediatr�a III", "Psiquiatr�a Infantil", "Dermatolog�a Pedi�trica", "Oftalmolog�a Pedi�trica", "Inform�tica M�dica VII"}},
        {8, {"Pediatr�a IV", "Toxicolog�a Pedi�trica", "Radiolog�a Pedi�trica", "Otorrinolaringolog�a Pedi�trica", "Inform�tica M�dica VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Pediatr�a", "Medicina Preventiva", "Medicina Alternativa", "Inform�tica M�dica IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigaci�n", "Medicina de Emergencia", "Medicina Comunitaria", "Inform�tica M�dica X"}}
    }}
};

// Funci�n para asignar cursos a un semestre
vector<string> obtenerCursosPorSemestre(const string& carrera, int semestre) {
    if (cursosPorCarrera.find(carrera) != cursosPorCarrera.end()) {
        auto& semestres = cursosPorCarrera[carrera];
        if (semestres.find(semestre) != semestres.end()) {
            return semestres[semestre];
        }
    }
    return {};
}

// Funci�n para preguntar si desea continuar o salir
bool preguntarContinuar() {
    char respuesta;
    cout << "�Desea realizar otra operaci�n? (s/n): ";
    cin >> respuesta;
    cin.ignore();
    limpiarPantalla();
    return (respuesta == 's' || respuesta == 'S');
}

// Funci�n principal
void gestionarEstudiantes() {
    vector<unique_ptr<Estudiante>> estudiantes;
    int opcion;

    do {
        cout << "------------\n";
        cout << "   Menu\n";
        cout << "------------\n";
        cout << "1. Asignar estudiante\n";
        cout << "2. Mostrar estudiantes\n";
        cout << "3. Buscar estudiante mediante Carnet\n";
        cout << "4. Eliminar Estudiante\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 1) {
            limpiarPantalla();
            string facultad = seleccionarFacultad();
            limpiarPantalla();
            cout << "Facultad: " << facultad << "\n";
            string carrera = seleccionarCarrera(facultad);
            limpiarPantalla();
            cout << "Facultad: " << facultad << "\n";
            cout << "Carrera: " << carrera << "\n";

            int semestre;
            cout << "-------------------\n";
            cout << "Menu de Asignacion\n";
            cout << "-------------------\n";
            cout << "1. Semestre I\n";
            cout << "2. Semestre II\n";
            cout << "3. Semestre III\n";
            cout << "4. Semestre IV\n";
            cout << "5. Semestre V\n";
            cout << "6. Semestre VI\n";
            cout << "7. Semestre VII\n";
            cout << "8. Semestre VIII\n";
            cout << "9. Semestre IX\n";
            cout << "10. Semestre X\n";
            cout << "Seleccione el semestre: ";
            cin >> semestre;
            cin.ignore();

            if (semestre < 1 || semestre > 10) {
                cout << "Semestre no v�lido. Seleccionando semestre 1 por defecto.\n";
                semestre = 1;
            }

            limpiarPantalla();
            cout << "Facultad: " << facultad << "\n";
            cout << "Carrera: " << carrera << "\n";
            cout << "Semestre: " << semestre << "\n";

            string seccion;
            cout << "Seleccione la secci�n (A/B): ";
            getline(cin, seccion);

            limpiarPantalla();
            cout << "Facultad: " << facultad << "\n";
            cout << "Carrera: " << carrera << "\n";
            cout << "Semestre: " << semestre << "\n";
            cout << "Secci�n: " << seccion << "\n";

            string carnet, n1, n2, a1, a2, fecha;
            cout << "Carnet (ejemplo: 2294-24-19629): ";
            getline(cin, carnet);
            cout << "Primer nombre: ";
            getline(cin, n1);
            cout << "Segundo nombre: ";
            getline(cin, n2);
            cout << "Primer apellido: ";
            getline(cin, a1);
            cout << "Segundo apellido: ";
            getline(cin, a2);
            cout << "Fecha de nacimiento (DD-MM-AAAA): ";
            getline(cin, fecha);

            auto est = make_unique<Estudiante>(carnet, n1, n2, a1, a2, fecha, seccion, carrera, semestre);
            vector<string> cursos = obtenerCursosPorSemestre(carrera, semestre);
            for (const string& curso : cursos) {
                est->agregarCurso(curso);
            }
            estudiantes.push_back(std::move(est));
            limpiarPantalla();
            cout << "Facultad: " << facultad << "\n";
            cout << "Carrera: " << carrera << "\n";
            cout << "Semestre: " << semestre << "\n";
            cout << "Secci�n: " << seccion << "\n";
            cout << "Estudiante asignado correctamente.\n";

            if (!preguntarContinuar()) {
                break;
            }
        }
        else if (opcion == 2) {
            limpiarPantalla();
            int subOpcion;
            cout << "--------\n";
            cout << "  Menu\n";
            cout << "--------\n";
            cout << "1. Mostrar todos los estudiantes agrupados por facultad, carrera y secci�n\n";
            cout << "2. Mostrar estudiantes por carrera y secci�n en orden alfab�tico\n";
            cout << "Seleccione una opcion: ";
            cin >> subOpcion;
            cin.ignore();

            if (subOpcion == 1) {
                mostrarTodosEstudiantesAgrupados(estudiantes);
            }
            else if (subOpcion == 2) {
                mostrarEstudiantesPorCarreraSeccion(estudiantes);
            }

            if (!preguntarContinuar()) {
                break;
            }
        }
        else if (opcion == 3) {
            limpiarPantalla();
            string carnet;
            cout << "Ingrese el carnet a buscar: ";
            getline(cin, carnet);
            limpiarPantalla();
            buscarPorCarnet(estudiantes, carnet);

            if (!preguntarContinuar()) {
                break;
            }
        }
        else if (opcion == 4) {
            limpiarPantalla();
            string carnet;
            cout << "Ingrese el carnet del estudiante a eliminar: ";
            getline(cin, carnet);
            limpiarPantalla();
            for (auto it = estudiantes.begin(); it != estudiantes.end(); ++it) {
                if ((*it)->getCarnet() == carnet) {
                    cout << "�Est� seguro de que desea eliminar al estudiante con carnet " << carnet << "? (s/n): ";
                    char confirmacion;
                    cin >> confirmacion;
                    cin.ignore();
                    if (confirmacion == 's' || confirmacion == 'S') {
                        estudiantes.erase(it);
                        cout << "Estudiante eliminado correctamente.\n";
                    }
                    else {
                        cout << "Eliminaci�n cancelada.\n";
                    }
                    break;
                }
            }

            if (!preguntarContinuar()) {
                break;
            }
        }
    } while (opcion != 5);
}

int main() {
    gestionarEstudiantes();
    return 0;
}