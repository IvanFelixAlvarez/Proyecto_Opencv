#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "funciones.h"
#include <fstream>
#include <iostream>
#include <algorithm>    // sort, max_element, min_element
#include <map>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;


int main()
{

	vector<string> orden;
	//orden.push_back("azul");
	//orden.push_back("verde");
	orden.push_back("rojo");

	string imagen_entrada;

	FILE* serial = fopen("/dev/ttyACM2", "w");

	if (serial == 0) {
		printf("Fallo al abrir el puerto serie\n");
	}

	cout << "Introduzca el nombre de la imagen de entrada: ";
	cin >> imagen_entrada;

	//Captación
	//0. La captación de la información proviene de una imagen almacenada en disco.
    Mat origen = imread(imagen_entrada, 1);
    Mat src, binaria, bordes, dst;

    vector<vector<Point> > contours;
  	vector<Vec4i> hierarchy;

  	map<int, Mat> matrices_cuadrados;

    //Preprocesamiento.
    //1. Redimensionamos la imagen original. (Hay perdida de información, puesto que reducimos el número de píxeles)
    //Nota: La imagen de destino debe ser diferente de la de origen.
    resize(origen, src, Size(origen.cols/3, origen.rows/3));
 	origen = src.clone(); //La copiamos en origen para mantener la imagen inicial - Se debe clonar la matriz sino, no
 	// se copia, se convierte en una referencia.
  	//src = origen;

 	//imshow("Origen", src);

 	Mat mascara = Mat::zeros( src.size(), CV_8UC3 );

 	//Segmentación
 	//2.1. Se segmenta la imagen mediante la localización de los bordes de la imagen mediante Canny o Laplace.
 	Canny(src, bordes, 30, 120);
 	//imshow("1. Bordes - Canny", bordes);

 	//2.2. Se dilata y erosiona para unir los píxeles de los distintos bordes
 	dilation(bordes, bordes, 1, 0);
 	erosion(bordes, bordes, 1, 0);
 	//imshow("1.1. Dilatacion_Erosion", bordes);

 	//2.3. Se localizan los contornos
  	findContours(bordes, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	if ( !contours.empty() && !hierarchy.empty() )
	    //Se examinan todos los contornos
	    for ( int i=0; i<contours.size(); i++ )
	        // Me quedo con los contornos de nivel 2
	        if ( hierarchy[i][2] != -1 ) {

	        	Mat mascara_1_obj = Mat::zeros( src.size(), CV_8UC3 );

	            Scalar colour1(0, 0, 255);
	            Scalar colour2(255,255,255); //Blanco
	            drawContours( mascara_1_obj, contours, i, colour2, CV_FILLED ); //Con relleno

	            Mat aux(origen.rows, origen.cols, CV_8UC3);
			    aux.setTo(Scalar(0,0,0));
			    origen.copyTo(aux, mascara_1_obj);
	            matrices_cuadrados[i] = aux;

	            drawContours( mascara, contours, i, colour2, CV_FILLED ); //Con relleno
	            drawContours( src, contours, i, colour1); //Sin rellenar
	        }
	//imshow("2. Contornos - Nivel 2 de jerarquia", src);		 			

	//imshow("2-3. Mascara", mascara);
	// Se aplica la máscara de 0 y 1 para extraer todo aquello de la imagen que sea distinto de 0 en la máscara.
    //Mat cuadrados(origen.rows, origen.cols, CV_8UC3);
    //cuadrados.setTo(Scalar(0,0,0));
    //origen.copyTo(cuadrados, mascara);
    //imshow("3. Recorte - Extraemos los cuadrados del fondo ", cuadrados);

    string clase = "";
    Point medio;

    //3. Reconocimiento
    for (std::map<int,Mat>::iterator it=matrices_cuadrados.begin(); it!=matrices_cuadrados.end() && clase != orden[orden.size()-1]; ++it){
    	//ostringstream ss;
    
    	clase = clasificadorCuadrados(it->second);

    	medio = puntoMedio(contours[it->first]);

    	//ss << it->first << " | " << clase << " PM: " << medio;

    	//imshow(ss.str(), it->second);
    }
    circle(src, medio, 7, Scalar(0, 233, 255), CV_FILLED);
    imshow("Hoaras", src);

    //3. Actuación.
    if(clase == orden[orden.size()-1]){
    	circle(src, medio, 7, Scalar(0, 233, 255), CV_FILLED);

    	Point img_medio1(src.cols/2, src.rows);
    	Point img_medio2(src.cols/2, 0);

    	circle(src, img_medio1, 7, Scalar(0, 153, 0), CV_FILLED);
    	line(src, img_medio1, img_medio2, Scalar(0, 153, 0));
    	line(src, img_medio1, medio, Scalar(0, 153, 0));

    	//Vectores directores
    	Point vect_director1(img_medio2.x - img_medio1.x, img_medio2.y - img_medio1.y);
    	Point vect_director2(medio.x - img_medio1.x, medio.y - img_medio1.y);

    	float arriba = abs(vect_director1.x * vect_director2.x + vect_director1.y * vect_director2.y);
    	float abajo = sqrt(pow(vect_director1.x,2) + pow(vect_director1.y,2)) * sqrt(pow(vect_director2.x,2) + pow(vect_director2.y,2));
    		
    	cout << "Arriba: "<<arriba << endl;
    	cout << "Abajo: "<<abajo << endl;

    	//Ángulo entre dos rectas
    	double angulo_ = acos(arriba/abajo) * 180/M_PI;

    	float decimal = angulo_ - (int)angulo_;
    	int angulo;

    	//Redondeamos, ya que a la arduino sólo le pueden llegar números enteros.
    	if(decimal >= 0.5) angulo = ceil(angulo_); //Redondeo para arriba
    	else angulo = floor(angulo_); //Redondeo para abajo

    	cout << "Angulo: " << angulo << endl;

    	/*Envío 4 datos en serie. 
    		- Le digo que vamos a mover el motor paso a paso
    		- Luego si para la derecha o la izquierda
    		- Cifras que tiene el angulo
    		- Le envío el ángulo
    	*/
    	if (serial != 0){
        	fprintf(serial, "%d", 1);
        	if(medio.x < img_medio1.x)
        		fprintf(serial, "%d", 1);
        	else
        		fprintf(serial, "%d", 2);
        	fprintf(serial, "%d", cifrasNumero(angulo));
        	fprintf(serial, "%d", angulo);

        	//Volvemos para atrás
        	fprintf(serial, "%d", 1);
        	if(medio.x < img_medio1.x)
        		fprintf(serial, "%d", 2);
        	else
        		fprintf(serial, "%d", 1);
        	fprintf(serial, "%d", cifrasNumero(angulo));
        	fprintf(serial, "%d", angulo);
    	}
    }

    imshow("Centros", src);

    waitKey();
    return 0;
}