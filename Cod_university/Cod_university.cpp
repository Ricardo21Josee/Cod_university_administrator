#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <memory>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <libpq-fe.h>

using namespace std;

// Mapeo de carreras a codigos
map<string, string> codigosCarrera = {
    {"Administracion de Empresas", "1010"},
    {"Auditoria", "1020"},
    {"Ingenieria en Sistemas", "1030"},
    {"Arquitectura de Redes", "1040"},
    {"Medicina General", "1050"},
    {"Traumatologia", "1060"},
    {"Pediatria", "1070"}
};

class DatabaseManager {
private:
    PGconn* conn;

    void verificarConexion() {
        if (PQstatus(conn) != CONNECTION_OK) {
            PQreset(conn);
            if (PQstatus(conn) != CONNECTION_OK) {
                throw runtime_error("Error reconectando a PostgreSQL: " + string(PQerrorMessage(conn)));
            }
        }
    }

public:
    DatabaseManager() {
        string conn_str = "host=# dbname=# user=# "
            "password=# port=# connect_timeout=#";

        conn = PQconnectdb(conn_str.c_str());

        if (PQstatus(conn) != CONNECTION_OK) {
            string err_msg = "Error de conexion: " + string(PQerrorMessage(conn));
            PQfinish(conn);
            throw runtime_error(err_msg);
        }

        cout << "Conexion exitosa a PostgreSQL" << endl;
        cout << "Version servidor: " << PQserverVersion(conn) << endl;
    }

    ~DatabaseManager() {
        if (conn) {
            PQfinish(conn);
            cout << "Conexion cerrada correctamente" << endl;
        }
    }

    PGconn* getConnection() {
        verificarConexion();
        return conn;
    }

    void verificarEstructuraBD() {
        const char* queries[] = {
            "SELECT 1 FROM pg_tables WHERE tablename = 'estudiantes'",
            "SELECT 1 FROM pg_tables WHERE tablename = 'carreras'",
            "SELECT 1 FROM pg_tables WHERE tablename = 'cursos'"
        };

        for (const char* query : queries) {
            PGresult* res = PQexec(conn, query);
            if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
                PQclear(res);
                throw runtime_error("Error en estructura de BD: Tabla requerida no encontrada");
            }
            PQclear(res);
        }
    }

    string generarCarnet(const string& carrera) {
        verificarConexion();

        if (codigosCarrera.find(carrera) == codigosCarrera.end()) {
            throw invalid_argument("Carrera no reconocida para generacion de carnet");
        }

        string codigo = codigosCarrera[carrera];
        time_t ahora = time(nullptr);
        tm tm_ahora;
        localtime_s(&tm_ahora, &ahora);
        int anio_actual = tm_ahora.tm_year + 1900;
        string anio_str = to_string(anio_actual).substr(2);

        string query = "SELECT carnet FROM estudiantes "
            "WHERE carnet LIKE $1 || '-' || $2 || '-%' "
            "ORDER BY carnet DESC LIMIT 1";

        const char* params[2];
        params[0] = codigo.c_str();
        params[1] = anio_str.c_str();

        PGresult* res = PQexecParams(conn, query.c_str(), 2, NULL, params, NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            string err = "Error generando carnet: " + string(PQerrorMessage(conn));
            PQclear(res);
            throw runtime_error(err);
        }

        int consecutivo = 1;

        if (PQntuples(res) > 0) {
            string ultimo_carnet = PQgetvalue(res, 0, 0);
            string consecutivo_str = ultimo_carnet.substr(8, 5);
            try {
                consecutivo = stoi(consecutivo_str) + 1;
            }
            catch (...) {
            }
        }

        PQclear(res);

        if (consecutivo <= 0 || consecutivo > 99999) {
            throw runtime_error("Numero consecutivo invalido generado: " + to_string(consecutivo));
        }

        char consecutivo_str[6];
        snprintf(consecutivo_str, sizeof(consecutivo_str), "%05d", consecutivo);

        return codigo + "-" + anio_str + "-" + consecutivo_str;
    }

    void registrarEstudiante(const string& carnet, const string& nombre1, const string& nombre2,
        const string& apellido1, const string& apellido2, const string& fechaNacimiento,
        const string& carrera, int semestre, const string& seccion,
        const vector<string>& cursos) {
        verificarConexion();

        if (carnet.empty() || nombre1.empty() || apellido1.empty() ||
            fechaNacimiento.size() != 10 || fechaNacimiento[2] != '-' || fechaNacimiento[5] != '-' ||
            carrera.empty() || seccion.empty()) {
            throw invalid_argument("Datos del estudiante incompletos o invalidos");
        }

        if (semestre < 1 || semestre > 10) {
            throw invalid_argument("Semestre debe estar entre 1 y 10");
        }

        string fecha_pg = fechaNacimiento.substr(6, 4) + "-" +
            fechaNacimiento.substr(3, 2) + "-" +
            fechaNacimiento.substr(0, 2);

        vector<string> param_values = {
            carnet,
            nombre1,
            nombre2.empty() ? "" : nombre2,
            apellido1,
            apellido2.empty() ? "" : apellido2,
            fecha_pg,
            carrera,
            to_string(semestre),
            seccion
        };

        vector<const char*> params;
        for (const auto& val : param_values) {
            params.push_back(val.c_str());
        }

        const char* query =
            "INSERT INTO estudiantes (carnet, primer_nombre, segundo_nombre, "
            "primer_apellido, segundo_apellido, fecha_nacimiento, id_carrera, "
            "semestre_actual, seccion) VALUES ($1, $2, $3, $4, $5, $6, "
            "(SELECT id_carrera FROM carreras WHERE nombre = $7), $8::integer, $9) "
            "RETURNING id_estudiante";

        PGresult* res = PQexecParams(conn, query, 9, NULL, params.data(), NULL, NULL, 0);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            string err = "Error al insertar estudiante: " + string(PQerrorMessage(conn));
            PQclear(res);
            throw runtime_error(err);
        }

        string id_estudiante = PQgetvalue(res, 0, 0);
        PQclear(res);

        // Obtener el ID de la carrera del estudiante
        string carrera_id_query = "SELECT id_carrera FROM carreras WHERE nombre = $1";
        const char* carrera_param = carrera.c_str();
        PGresult* res_carrera = PQexecParams(conn, carrera_id_query.c_str(), 1, NULL, &carrera_param, NULL, NULL, 0);
        if (PQresultStatus(res_carrera) != PGRES_TUPLES_OK || PQntuples(res_carrera) == 0) {
            string err = "Error obteniendo ID de carrera: " + string(PQerrorMessage(conn));
            PQclear(res_carrera);
            throw runtime_error(err);
        }
        string id_carrera = PQgetvalue(res_carrera, 0, 0);
        PQclear(res_carrera);

        for (const auto& curso : cursos) {
            vector<string> curso_param_values = {
                id_estudiante,
                curso,
                id_carrera,
                to_string(semestre)
            };

            vector<const char*> curso_params;
            for (const auto& val : curso_param_values) {
                curso_params.push_back(val.c_str());
            }

            const char* curso_query =
                "INSERT INTO matriculacion (id_estudiante, id_curso, semestre) "
                "VALUES ($1, (SELECT id_curso FROM cursos WHERE nombre = $2 AND id_carrera = $3), $4::integer)";

            PGresult* res_curso = PQexecParams(conn, curso_query, 4, NULL, curso_params.data(), NULL, NULL, 0);
            if (PQresultStatus(res_curso) != PGRES_COMMAND_OK) {
                string err = "Error al insertar curso '" + curso + "': " + string(PQerrorMessage(conn));
                PQclear(res_curso);
                throw runtime_error(err);
            }
            PQclear(res_curso);
        }
    }

    vector<map<string, string>> obtenerTodosEstudiantes() {
        verificarConexion();

        PGresult* res = PQexec(conn,
            "SELECT e.carnet, e.primer_nombre, COALESCE(e.segundo_nombre, ''), "
            "e.primer_apellido, COALESCE(e.segundo_apellido, ''), "
            "TO_CHAR(e.fecha_nacimiento, 'DD-MM-YYYY'), "
            "EXTRACT(YEAR FROM age(e.fecha_nacimiento))::int, "
            "c.nombre as carrera, e.semestre_actual, e.seccion "
            "FROM estudiantes e JOIN carreras c ON e.id_carrera = c.id_carrera "
            "ORDER BY e.primer_apellido, e.primer_nombre");

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            string err = "Error obteniendo estudiantes: " + string(PQerrorMessage(conn));
            PQclear(res);
            throw runtime_error(err);
        }

        vector<map<string, string>> estudiantes;
        int rows = PQntuples(res);

        for (int i = 0; i < rows; i++) {
            map<string, string> estudiante;
            estudiante["carnet"] = PQgetvalue(res, i, 0);
            estudiante["nombre1"] = PQgetvalue(res, i, 1);
            estudiante["nombre2"] = PQgetvalue(res, i, 2);
            estudiante["apellido1"] = PQgetvalue(res, i, 3);
            estudiante["apellido2"] = PQgetvalue(res, i, 4);
            estudiante["fechaNacimiento"] = PQgetvalue(res, i, 5);
            estudiante["edad"] = PQgetvalue(res, i, 6);
            estudiante["carrera"] = PQgetvalue(res, i, 7);
            estudiante["semestre"] = PQgetvalue(res, i, 8);
            estudiante["seccion"] = PQgetvalue(res, i, 9);

            estudiantes.push_back(estudiante);
        }

        PQclear(res);
        return estudiantes;
    }

    map<string, string> buscarEstudiante(const string& carnet) {
        verificarConexion();

        string param_value = carnet;
        const char* param = param_value.c_str();

        PGresult* res = PQexecParams(conn,
            "SELECT e.carnet, e.primer_nombre, COALESCE(e.segundo_nombre, ''), "
            "e.primer_apellido, COALESCE(e.segundo_apellido, ''), "
            "TO_CHAR(e.fecha_nacimiento, 'DD-MM-YYYY'), "
            "EXTRACT(YEAR FROM age(e.fecha_nacimiento))::int, "
            "c.nombre as carrera, e.semestre_actual, e.seccion "
            "FROM estudiantes e JOIN carreras c ON e.id_carrera = c.id_carrera "
            "WHERE e.carnet = $1",
            1, NULL, &param, NULL, NULL, 0);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            string err = "Error buscando estudiante: " + string(PQerrorMessage(conn));
            PQclear(res);
            throw runtime_error(err);
        }

        if (PQntuples(res) == 0) {
            PQclear(res);
            return {};
        }

        map<string, string> estudiante;
        estudiante["carnet"] = PQgetvalue(res, 0, 0);
        estudiante["nombre1"] = PQgetvalue(res, 0, 1);
        estudiante["nombre2"] = PQgetvalue(res, 0, 2);
        estudiante["apellido1"] = PQgetvalue(res, 0, 3);
        estudiante["apellido2"] = PQgetvalue(res, 0, 4);
        estudiante["fechaNacimiento"] = PQgetvalue(res, 0, 5);
        estudiante["edad"] = PQgetvalue(res, 0, 6);
        estudiante["carrera"] = PQgetvalue(res, 0, 7);
        estudiante["semestre"] = PQgetvalue(res, 0, 8);
        estudiante["seccion"] = PQgetvalue(res, 0, 9);

        PQclear(res);
        return estudiante;
    }

    bool eliminarEstudiante(const string& carnet) {
        verificarConexion();

        string param_value = carnet;
        const char* param = param_value.c_str();

        PGresult* res = PQexecParams(conn,
            "DELETE FROM estudiantes WHERE carnet = $1 RETURNING carnet",
            1, NULL, &param, NULL, NULL, 0);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            string err = "Error eliminando estudiante: " + string(PQerrorMessage(conn));
            PQclear(res);
            throw runtime_error(err);
        }

        bool eliminado = (PQntuples(res) > 0);
        PQclear(res);
        return eliminado;
    }
};

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int calcularEdad(const string& fechaNacimiento) {
    if (fechaNacimiento.size() != 10 || fechaNacimiento[2] != '-' || fechaNacimiento[5] != '-') {
        throw invalid_argument("Formato de fecha invalido. Use DD-MM-AAAA");
    }

    int dia = stoi(fechaNacimiento.substr(0, 2));
    int mes = stoi(fechaNacimiento.substr(3, 2));
    int anio = stoi(fechaNacimiento.substr(6, 4));

    time_t ahora = time(nullptr);
    tm tm_ahora;
    localtime_s(&tm_ahora, &ahora);
    int edad = tm_ahora.tm_year + 1900 - anio;

    if (tm_ahora.tm_mon + 1 < mes || (tm_ahora.tm_mon + 1 == mes && tm_ahora.tm_mday < dia)) {
        edad--;
    }

    return edad > 0 ? edad : 0;
}

string seleccionarFacultad() {
    limpiarPantalla();
    cout << "==========================\n"
        << "      MENU FACULTADES     \n"
        << "==========================\n"
        << "1. Finanzas Economicas\n"
        << "2. Sistemas de la Comunicacion\n"
        << "3. Medicina y Salud\n"
        << "==========================\n"
        << "Seleccione una opcion: ";

    int opcion;
    while (!(cin >> opcion) || opcion < 1 || opcion > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Opcion no valida. Intente nuevamente: ";
    }
    cin.ignore();

    switch (opcion) {
    case 1: return "Finanzas Economicas";
    case 2: return "Sistemas de la Comunicacion";
    case 3: return "Medicina y Salud";
    default: return "Finanzas Economicas";
    }
}

string seleccionarCarrera(const string& facultad) {
    limpiarPantalla();
    cout << "==========================\n"
        << "FACULTAD: " << facultad << "\n"
        << "      MENU CARRERAS      \n"
        << "==========================\n";

    vector<string> carreras;
    if (facultad == "Finanzas Economicas") {
        cout << "1. Administracion de Empresas\n"
            << "2. Auditoria\n";
        carreras = { "Administracion de Empresas", "Auditoria" };
    }
    else if (facultad == "Sistemas de la Comunicacion") {
        cout << "1. Ingenieria en Sistemas\n"
            << "2. Arquitectura de Redes\n";
        carreras = { "Ingenieria en Sistemas", "Arquitectura de Redes" };
    }
    else if (facultad == "Medicina y Salud") {
        cout << "1. Medicina General\n"
            << "2. Traumatologia\n"
            << "3. Pediatria\n";
        carreras = { "Medicina General", "Traumatologia", "Pediatria" };
    }

    cout << "==========================\n"
        << "Seleccione una opcion: ";

    int opcion;
    while (!(cin >> opcion) || opcion < 1 || opcion > carreras.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Opcion no valida. Intente nuevamente: ";
    }
    cin.ignore();

    return carreras[opcion - 1];
}

bool preguntarContinuar() {
    cout << "\nDesea realizar otra operacion? (s/n): ";
    char respuesta;
    cin >> respuesta;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return tolower(respuesta) == 's';
}

vector<string> obtenerCursosPorSemestre(const string& carrera, int semestre) {
    static const map<string, map<int, vector<string>>> cursosPorCarrera = {
        {"Administracion de Empresas", {
            {1, {"Introduccion a la Administracion", "Matematicas Basicas", "Contabilidad I", "Economia General", "Informatica I"}},
            {2, {"Estadistica I", "Contabilidad II", "Microeconomia", "Derecho Empresarial", "Informatica II"}},
            {3, {"Administracion Financiera", "Macroeconomia", "Gestion de Recursos Humanos", "Marketing I", "Informatica III"}},
            {4, {"Estadistica II", "Contabilidad III", "Economia Internacional", "Derecho Laboral", "Informatica IV"}},
            {5, {"Gestion de Proyectos", "Finanzas Corporativas", "Comportamiento Organizacional", "Marketing II", "Informatica V"}},
            {6, {"Auditoria I", "Gestion de Calidad", "Logistica y Operaciones", "Investigacion de Mercados", "Informatica VI"}},
            {7, {"Auditoria II", "Gestion Estrategica", "Comercio Internacional", "Etica Empresarial", "Informatica VII"}},
            {8, {"Sistemas de Informacion Gerencial", "Gestion de Riesgos", "Innovacion Empresarial", "Responsabilidad Social", "Informatica VIII"}},
            {9, {"Proyecto de Grado I", "Seminario de Administracion", "Emprendimiento", "Liderazgo", "Informatica IX"}},
            {10, {"Proyecto de Grado II", "Seminario de Investigacion", "Consultoria Empresarial", "Negocios Internacionales", "Informatica X"}}
        }},
        {"Auditoria", {
            {1, {"Introduccion a la Auditoria", "Matematicas Basicas", "Contabilidad I", "Economia General", "Informatica I"}},
            {2, {"Estadistica I", "Contabilidad II", "Microeconomia", "Derecho Empresarial", "Informatica II"}},
            {3, {"Auditoria I", "Macroeconomia", "Gestion de Recursos Humanos", "Marketing I", "Informatica III"}},
            {4, {"Auditoria II", "Contabilidad III", "Economia Internacional", "Derecho Laboral", "Informatica IV"}},
            {5, {"Auditoria III", "Finanzas Corporativas", "Comportamiento Organizacional", "Marketing II", "Informatica V"}},
            {6, {"Auditoria IV", "Gestion de Calidad", "Logistica y Operaciones", "Investigacion de Mercados", "Informatica VI"}},
            {7, {"Auditoria V", "Gestion Estrategica", "Comercio Internacional", "Etica Empresarial", "Informatica VII"}},
            {8, {"Auditoria VI", "Gestion de Riesgos", "Innovacion Empresarial", "Responsabilidad Social", "Informatica VIII"}},
            {9, {"Proyecto de Grado I", "Seminario de Auditoria", "Emprendimiento", "Liderazgo", "Informatica IX"}},
            {10, {"Proyecto de Grado II", "Seminario de Investigacion", "Consultoria Empresarial", "Negocios Internacionales", "Informatica X"}}
        }},
        {"Ingenieria en Sistemas", {
            {1, {"Introduccion a la Programacion", "Matematicas Basicas", "Fisica I", "Logica de Sistemas", "Informatica I"}},
            {2, {"Programacion I", "Matematicas Discretas", "Fisica II", "Algoritmos", "Informatica II"}},
            {3, {"Programacion II", "Estructuras de Datos", "Calculo I", "Electronica Digital", "Informatica III"}},
            {4, {"Programacion III", "Bases de Datos I", "Calculo II", "Circuitos Electricos", "Informatica IV"}},
            {5, {"Sistemas Operativos", "Bases de Datos II", "Redes de Computadoras I", "Ingenieria de Software I", "Informatica V"}},
            {6, {"Arquitectura de Computadoras", "Redes de Computadoras II", "Ingenieria de Software II", "Inteligencia Artificial", "Informatica VI"}},
            {7, {"Compiladores", "Sistemas Distribuidos", "Seguridad Informatica", "Proyecto de Software I", "Informatica VII"}},
            {8, {"Administracion de Proyectos", "Sistemas Embebidos", "Cloud Computing", "Proyecto de Software II", "Informatica VIII"}},
            {9, {"Proyecto de Grado I", "Seminario de Sistemas", "Big Data", "Machine Learning", "Informatica IX"}},
            {10, {"Proyecto de Grado II", "Seminario de Investigacion", "Blockchain", "Ciberseguridad", "Informatica X"}}
        }},
        {"Arquitectura de Redes", {
            {1, {"Introduccion a las Redes", "Matematicas Basicas", "Fisica I", "Logica de Sistemas", "Informatica I"}},
            {2, {"Redes I", "Matematicas Discretas", "Fisica II", "Algoritmos", "Informatica II"}},
            {3, {"Redes II", "Estructuras de Datos", "Calculo I", "Electronica Digital", "Informatica III"}},
            {4, {"Redes III", "Bases de Datos I", "Calculo II", "Circuitos Electricos", "Informatica IV"}},
            {5, {"Sistemas Operativos", "Bases de Datos II", "Redes de Computadoras I", "Ingenieria de Software I", "Informatica V"}},
            {6, {"Arquitectura de Computadoras", "Redes de Computadoras II", "Ingenieria de Software II", "Inteligencia Artificial", "Informatica VI"}},
            {7, {"Compiladores", "Sistemas Distribuidos", "Seguridad Informatica", "Proyecto de Redes I", "Informatica VII"}},
            {8, {"Administracion de Proyectos", "Sistemas Embebidos", "Cloud Computing", "Proyecto de Redes II", "Informatica VIII"}},
            {9, {"Proyecto de Grado I", "Seminario de Redes", "Big Data", "Machine Learning", "Informatica IX"}},
            {10, {"Proyecto de Grado II", "Seminario de Investigacion", "Blockchain", "Ciberseguridad", "Informatica X"}}
        }},
        {"Medicina General", {
            {1, {"Anatomia Humana", "Biologia Celular", "Quimica General", "Histologia", "Informatica Medica I"}},
            {2, {"Fisiologia Humana", "Bioquimica", "Genetica", "Embriologia", "Informatica Medica II"}},
            {3, {"Microbiologia", "Patologia General", "Farmacologia I", "Semiologia", "Informatica Medica III"}},
            {4, {"Farmacologia II", "Inmunologia", "Parasitologia", "Epidemiologia", "Informatica Medica IV"}},
            {5, {"Medicina Interna I", "Cirugia General", "Pediatria I", "Ginecologia y Obstetricia I", "Informatica Medica V"}},
            {6, {"Medicina Interna II", "Cirugia Especial", "Pediatria II", "Ginecologia y Obstetricia II", "Informatica Medica VI"}},
            {7, {"Medicina Familiar", "Psiquiatria", "Dermatologia", "Oftalmologia", "Informatica Medica VII"}},
            {8, {"Medicina Legal", "Toxicologia", "Radiologia", "Otorrinolaringologia", "Informatica Medica VIII"}},
            {9, {"Proyecto de Grado I", "Seminario de Medicina", "Medicina Preventiva", "Medicina Alternativa", "Informatica Medica IX"}},
            {10, {"Proyecto de Grado II", "Seminario de Investigacion", "Medicina de Emergencia", "Medicina Comunitaria", "Informatica Medica X"}}
        }},
        {"Traumatologia", {
            {1, {"Anatomia Humana", "Biologia Celular", "Quimica General", "Histologia", "Informatica Medica I"}},
            {2, {"Fisiologia Humana", "Bioquimica", "Genetica", "Embriologia", "Informatica Medica II"}},
            {3, {"Microbiologia", "Patologia General", "Farmacologia I", "Semiologia", "Informatica Medica III"}},
            {4, {"Farmacologia II", "Inmunologia", "Parasitologia", "Epidemiologia", "Informatica Medica IV"}},
            {5, {"Traumatologia I", "Cirugia General", "Pediatria I", "Ginecologia y Obstetricia I", "Informatica Medica V"}},
            {6, {"Traumatologia II", "Cirugia Especial", "Pediatria II", "Ginecologia y Obstetricia II", "Informatica Medica VI"}},
            {7, {"Traumatologia III", "Psiquiatria", "Dermatologia", "Oftalmologia", "Informatica Medica VII"}},
            {8, {"Traumatologia IV", "Toxicologia", "Radiologia", "Otorrinolaringologia", "Informatica Medica VIII"}},
            {9, {"Proyecto de Grado I", "Seminario de Traumatologia", "Medicina Preventiva", "Medicina Alternativa", "Informatica Medica IX"}},
            {10, {"Proyecto de Grado II", "Seminario de Investigacion", "Medicina de Emergencia", "Medicina Comunitaria", "Informatica Medica X"}}
        }},
        {"Pediatria", {
            {1, {"Anatomia Humana", "Biologia Celular", "Quimica General", "Histologia", "Informatica Medica I"}},
            {2, {"Fisiologia Humana", "Bioquimica", "Genetica", "Embriologia", "Informatica Medica II"}},
            {3, {"Microbiologia", "Patologia General", "Farmacologia I", "Semiologia", "Informatica Medica III"}},
            {4, {"Farmacologia II", "Inmunologia", "Parasitologia", "Epidemiologia", "Informatica Medica IV"}},
            {5, {"Pediatria I", "Cirugia General", "Ginecologia y Obstetricia I", "Nutricion Infantil", "Informatica Medica V"}},
            {6, {"Pediatria II", "Cirugia Especial", "Ginecologia y Obstetricia II", "Psicologia Infantil", "Informatica Medica VI"}},
            {7, {"Pediatria III", "Psiquiatria Infantil", "Dermatologia Pediatrica", "Oftalmologia Pediatrica", "Informatica Medica VII"}},
            {8, {"Pediatria IV", "Toxicologia Pediatrica", "Radiologia Pediatrica", "Otorrinolaringologia Pediatrica", "Informatica Medica VIII"}},
            {9, {"Proyecto de Grado I", "Seminario de Pediatria", "Medicina Preventiva", "Medicina Alternativa", "Informatica Medica IX"}},
            {10, {"Proyecto de Grado II", "Seminario de Investigacion", "Medicina de Emergencia", "Medicina Comunitaria", "Informatica Medica X"}}
        }}
    };

    auto it_carrera = cursosPorCarrera.find(carrera);
    if (it_carrera != cursosPorCarrera.end()) {
        auto it_semestre = it_carrera->second.find(semestre);
        if (it_semestre != it_carrera->second.end()) {
            return it_semestre->second;
        }
    }
    return {};
}

void mostrarMenuPrincipal() {
    limpiarPantalla();
    cout << "================================\n"
        << " SISTEMA DE GESTION UNIVERSITARIA\n"
        << "================================\n"
        << "1. Registrar nuevo estudiante\n"
        << "2. Mostrar todos los estudiantes\n"
        << "3. Buscar estudiante por carnet\n"
        << "4. Eliminar estudiante\n"
        << "5. Probar conexion con PostgreSQL\n"
        << "6. Salir\n"
        << "================================\n"
        << "Seleccione una opcion: ";
}

void registrarEstudiante(DatabaseManager& db) {
    try {
        string facultad = seleccionarFacultad();
        string carrera = seleccionarCarrera(facultad);

        limpiarPantalla();
        cout << "==========================\n"
            << "      SELECCION SEMESTRE   \n"
            << "==========================\n";
        for (int i = 1; i <= 10; i++) {
            cout << i << ". Semestre " << i << endl;
        }
        cout << "==========================\n"
            << "Seleccione el semestre: ";

        int semestre;
        while (!(cin >> semestre) || semestre < 1 || semestre > 10) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Semestre invalido. Intente nuevamente (1-10): ";
        }
        cin.ignore();

        limpiarPantalla();
        cout << "==========================\n"
            << "      SELECCION SECCION    \n"
            << "==========================\n"
            << "Ingrese la seccion (A/B/C): ";
        string seccion;
        getline(cin, seccion);
        while (seccion.empty() || (toupper(seccion[0]) != 'A' && toupper(seccion[0]) != 'B' && toupper(seccion[0]) != 'C')) {
            cout << "Seccion invalida. Ingrese A, B o C: ";
            getline(cin, seccion);
        }
        seccion = string(1, toupper(seccion[0]));

        limpiarPantalla();
        cout << "==========================\n"
            << "  REGISTRO DE ESTUDIANTE  \n"
            << "==========================\n";

        string carnet = db.generarCarnet(carrera);
        cout << "Carnet generado: " << carnet << endl;

        cout << "Primer nombre: ";
        string n1;
        getline(cin, n1);
        if (n1.empty()) throw invalid_argument("El primer nombre es obligatorio");

        cout << "Segundo nombre (opcional): ";
        string n2;
        getline(cin, n2);

        cout << "Primer apellido: ";
        string a1;
        getline(cin, a1);
        if (a1.empty()) throw invalid_argument("El primer apellido es obligatorio");

        cout << "Segundo apellido (opcional): ";
        string a2;
        getline(cin, a2);

        cout << "Fecha de nacimiento (DD-MM-AAAA): ";
        string fecha;
        getline(cin, fecha);
        if (fecha.size() != 10 || fecha[2] != '-' || fecha[5] != '-') {
            throw invalid_argument("Formato de fecha invalido. Use DD-MM-AAAA");
        }

        vector<string> cursos = obtenerCursosPorSemestre(carrera, semestre);
        db.registrarEstudiante(carnet, n1, n2, a1, a2, fecha, carrera, semestre, seccion, cursos);

        limpiarPantalla();
        cout << "==========================\n"
            << " ESTUDIANTE REGISTRADO    \n"
            << "==========================\n"
            << "Carnet: " << carnet << endl
            << "Nombre: " << n1 << (n2.empty() ? "" : " " + n2) << " "
            << a1 << (a2.empty() ? "" : " " + a2) << endl
            << "Carrera: " << carrera << endl
            << "Semestre: " << semestre << endl
            << "Seccion: " << seccion << endl
            << "Cursos matriculados: " << cursos.size() << endl
            << "==========================\n";
    }
    catch (const exception& e) {
        cerr << "\nError al registrar: " << e.what() << endl;
        cout << "Presione Enter para continuar...";
        cin.ignore();
    }
}

void mostrarTodosEstudiantes(DatabaseManager& db) {
    try {
        limpiarPantalla();
        auto estudiantes = db.obtenerTodosEstudiantes();

        if (estudiantes.empty()) {
            cout << "No hay estudiantes registrados.\n";
            return;
        }

        cout << "================================\n"
            << "    LISTADO DE ESTUDIANTES     \n"
            << "================================\n"
            << "Total de estudiantes: " << estudiantes.size() << "\n\n";

        for (const auto& est : estudiantes) {
            cout << "Carnet: " << est.at("carnet") << endl
                << "Nombre: " << est.at("nombre1")
                << (est.at("nombre2").empty() ? "" : " " + est.at("nombre2")) << " "
                << est.at("apellido1")
                << (est.at("apellido2").empty() ? "" : " " + est.at("apellido2")) << endl
                << "Fecha Nacimiento: " << est.at("fechaNacimiento")
                << " (Edad: " << est.at("edad") << ")" << endl
                << "Carrera: " << est.at("carrera") << endl
                << "Semestre: " << est.at("semestre")
                << ", Seccion: " << est.at("seccion") << endl
                << "--------------------------------\n";
        }
    }
    catch (const exception& e) {
        cerr << "\nError al mostrar estudiantes: " << e.what() << endl;
        cout << "Presione Enter para continuar...";
        cin.ignore();
    }
}

void buscarEstudiante(DatabaseManager& db) {
    try {
        limpiarPantalla();
        cout << "==========================\n"
            << "   BUSQUEDA DE ESTUDIANTE \n"
            << "==========================\n"
            << "Ingrese el carnet: ";
        string carnet;
        getline(cin, carnet);

        auto est = db.buscarEstudiante(carnet);
        if (est.empty()) {
            cout << "\nNo se encontro estudiante con ese carnet.\n";
            return;
        }

        cout << "\n================================\n"
            << "   INFORMACION DEL ESTUDIANTE\n"
            << "================================\n"
            << "Carnet: " << est["carnet"] << endl
            << "Nombre: " << est["nombre1"]
            << (est["nombre2"].empty() ? "" : " " + est["nombre2"]) << " "
            << est["apellido1"]
            << (est["apellido2"].empty() ? "" : " " + est["apellido2"]) << endl
            << "Fecha Nacimiento: " << est["fechaNacimiento"]
            << " (Edad: " << est["edad"] << ")" << endl
            << "Carrera: " << est["carrera"] << endl
            << "Semestre: " << est["semestre"] << endl
            << "Seccion: " << est["seccion"] << endl
            << "================================\n";
    }
    catch (const exception& e) {
        cerr << "\nError al buscar estudiante: " << e.what() << endl;
        cout << "Presione Enter para continuar...";
        cin.ignore();
    }
}

void eliminarEstudiante(DatabaseManager& db) {
    try {
        limpiarPantalla();
        cout << "==========================\n"
            << "   ELIMINAR ESTUDIANTE   \n"
            << "==========================\n"
            << "Ingrese el carnet: ";
        string carnet;
        getline(cin, carnet);

        if (db.eliminarEstudiante(carnet)) {
            cout << "\nEstudiante eliminado correctamente.\n";
        }
        else {
            cout << "\nNo se encontro estudiante con ese carnet.\n";
        }
    }
    catch (const exception& e) {
        cerr << "\nError al eliminar estudiante: " << e.what() << endl;
        cout << "Presione Enter para continuar...";
        cin.ignore();
    }
}

void verificarConexion(DatabaseManager& db) {
    try {
        PGconn* conn = db.getConnection();

        limpiarPantalla();
        cout << "==========================\n"
            << " PRUEBA DE CONEXION PG    \n"
            << "==========================\n";

        PGresult* res = PQexec(conn, "SELECT version(), current_timestamp, current_user");
        if (PQresultStatus(res) == PGRES_TUPLES_OK) {
            cout << "Version PostgreSQL: " << PQgetvalue(res, 0, 0) << endl;
            cout << "Fecha/Hora actual: " << PQgetvalue(res, 0, 1) << endl;
            cout << "Usuario actual: " << PQgetvalue(res, 0, 2) << endl;

            PGresult* res_tablas = PQexec(conn,
                "SELECT table_name FROM information_schema.tables "
                "WHERE table_schema = 'public' ORDER BY table_name");

            if (PQresultStatus(res_tablas) == PGRES_TUPLES_OK) {
                cout << "\nTablas en la base de datos:\n";
                for (int i = 0; i < PQntuples(res_tablas); i++) {
                    cout << "- " << PQgetvalue(res_tablas, i, 0) << endl;
                }
            }
            PQclear(res_tablas);
        }
        PQclear(res);

        cout << "\nPrueba de conexion exitosa!\n";
    }
    catch (const exception& e) {
        cerr << "\nError en la conexion: " << e.what() << endl;
    }

    cout << "\nPresione Enter para continuar...";
    cin.ignore();
}

int main() {
    cout << "Iniciando sistema de gestion universitaria...\n";

    try {
        DatabaseManager db;

        int opcion;
        do {
            mostrarMenuPrincipal();

            while (!(cin >> opcion)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada invalida. Intente nuevamente: ";
            }
            cin.ignore();

            switch (opcion) {
            case 1: registrarEstudiante(db); break;
            case 2: mostrarTodosEstudiantes(db); break;
            case 3: buscarEstudiante(db); break;
            case 4: eliminarEstudiante(db); break;
            case 5: verificarConexion(db); break;
            case 6: cout << "Saliendo del sistema...\n"; break;
            default: cout << "Opcion no valida!\n";
            }

            if (opcion != 6 && !preguntarContinuar()) {
                opcion = 6;
            }
        } while (opcion != 6);
    }
    catch (const exception& e) {
        cerr << "\nError critico: " << e.what() << endl
            << "Verifique:\n1. PostgreSQL esta corriendo\n2. Credenciales correctas\n"
            << "3. La base de datos existe\n4. El usuario tiene permisos\n";

        return 1;
    }

    return 0;
}
