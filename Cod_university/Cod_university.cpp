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
string seleccionarFacultad(); // Declaración de seleccionarFacultad
string seleccionarCarrera(const string& facultad); // Declaración de seleccionarCarrera
bool preguntarContinuar();

// Función para limpiar la pantalla
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Función para calcular la edad
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

// Función para seleccionar facultad
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
        cout << "Opción no válida. Seleccionando Finanzas Economicas por defecto.\n";
        return "Finanzas Economicas";
    }
}

// Función para seleccionar carrera
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
            cout << "Opción no válida. Seleccionando Administracion de Empresas por defecto.\n";
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
            cout << "Opción no válida. Seleccionando Ingenieria en Sistemas por defecto.\n";
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
            cout << "Opción no válida. Seleccionando Medicina General por defecto.\n";
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
        cout << "Información del Estudiante\n";
        cout << "--------------------------\n";
        cout << "Carnet: " << carnet << endl;
        cout << "Nombre: " << nombre1 << " " << nombre2 << " " << apellido1 << " " << apellido2 << endl;
        cout << "Fecha de Nacimiento: " << fechaNacimiento << endl;
        cout << "Edad: " << edad << " años" << endl;
        cout << "Carrera: " << carrera << endl;
        cout << "Semestre: " << semestre << endl;
        cout << "Sección: " << seccion << endl;
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

// Función para buscar un estudiante por carnet en todas las carreras
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

// Función para mostrar estudiantes por semestre y sección
void mostrarEstudiantesPorSemestre(const vector<unique_ptr<Estudiante>>& estudiantes, int semestre, const string& seccion) {
    cout << "---------------------------------\n";
    cout << "Estudiantes del Semestre " << semestre << ", Sección " << seccion << ":\n";
    cout << "---------------------------------\n";
    for (const auto& est : estudiantes) {
        if (est->getSeccion() == seccion && est->getSemestre() == semestre) {
            est->mostrarInfo();
        }
    }
    cout << "----------------------\n";
}

// Función para mostrar todos los estudiantes agrupados por facultad, carrera y sección
void mostrarTodosEstudiantesAgrupados(const vector<unique_ptr<Estudiante>>& estudiantes) {
    map<string, map<string, map<string, vector<Estudiante*>>>> agrupados;

    // Agrupar estudiantes por facultad, carrera y sección
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
                cout << "    Sección: " << seccion.first << "\n";
                for (const auto& est : seccion.second) {
                    est->mostrarInfo();
                }
            }
        }
    }
}

// Función para mostrar estudiantes por carrera y sección en orden alfabético
void mostrarEstudiantesPorCarreraSeccion(const vector<unique_ptr<Estudiante>>& estudiantes) {
    limpiarPantalla();
    string facultad = seleccionarFacultad();
    limpiarPantalla();
    string carrera = seleccionarCarrera(facultad);
    limpiarPantalla();
    string seccion;
    cout << "Seleccione la sección (A/B): ";
    getline(cin, seccion);
    limpiarPantalla();

    // Filtrar estudiantes por carrera y sección
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
    cout << "Estudiantes de la Carrera: " << carrera << ", Sección " << seccion << ":\n";
    cout << "---------------------------------\n";
    for (const auto& est : estudiantesFiltrados) {
        est->mostrarInfo();
    }
    cout << "----------------------\n";
}

map<string, map<int, vector<string>>> cursosPorCarrera = {
    {"Administracion de Empresas", {
        {1, {"Introducción a la Administración", "Matemáticas Básicas", "Contabilidad I", "Economía General", "Informática I"}},
        {2, {"Estadística I", "Contabilidad II", "Microeconomía", "Derecho Empresarial", "Informática II"}},
        {3, {"Administración Financiera", "Macroeconomía", "Gestión de Recursos Humanos", "Marketing I", "Informática III"}},
        {4, {"Estadística II", "Contabilidad III", "Economía Internacional", "Derecho Laboral", "Informática IV"}},
        {5, {"Gestión de Proyectos", "Finanzas Corporativas", "Comportamiento Organizacional", "Marketing II", "Informática V"}},
        {6, {"Auditoría I", "Gestión de Calidad", "Logística y Operaciones", "Investigación de Mercados", "Informática VI"}},
        {7, {"Auditoría II", "Gestión Estratégica", "Comercio Internacional", "Ética Empresarial", "Informática VII"}},
        {8, {"Sistemas de Información Gerencial", "Gestión de Riesgos", "Innovación Empresarial", "Responsabilidad Social", "Informática VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Administración", "Emprendimiento", "Liderazgo", "Informática IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigación", "Consultoría Empresarial", "Negocios Internacionales", "Informática X"}}
    }},
    {"Auditoria", {
        {1, {"Introducción a la Auditoría", "Matemáticas Básicas", "Contabilidad I", "Economía General", "Informática I"}},
        {2, {"Estadística I", "Contabilidad II", "Microeconomía", "Derecho Empresarial", "Informática II"}},
        {3, {"Auditoría I", "Macroeconomía", "Gestión de Recursos Humanos", "Marketing I", "Informática III"}},
        {4, {"Auditoría II", "Contabilidad III", "Economía Internacional", "Derecho Laboral", "Informática IV"}},
        {5, {"Auditoría III", "Finanzas Corporativas", "Comportamiento Organizacional", "Marketing II", "Informática V"}},
        {6, {"Auditoría IV", "Gestión de Calidad", "Logística y Operaciones", "Investigación de Mercados", "Informática VI"}},
        {7, {"Auditoría V", "Gestión Estratégica", "Comercio Internacional", "Ética Empresarial", "Informática VII"}},
        {8, {"Auditoría VI", "Gestión de Riesgos", "Innovación Empresarial", "Responsabilidad Social", "Informática VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Auditoría", "Emprendimiento", "Liderazgo", "Informática IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigación", "Consultoría Empresarial", "Negocios Internacionales", "Informática X"}}
    }},
    {"Ingenieria en Sistemas", {
        {1, {"Introducción a la Programación", "Matemáticas Básicas", "Física I", "Lógica de Sistemas", "Informática I"}},
        {2, {"Programación I", "Matemáticas Discretas", "Física II", "Algoritmos", "Informática II"}},
        {3, {"Programación II", "Estructuras de Datos", "Cálculo I", "Electrónica Digital", "Informática III"}},
        {4, {"Programación III", "Bases de Datos I", "Cálculo II", "Circuitos Eléctricos", "Informática IV"}},
        {5, {"Sistemas Operativos", "Bases de Datos II", "Redes de Computadoras I", "Ingeniería de Software I", "Informática V"}},
        {6, {"Arquitectura de Computadoras", "Redes de Computadoras II", "Ingeniería de Software II", "Inteligencia Artificial", "Informática VI"}},
        {7, {"Compiladores", "Sistemas Distribuidos", "Seguridad Informática", "Proyecto de Software I", "Informática VII"}},
        {8, {"Administración de Proyectos", "Sistemas Embebidos", "Cloud Computing", "Proyecto de Software II", "Informática VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Sistemas", "Big Data", "Machine Learning", "Informática IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigación", "Blockchain", "Ciberseguridad", "Informática X"}}
    }},
    {"Arquitectura de Redes", {
        {1, {"Introducción a las Redes", "Matemáticas Básicas", "Física I", "Lógica de Sistemas", "Informática I"}},
        {2, {"Redes I", "Matemáticas Discretas", "Física II", "Algoritmos", "Informática II"}},
        {3, {"Redes II", "Estructuras de Datos", "Cálculo I", "Electrónica Digital", "Informática III"}},
        {4, {"Redes III", "Bases de Datos I", "Cálculo II", "Circuitos Eléctricos", "Informática IV"}},
        {5, {"Sistemas Operativos", "Bases de Datos II", "Redes de Computadoras I", "Ingeniería de Software I", "Informática V"}},
        {6, {"Arquitectura de Computadoras", "Redes de Computadoras II", "Ingeniería de Software II", "Inteligencia Artificial", "Informática VI"}},
        {7, {"Compiladores", "Sistemas Distribuidos", "Seguridad Informática", "Proyecto de Redes I", "Informática VII"}},
        {8, {"Administración de Proyectos", "Sistemas Embebidos", "Cloud Computing", "Proyecto de Redes II", "Informática VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Redes", "Big Data", "Machine Learning", "Informática IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigación", "Blockchain", "Ciberseguridad", "Informática X"}}
    }},
    {"Medicina General", {
        {1, {"Anatomía Humana", "Biología Celular", "Química General", "Histología", "Informática Médica I"}},
        {2, {"Fisiología Humana", "Bioquímica", "Genética", "Embriología", "Informática Médica II"}},
        {3, {"Microbiología", "Patología General", "Farmacología I", "Semiología", "Informática Médica III"}},
        {4, {"Farmacología II", "Inmunología", "Parasitología", "Epidemiología", "Informática Médica IV"}},
        {5, {"Medicina Interna I", "Cirugía General", "Pediatría I", "Ginecología y Obstetricia I", "Informática Médica V"}},
        {6, {"Medicina Interna II", "Cirugía Especial", "Pediatría II", "Ginecología y Obstetricia II", "Informática Médica VI"}},
        {7, {"Medicina Familiar", "Psiquiatría", "Dermatología", "Oftalmología", "Informática Médica VII"}},
        {8, {"Medicina Legal", "Toxicología", "Radiología", "Otorrinolaringología", "Informática Médica VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Medicina", "Medicina Preventiva", "Medicina Alternativa", "Informática Médica IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigación", "Medicina de Emergencia", "Medicina Comunitaria", "Informática Médica X"}}
    }},
    {"Traumatologia", {
        {1, {"Anatomía Humana", "Biología Celular", "Química General", "Histología", "Informática Médica I"}},
        {2, {"Fisiología Humana", "Bioquímica", "Genética", "Embriología", "Informática Médica II"}},
        {3, {"Microbiología", "Patología General", "Farmacología I", "Semiología", "Informática Médica III"}},
        {4, {"Farmacología II", "Inmunología", "Parasitología", "Epidemiología", "Informática Médica IV"}},
        {5, {"Traumatología I", "Cirugía General", "Pediatría I", "Ginecología y Obstetricia I", "Informática Médica V"}},
        {6, {"Traumatología II", "Cirugía Especial", "Pediatría II", "Ginecología y Obstetricia II", "Informática Médica VI"}},
        {7, {"Traumatología III", "Psiquiatría", "Dermatología", "Oftalmología", "Informática Médica VII"}},
        {8, {"Traumatología IV", "Toxicología", "Radiología", "Otorrinolaringología", "Informática Médica VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Traumatología", "Medicina Preventiva", "Medicina Alternativa", "Informática Médica IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigación", "Medicina de Emergencia", "Medicina Comunitaria", "Informática Médica X"}}
    }},
    {"Pediatria", {
        {1, {"Anatomía Humana", "Biología Celular", "Química General", "Histología", "Informática Médica I"}},
        {2, {"Fisiología Humana", "Bioquímica", "Genética", "Embriología", "Informática Médica II"}},
        {3, {"Microbiología", "Patología General", "Farmacología I", "Semiología", "Informática Médica III"}},
        {4, {"Farmacología II", "Inmunología", "Parasitología", "Epidemiología", "Informática Médica IV"}},
        {5, {"Pediatría I", "Cirugía General", "Ginecología y Obstetricia I", "Nutrición Infantil", "Informática Médica V"}},
        {6, {"Pediatría II", "Cirugía Especial", "Ginecología y Obstetricia II", "Psicología Infantil", "Informática Médica VI"}},
        {7, {"Pediatría III", "Psiquiatría Infantil", "Dermatología Pediátrica", "Oftalmología Pediátrica", "Informática Médica VII"}},
        {8, {"Pediatría IV", "Toxicología Pediátrica", "Radiología Pediátrica", "Otorrinolaringología Pediátrica", "Informática Médica VIII"}},
        {9, {"Proyecto de Grado I", "Seminario de Pediatría", "Medicina Preventiva", "Medicina Alternativa", "Informática Médica IX"}},
        {10, {"Proyecto de Grado II", "Seminario de Investigación", "Medicina de Emergencia", "Medicina Comunitaria", "Informática Médica X"}}
    }}
};

// Función para asignar cursos a un semestre
vector<string> obtenerCursosPorSemestre(const string& carrera, int semestre) {
    if (cursosPorCarrera.find(carrera) != cursosPorCarrera.end()) {
        auto& semestres = cursosPorCarrera[carrera];
        if (semestres.find(semestre) != semestres.end()) {
            return semestres[semestre];
        }
    }
    return {};
}

// Función para preguntar si desea continuar o salir
bool preguntarContinuar() {
    char respuesta;
    cout << "¿Desea realizar otra operación? (s/n): ";
    cin >> respuesta;
    cin.ignore();
    limpiarPantalla();
    return (respuesta == 's' || respuesta == 'S');
}

// Función principal
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
                cout << "Semestre no válido. Seleccionando semestre 1 por defecto.\n";
                semestre = 1;
            }

            limpiarPantalla();
            cout << "Facultad: " << facultad << "\n";
            cout << "Carrera: " << carrera << "\n";
            cout << "Semestre: " << semestre << "\n";

            string seccion;
            cout << "Seleccione la sección (A/B): ";
            getline(cin, seccion);

            limpiarPantalla();
            cout << "Facultad: " << facultad << "\n";
            cout << "Carrera: " << carrera << "\n";
            cout << "Semestre: " << semestre << "\n";
            cout << "Sección: " << seccion << "\n";

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
            cout << "Sección: " << seccion << "\n";
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
            cout << "1. Mostrar todos los estudiantes agrupados por facultad, carrera y sección\n";
            cout << "2. Mostrar estudiantes por carrera y sección en orden alfabético\n";
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
                    cout << "¿Está seguro de que desea eliminar al estudiante con carnet " << carnet << "? (s/n): ";
                    char confirmacion;
                    cin >> confirmacion;
                    cin.ignore();
                    if (confirmacion == 's' || confirmacion == 'S') {
                        estudiantes.erase(it);
                        cout << "Estudiante eliminado correctamente.\n";
                    }
                    else {
                        cout << "Eliminación cancelada.\n";
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