<h1 align="center"><b>Código para un Sistema de Administración de Estudiantes Universitarios</b><img src="https://media.giphy.com/media/hvRJCLFzcasrR4ia7z/giphy.gif" width="35"></h1>
    <br>

  <h1>Acerca del Código</h1>
    <p>Este programa en C++ es un <strong>Sistema de Gestión de Estudiantes Universitarios</strong> diseñado para administrar, inscribir, buscar y eliminar estudiantes universitarios. Proporciona una interfaz amigable basada en menús que permite a los usuarios realizar diversas operaciones relacionadas con la administración de estudiantes.</p>
<br>
  <br>
    <h1 align="center">Documentación del Código - Sistema de Gestión de Estudiantes Universitarios</h1>

  <h2>1. Librerías Incluidas</h2>
    <p>El código utiliza varias librerías de C++ para su funcionamiento:</p>
    <pre><code>
#include &lt;iostream&gt;
#include &lt;vector&gt;
#include &lt;string&gt;
#include &lt;ctime&gt;
#include &lt;map&gt;
#include &lt;memory&gt;
#include &lt;algorithm&gt; // Para usar sort
#ifdef _WIN32
#include &lt;windows.h&gt;
#else
#include &lt;cstdlib&gt;
#endif
    </code></pre>
    <p><strong>Explicación de las librerías:</strong></p>
    <ul>
        <li><code>&lt;iostream&gt;</code>: Proporciona funcionalidades para la entrada y salida estándar (como <code>cin</code> y <code>cout</code>).</li>
        <li><code>&lt;vector&gt;</code>: Permite el uso de vectores, que son arreglos dinámicos para almacenar datos.</li>
        <li><code>&lt;string&gt;</code>: Facilita el manejo de cadenas de texto.</li>
        <li><code>&lt;ctime&gt;</code>: Proporciona funciones para trabajar con fechas y horas (usado para calcular la edad).</li>
        <li><code>&lt;map&gt;</code>: Permite el uso de mapas, que son contenedores asociativos para almacenar pares clave-valor.</li>
        <li><code>&lt;memory&gt;</code>: Facilita el uso de punteros inteligentes (<code>unique_ptr</code>), que ayudan a gestionar la memoria automáticamente.</li>
        <li><code>&lt;algorithm&gt;</code>: Proporciona funciones útiles como <code>sort</code> para ordenar elementos.</li>
        <li><code>#ifdef _WIN32</code>: Condicional para detectar si el sistema operativo es Windows. Si es así, se incluye <code>&lt;windows.h&gt;</code> para usar <code>system("cls")</code>. En otros sistemas, se usa <code>&lt;cstdlib&gt;</code> para <code>system("clear")</code>.</li>
    </ul>

  <h2>2. Declaraciones de Funciones</h2>
    <p>El código declara varias funciones para modularizar la lógica del programa:</p>
    <pre><code>
void limpiarPantalla();
int calcularEdad(const string& fechaNacimiento);
string seleccionarFacultad();
string seleccionarCarrera(const string& facultad);
bool preguntarContinuar();
    </code></pre>
    <p><strong>Explicación de las funciones:</strong></p>
    <ul>
        <li><code>limpiarPantalla()</code>: Limpia la consola para mejorar la legibilidad.</li>
        <li><code>calcularEdad()</code>: Calcula la edad del estudiante a partir de su fecha de nacimiento.</li>
        <li><code>seleccionarFacultad()</code>: Muestra un menú para seleccionar la facultad del estudiante.</li>
        <li><code>seleccionarCarrera()</code>: Muestra un menú para seleccionar la carrera, dependiendo de la facultad elegida.</li>
        <li><code>preguntarContinuar()</code>: Pregunta al usuario si desea realizar otra operación o salir del programa.</li>
    </ul>

  <h2>3. Función <code>limpiarPantalla()</code></h2>
    <p>Esta función limpia la pantalla de la consola:</p>
    <pre><code>
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
    </code></pre>
    <p><strong>Funcionamiento:</strong></p>
    <ul>
        <li>Usa <code>system("cls")</code> en Windows y <code>system("clear")</code> en otros sistemas operativos.</li>
    </ul>

  <h2>4. Función <code>calcularEdad()</code></h2>
    <p>Esta función calcula la edad del estudiante a partir de su fecha de nacimiento:</p>
    <pre><code>
int calcularEdad(const string& fechaNacimiento) {
    time_t ahora = time(0);
    tm fechaActual;
    localtime_s(&fechaActual, &ahora);

  int anioActual = fechaActual.tm_year + 1900;
    int mesActual = fechaActual.tm_mon + 1;
    int diaActual = fechaActual.tm_mday;

  int diaNacimiento = stoi(fechaNacimiento.substr(0, 2));
    int mesNacimiento = stoi(fechaNacimiento.substr(3, 2));
    int anioNacimiento = stoi(fechaNacimiento.substr(6, 4));

  int edad = anioActual - anioNacimiento;

  if (mesActual &lt; mesNacimiento || (mesActual == mesNacimiento && diaActual &lt; diaNacimiento)) {
        edad--;
    }

  return edad;
}
    </code></pre>
    <p><strong>Funcionamiento:</strong></p>
    <ul>
        <li>Obtiene la fecha actual usando <code>time()</code> y <code>localtime_s()</code>.</li>
        <li>Compara la fecha de nacimiento con la fecha actual para calcular la edad.</li>
    </ul>

  <h2>5. Clase <code>Estudiante</code></h2>
    <p>La clase <code>Estudiante</code> representa a un estudiante con sus datos personales, académicos y cursos:</p>
    <pre><code>
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
    vector&lt;string&gt; cursos;

public:
    Estudiante(string carnet, string n1, string n2, string a1, string a2, string fecha, string sec, string carr, int sem)
        : carnet(carnet), nombre1(n1), nombre2(n2), apellido1(a1), apellido2(a2), fechaNacimiento(fecha), seccion(sec), carrera(carr), semestre(sem) {
    }

  void agregarCurso(const string& curso) {
        cursos.push_back(curso);
    }

  virtual void mostrarInfo() const {
        int edad = calcularEdad(fechaNacimiento);
        cout &lt;&lt; "--------------------------\n";
        cout &lt;&lt; "Información del Estudiante\n";
        cout &lt;&lt; "--------------------------\n";
        cout &lt;&lt; "Carnet: " &lt;&lt; carnet &lt;&lt; endl;
        cout &lt;&lt; "Nombre: " &lt;&lt; nombre1 &lt;&lt; " " &lt;&lt; nombre2 &lt;&lt; " " &lt;&lt; apellido1 &lt;&lt; " " &lt;&lt; apellido2 &lt;&lt; endl;
        cout &lt;&lt; "Fecha de Nacimiento: " &lt;&lt; fechaNacimiento &lt;&lt; endl;
        cout &lt;&lt; "Edad: " &lt;&lt; edad &lt;&lt; " años" &lt;&lt; endl;
        cout &lt;&lt; "Carrera: " &lt;&lt; carrera &lt;&lt; endl;
        cout &lt;&lt; "Semestre: " &lt;&lt; semestre &lt;&lt; endl;
        cout &lt;&lt; "Sección: " &lt;&lt; seccion &lt;&lt; endl;
        cout &lt;&lt; "--------------------------\n";
        cout &lt;&lt; "Cursos:\n";
        for (size_t i = 0; i &lt; cursos.size(); ++i) {
            cout &lt;&lt; "  " &lt;&lt; i + 1 &lt;&lt; ". " &lt;&lt; cursos[i] &lt;&lt; endl;
        }
        cout &lt;&lt; "----------------------\n";
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

  const vector&lt;string&gt;& getCursos() const {
        return cursos;
    }

  virtual ~Estudiante() = default;
};
    </code></pre>
    <p><strong>Explicación:</strong></p>
    <ul>
        <li>Almacena información como carnet, nombres, apellidos, fecha de nacimiento, sección, carrera y semestre.</li>
        <li>Incluye métodos para agregar cursos y mostrar la información del estudiante.</li>
    </ul>

  <h2>6. Funciones de Búsqueda y Visualización</h2>
    <p>El código incluye funciones para buscar y mostrar estudiantes:</p>
    <pre><code>
void buscarPorCarnet(const vector&lt;unique_ptr&lt;Estudiante&gt;&gt;& estudiantes, const string& carnet);
void mostrarEstudiantesPorSemestre(const vector&lt;unique_ptr&lt;Estudiante&gt;&gt;& estudiantes, int semestre, const string& seccion);
void mostrarTodosEstudiantesAgrupados(const vector&lt;unique_ptr&lt;Estudiante&gt;&gt;& estudiantes);
void mostrarEstudiantesPorCarreraSeccion(const vector&lt;unique_ptr&lt;Estudiante&gt;&gt;& estudiantes);
    </code></pre>
    <p><strong>Explicación:</strong></p>
    <ul>
        <li><code>buscarPorCarnet()</code>: Busca un estudiante por su carnet y muestra su información.</li>
        <li><code>mostrarEstudiantesPorSemestre()</code>: Muestra estudiantes de un semestre y sección específicos.</li>
        <li><code>mostrarTodosEstudiantesAgrupados()</code>: Agrupa y muestra todos los estudiantes por facultad, carrera y sección.</li>
        <li><code>mostrarEstudiantesPorCarreraSeccion()</code>: Muestra estudiantes de una carrera y sección en orden alfabético.</li>
    </ul>

  <h2>7. Función Principal <code>gestionarEstudiantes()</code></h2>
    <p>Esta función controla el flujo principal del programa:</p>
    <pre><code>
void gestionarEstudiantes() {
    vector&lt;unique_ptr&lt;Estudiante&gt;&gt; estudiantes;
    int opcion;

  do {
        // Menú principal
    } while (opcion != 5);
}
    </code></pre>
    <p><strong>Funcionamiento:</strong></p>
    <ul>
        <li>Muestra un menú con opciones para asignar, mostrar, buscar o eliminar estudiantes.</li>
        <li>Usa un bucle <code>do-while</code> para mantener el programa en ejecución hasta que el usuario elija salir.</li>
    </ul>

  <h2>8. Función <code>main()</code></h2>
    <p>Punto de entrada del programa:</p>
    <pre><code>
int main() {
    gestionarEstudiantes();
    return 0;
}
    </code></pre>
    <p><strong>Funcionamiento:</strong></p>
    <ul>
        <li>Llama a la función <code>gestionarEstudiantes()</code> para iniciar el programa.</li>
    </ul>

  <h2>9. Resumen de Librerías y su Uso</h2>
    <table>
        <thead>
            <tr>
                <th>Librería</th>
                <th>Propósito</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td><code>&lt;iostream&gt;</code></td>
                <td>Entrada y salida estándar (cin, cout).</td>
            </tr>
            <tr>
                <td><code>&lt;vector&gt;</code></td>
                <td>Almacenar y gestionar listas dinámicas de estudiantes.</td>
            </tr>
            <tr>
                <td><code>&lt;string&gt;</code></td>
                <td>Manejo de cadenas de texto (nombres, carreras, etc.).</td>
            </tr>
            <tr>
                <td><code>&lt;ctime&gt;</code></td>
                <td>Cálculo de la edad a partir de la fecha de nacimiento.</td>
            </tr>
            <tr>
                <td><code>&lt;map&gt;</code></td>
                <td>Agrupar estudiantes por facultad, carrera y sección.</td>
            </tr>
            <tr>
                <td><code>&lt;memory&gt;</code></td>
                <td>Uso de <code>unique_ptr</code> para gestionar la memoria de los estudiantes.</td>
            </tr>
            <tr>
                <td><code>&lt;algorithm&gt;</code></td>
                <td>Ordenar estudiantes alfabéticamente.</td>
            </tr>
            <tr>
                <td><code>&lt;windows.h&gt;</code>/<code>&lt;cstdlib&gt;</code></td>
                <td>Limpiar la pantalla de la consola según el sistema operativo.</td>
            </tr>
        </tbody>
    </table>

  <h2>10. Conclusión</h2>
    <p>Este código es un sistema de gestión de estudiantes universitarios que permite:</p>
    <ul>
        <li><strong>Asignar estudiantes</strong>: Registrar nuevos estudiantes con sus datos personales y académicos.</li>
        <li><strong>Mostrar estudiantes</strong>: Visualizar la información de los estudiantes agrupados por facultad, carrera y sección.</li>
        <li><strong>Buscar estudiantes</strong>: Encontrar estudiantes por su carnet.</li>
        <li><strong>Eliminar estudiantes</strong>: Remover estudiantes del sistema.</li>
    </ul>
    <p>Las librerías y estructuras utilizadas permiten un manejo eficiente de la información y una interacción amigable con el usuario.</p>

<br><br>
  <h1 align="center">Cómo Funciona</h1>
    <p>Muestra un menú inicial que nos permite elegir entre las siguientes opciones:</p>
    <pre>
------------
   Menú
------------
1. Asignar estudiante
2. Mostrar estudiantes
3. Buscar estudiante por Carnet
4. Eliminar estudiante
5. Salir
Seleccione una opción:
  </pre>
    <p>Elegiremos una opción. Al seleccionar la opción 1, nos lleva al siguiente menú:</p>
    <pre>
----------
   Menú
----------
1. Finanzas Económicas
2. Sistemas de Comunicación
3. Medicina y Salud
Seleccione una opción:
    </pre>
    <p>Dependiendo de la facultad que elijamos, nos llevará a los siguientes menús posibles:</p>

  <h3>Menú 1:</h3>
    <pre>
----------
   Menú
----------
Facultad: Finanzas Económicas
1. Administración de Empresas
2. Auditoría
Seleccione una opción:
    </pre>

  <h3>Menú 2:</h3>
    <pre>
----------
   Menú
----------
Facultad: Sistemas de Comunicación
1. Ingeniería en Sistemas
2. Arquitectura de Redes
Seleccione una opción:
    </pre>

  <h3>Menú 3:</h3>
    <pre>
----------
   Menú
----------
Facultad: Medicina y Salud
1. Medicina General
2. Traumatología
3. Pediatría
Seleccione una opción:
    </pre>

  <p>Después de seleccionar nuestra facultad y carrera, aparecerá lo siguiente:</p>
    <p>En la parte superior, se mostrará la facultad y la carrera que seleccionamos. Por ejemplo:</p>
    <pre>
Facultad: Sistemas de Comunicación
Carrera: Ingeniería en Sistemas
-------------------
Menú de Asignación
-------------------
1. Semestre I
2. Semestre II
3. Semestre III
4. Semestre IV
5. Semestre V
6. Semestre VI
7. Semestre VII
8. Semestre VIII
9. Semestre IX
10. Semestre X
Seleccione el semestre:
    </pre>

  <p>Después de elegir el semestre, nos preguntará en qué sección deseamos inscribir al estudiante:</p>
    <pre>
Facultad: Sistemas de Comunicación
Carrera: Ingeniería en Sistemas
Semestre: 1
Seleccione la sección (A/B): A
    </pre>

  <p>Luego, podremos llenar los datos del estudiante de la siguiente forma:</p>
    <pre>
Facultad: Sistemas de Comunicación
Carrera: Ingeniería en Sistemas
Semestre: 1
Sección: A
Carnet (ejemplo: 2294-24-19629): 2294-24-19629
Primer nombre: Ricardo
Segundo nombre: Jose
Primer apellido: Marquez
Segundo apellido: Garcia
Fecha de nacimiento (DD-MM-AAAA): 21/10/2004
    </pre>

  <p>Al presionar enter, se mostrará lo siguiente:</p>
    <pre>
Facultad: Sistemas de Comunicación
Carrera: Ingeniería en Sistemas
Semestre: 1
Sección: A
Estudiante asignado correctamente.
¿Desea realizar otra operación? (s/n):
    </pre>

  <p>Podemos elegir terminar el programa o continuar con otra función. Si presionamos "s", nos llevará al menú inicial.</p>
    <pre>
------------
   Menú
------------
1. Asignar estudiante
2. Mostrar estudiantes
3. Buscar estudiante por Carnet
4. Eliminar estudiante
5. Salir
Seleccione una opción:
    </pre>

  <h3>Opción 2: Mostrar Estudiantes</h3>
    <p>Si elegimos la opción 2, se desplegará el siguiente menú, donde podemos mostrar todos los estudiantes y sus datos o mostrar los estudiantes por carrera y sección en orden alfabético.</p>
    <pre>
--------
  Menú
--------
1. Mostrar todos los estudiantes agrupados por facultad, carrera y sección
2. Mostrar estudiantes por carrera y sección en orden alfabético
Seleccione una opción:
    </pre>

  <p>Si seleccionamos la opción 1, se mostrará algo como lo siguiente:</p>
    <pre>
Facultad: Finanzas Económicas
Carrera: Auditoría
Sección: A
--------------------------
Información del Estudiante
--------------------------
Carnet: 2294-24-19458
Nombre: Emily Giovanna Yancoba Ixcotop
Fecha de Nacimiento: 09/02/2005
Edad: 20 años
Carrera: Auditoría
Semestre: 1
Sección: A
--------------------------
Cursos:
  1. Introducción a la Auditoría
  2. Matemáticas Básicas
  3. Contabilidad I
  4. Economía General
  5. Informática I
----------------------
    </pre>

  <pre>
Facultad: Sistemas de Comunicación
  Carrera: Ingeniería en Sistemas
    Sección: A
--------------------------
Información del Estudiante
--------------------------
Carnet: 2294-24-19629
Nombre: Ricardo Jose Marquez Garcia
Fecha de Nacimiento: 21/10/2004
Edad: 20 años
Carrera: Ingeniería en Sistemas
Semestre: 1
Sección: A
--------------------------
Cursos:
  1. Introducción a la Programación
  2. Matemáticas Básicas
  3. Física I
  4. Lógica de Sistemas
  5. Informática I
----------------------
    </pre>

  <h3>Opción 2: Buscar por Facultad</h3>
    <p>Si elegimos la opción 2, nos pedirá seleccionar la facultad:</p>
    <pre>
----------
   Menú
----------
1. Finanzas Económicas
2. Sistemas de Comunicación
3. Medicina y Salud
Seleccione una opción:
    </pre>

  <p>Después de seleccionar la facultad, podremos elegir la carrera:</p>
    <pre>
----------
   Menú
----------
Facultad: Finanzas Económicas
1. Administración de Empresas
2. Auditoría
Seleccione una opción: 2
    </pre>

  <p>Luego, tendremos que seleccionar la sección en la que deseamos realizar la búsqueda:</p>
    <pre>
Seleccione la sección (A/B): A
    </pre>

  <p>Al presionar enter, se mostrará la siguiente información:</p>
    <pre>
---------------------------------
Estudiantes de la Carrera: Auditoría, Sección A:
---------------------------------
--------------------------
Información del Estudiante
--------------------------
Carnet: 2294-24-19458
Nombre: Emily Giovanna Yancoba Ixcotop
Fecha de Nacimiento: 09/02/2005
Edad: 20 años
Carrera: Auditoría
Semestre: 1
Sección: A
--------------------------
Cursos:
  1. Introducción a la Auditoría
  2. Matemáticas Básicas
  3. Contabilidad I
  4. Economía General
  5. Informática I
----------------------
----------------------
¿Desea realizar otra operación? (s/n):
    </pre>

  <h3>Opción 3: Buscar Estudiante por Carnet</h3>
    <p>Si deseamos buscar un estudiante por su carnet, debemos regresar al menú principal:</p>
    <pre>
------------
   Menú
------------
1. Asignar estudiante
2. Mostrar estudiantes
3. Buscar estudiante por Carnet
4. Eliminar estudiante
5. Salir
Seleccione una opción:
    </pre>

  <p>Al elegir la opción 3, se mostrará lo siguiente:</p>
    <pre>
Ingrese el carnet a buscar:
    </pre>

  <p>Al ingresar el carnet, por ejemplo, 2294-24-19458, se mostrará la siguiente información:</p>
    <pre>
--------------------------
Información del Estudiante
--------------------------
Carnet: 2294-24-19458
Nombre: Emily Giovanna Yancoba Ixcotop
Fecha de Nacimiento: 09/02/2005
Edad: 20 años
Carrera: Auditoría
Semestre: 1
Sección: A
--------------------------
Cursos:
  1. Introducción a la Auditoría
  2. Matemáticas Básicas
  3. Contabilidad I
  4. Economía General
  5. Informática I
----------------------
¿Desea realizar otra operación? (s/n):
    </pre>

  <h3>Opción 4: Eliminar Estudiante</h3>
    <p>Si deseamos eliminar un estudiante, regresamos al menú principal:</p>
    <pre>
------------
   Menú
------------
1. Asignar estudiante
2. Mostrar estudiantes
3. Buscar estudiante por Carnet
4. Eliminar estudiante
5. Salir
Seleccione una opción: 4
    </pre>

  <p>Una vez dentro de la opción 4, tendremos que ingresar el carnet del estudiante que deseamos eliminar, de la siguiente manera:</p>
    <pre>
Ingrese el carnet del estudiante a eliminar: 2294-24-19629
    </pre>

  <p>Luego, nos preguntará si realmente deseamos eliminar al estudiante:</p>
    <pre>
¿Está seguro de que desea eliminar al estudiante con carnet 2294-24-19629? (s/n): s
    </pre>

  <p>Después, mostrará un mensaje que dice:</p>
    <pre>
"Estudiante eliminado correctamente."
    </pre>

  <h3>Opción 5: Salir del Programa</h3>
    <p>La opción 5 en el menú principal nos permite salir del programa.</p>
