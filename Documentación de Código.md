<h1 align="center"><b>Código para un Sistema de Administración de Estudiantes Universitarios</b><img src="https://media.giphy.com/media/hvRJCLFzcasrR4ia7z/giphy.gif" width="35"></h1>
<br>
<div class="feature-box">
    <h2>Acerca del Sistema</h2>
    <p>Este programa en C++ es un Sistema Integral de Gestión Estudiantil Universitario diseñado para administrar, matricular, buscar y eliminar estudiantes universitarios. Proporciona una interfaz amigable basada en menús para todas las operaciones de administración estudiantil con integración completa a base de datos PostgreSQL.</p>
</div>

<div class="database-feature">
    <h2>Integración con PostgreSQL</h2>
    <p>El sistema cuenta con integración completa con base de datos PostgreSQL para almacenamiento persistente:</p>
    <ul>
        <li><strong>Almacenamiento confiable</strong>: Todos los registros se guardan de forma segura en PostgreSQL</li>
        <li><strong>Cumplimiento ACID</strong>: Garantiza integridad de datos con soporte para transacciones</li>
        <li><strong>Solución escalable</strong>: Maneja grandes volúmenes de registros eficientemente</li>
        <li><strong>Búsquedas avanzadas</strong>: Permite búsquedas complejas y generación de reportes</li>
        <li><strong>Organización estructurada</strong>: Datos organizados en esquema normalizado</li>
        <li><strong>Generación automática de carnets</strong>: Los IDs se generan basados en códigos de carrera y año</li>
    </ul>
</div>

<h2>Características Principales</h2>
<ul>
    <li>Sistema completo de matrícula estudiantil por facultades y carreras</li>
    <li>Asignación de secciones (A/B/C) para agrupamiento organizado</li>
    <li>Búsqueda avanzada por carnet estudiantil</li>
    <li>Eliminación segura con confirmación</li>
    <li>Validación de datos para todos los campos</li>
    <li>Interfaz limpia basada en menús</li>
    <li>Asignación automática de cursos por semestre</li>
    <li>Manejo de conexiones a base de datos con reconexión automática</li>
</ul>

<h1 align="center">Documentación Técnica</h1>

<h2>1. Estructura de la Base de Datos</h2>
<p>El sistema utiliza PostgreSQL con las siguientes tablas principales:</p>
<ul>
    <li><strong>estudiantes</strong>: Almacena información personal de estudiantes</li>
    <li><strong>carreras</strong>: Contiene información de carreras universitarias</li>
    <li><strong>cursos</strong>: Almacena datos de cursos académicos</li>
    <li><strong>matriculacion</strong>: Maneja la relación entre estudiantes y cursos</li>
</ul>

<h2>2. Clase DatabaseManager</h2>
<p>Clase central que maneja todas las operaciones con la base de datos:</p>
<pre><code>
class DatabaseManager {
private:
    PGconn* conn;
    void verificarConexion(); // Verifica/restaura conexión
    
public:
    DatabaseManager(); // Establece conexión
    ~DatabaseManager(); // Libera recursos
    
  // Métodos principales
    string generarCarnet(const string& carrera); // Genera ID estudiantil
    void registrarEstudiante(...); // Registra nuevo estudiante
    vector&lt;map&lt;string, string&gt;&gt; obtenerTodosEstudiantes(); // Obtiene todos
    map&lt;string, string&gt; buscarEstudiante(const string& carnet); // Busca
    bool eliminarEstudiante(const string& carnet); // Elimina
    void verificarEstructuraBD(); // Valida estructura BD
};
</code></pre>

<h2>3. Funciones Clave</h2>

<h3>Manejo de Conexiones</h3>
<pre><code>
DatabaseManager::DatabaseManager() {
    string conn_str = "host=localhost dbname=gestion_universitaria "
                   "user=ricardo password=ricgio921 port=5432";
    conn = PQconnectdb(conn_str.c_str());
    // Manejo de errores omitido por brevedad
}
</code></pre>

<h3>Generación de Carnets</h3>
<pre><code>
string DatabaseManager::generarCarnet(const string& carrera) {
    // Formato: [CódigoCarrera]-[Últimos2DígitosAño]-[Secuencia5Dígitos]
    // Ejemplo: 1030-24-00001 para Ingeniería en Sistemas en 2024
}
</code></pre>

<h3>Registro de Estudiantes</h3>
<pre><code>
void DatabaseManager::registrarEstudiante(...) {
    // Inserta registro de estudiante
    // Luego inserta todos sus cursos matriculados
    // Usa transacciones para integridad de datos
}
</code></pre>

<h2>4. Flujo Principal del Programa</h2>
<pre><code>
int main() {
    try {
        DatabaseManager db; // Inicializa conexión a BD
        while (true) {
            mostrarMenuPrincipal(); // Muestra menú principal
            // Procesa selección de usuario
            switch (opcion) {
                case 1: registrarEstudiante(db); break;
                case 2: mostrarTodosEstudiantes(db); break;
                // ... otras opciones
            }
        }
    } catch (...) {
        // Manejo de errores
    }
}
</code></pre>

<h2>5. Validación de Datos</h2>
<p>El sistema incluye extensa validación de datos:</p>
<ul>
    <li>Validación de formato de fecha (DD-MM-AAAA)</li>
    <li>Validación de campos obligatorios</li>
    <li>Validación de rango de semestres (1-10)</li>
    <li>Validación de secciones (A/B/C)</li>
    <li>Aplicación de restricciones de base de datos</li>
</ul>

<h2>6. Manejo de Errores</h2>
<p>El sistema incluye manejo completo de errores:</p>
<ul>
    <li>Errores de conexión a base de datos</li>
    <li>Errores en ejecución de consultas</li>
    <li>Errores de validación de datos</li>
    <li>Rollback de transacciones en fallos</li>
</ul>

<h2>7. Interfaz de Usuario</h2>
<p>El sistema provee una interfaz basada en consola con:</p>
<ul>
    <li>Manejo de pantalla limpia (funciona en Windows y Unix)</li>
    <li>Salida formateada</li>
    <li>Validación de entrada</li>
    <li>Preguntas de confirmación</li>
</ul>

<h2>8. Gestión de Cursos</h2>
<p>Los cursos se asignan automáticamente según:</p>
<pre><code>
vector&lt;string&gt; obtenerCursosPorSemestre(const string& carrera, int semestre) {
    // Devuelve cursos predefinidos para cada carrera/semestre
    // Ejemplo: Semestre 1 de Ingeniería en Sistemas:
    // - Introducción a la Programación
    // - Matemáticas Básicas
    // - Física I
    // - Lógica de Sistemas
    // - Informática I
}
</code></pre>

<h2>9. Requerimientos del Sistema</h2>
<ul>
    <li>Compilador compatible con C++17</li>
    <li>Bibliotecas cliente de PostgreSQL (libpq)</li>
    <li>Servidor PostgreSQL (versión 12+)</li>
    <li>Esquema de base de datos preconfigurado</li>
</ul>

<h2>10. Flujo de Trabajo</h2>
<p>El sistema sigue este flujo de operación:</p>
<ol>
    <li>Establece conexión a base de datos</li>
    <li>Presenta menú principal</li>
    <li>Procesa selección del usuario</li>
    <li>Valida todas las entradas</li>
    <li>Ejecuta operaciones en base de datos</li>
    <li>Muestra resultados</li>
    <li>Regresa al menú principal</li>
</ol>

<h3>Ejemplo de Flujo de Registro</h3>
<pre>
1. Seleccionar facultad (Finanzas Económicas, Sistemas de Comunicación, Medicina y Salud)
2. Seleccionar carrera según facultad
3. Seleccionar semestre (1-10)
4. Seleccionar sección (A/B/C)
5. Ingresar datos del estudiante:
   - Primer nombre (obligatorio)
   - Segundo nombre (opcional)
   - Primer apellido (obligatorio)
   - Segundo apellido (opcional)
   - Fecha de nacimiento (DD-MM-AAAA)
6. El sistema genera carnet automáticamente
7. El sistema asigna cursos automáticamente
8. Los datos se guardan en PostgreSQL
</pre>

<h2>11. Esquema Detallado de la Base de Datos</h2>
<p>La base de datos PostgreSQL incluye estas tablas clave:</p>

<h3>Tabla: estudiantes</h3>
<table>
    <tr><th>Columna</th><th>Tipo</th><th>Descripción</th></tr>
    <tr><td>id_estudiante</td><td>SERIAL</td><td>Llave primaria</td></tr>
    <tr><td>carnet</td><td>VARCHAR(20)</td><td>ID estudiantil (formateado)</td></tr>
    <tr><td>primer_nombre</td><td>VARCHAR(50)</td><td>Primer nombre</td></tr>
    <tr><td>segundo_nombre</td><td>VARCHAR(50)</td><td>Segundo nombre</td></tr>
    <tr><td>primer_apellido</td><td>VARCHAR(50)</td><td>Apellido paterno</td></tr>
    <tr><td>segundo_apellido</td><td>VARCHAR(50)</td><td>Apellido materno</td></tr>
    <tr><td>fecha_nacimiento</td><td>DATE</td><td>Fecha de nacimiento</td></tr>
    <tr><td>id_carrera</td><td>INTEGER</td><td>Llave foránea a carrera</td></tr>
    <tr><td>semestre_actual</td><td>INTEGER</td><td>Semestre actual (1-10)</td></tr>
    <tr><td>seccion</td><td>CHAR(1)</td><td>Sección (A/B/C)</td></tr>
</table>

<h3>Tabla: matriculacion</h3>
<table>
    <tr><th>Columna</th><th>Tipo</th><th>Descripción</th></tr>
    <tr><td>id_matricula</td><td>SERIAL</td><td>Llave primaria</td></tr>
    <tr><td>id_estudiante</td><td>INTEGER</td><td>Llave foránea a estudiante</td></tr>
    <tr><td>id_curso</td><td>INTEGER</td><td>Llave foránea a curso</td></tr>
    <tr><td>semestre</td><td>INTEGER</td><td>Semestre cursado</td></tr>
</table>

<h2>12. Consideraciones de Seguridad</h2>
<ul>
    <li>Usa consultas parametrizadas para prevenir inyección SQL</li>
    <li>Cadena de conexión incluye tiempos de espera</li>
    <li>Reconexión automática para conexiones caídas</li>
    <li>Datos sensibles (como contraseñas) no están hardcodeadas en producción</li>
</ul>

<h2>13. Características de Rendimiento</h2>
<ul>
    <li>Inserción en lote de cursos matriculados</li>
    <li>Generación eficiente de carnets</li>
    <li>Pool de conexiones (implícito a través de PostgreSQL)</li>
    <li>Tablas de base de datos indexadas</li>
</ul>

<h2>14. Limitaciones</h2>
<ul>
    <li>Solo interfaz de consola (sin GUI)</li>
    <li>Sin soporte para múltiples usuarios en esta versión</li>
    <li>Sin registro de auditoría</li>
    <li>Sin funcionalidad de backup (depende de backups de PostgreSQL)</li>
</ul>

<h2>15. Mejoras Futuras</h2>
<ul>
    <li>Agregar autenticación de usuarios</li>
    <li>Implementar funciones de reportes</li>
    <li>Añadir seguimiento de calificaciones</li>
    <li>Soporte para múltiples motores de base de datos</li>
    <li>Versión con interfaz web</li>
</ul>
