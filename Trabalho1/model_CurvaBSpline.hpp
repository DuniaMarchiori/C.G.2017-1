#ifndef POLIGONO_HPP
#define POLIGONO_HPP

#include "model_Curva.hpp"
#include "model_Coordenada.hpp"
#include "ListaEnc.hpp"

class CurvaBSpline : public Curva {

public:

	//! Construtor
	CurvaBSpline() : Curva() {
		setTipo(CURVABSPLINE);
	}

	//! Construtor
	/*
		/param nome é o nome da curva
	*/
	CurvaBSpline() : Curva(string nome) {
		setTipo(CURVABSPLINE);
	}

	//! Construtor
	/*
		/param nome é o nome da curva
		/param novaLista é a lista de coordenadas da curva
	*/
	CurvaBSpline() : Curva(string nome, ListaEnc<Coordenada*> *novaLista) {
		setTipo(CURVABSPLINE);
	}

	//! Método que retorna os pontos da curva.
	/*!
		/param segmentos em quantos segmentos a curva sera dividida.
		/return uma lista de coordenadas
	*/
	ListaEnc<Coordenada*>* getCurvaFinal(int segmentos) {
		double d = 1/segmentos; // deltinha
		
		// Calcula os pontos da curva e retorna.
		
	}

};

#endif