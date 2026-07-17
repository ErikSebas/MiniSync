# MiniSync

Sistema de sincronización automática de archivos para Linux desarrollado en lenguaje C.

## Descripción

MiniSync es una aplicación desarrollada en lenguaje C para Linux que permite sincronizar automáticamente los archivos de un directorio de origen (`origin`) hacia un directorio de respaldo (`backup`) de forma local.

El sistema monitorea los siguientes eventos:

- Creación de archivos
- Modificación de archivos
- Eliminación de archivos

La detección de cambios se realiza mediante un escaneo recursivo ejecutado cada 5 segundos.

Además, todos los eventos detectados son registrados en un archivo de log (`minisync.log`), incluyendo la fecha y hora en que ocurrieron.

---

## Características

- Monitoreo del directorio `origin` mediante un escaneo recursivo.
- Sincronización automática de archivos hacia el directorio `backup`.
- Detección de creación de archivos.
- Detección de modificación de archivos.
- Detección de eliminación de archivos.
- Registro de eventos en un archivo de log.
- Ejecución del monitor como proceso daemon.
- Comunicación entre procesos mediante mecanismos IPC.
- Uso de colas POSIX y pipes para la gestión de eventos.

El escáner utiliza una estrategia basada en dos listas (`new` y `old`) que almacenan los metadatos de los archivos. Mediante la comparación de estas listas y de los contadores `newCount` y `oldCount`, es posible identificar los cambios ocurridos entre cada ciclo de monitoreo.

---

## Requisitos

- Linux
- GCC
- Make
- Soporte para POSIX Message Queues
- Soporte para Pipes

---

## Estructura del proyecto

```text
MiniSync/
│
├── origin/          # Directorio monitoreado
├── backup/          # Directorio de respaldo
├── include/         # Archivos de cabecera (.h)
├── src/             # Código fuente
├── launcher         # Controlador del daemon
├── minisync.log     # Registro de eventos
├── Makefile
└── README.md
```

---

## Compilación

```bash
make
```

---

## Ejecución

El proyecto incluye el ejecutable `launcher`, encargado de iniciar, detener y consultar el estado del monitor.

### Iniciar el monitor

```bash
./launcher start
```

### Detener el monitor

```bash
./launcher stop
```

### Consultar el estado

```bash
./launcher status
```

---

## Pruebas de funcionamiento

### Crear un nuevo archivo

```bash
touch origin/prueba.txt
```

### Modificar un archivo

```bash
echo "Hola MiniSync" >> origin/prueba.txt
```

### Eliminar un archivo

```bash
rm origin/prueba.txt
```

---

## Visualizar registros en tiempo real

```bash
tail -f minisync.log
```

### Ejemplo de registros

```text
[16-07-2026 22:55:27] [CREATE] origin/prueba.txt
[16-07-2026 23:29:33] [DELETE] origin/prueba.txt
[16-07-2026 23:31:47] [MODIFY] origin/prueba.txt
```

---

## Tecnologías utilizadas

- Lenguaje C
- Linux
- POSIX IPC
- POSIX Message Queues
- Pipes
- Daemons
- Sistema de archivos Linux

---

## Autor

**Erik Yupa**
