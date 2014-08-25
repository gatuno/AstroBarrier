/*
 * astro-write-level-file.c
 * This file is part of Astro Barrier
 *
 * Copyright (C) 2014 - Félix Arreola Rodríguez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdint.h>
#include <string.h>

int main (int argc, char *argv[]) {
	char buffer[255];
	int m, n, o, k;
	int niveles;
	
	int temp, temp2, temp3;
	uint32_t out;
	
	int fd;
	FILE *fd_entrada;
	
	if (argc < 3) {
		printf ("Uso: %s [input filename] [output filename]\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	fd_entrada = fopen (argv[1], "r");
	
	if (fd_entrada == NULL) {
		fprintf (stderr, "Imposible abrir el archivo de lectura\n");
		
		return EXIT_FAILURE;
	}
	
	fd = open (argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	
	if (fd < 0) {
		fprintf (stderr, "Imposible abrir el archivo de salida\n");
		fclose (fd_entrada);
		
		return EXIT_FAILURE;
	}
	
	/* Escribimos la cabecera */
	write (fd, "ASTRO", strlen ("ASTRO"));
	
	/* Recorrer hasta encontrar la palabra "START_ASTRO" */
	do {
		fgets (buffer, sizeof (buffer), fd_entrada);
		printf ("Descartando: %s\n", buffer);
	} while (strcmp (buffer, "START_ASTRO\n") != 0);
	
	/* Leer la versión del archivo */
	fgets (buffer, sizeof (buffer), fd_entrada);
	
	sscanf (buffer, "%i", &temp);
	out = temp;
	write (fd, &out, sizeof (out));
	
	/* Leer la cantidad de niveles en este archivo */
	fgets (buffer, sizeof (buffer), fd_entrada);
	
	sscanf (buffer, "%i", &niveles);
	out = niveles;
	write (fd, &out, sizeof (out));
	
	/* El primer nivel que hay que ccargar */
	fgets (buffer, sizeof (buffer), fd_entrada);
	
	sscanf (buffer, "%i", &temp);
	out = temp;
	write (fd, &out, sizeof (out));
	
	int g, h, i;
	uint32_t *arr_niveles;
	off_t *arr_pos;
	
	off_t posiciones;
	
	arr_niveles = (uint32_t *) malloc (sizeof (uint32_t) * niveles);
	arr_pos = (off_t *) malloc (sizeof (off_t) * niveles);
	
	posiciones = lseek (fd, 0, SEEK_CUR);
	
	for (g = 0; g < niveles; g++) {
		write (fd, &arr_niveles[0], sizeof (uint32_t));
		write (fd, &arr_pos[0], sizeof (off_t));
	}
	
	for (g = 0; g < niveles; g++) {
		/* Leer el nivel */
		arr_pos[g] = lseek (fd, 0, SEEK_CUR);
		
		fgets (buffer, sizeof (buffer), fd_entrada);
		sscanf (buffer, "%i", &temp);
		
		fgets (buffer, sizeof (buffer), fd_entrada);
printf ("Debug, Si es explicación RAW: %s", buffer);
		sscanf (buffer, "%i", &temp2);
		
		fgets (buffer, sizeof (buffer), fd_entrada);
printf ("Debug, Paquete del nivel secreto RAW: %s", buffer);
		sscanf (buffer, "%i", &temp3);
		
		/* Recortar los bits sobrantes */
		temp = temp & 0xFFFF;
		temp2 = temp2 & 0x01;
		temp3 = temp3 & 0x07;
		
		temp = (temp3 << 17) | (temp2 << 16) | temp;
		
		printf ("Depuración: Leo el número de nivel: %i\n", temp);
		arr_niveles[g] = temp;
		write (fd, &arr_niveles[g], sizeof (uint32_t));
		
		/* Leer el siguiente nivel */
		fgets (buffer, sizeof (buffer), fd_entrada);
printf ("Debug, RAW: %s", buffer);
		sscanf (buffer, "%i", &temp);
		out = temp;
		printf ("Depuración: El siguiente nivel debería ser: %i\n", temp);
		write (fd, &out, sizeof (uint32_t));
		
		/* Leer las balas y hits requeridos */
		fgets (buffer, sizeof (buffer), fd_entrada);
printf ("Debug, RAW: %s", buffer);
		sscanf (buffer, "%i", &temp);
		out = temp;
		printf ("Depuración, las balas para este nivel son: %i\n", temp);
		write (fd, &out, sizeof (uint32_t));
		
		fgets (buffer, sizeof (buffer), fd_entrada);
printf ("Debug, RAW: %s", buffer);
		sscanf (buffer, "%i", &temp);
		out = temp;
		printf ("Depuración, los hits requeridos para este nivel son: %i\n", temp);
		write (fd, &out, sizeof (uint32_t));
		
		/* Leer los objetos por nivel */
		fgets (buffer, sizeof (buffer), fd_entrada);
		sscanf (buffer, "%i", &m);
		
		fgets (buffer, sizeof (buffer), fd_entrada);
		sscanf (buffer, "%i", &n);
		
		fgets (buffer, sizeof (buffer), fd_entrada);
		sscanf (buffer, "%i", &o);
		
		out = (o << 16) | (n << 8) | m;
		
		write (fd, &out, sizeof (uint32_t));
		
		/* Leer las coordenadas para cada linea */
		for (h = 0; h < m; h++) {
			
			/* ID del objeto */
			fgets (buffer, sizeof (buffer), fd_entrada);
			sscanf (buffer, "%i", &temp);
			out = temp;
			
			write (fd, &out, sizeof (uint32_t));
			
			/* X del objeto */
			fgets (buffer, sizeof (buffer), fd_entrada);
			sscanf (buffer, "%i", &temp);
			out = temp;
			
			write (fd, &out, sizeof (uint32_t));
			
			/* Y del objeto */
			fgets (buffer, sizeof (buffer), fd_entrada);
			sscanf (buffer, "%i", &temp);
			out = temp;
			
			write (fd, &out, sizeof (uint32_t));
		}
		
		/* Leer las coordenadas para cada bloque */
		for (h = 0; h < n; h++) {
			/* ID del bloque */
			fgets (buffer, sizeof (buffer), fd_entrada);
			sscanf (buffer, "%i", &temp);
			out = temp;
			
			write (fd, &out, sizeof (uint32_t));
			
			/* X del objeto */
			fgets (buffer, sizeof (buffer), fd_entrada);
			sscanf (buffer, "%i", &temp);
			out = temp;
			
			write (fd, &out, sizeof (uint32_t));
			
			/* Y del objeto */
			fgets (buffer, sizeof (buffer), fd_entrada);
			sscanf (buffer, "%i", &temp);
			out = temp;
			
			write (fd, &out, sizeof (uint32_t));
		}
		
		/* Leer las coordenadas para cada target */
		for (h = 0; h < o; h++) {
			/* ID del Target */
			fgets (buffer, sizeof (buffer), fd_entrada);
			sscanf (buffer, "%i", &temp);
			out = temp;
			
			write (fd, &out, sizeof (uint32_t));
			
			fgets (buffer, sizeof (buffer), fd_entrada);
			sscanf (buffer, "%i", &k);
			out = k;
			
			write (fd, &out, sizeof (uint32_t));
			
			for (i = 0; i < k; i++) {
				/* X del objeto */
				fgets (buffer, sizeof (buffer), fd_entrada);
				sscanf (buffer, "%i", &temp);
				out = temp;
			
				write (fd, &out, sizeof (uint32_t));
			
				/* Y del objeto */
				fgets (buffer, sizeof (buffer), fd_entrada);
				sscanf (buffer, "%i", &temp);
				out = temp;
			
				write (fd, &out, sizeof (uint32_t));
				
				/* Velocidad del objeto */
				fgets (buffer, sizeof (buffer), fd_entrada);
				sscanf (buffer, "%i", &temp);
				out = temp;
			
				write (fd, &out, sizeof (uint32_t));
			}
		}
	}
	
	/* Regresar para escribir las posiciones de los niveles */
	lseek (fd, posiciones, SEEK_SET);
	
	for (g = 0; g < niveles; g++) {
		write (fd, &arr_niveles[g], sizeof (uint32_t));
		write (fd, &arr_pos[g], sizeof (off_t));
	}
	
	fclose (fd_entrada);
	close (fd);
	
	free (arr_niveles);
	free (arr_pos);
	
	printf ("Tratando de leer el archivo de vuelta\n");
	
	fd = open (argv[2], O_RDONLY);
	
	read (fd, buffer, strlen ("ASTRO"));
	
	buffer[5] = '\0';
	
	printf ("Recuperando la cabecera: \"%s\"\n", buffer);
	
	read (fd, &out, sizeof (uint32_t));
	temp = out;
	printf ("Versión del archivo: %i\n", temp);
	
	read (fd, &out, sizeof (uint32_t));
	niveles = out;
	printf ("Cantidad de niveles en el archivo: %i\n", niveles);
	
	arr_pos = (off_t *) malloc (sizeof (off_t) * niveles);
	arr_niveles = (uint32_t *) malloc (sizeof (uint32_t) * niveles);
	
	read (fd, &out, sizeof (uint32_t));
	temp = out;
	printf ("El primer nivel que deberías correr es: %i\n", temp);
	
	printf ("Voy a imprimir cada nivel con sus respectivas posiciones dentro del archivo\n");
	for (g = 0; g < niveles; g++) {
		read (fd, &arr_niveles[g], sizeof (uint32_t));
		read (fd, &arr_pos[g], sizeof (off_t));
		
		temp = arr_niveles[g];
		
		printf ("Nivel %i @ POS: %i\n", temp, arr_pos[g]);
	}
	
	printf ("Brincando a los niveles directamente:\n");
	
	for (g = 0; g < niveles; g++) {
		printf ("\tBrincando a %i\n", arr_pos[g]);
		
		lseek (fd, arr_pos[g], SEEK_SET);
		
		read (fd, &out, sizeof (uint32_t));
		temp = out;
		
		printf ("\tEstá el número de nivel %i\n", temp);
		
		read (fd, &out, sizeof (uint32_t));
		temp = out;
		
		printf ("\tEl siguiente nivel es: %i\n", temp);
		
		read (fd, &out, sizeof (uint32_t));
		temp = out;
		printf ("\tLas balas para este nivel son: %i\n", temp);
		
		read (fd, &out, sizeof (uint32_t));
		temp = out;
		printf ("\tLos hits requeridos para este nivel son: %i\n", temp);
		
		read (fd, &out, sizeof (uint32_t));
		m = (out & 0xFF);
		n = (out & 0xFF00) >> 8;
		o = (out & 0xFF0000) >> 16;
		
		printf ("\tListando la cantidad de objetos:\n");
		printf ("\tLineas de fondo: %i\n", m);
		printf ("\tNúmero de bloques: %i\n", n);
		printf ("\tNúmero de targets: %i\n", o);
		
		printf ("\tRecorriendo las lineas:\n");
		for (h = 0; h < m; h++) {
			read (fd, &out, sizeof (uint32_t));
			temp = out;
			
			read (fd, &out, sizeof (uint32_t));
			temp2 = out;
			
			read (fd, &out, sizeof (uint32_t));
			temp3 = out;
			
			printf ("\t\tLinea %i, ID: %i, X: %i, Y: %i\n", h, temp, temp2, temp3);
		}
		
		printf ("\tRecorriendo los bloques:\n");
		for (h = 0; h < n; h++) {
			read (fd, &out, sizeof (uint32_t));
			temp = out;
			
			read (fd, &out, sizeof (uint32_t));
			temp2 = out;
			
			read (fd, &out, sizeof (uint32_t));
			temp3 = out;
			
			printf ("\t\tBloque %i, ID: %i, X: %i, Y: %i\n", h, temp, temp2, temp3);
		}
		
		printf ("\tRecorriendo los targets:\n");
		for (h = 0; h < o; h++) {
			read (fd, &out, sizeof (uint32_t));
			temp = out;
			
			printf ("\t\tTarget %i, ID: %i\n", g, temp);
			
			read (fd, &out, sizeof (uint32_t));
			k = out;
			
			for (i = 0; i < k; i++) {
				read (fd, &out, sizeof (uint32_t));
				temp = out;
				
				read (fd, &out, sizeof (uint32_t));
				temp2 = out;
				
				read (fd, &out, sizeof (uint32_t));
				temp3 = out;
				
				printf ("\t\t\tPunto (%i, %i), vel: %i\n", temp, temp2, temp3);
			}
		}
	}
	
	close (fd);
	
	printf ("Para depuración sizeof (off_t) = %i\n", sizeof (off_t));
	return EXIT_SUCCESS;
}
