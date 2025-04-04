<h1 align="center"><b>University Student Management System with PostgreSQL</b><img src="https://media.giphy.com/media/hvRJCLFzcasrR4ia7z/giphy.gif" width="35"></h1>

<div class="feature-box">
    <h2>About the System</h2>
    <p>This C++ program is a comprehensive University Student Management System designed to manage, enroll, search, and delete university students. It provides a user-friendly, menu-driven interface for all student administration operations with full PostgreSQL database integration.</p>
</div>

<div class="database-feature">
    <h2>PostgreSQL Database Integration</h2>
    <p>The system features complete PostgreSQL database integration for persistent data storage:</p>
    <ul>
        <li><strong>Reliable Data Storage</strong>: All student records are securely stored in a PostgreSQL database</li>
        <li><strong>ACID Compliance</strong>: Ensures data integrity with transactions support</li>
        <li><strong>Scalable Solution</strong>: Handles large volumes of student records efficiently</li>
        <li><strong>Advanced Querying</strong>: Enables complex searches and reporting capabilities</li>
        <li><strong>Structured Organization</strong>: Data is organized in a normalized database schema</li>
        <li><strong>Automatic ID Generation</strong>: Student IDs are generated based on career codes and year</li>
    </ul>
</div>

<h2>Key Features</h2>
<ul>
    <li>Complete student enrollment system across multiple faculties and majors</li>
    <li>Section assignment (A/B/C) for organized student grouping</li>
    <li>Advanced search functionality by student ID</li>
    <li>Secure student deletion with confirmation</li>
    <li>Data validation for all input fields</li>
    <li>Clean, menu-driven interface</li>
    <li>Automatic course assignment based on semester</li>
    <li>Database connection management with automatic reconnection</li>
</ul>

<h1 align="center">Code Documentation</h1>

<h2>1. Database Structure</h2>
<p>The system uses PostgreSQL with the following tables:</p>
<ul>
    <li><strong>estudiantes</strong>: Stores student personal information</li>
    <li><strong>carreras</strong>: Contains career information</li>
    <li><strong>cursos</strong>: Stores course data</li>
    <li><strong>matriculacion</strong>: Manages student-course relationships</li>
</ul>

<h2>2. DatabaseManager Class</h2>
<p>Core class handling all database operations:</p>
<pre><code>
class DatabaseManager {
private:
    PGconn* conn;
    void verificarConexion(); // Verifies/restores connection
    
public:
    DatabaseManager(); // Establishes connection
    ~DatabaseManager(); // Cleans up connection
    
  // Core methods
    string generarCarnet(const string& carrera); // Generates student ID
    void registrarEstudiante(...); // Registers new student
    vector&lt;map&lt;string, string&gt;&gt; obtenerTodosEstudiantes(); // Gets all students
    map&lt;string, string&gt; buscarEstudiante(const string& carnet); // Finds student
    bool eliminarEstudiante(const string& carnet); // Deletes student
    void verificarEstructuraBD(); // Validates DB structure
};
</code></pre>

<h2>3. Key Functions</h2>
<h3>Connection Management</h3>
<pre><code>
DatabaseManager::DatabaseManager() {
    string conn_str = "host=localhost dbname=gestion_universitaria "
                   "user=ricardo password=ricgio921 port=5432";
    conn = PQconnectdb(conn_str.c_str());
    // Error handling omitted for brevity
}
</code></pre>

<h3>Student ID Generation</h3>
<pre><code>
string DatabaseManager::generarCarnet(const string& carrera) {
    // Format: [CareerCode]-[Last2YearDigits]-[5DigitSequence]
    // Example: 1030-24-00001 for Systems Engineering in 2024
}
</code></pre>

<h3>Student Registration</h3>
<pre><code>
void DatabaseManager::registrarEstudiante(...) {
    // Inserts student record
    // Then inserts all enrolled courses
    // Uses transactions for data integrity
}
</code></pre>

<h2>4. Main Program Flow</h2>
<pre><code>
int main() {
    try {
        DatabaseManager db; // Initialize DB connection
        while (true) {
            mostrarMenuPrincipal(); // Show main menu
            // Handle user selection
            switch (opcion) {
                case 1: registrarEstudiante(db); break;
                case 2: mostrarTodosEstudiantes(db); break;
                // ... other options
            }
        }
    } catch (...) {
        // Error handling
    }
}
</code></pre>

<h2>5. Data Validation</h2>
<p>The system includes extensive validation:</p>
<ul>
    <li>Date format validation (DD-MM-YYYY)</li>
    <li>Required field validation</li>
    <li>Semester range validation (1-10)</li>
    <li>Section validation (A/B/C)</li>
    <li>Database constraint enforcement</li>
</ul>

<h2>6. Error Handling</h2>
<p>Comprehensive error handling includes:</p>
<ul>
    <li>Database connection errors</li>
    <li>Query execution errors</li>
    <li>Data validation errors</li>
    <li>Transaction rollback on failures</li>
</ul>

<h2>7. User Interface</h2>
<p>The system provides a console-based menu interface with:</p>
<ul>
    <li>Clear screen management (works on Windows and Unix)</li>
    <li>Formatted output</li>
    <li>Input validation</li>
    <li>Confirmation prompts</li>
</ul>

<h2>8. Course Management</h2>
<p>Courses are automatically assigned based on:</p>
<pre><code>
vector&lt;string&gt; obtenerCursosPorSemestre(const string& carrera, int semestre) {
    // Returns predefined courses for each career/semester
    // Example: Semester 1 Systems Engineering:
    // - Introduction to Programming
    // - Basic Mathematics
    // - Physics I
    // - Systems Logic
    // - Computer Science I
}
</code></pre>

<h2>9. System Requirements</h2>
<ul>
    <li>C++17 compatible compiler</li>
    <li>PostgreSQL client libraries (libpq)</li>
    <li>PostgreSQL server (version 12+)</li>
    <li>Preconfigured database schema</li>
</ul>

<h2>10. How It Works</h2>
<p>The system follows this workflow:</p>
<ol>
    <li>Establish database connection</li>
    <li>Present main menu</li>
    <li>Process user selection</li>
    <li>Validate all inputs</li>
    <li>Execute database operations</li>
    <li>Display results</li>
    <li>Return to main menu</li>
</ol>

<h3>Sample Student Registration Flow</h3>
<pre>
1. Select faculty (Economic Finance, Communication Systems, Medicine and Health)
2. Select career based on faculty
3. Select semester (1-10)
4. Select section (A/B/C)
5. Enter student details:
   - First name (required)
   - Middle name (optional)
   - Last name (required)
   - Second last name (optional)
   - Birth date (DD-MM-YYYY)
6. System generates student ID
7. System assigns courses automatically
8. Data is saved to PostgreSQL
</pre>

<h2>11. Database Schema Details</h2>
<p>The PostgreSQL database includes these key tables:</p>

<h3>estudiantes</h3>
<table>
    <tr><th>Column</th><th>Type</th><th>Description</th></tr>
    <tr><td>id_estudiante</td><td>SERIAL</td><td>Primary key</td></tr>
    <tr><td>carnet</td><td>VARCHAR(20)</td><td>Student ID (formatted)</td></tr>
    <tr><td>primer_nombre</td><td>VARCHAR(50)</td><td>First name</td></tr>
    <tr><td>segundo_nombre</td><td>VARCHAR(50)</td><td>Middle name</td></tr>
    <tr><td>primer_apellido</td><td>VARCHAR(50)</td><td>Last name</td></tr>
    <tr><td>segundo_apellido</td><td>VARCHAR(50)</td><td>Second last name</td></tr>
    <tr><td>fecha_nacimiento</td><td>DATE</td><td>Birth date</td></tr>
    <tr><td>id_carrera</td><td>INTEGER</td><td>Career foreign key</td></tr>
    <tr><td>semestre_actual</td><td>INTEGER</td><td>Current semester (1-10)</td></tr>
    <tr><td>seccion</td><td>CHAR(1)</td><td>Section (A/B/C)</td></tr>
</table>

<h3>matriculacion</h3>
<table>
    <tr><th>Column</th><th>Type</th><th>Description</th></tr>
    <tr><td>id_matricula</td><td>SERIAL</td><td>Primary key</td></tr>
    <tr><td>id_estudiante</td><td>INTEGER</td><td>Student foreign key</td></tr>
    <tr><td>id_curso</td><td>INTEGER</td><td>Course foreign key</td></tr>
    <tr><td>semestre</td><td>INTEGER</td><td>Semester taken</td></tr>
</table>

<h2>12. Security Considerations</h2>
<ul>
    <li>Uses parameterized queries to prevent SQL injection</li>
    <li>Connection string includes timeout settings</li>
    <li>Automatic reconnection for dropped connections</li>
    <li>Sensitive data (like passwords) not hardcoded in production</li>
</ul>

<h2>13. Performance Features</h2>
<ul>
    <li>Batched course enrollment</li>
    <li>Efficient student ID generation</li>
    <li>Connection pooling (implicit through PostgreSQL)</li>
    <li>Indexed database tables</li>
</ul>

<h2>14. Limitations</h2>
<ul>
    <li>Console interface only (no GUI)</li>
    <li>No multi-user support in this version</li>
    <li>No audit logging</li>
    <li>No backup functionality (rely on PostgreSQL backups)</li>
</ul>

<h2>15. Future Enhancements</h2>
<ul>
    <li>Add user authentication</li>
    <li>Implement reporting features</li>
    <li>Add grade tracking</li>
    <li>Support for multiple database backends</li>
    <li>Web interface version</li>
</ul>
