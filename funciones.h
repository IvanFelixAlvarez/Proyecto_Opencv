#ifndef FUNCIONES_H_
#define FUNCIONES_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <algorithm>    // sort, max_element, min_element

using namespace cv;

inline bool comp_x(Point p1, Point p2) { return p1.x < p2.x; }
inline bool comp_y(Point p1, Point p2) { return p1.y < p2.y; }

void dilation( const Mat& src, Mat& dilation_dst, int dilation_size, int dilation_elem){
	int dilation_type;
	//Tipo de dilatación
	if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
	else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
	else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement( dilation_type,
	                                   Size( 2*dilation_size + 1, 2*dilation_size+1 ),
	                                   Point( dilation_size, dilation_size ) );
	dilate( src, dilation_dst, element );
}

void erosion( const Mat& src, Mat& erosion_dst, int erosion_size, int erosion_elem){
	int erosion_type;
	//Tipo de erosión
	if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
	else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
	else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement( erosion_type,
	                                   Size( 2*erosion_size + 1, 2*erosion_size+1 ),
	                                   Point( erosion_size, erosion_size ) );
	erode( src, erosion_dst, element );
}

string clasificadorCuadrados(const Mat& imagen){

	double media_azul = 0, media_verde = 0, media_rojo = 0;

	//Calculo las medias para cada componente RGB
	for(int i = 0; i < imagen.rows; ++i)
 		for(int j=0; j < imagen.cols; ++j){

 			media_azul += imagen.at<Vec3b>(i,j)[0]; 
 			media_verde += imagen.at<Vec3b>(i,j)[1];
 			media_rojo += imagen.at<Vec3b>(i,j)[2];
		}

	media_azul /= imagen.rows * imagen.cols;
	media_verde /= imagen.rows * imagen.cols;
	media_rojo /= imagen.rows * imagen.cols;

	//Clasificador
	if(media_azul >= media_verde && media_azul >= media_rojo) return "azul";
	else if (media_verde >= media_azul && media_verde >= media_rojo) return "verde";
	else return "rojo";

}

Point puntoMedio(vector<Point> contorno){

	int x_min = min_element(contorno.begin(), contorno.end(), comp_x)->x; //Calculo la menor 'x' del contorno del cuadrado
	int x_max = max_element(contorno.begin(), contorno.end(), comp_x)->x; //Calculo la mayor 'x' del contorno del cuadrado
	int y_min = min_element(contorno.begin(), contorno.end(), comp_y)->y; //Calculo la menor 'y' del contorno del cuadrado
	int y_max = max_element(contorno.begin(), contorno.end(), comp_y)->y; //Calculo la mayor 'y' del contorno del cuadrado

	return Point( (x_max + x_min) / 2, (y_max + y_min) / 2 ); //Calculo el punto medio

}

int cifrasNumero(int num){ //Algoritmo para conocer el número de cifras de un número dado
	int contador=0;
	while(num > 0)
    {
        num = num / 10;
        contador++;
    }
    return contador;
}

#endif