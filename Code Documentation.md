<h1 align="center"><b>Code for a University Student Administration System</b><img src="https://media.giphy.com/media/hvRJCLFzcasrR4ia7z/giphy.gif" width="35"></h1>
<br>

<h1>About the Code</h1>
<p>This C++ program is a <strong>University Student Management System</strong> designed to manage, enroll, search, and delete university students. It provides a user-friendly, menu-driven interface that allows users to perform various operations related to student administration. The system now includes the capability to save and load student data in JSON format, providing persistent storage between program executions.</p>



<br><br>
<h1 align="center">Code Documentation - University Student Management System</h1>

  <h2>1. Included Libraries</h2>
    <p>The code uses several C++ libraries for its functionality:</p>
    <pre><code>
#include &lt;iostream&gt;
#include &lt;vector&gt;
#include &lt;string&gt;
#include &lt;ctime&gt;
#include &lt;map&gt;
#include &lt;memory&gt;
#include &lt;algorithm&gt; // For using sort
#ifdef _WIN32
#include &lt;windows.h&gt;
#else
#include &lt;cstdlib&gt;
#endif
    </code></pre>
    <p><strong>Explanation of the libraries:</strong></p>
    <ul>
        <li><code>&lt;iostream&gt;</code>: Provides functionalities for standard input and output (like <code>cin</code> and <code>cout</code>).</li>
        <li><code>&lt;vector&gt;</code>: Allows the use of vectors, which are dynamic arrays for storing data.</li>
        <li><code>&lt;string&gt;</code>: Facilitates the handling of text strings.</li>
        <li><code>&lt;ctime&gt;</code>: Provides functions for working with dates and times (used to calculate age).</li>
        <li><code>&lt;map&gt;</code>: Allows the use of maps, which are associative containers for storing key-value pairs.</li>
        <li><code>&lt;memory&gt;</code>: Facilitates the use of smart pointers (<code>unique_ptr</code>), which help manage memory automatically.</li>
        <li><code>&lt;algorithm&gt;</code>: Provides useful functions like <code>sort</code> for sorting elements.</li>
        <li><code>#ifdef _WIN32</code>: Conditional to detect if the operating system is Windows. If so, it includes <code>&lt;windows.h&gt;</code> to use <code>system("cls")</code>. On other systems, it uses <code>&lt;cstdlib&gt;</code> for <code>system("clear")</code>.</li>
    </ul>

  <h2>2. Function Declarations</h2>
    <p>The code declares several functions to modularize the program's logic:</p>
    <pre><code>
void limpiarPantalla();
int calcularEdad(const string& fechaNacimiento);
string seleccionarFacultad();
string seleccionarCarrera(const string& facultad);
bool preguntarContinuar();
    </code></pre>
    <p><strong>Explanation of the functions:</strong></p>
    <ul>
        <li><code>limpiarPantalla()</code>: Clears the console to improve readability.</li>
        <li><code>calcularEdad()</code>: Calculates the student's age based on their birthdate.</li>
        <li><code>seleccionarFacultad()</code>: Displays a menu to select the student's faculty.</li>
        <li><code>seleccionarCarrera()</code>: Displays a menu to select the student's major, depending on the chosen faculty.</li>
        <li><code>preguntarContinuar()</code>: Asks the user if they want to perform another operation or exit the program.</li>
    </ul>

  <h2>3. Function <code>limpiarPantalla()</code></h2>
    <p>This function clears the console screen:</p>
    <pre><code>
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
    </code></pre>
    <p><strong>How it works:</strong></p>
    <ul>
        <li>Uses <code>system("cls")</code> on Windows and <code>system("clear")</code> on other operating systems.</li>
    </ul>

  <h2>4. Function <code>calcularEdad()</code></h2>
    <p>This function calculates the student's age based on their birthdate:</p>
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
    <p><strong>How it works:</strong></p>
    <ul>
        <li>Gets the current date using <code>time()</code> and <code>localtime_s()</code>.</li>
        <li>Compares the birthdate with the current date to calculate the age.</li>
    </ul>

  <h2>5. Class <code>Estudiante</code></h2>
    <p>The <code>Estudiante</code> class represents a student with their personal, academic, and course data:</p>
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
        cout &lt;&lt; "Student Information\n";
        cout &lt;&lt; "--------------------------\n";
        cout &lt;&lt; "ID: " &lt;&lt; carnet &lt;&lt; endl;
        cout &lt;&lt; "Name: " &lt;&lt; nombre1 &lt;&lt; " " &lt;&lt; nombre2 &lt;&lt; " " &lt;&lt; apellido1 &lt;&lt; " " &lt;&lt; apellido2 &lt;&lt; endl;
        cout &lt;&lt; "Date of Birth: " &lt;&lt; fechaNacimiento &lt;&lt; endl;
        cout &lt;&lt; "Age: " &lt;&lt; edad &lt;&lt; " years" &lt;&lt; endl;
        cout &lt;&lt; "Major: " &lt;&lt; carrera &lt;&lt; endl;
        cout &lt;&lt; "Semester: " &lt;&lt; semestre &lt;&lt; endl;
        cout &lt;&lt; "Section: " &lt;&lt; seccion &lt;&lt; endl;
        cout &lt;&lt; "--------------------------\n";
        cout &lt;&lt; "Courses:\n";
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
    <p><strong>Explanation:</strong></p>
    <ul>
        <li>Stores information such as ID, names, last names, birthdate, section, major, and semester.</li>
        <li>Includes methods to add courses and display the student's information.</li>
    </ul>

  <h2>6. Search and Display Functions</h2>
    <p>The code includes functions to search and display students:</p>
    <pre><code>
void buscarPorCarnet(const vector&lt;unique_ptr&lt;Estudiante&gt;&gt;& estudiantes, const string& carnet);
void mostrarEstudiantesPorSemestre(const vector&lt;unique_ptr&lt;Estudiante&gt;&gt;& estudiantes, int semestre, const string& seccion);
void mostrarTodosEstudiantesAgrupados(const vector&lt;unique_ptr&lt;Estudiante&gt;&gt;& estudiantes);
void mostrarEstudiantesPorCarreraSeccion(const vector&lt;unique_ptr&lt;Estudiante&gt;&gt;& estudiantes);
    </code></pre>
    <p><strong>Explanation:</strong></p>
    <ul>
        <li><code>buscarPorCarnet()</code>: Searches for a student by their ID and displays their information.</li>
        <li><code>mostrarEstudiantesPorSemestre()</code>: Displays students from a specific semester and section.</li>
        <li><code>mostrarTodosEstudiantesAgrupados()</code>: Groups and displays all students by faculty, major, and section.</li>
        <li><code>mostrarEstudiantesPorCarreraSeccion()</code>: Displays students from a major and section in alphabetical order.</li>
    </ul>

  <h2>7. Main Function <code>gestionarEstudiantes()</code></h2>
    <p>This function controls the main flow of the program:</p>
    <pre><code>
void gestionarEstudiantes() {
    vector&lt;unique_ptr&lt;Estudiante&gt;&gt; estudiantes;
    int opcion;

  do {
        // Main menu
    } while (opcion != 5);
}
    </code></pre>
    <p><strong>How it works:</strong></p>
    <ul>
        <li>Displays a menu with options to assign, display, search, or delete students.</li>
        <li>Uses a <code>do-while</code> loop to keep the program running until the user chooses to exit.</li>
    </ul>

  <h2>8. Function <code>main()</code></h2>
    <p>Entry point of the program:</p>
    <pre><code>
int main() {
    gestionarEstudiantes();
    return 0;
}
    </code></pre>
    <p><strong>How it works:</strong></p>
    <ul>
        <li>Calls the <code>gestionarEstudiantes()</code> function to start the program.</li>
    </ul>

  <h2>9. Summary of Libraries and Their Use</h2>
    <table>
        <thead>
            <tr>
                <th>Library</th>
                <th>Purpose</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td><code>&lt;iostream&gt;</code></td>
                <td>Standard input and output (cin, cout).</td>
            </tr>
            <tr>
                <td><code>&lt;vector&gt;</code></td>
                <td>Store and manage dynamic lists of students.</td>
            </tr>
            <tr>
                <td><code>&lt;string&gt;</code></td>
                <td>Handling of text strings (names, majors, etc.).</td>
            </tr>
            <tr>
                <td><code>&lt;ctime&gt;</code></td>
                <td>Calculate age based on birthdate.</td>
            </tr>
            <tr>
                <td><code>&lt;map&gt;</code></td>
                <td>Group students by faculty, major, and section.</td>
            </tr>
            <tr>
                <td><code>&lt;memory&gt;</code></td>
                <td>Use of <code>unique_ptr</code> to manage student memory.</td>
            </tr>
            <tr>
                <td><code>&lt;algorithm&gt;</code></td>
                <td>Sort students alphabetically.</td>
            </tr>
            <tr>
                <td><code>&lt;windows.h&gt;</code>/<code>&lt;cstdlib&gt;</code></td>
                <td>Clear the console screen based on the operating system.</td>
            </tr>
        </tbody>
    </table>

  <h2>10. Conclusion</h2>
    <p>This code is a university student management system that allows:</p>
    <ul>
        <li><strong>Assign students</strong>: Register new students with their personal and academic data.</li>
        <li><strong>Display students</strong>: View student information grouped by faculty, major, and section.</li>
        <li><strong>Search students</strong>: Find students by their ID.</li>
        <li><strong>Delete students</strong>: Remove students from the system.</li>
    </ul>
    <p>The libraries and structures used enable efficient information management and user-friendly interaction.</p>

<br>
<h2>New JSON Storage Feature</h2>
<p>The system now includes functionality to save and load student data in JSON format:</p>
<ul>
    <li><strong>Automatic Saving</strong>: Student data is automatically saved to a JSON file whenever changes are made</li>
    <li><strong>Data Persistence</strong>: Student information persists between program executions</li>
    <li><strong>Portable Data</strong>: JSON format allows easy data exchange with other systems</li>
    <li><strong>Backup Capability</strong>: JSON files can serve as backups of student records</li>
</ul>

<p>The JSON file structure organizes student data by faculty, major, and section, maintaining all the relationships present in the program's memory. When the program starts, it automatically loads any existing student data from the JSON file.</p>









<br><br>
  <h1 align="center">How It Works</h1>
    <p>It displays an initial menu that allows us to choose from the following options:</p>
    <pre>
------------
   Menu
------------
1. Assign student
2. Show students
3. Search student by ID
4. Delete student
5. Exit
Select an option:
    </pre>
    <p>We will choose an option. When selecting option 1, it takes us to the following menu:</p>
    <pre>
----------
   Menu
----------
1. Economic Finance
2. Communication Systems
3. Medicine and Health
Select an option:
    </pre>
    <p>Depending on the faculty we choose, it will take us to the following possible menus:</p>

  <h3>Menu 1:</h3>
    <pre>
----------
   Menu
----------
Faculty: Economic Finance
1. Business Administration
2. Auditing
Select an option:
    </pre>

  <h3>Menu 2:</h3>
    <pre>
----------
   Menu
----------
Faculty: Communication Systems
1. Systems Engineering
2. Network Architecture
Select an option:
    </pre>

  <h3>Menu 3:</h3>
    <pre>
----------
   Menu
----------
Faculty: Medicine and Health
1. General Medicine
2. Traumatology
3. Pediatrics
Select an option:
    </pre>

  <p>After selecting our faculty and major, the following will appear:</p>
    <p>At the top, it will show the faculty and major we selected. For example:</p>
    <pre>
Faculty: Communication Systems
Major: Systems Engineering
-------------------
Assignment Menu
-------------------
1. Semester I
2. Semester II
3. Semester III
4. Semester IV
5. Semester V
6. Semester VI
7. Semester VII
8. Semester VIII
9. Semester IX
10. Semester X
Select the semester:
    </pre>

  <p>After choosing the semester, it will ask us in which section we want to enroll the student:</p>
    <pre>
Faculty: Communication Systems
Major: Systems Engineering
Semester: 1
Select the section (A/B): A
    </pre>

  <p>Then, we can fill in the student's information as follows:</p>
    <pre>
Faculty: Communication Systems
Major: Systems Engineering
Semester: 1
Section: A
ID (example: 2294-24-19629): 2294-24-19629
First name: Ricardo
Middle name: Jose
Last name: Marquez
Second last name: Garcia
Date of birth (DD-MM-YYYY): 21/10/2004
    </pre>

  <p>After pressing enter, it will show the following:</p>
    <pre>
Faculty: Communication Systems
Major: Systems Engineering
Semester: 1
Section: A
Student assigned successfully.
Do you want to perform another operation? (y/n):
    </pre>

  <p>We can choose to end the program or continue with another function. If we press "y," it will take us back to the main menu.</p>
    <pre>
------------
   Menu
------------
1. Assign student
2. Show students
3. Search student by ID
4. Delete student
5. Exit
Select an option:
    </pre>

  <h3>Option 2: Show Students</h3>
    <p>If we choose option 2, it will display the following menu, where we can show all students and their data or show students by major and section in alphabetical order.</p>
    <pre>
--------
  Menu
--------
1. Show all students grouped by faculty, major, and section
2. Show students by major and section in alphabetical order
Select an option:
    </pre>

  <p>If we select option 1, it will display something like the following:</p>
    <pre>
Faculty: Economic Finance
Major: Auditing
Section: A
--------------------------
Student Information
--------------------------
ID: 2294-24-19458
Name: Emily Giovanna Yancoba Ixcotop
Date of Birth: 09/02/2005
Age: 20 years
Major: Auditing
Semester: 1
Section: A
--------------------------
Courses:
  1. Introduction to Auditing
  2. Basic Mathematics
  3. Accounting I
  4. General Economics
  5. Computer Science I
----------------------
    </pre>

  <pre>
Faculty: Communication Systems
  Major: Systems Engineering
    Section: A
--------------------------
Student Information
--------------------------
ID: 2294-24-19629
Name: Ricardo Jose Marquez Garcia
Date of Birth: 21/10/2004
Age: 20 years
Major: Systems Engineering
Semester: 1
Section: A
--------------------------
Courses:
  1. Introduction to Programming
  2. Basic Mathematics
  3. Physics I
  4. Systems Logic
  5. Computer Science I
----------------------
    </pre>

  <h3>Option 2: Search by Faculty</h3>
    <p>If we choose option 2, it will ask us to select the faculty:</p>
    <pre>
----------
   Menu
----------
1. Economic Finance
2. Communication Systems
3. Medicine and Health
Select an option:
    </pre>

  <p>After selecting the faculty, we can choose the major:</p>
    <pre>
----------
   Menu
----------
Faculty: Economic Finance
1. Business Administration
2. Auditing
Select an option: 2
    </pre>

  <p>Then, we must select the section in which we want to perform the search:</p>
    <pre>
Select the section (A/B): A
    </pre>

  <p>After pressing enter, it will display the following information:</p>
    <pre>
---------------------------------
Students of Major: Auditing, Section A:
---------------------------------
--------------------------
Student Information
--------------------------
ID: 2294-24-19458
Name: Emily Giovanna Yancoba Ixcotop
Date of Birth: 09/02/2005
Age: 20 years
Major: Auditing
Semester: 1
Section: A
--------------------------
Courses:
  1. Introduction to Auditing
  2. Basic Mathematics
  3. Accounting I
  4. General Economics
  5. Computer Science I
----------------------
----------------------
Do you want to perform another operation? (y/n):
    </pre>

  <h3>Option 3: Search Student by ID</h3>
    <p>If we want to search for a student by their ID, we must return to the main menu:</p>
    <pre>
------------
   Menu
------------
1. Assign student
2. Show students
3. Search student by ID
4. Delete student
5. Exit
Select an option:
    </pre>

  <p>When we choose option 3, it will display the following:</p>
    <pre>
Enter the ID to search:
    </pre>

  <p>After entering the ID, for example, 2294-24-19458, it will show the following information:</p>
    <pre>
--------------------------
Student Information
--------------------------
ID: 2294-24-19458
Name: Emily Giovanna Yancoba Ixcotop
Date of Birth: 09/02/2005
Age: 20 years
Major: Auditing
Semester: 1
Section: A
--------------------------
Courses:
  1. Introduction to Auditing
  2. Basic Mathematics
  3. Accounting I
  4. General Economics
  5. Computer Science I
----------------------
Do you want to perform another operation? (y/n):
    </pre>

  <h3>Option 4: Delete Student</h3>
    <p>If we want to delete a student, we return to the main menu:</p>
    <pre>
------------
   Menu
------------
1. Assign student
2. Show students
3. Search student by ID
4. Delete student
5. Exit
Select an option: 4
    </pre>

  <p>Once inside option 4, we must enter the ID of the student we want to delete, as follows:</p>
    <pre>
Enter the ID of the student to delete: 2294-24-19629
    </pre>

  <p>Then, it will ask us if we are sure we want to delete the student:</p>
    <pre>
Are you sure you want to delete the student with ID 2294-24-19629? (y/n): y
    </pre>

  <p>After that, it will display a message saying:</p>
    <pre>
"Student deleted successfully."
    </pre>

  <h3>Option 5: Exit the Program</h3>
    <p>Option 5 in the main menu allows us to exit the program.</p>
