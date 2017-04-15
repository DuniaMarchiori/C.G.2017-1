#include <gtk/gtk.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include "view_Console.hpp"
#include "view_Desenhista.hpp"

#ifndef VIEW_HPP
#define VIEW_HPP

//using namespace std;
using std::string;
using std::stod;
using std::invalid_argument;
using std::to_string;

class View {

private:
	Console* console; /*!< Uma instância do administrador do Console da interface grafica.*/
	Desenhista* desenhista; /*!< Uma instância da classe que desenha os objetos na tela.*/
	ListaEnc<Coordenada*> *listaCoordsPoligono; /*!< Uma lista de coordenadas para criar novos poligonos.*/
	ListaEnc<Coordenada*> *listaCoordsCurva; /*!< Uma lista de coordenadas para criar novas curvas.*/

	// Objetos relacionados à interface
	GtkWidget *window_Main; /*!< Referência para a janela principal.*/

	GtkListBox *elmnt_List; /*!< Referência para a lista de elementos.*/
	GtkButton *elmnt_Btn_Novo, *elmnt_Btn_Del, *elmnt_Btn_Edit; /*!< Referência para os botões de novo, deletar e editar elementos.*/

	GtkEntry *pos_Txt_Fator; /*!< Referência para a caixa de texto de Fator da movimentação.*/
	GtkButton *pos_Btn_Cima, *pos_Btn_Baixo, *pos_Btn_Esq, *pos_Btn_Dir; /*!< Referência para os botões das quatro direções.*/

	GtkEntry *zoom_Txt_Fator; /*!< Referência para a caixa de texto de Fator de zoom.*/
	GtkButton *zoom_Btn_Menos, *zoom_Btn_Mais; /*!< Referência para os botões de mais e menos zoom.*/

	GtkWidget *viewport_DrawingArea; /*!< A área de desenho*/

	GtkRadioButton *clipping_radio_0, *clipping_radio_1; /*!< Referência para os botões de seleção do tipo de clipping de reta.*/

	GtkTextView *consoleWidget; /*!< Referência para a caixa de texto do console.*/

	GtkWindow *window_NovoElemento; /*!< Referência para a janela de novo elemento.*/

	GtkEntry *textoNomeElemento; /*!< Referência para a caixa de texto de inserção de nome para um elemento.*/
	GtkEntry *textoPontoX, *textoPontoY, *textoRetaInicialX, *textoRetaInicialY,
			*textoRetaFinalX, *textoRetaFinalY, *textoPoligonoX, *textoPoligonoY, *textoCurvaX, *textoCurvaY; /*!< Referência para as caixas de texto que recebem valores de coordenadas.*/

	GtkButton *poligono_Btn_Add, *poligono_Btn_Del; /*!< Referência para os botões de adicionar e deletar coordenadas na criação de um poligono.*/
	GtkListBox *poligono_Listbox; /*!< Referência para a listbox com as coordenadas do poligono.*/
	GtkCheckButton *poligono_Preenchido; /*!< Caixa que marca se o poligono cirado deve ou não ser preenchido.*/

	GtkButton *curva_Btn_Add, *curva_Btn_Del; /*!< Referência para os botões de adicionar e deletar coordenadas na criação de uma curva.*/
	GtkListBox *curva_Listbox; /*!< Referência para a listbox com as coordenadas da curva.*/
	GtkRadioButton *curva_radio_0, *curva_radio_1; /*!< Referência para os botões de seleção do tipo de clipping de reta.*/

	GtkNotebook *novoElmnt_Notebook; /*!< Referência para o notebook na criação de elemento.*/

	GtkWindow *window_EditElemento; /*!< Referência para a janela de editar elemento.*/

	GtkEntry *editElmnt_trans_X, *editElmnt_trans_Y; /*!< Referência para a caixa de texto das coordenadas de translação.*/

	GtkEntry *editElmnt_escal_X, *editElmnt_escal_Y; /*!< Referência para a caixa de texto da quantidade de escalonamento.*/

	GtkEntry *editElmnt_rot_angulo; /*!< Referência para a caixa de texto do angulo de rotação.*/

	GtkRadioButton *editElmnt_radio_0, *editElmnt_radio_1, *editElmnt_radio_2; /*!< Referência para os botões de seleção do tipo de rotação.*/

	GtkEntry *editElmnt_rot_X, *editElmnt_rot_Y; /*!< Referência para a caixa de texto do ponto arbitrário da rotação.*/

	GtkNotebook *editElmnt_Notebook; /*!< Referência para o notebook na edição de elemento.*/

	GtkButton *editElmnt_aplicar; /*!< Botão que confirma a edição de um elemento.*/

	//! Método que obtem um valor numérico de um GtkEntry
    /*!
        Converte o valor contido em um GtkEntry para um valor double.
        /param area é a GtkEntry cujo valor sera convertido.
		/return um valor double correspondente ao valor em string contido na GtkEntry.
    */
	double getFator(GtkEntry* area) {
		double fator = 0;

		try {
			fator = stod(gtk_entry_get_text(area));
		} catch (const invalid_argument& e) {
			throw -1;
		}

		if (fator != 0) {
			return fator;
		} else {
			throw -2;
		}
	}

public:

	//! Método que inicializa todos os atributos desta classe.
    /*!
        Liga os atributos aos seus correspondentes widgets contidos na janela criada no glade,
		assim como inicializa outros elementos necessários para o funcionamento da interface grafica.
    */
	void inicializarComponentes(int argc, char *argv[]) {
		GtkBuilder  *gtkBuilder;
		gtk_init(&argc, &argv);

		//gtkBuilder = gtk_builder_new();
		//gtk_builder_add_from_file(gtkBuilder, "janela.glade", NULL);
		gtkBuilder = gtk_builder_new_from_file("janela.glade");

		window_Main = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Window_Main"));

		viewport_DrawingArea = GTK_WIDGET(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Viewport_DrawingArea"));

		elmnt_List = GTK_LIST_BOX(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Elmnt_List"));
		elmnt_Btn_Del = GTK_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Elmnt_Btn_Del"));
		gtk_widget_set_sensitive ((GtkWidget*) elmnt_Btn_Del, FALSE); // Esse botão começa desativado.
		elmnt_Btn_Edit = GTK_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Elmnt_Btn_Edit"));
		gtk_widget_set_sensitive ((GtkWidget*) elmnt_Btn_Edit, FALSE); // Esse botão começa desativado.

		pos_Txt_Fator = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Pos_Txt_Fator"));
		gtk_entry_set_text(pos_Txt_Fator, "1");

		zoom_Txt_Fator = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Zoom_Txt_Fator"));
		gtk_entry_set_text(zoom_Txt_Fator, "1");
		window_NovoElemento = GTK_WINDOW(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Window_NovoElmnt"));
		g_signal_connect (window_NovoElemento, "delete-event", G_CALLBACK (gtk_widget_hide_on_delete), NULL); // Essa janela não se deletará ao fechá-la, apenas esconderá.

		textoNomeElemento = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Nome"));

		textoPontoX = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Ponto_X"));
		textoPontoY = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Ponto_Y"));

		textoRetaInicialX = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Reta_X1"));
		textoRetaInicialY = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Reta_Y1"));
		textoRetaFinalX = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Reta_X2"));
		textoRetaFinalY = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Reta_Y2"));

		textoPoligonoX = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Pol_X"));
		textoPoligonoY = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Pol_Y"));
		poligono_Listbox = GTK_LIST_BOX(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Listbox_Pol"));
		poligono_Preenchido = GTK_CHECK_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Pol_Preenchido"));
		poligono_Btn_Del = GTK_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Pol_Del"));
		gtk_widget_set_sensitive ((GtkWidget*) poligono_Btn_Del, FALSE); // Esse botão começa desativado.

		textoCurvaX = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Curv_X"));
		textoCurvaY = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Curv_Y"));
		curva_Listbox = GTK_LIST_BOX(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Listbox_Curv"));
		curva_Btn_Del = GTK_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Curv_Del"));
		gtk_widget_set_sensitive ((GtkWidget*) curva_Btn_Del, FALSE); // Esse botão começa desativado.
		curva_radio_0 = GTK_RADIO_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Curv_Radio_0"));
		curva_radio_1 = GTK_RADIO_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Curv_Radio_1"));

		novoElmnt_Notebook = GTK_NOTEBOOK(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "NovoElmnt_Notebook"));
		consoleWidget = GTK_TEXT_VIEW(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Console_Text"));

		clipping_radio_0 = GTK_RADIO_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Clipping_Radio_0"));
		clipping_radio_1 = GTK_RADIO_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Clipping_Radio_1"));

		window_EditElemento = GTK_WINDOW(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "Window_EditElmnt"));
		g_signal_connect (window_EditElemento, "delete-event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

		editElmnt_trans_X = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Trans_X"));
		editElmnt_trans_Y = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Trans_Y"));
		editElmnt_escal_X = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Escal_Fator_X"));
		editElmnt_escal_Y = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Escal_Fator_Y"));
		editElmnt_rot_angulo = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Rot_Angulo"));

		editElmnt_radio_0 = GTK_RADIO_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Rot_Op_0"));
		editElmnt_radio_1 = GTK_RADIO_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Rot_Op_1"));
		editElmnt_radio_2 = GTK_RADIO_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Rot_Op_2"));
		editElmnt_rot_X = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Rot_X"));
		editElmnt_rot_Y = GTK_ENTRY(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Rot_Y"));
		editElmnt_Notebook = GTK_NOTEBOOK(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Notebook"));

		editElmnt_aplicar = GTK_BUTTON(gtk_builder_get_object(GTK_BUILDER(gtkBuilder), "EditElmnt_Aplicar"));

		console = new Console(consoleWidget);
		desenhista = new Desenhista();

		gtk_builder_connect_signals(gtkBuilder, NULL);
		gtk_widget_show_all(window_Main);
		gtk_main ();

	}

	//! Método que adiciona a uma GtkListBox um novo elemento.
	/*!
		/param listBox é a GtkListBox que queremos adicionar o elemento.
		/param nome é o texto a ser adicionado.
	*/
	void addToListBox(GtkListBox* listBox, string nome) {
		GtkWidget* row = gtk_list_box_row_new();
		GtkWidget* label = gtk_label_new(nome.c_str());
		gtk_container_add((GtkContainer*) listBox, label);
		gtk_widget_show_all((GtkWidget*) listBox);
	}

	//! Método que remove de uma GtkListBox a linha selecionada e retorna seu índice.
	/*!
		/param list é a GtkListBox que queremos deletar o elemento selecionado.
		/return o valor do índice do elemento removido.
	*/
	int getIndexElementoDeletado(GtkListBox* list) {
		GtkListBoxRow* row = gtk_list_box_get_selected_row (list);
		if (row != NULL) {
			int index = gtk_list_box_row_get_index(row);
			gtk_container_remove((GtkContainer*) list, (GtkWidget*) row);
			return index;
		}
		return 0;
	}

	//! Método que limpa a caixa de texto do nome da janela de novo objeto.
	void limparTextoNomeNovoElmnt() {
		gtk_entry_set_text(textoNomeElemento, "");
	}

	//! Método que limpa as caixas de texto de ponto da janela de novo objeto.
	void limparTextoNovoPonto() {
		limparTextoNomeNovoElmnt();
		gtk_entry_set_text(textoPontoX, "");
		gtk_entry_set_text(textoPontoY, "");
	}

	//! Método que limpa as caixas de texto de reta da janela de novo objeto.
	void limparTextoNovaReta() {
		limparTextoNomeNovoElmnt();
		gtk_entry_set_text(textoRetaInicialX, "");
		gtk_entry_set_text(textoRetaInicialY, "");
		gtk_entry_set_text(textoRetaFinalX, "");
		gtk_entry_set_text(textoRetaFinalY, "");
	}

	//! Método que limpa as caixa de texto da coordenada de um novo poligono.
	void limparTextoCoordPoligono() {
		gtk_entry_set_text(textoPoligonoX, "");
		gtk_entry_set_text(textoPoligonoY, "");
	}


	//! Método que limpa as caixas de texto de polígono da janela de novo objeto.
	void limparTextoNovoPoligono() {
		limparTextoNomeNovoElmnt();
		limparTextoCoordPoligono();
		// Limpa a listBox
		GtkListBoxRow* row = gtk_list_box_get_row_at_index (poligono_Listbox, 0);
		while (row != NULL) {
			gtk_container_remove((GtkContainer*) poligono_Listbox, (GtkWidget*) row);
			row = gtk_list_box_get_row_at_index (poligono_Listbox, 0);
		}
	}

	//! Método que limpa as caixa de texto da coordenada de um novo poligono.
	void limparTextoCoordCurva() {
		gtk_entry_set_text(textoCurvaX, "");
		gtk_entry_set_text(textoCurvaY, "");
	}


	//! Método que limpa as caixas de texto de polígono da janela de novo objeto.
	void limparTextoNovaCurva() {
		limparTextoNomeNovoElmnt();
		limparTextoCoordCurva();
		// Limpa a listBox
		GtkListBoxRow* row = gtk_list_box_get_row_at_index (curva_Listbox, 0);
		while (row != NULL) {
			gtk_container_remove((GtkContainer*) curva_Listbox, (GtkWidget*) row);
			row = gtk_list_box_get_row_at_index (curva_Listbox, 0);
		}
	}

	//! Método que retorna o valor do ponto máximo do viewport.
	/*!
		/return a coordenada do ponto máximo.
	*/
	Coordenada* getViewportMax () {
		Coordenada* coord = new Coordenada();
		coord->setX(gtk_widget_get_allocated_width(viewport_DrawingArea));
		coord->setY(gtk_widget_get_allocated_height(viewport_DrawingArea));
		return coord;
	}

	//! Método que limpa a tela(pintando-a de branco).
	void clear_surface (){
		desenhista->clear_surface();
	}

	//! Desenha a margem para a observação do clipping.
	void desenhaMargem() {
		desenhista->desenhaMargem(getViewportMax());
	}

	//! Método que altera a surface do cairo usada.
	/*!
		/param cr é um objeto cairo_t utilizado para desenhar.
	*/
	void modifica_surface(cairo_t *cr){
		desenhista->modifica_surface(cr);
	}

	//! Método que inicializa a surface do cairo usada.
	/*!
		/param widget é a window que será feita a nova "surface".
	*/
	void nova_surface(GtkWidget *widget) {
		desenhista->nova_surface(widget);
	}

	//! Método que repassa a instrução de desenhar um ponto para o desenhista.
	/*!
		/param coord é a coordenada do ponto a ser desenhado.
	*/
	void desenhaPonto(Coordenada* coord) {
		desenhista->desenhaPonto(coord);
		gtk_widget_queue_draw ((GtkWidget*) window_Main);
	}

	//! Método que repassa a instrução de desenhar uma reta para o desenhista.
	/*!
		/param coordInicial é a coordenada inicial da reta.
		/param coordFinal é a coordenada final da reta.
	*/
	void desenhaReta(Coordenada* coordInicial, Coordenada* coordFinal) {
		desenhista->desenhaReta(coordInicial, coordFinal);
		gtk_widget_queue_draw ((GtkWidget*) window_Main);
	}

	//! Método que repassa a instrução de desenhar um poligono para o desenhista.
	/*!
		/param lista é a lista de coordenadas do poligono a ser desenhado.
		/param preenchido é um valor booleado que diz se este poligono deve ser preenchido ou não.
	*/
	void desenhaPoligono(ListaEnc<Coordenada*>* lista, bool preenchido) {
		desenhista->desenhaPoligono(lista, preenchido);
		gtk_widget_queue_draw ((GtkWidget*) window_Main);
	}

	//! Método que insere em uma lista as coordenadas do polígono a ser criado.
	void inserirCoordListaEncPoligono() {
		// Pega coordenadas
		string polX = gtk_entry_get_text(textoPoligonoX);
		string polY = gtk_entry_get_text(textoPoligonoY);

		/// Se os campos de coordenada não estão em branco
		if ( !(polX.empty()) && !(polY.empty()) ) {
			// Cria novo objeto
			Coordenada* c = new Coordenada();
			// Verifica se os campos são números
			try {
				// stod = string to double
				c->setX(stod(polX));
				c->setY(stod(polY));
			} catch (const invalid_argument& e) {
				console->inserirTexto("ERRO: coordenadas devem ser valores numéricos.");
				throw -1;
			}
			listaCoordsPoligono->adiciona(c);

			string nomeNaLista = "(" + polX + "," + polY + ")";
			addToListBox(poligono_Listbox, nomeNaLista);
		} else {
			console->inserirTexto("ERRO: não é possível inserir coordenada sem valor X ou Y.");
			throw -2;
		}
	}

	//! Método que insere em uma lista as coordenadas da curva a ser criada.
	void inserirCoordListaEncCurva() {
		// Pega coordenadas
		string polX = gtk_entry_get_text(textoCurvaX);
		string polY = gtk_entry_get_text(textoCurvaY);

		/// Se os campos de coordenada não estão em branco
		if ( !(polX.empty()) && !(polY.empty()) ) {
			// Cria novo objeto
			Coordenada* c = new Coordenada();
			// Verifica se os campos são números
			try {
				// stod = string to double
				c->setX(stod(polX));
				c->setY(stod(polY));
			} catch (const invalid_argument& e) {
				console->inserirTexto("ERRO: coordenadas devem ser valores numéricos.");
				throw -1;
			}
			listaCoordsCurva->adiciona(c);

			string nomeNaLista = "(" + polX + "," + polY + ")";
			addToListBox(curva_Listbox, nomeNaLista);
		} else {
			console->inserirTexto("ERRO: não é possível inserir coordenada sem valor X ou Y.");
			throw -2;
		}
	}

	//! Método que fecha o programa.
	void fecharPrograma() {
		gtk_main_quit();
	}

	//! Método que altera a sensibilidade do botao de deletar.
	/*!
		/param valor é o novo valor da sensibilidade do botao (TRUE ou FALSE).
	*/
	void setElmnt_Btn_DelSensitive(gboolean valor) {
		gtk_widget_set_sensitive ((GtkWidget*) elmnt_Btn_Del, valor);
	}

	//! Método que altera a sensibilidade do botao de editar.
	/*!
		/param valor é o novo valor da sensibilidade do botao (TRUE ou FALSE).
	*/
	void setElmnt_Btn_EditSensitive(gboolean valor) {
		gtk_widget_set_sensitive ((GtkWidget*) elmnt_Btn_Edit, valor);
	}

	//! Metodo que exibe a janela de novo elemento.
	void elmnt_Btn_Novo_Clicado() {
		listaCoordsPoligono = new ListaEnc<Coordenada*>();
		listaCoordsCurva = new ListaEnc<Coordenada*>();
		gtk_widget_show((GtkWidget*) window_NovoElemento);
	}

	//! Metodo que exibe a janela de editar elemento.
	void elmnt_Btn_Edit_Clicado() {
		gtk_widget_show((GtkWidget*) window_EditElemento);
	}

	//! Metodo que pega o elemento atualmente seleciona na listbox e o deleta.
	/*!
		/return o indice na lista encadeada onde se encontra esse elemento.
	*/
	int deletarElemento(){
		GtkListBoxRow* row = gtk_list_box_get_selected_row (elmnt_List);
		if (row != NULL) {
			int index = gtk_list_box_row_get_index(row);
			gtk_container_remove((GtkContainer*) elmnt_List, (GtkWidget*) row);
			return index;
		} else {
			throw -1;
		}
	}

	//! Metodo que repassa a mensagem para o console para que esta seja exibida.
	/*!
		/param texto é o texto que sera adicionado ao final do console.
	*/
	void inserirTextoConsole(const gchar *texto) {
		console->inserirTexto(texto);
	}

	//! Metodo que retorna o valor numérico contido na caixa de Fator na regição da edição de posição.
	/*!
		/return o fator da caixa na região da posição.
	*/
	double getFatorPosicao() {
		try {
			return getFator(pos_Txt_Fator);
		} catch (int erro) {
			if (erro == -1) {
				console->inserirTexto("ERRO: Você deve inserir um valor numérico como fator de movimento");
				gtk_entry_set_text(pos_Txt_Fator, "1");
				throw -1;
			} else if (erro == -2) {
				console->inserirTexto("ERRO: Você deve inserir um valor diferente de 0 como fator de movimento");
				gtk_entry_set_text(pos_Txt_Fator, "1");
				throw -2;
			}
		}
	}

	//! Metodo que retorna o valor numérico contido na caixa de Fator de Zoom.
	/*!
		/return o fator de zoom.
	*/
	double getFatorZoom() {
		try {
			return getFator(zoom_Txt_Fator);
		} catch (int erro) {
			if (erro == -1) {
				console->inserirTexto("ERRO: Você deve inserir um valor numérico como fator de zoom");
				gtk_entry_set_text(zoom_Txt_Fator, "1");
				throw -1;
			} else if (erro == -2) {
				console->inserirTexto("ERRO: Você deve inserir um valor diferente de 0 como fator de zoom");
				gtk_entry_set_text(zoom_Txt_Fator, "1");
				throw -2;
			}
		}
	}

	//! Metodo que retorna o tipo de clipping de reta.
	/*!
		/return inteiro correspondendo à um dos dois tipos de clipping de reta.
	*/
	int getTipoClippingReta() {
		if (gtk_toggle_button_get_active((GtkToggleButton*) clipping_radio_0)) {
			return 0;
		} else {
			return 1;
		}
	}

	// ------------------------------------------------------------------------------------------------
	// Comandos Da Janela de Novo Elemento


	//! Metodo que retorna o valor contido na caixa de Nome do novo elemento.
	/*!
		/return o nome do novo elemento.
	*/
	string getNomeElemento () {
		return gtk_entry_get_text(textoNomeElemento);
	}

	//! Metodo que retorna o tipo do novo elemento.
	/*!
		/return inteiro correspondendo a qual o tipo do novo elemento que esta sendo criaddo.
	*/
	int getTipoNovoElemento () {
		return gtk_notebook_get_current_page(novoElmnt_Notebook);
	}

	//! Metodo que retorna o valor contido na caixa de coordenada X da criação de um ponto.
	/*!
		/return a coordenada X desse novo ponto.
	*/
	string getCoordXNovoPonto() {
		string entradaX = gtk_entry_get_text(textoPontoX);
		return entradaX;
	}

	//! Metodo que retorna o valor contido na caixa de coordenada Y da criação de um ponto.
	/*!
		/return a coordenada Y desse novo ponto.
	*/
	string getCoordYNovoPonto() {
		string entradaY = gtk_entry_get_text(textoPontoY);
		return entradaY;
	}

	//! Metodo que retorna o valor contido na caixa de coordenada X Inicial da criação de uma reta.
	/*!
		/return a coordenada X Inicial dessa nova reta.
	*/
	string getCoordIniXNovaReta() {
		string entradaXini = gtk_entry_get_text(textoRetaInicialX);
		return entradaXini;
	}

	//! Metodo que retorna o valor contido na caixa de coordenada Y Inicial da criação de uma reta.
	/*!
		/return a coordenada Y Inicial dessa nova reta.
	*/
	string getCoordIniYNovaReta() {
		string entradaYini = gtk_entry_get_text(textoRetaInicialY);
		return entradaYini;
	}

	//! Metodo que retorna o valor contido na caixa de coordenada X Final da criação de uma reta.
	/*!
		/return a coordenada X Final dessa nova reta.
	*/
	string getCoordFinXNovaReta() {
		string entradaXfin = gtk_entry_get_text(textoRetaFinalX);
		return entradaXfin;
	}

	//! Metodo que retorna o valor contido na caixa de coordenada Y Final da criação de uma reta.
	/*!
		/return a coordenada Y Final dessa nova reta.
	*/
	string getCoordFinYNovaReta() {
		string entradaYfin = gtk_entry_get_text(textoRetaFinalY);
		return entradaYfin;
	}

	//! Metodo que retorna o indice do elemento selecionado na ListBox de elementos.
	/*!
		/return o indice do elemento selecionado.
	*/
	int getIndexLinhaElementosSelecionada() {
		return gtk_list_box_row_get_index(gtk_list_box_get_selected_row (elmnt_List));
	}

	//! Metodo que retorna a lista de coordenadas na criação de um novo poligono.
	/*!
		/return a lista de coordenadas do novo poligono.
	*/
	ListaEnc<Coordenada*>* getListaCoordsPoligono() {
		return listaCoordsPoligono;
	}

	//! Metodo que adiciona o nome de um elemento à listbox de elementos.
	/*!
		/param nome o nome desse novo elemento.
		/param tipo é o tipo do elemento.
	*/
	void adicionaElementoListbox(string nome, string tipo) {
		addToListBox(elmnt_List, nome + " (" + tipo + ")");
	}

	//! Método que altera a sensibilidade do botao de deletar na criação de poligonos.
	/*!
		/param valor é o novo valor da sensibilidade do botao (TRUE ou FALSE).
	*/
	void setPoligono_Btn_DelSensitive(gboolean valor) {
		gtk_widget_set_sensitive((GtkWidget*) poligono_Btn_Del, valor);
	}

	//! Método que altera a sensibilidade do botao de deletar na criação de curvas.
	/*!
		/param valor é o novo valor da sensibilidade do botao (TRUE ou FALSE).
	*/
	void setCurva_Btn_DelSensitive(gboolean valor) {
		gtk_widget_set_sensitive((GtkWidget*) curva_Btn_Del, valor);
	}
	
	//! Metodo que retorna o tipo de curva sendo criada.
	/*!
		/return inteiro correspondendo à um dos dois tipos de curva.
	*/
	int getTipoCurva() {
		if (gtk_toggle_button_get_active((GtkToggleButton*) curva_radio_0)) {
			return 0;
		} else {
			return 1;
		}
	}

	//! Método que passa o foco do cursor para a caixa de Nome na janela de novo elemento.
	void focusNome() {
		gtk_widget_grab_focus((GtkWidget*) textoNomeElemento);
	}

	//! Método que passa o foco do cursor para a caixa de Coordenada X na janela de criação de poligono.
	void focusCoordPoligono() {
		gtk_widget_grab_focus((GtkWidget*) textoPoligonoX);
	}

	//! Método que passa o foco do cursor para a caixa de Coordenada X na janela de criação de curva.
	void focusCoordCurva() {
		gtk_widget_grab_focus((GtkWidget*) textoCurvaX);
	}

	//! Método que deleta a coordenada selecionada na janela de criação de poligono.
	void deletarCoordPoligono() {
		listaCoordsPoligono->retiraDaPosicao(getIndexElementoDeletado(poligono_Listbox));
		setPoligono_Btn_DelSensitive(FALSE);
	}

	//! Método que deleta a coordenada selecionada na janela de criação de poligono.
	void deletarCoordCurva() {
		listaCoordsCurva->retiraDaPosicao(getIndexElementoDeletado(curva_Listbox));
		setCurva_Btn_DelSensitive(FALSE);
	}

	//! Método que retorna se a caixa de Preenchimento na criação de poligono esta marcada.
	/*!
		/return true se a caixa esta marcada.
	*/
	bool poligonoPreenchido() {
		return gtk_toggle_button_get_active((GtkToggleButton*) poligono_Preenchido);
	}

	//! Método que reinicia todos os valores da janela de novo elemento para seus valores iniciais.
	void resetarJanelaNovoElemento() {
		limparTextoNomeNovoElmnt();
		limparTextoNovoPonto();
		limparTextoNovaReta();
		limparTextoNovoPoligono();
		limparTextoNovaCurva();
		setPoligono_Btn_DelSensitive(FALSE);
		setCurva_Btn_DelSensitive(FALSE);
		gtk_toggle_button_set_active((GtkToggleButton*) poligono_Preenchido, FALSE);
		gtk_notebook_set_current_page(novoElmnt_Notebook, 0);
		free(listaCoordsPoligono);
		free(listaCoordsCurva);
	}

	//! Método que reinicia a lista de coordenadas na janela de ciração de poligono.
	void resetarListaCoordenadasPoligono() {
		listaCoordsPoligono = new ListaEnc<Coordenada*>();
	}

	// ------------------------------------------------------------------------------------------------
	// Comandos Da Janela de Editar

	//! Método que limpa as caixas de texto de translação da janela de editar objeto.
	void limparTextoTranslacao() {
		gtk_entry_set_text(editElmnt_trans_X, "");
		gtk_entry_set_text(editElmnt_trans_Y, "");
	}

	//! Método que passa o foco do cursor para a caixa de Coordenada X na janela de translação.
	void focusTransX() {
		gtk_widget_grab_focus((GtkWidget*) editElmnt_trans_X);
	}

	//! Método que limpa as caixas de texto de escalonamento da janela de editar objeto.
	void limparTextoEscalonamento() {
		gtk_entry_set_text(editElmnt_escal_X, "");
		gtk_entry_set_text(editElmnt_escal_Y, "");
	}

	//! Método que passa o foco do cursor para a caixa de Coordenada X na janela de escalonamento.
	void focusEscalX() {
		gtk_widget_grab_focus((GtkWidget*) editElmnt_escal_X);
	}

	//! Método que limpa as caixas de texto de rotacao da janela de editar objeto.
	void limparTextoRotacao() {
		gtk_entry_set_text(editElmnt_rot_X, "");
		gtk_entry_set_text(editElmnt_rot_Y, "");
		gtk_entry_set_text(editElmnt_rot_angulo, "");
		gtk_toggle_button_set_active((GtkToggleButton*) editElmnt_radio_0, TRUE);
		gtk_toggle_button_set_active((GtkToggleButton*) editElmnt_radio_1, FALSE);
		gtk_toggle_button_set_active((GtkToggleButton*) editElmnt_radio_2, FALSE);
	}

	//! Método que passa o foco do cursor para a caixa de valor do ângulo na janela de rotação.
	void focusRotAngulo() {
		gtk_widget_grab_focus((GtkWidget*) editElmnt_rot_angulo);
	}

	//! Método que restaura a janela de editar elementos ao seu estado original.
	void resetarJanelaEditElemento() {
		limparTextoTranslacao();
		limparTextoEscalonamento();
		limparTextoRotacao();
		gtk_notebook_set_current_page(editElmnt_Notebook, 0);
		focusTransX();
	}

	//! Metodo que retorna o tipo de transformação.
	/*!
		/return inteiro correspondendo a qual o tipo de transformação esta sendo aplicada.
	*/
	int getTipoTransformacao () {
		return gtk_notebook_get_current_page(editElmnt_Notebook);
	}

	//! Metodo que retorna o valor numérico contido na caixa de Translação em X.
	/*!
		/return a quantidade de translação em x.
	*/
	double getTransX() {
		try {
			return getFator(editElmnt_trans_X);
		} catch (int erro) {
			if (erro == -1) {
				//gtk_entry_set_text(editElmnt_trans_X, "0");
				throw -1;
			} else if (erro == -2) {
				return 0;
			}
		}
	}

	//! Metodo que retorna o valor numérico contido na caixa de Translação em Y.
	/*!
		/return a quantidade de translação em y.
	*/
	double getTransY() {
		try {
			return getFator(editElmnt_trans_Y);
		} catch (int erro) {
			if (erro == -1) {
				//gtk_entry_set_text(editElmnt_trans_Y, "0");
				throw -1;
			} else if (erro == -2) {
				return 0;
			}
		}
	}

	//! Metodo que retorna o valor numérico contido na caixa de fator de Escala em X.
	/*!
		/return o fator de escala em X.
	*/
	double getEscalFatorX() {
		try {
			return getFator(editElmnt_escal_X);
		} catch (int erro) {
			if (erro == -1) {
				//gtk_entry_set_text(editElmnt_escal_X, "1");
				throw -1;
			} else if (erro == -2) {
				//gtk_entry_set_text(editElmnt_escal_X, "1");
				throw -2;
			}
		}
	}

	//! Metodo que retorna o valor numérico contido na caixa de fator de Escala em Y.
	/*!
		/return o fator de escala em Y.
	*/
	double getEscalFatorY() {
		try {
			return getFator(editElmnt_escal_Y);
		} catch (int erro) {
			if (erro == -1) {
				//gtk_entry_set_text(editElmnt_escal_Y, "1");
				throw -1;
			} else if (erro == -2) {
				//gtk_entry_set_text(editElmnt_escal_Y, "1");
				throw -2;
			}
		}
	}

	//! Metodo que retorna o valor numérico contido na caixa de angulo de rotacao.
	/*!
		/return o angulo a ser rotacionado.
	*/
	double getRotAngulo() {
		try {
			return getFator(editElmnt_rot_angulo);
		} catch (int erro) {
			if (erro == -1) {
				//gtk_entry_set_text(editElmnt_rot_angulo, "1");
				throw -1;
			} else if (erro == -2) {
				//gtk_entry_set_text(editElmnt_rot_angulo, "1");
				throw -2;
			}
		}
	}

	//! Metodo que retorna a relatividade da rotacao.
	/*!
		/return inteiro correspondendo à uma das três opções de relatividade para a rotação.
	*/
	int getRelatividadeRotacao() {
		if (gtk_toggle_button_get_active((GtkToggleButton*) editElmnt_radio_0)) {
			return 0;
		} else if (gtk_toggle_button_get_active((GtkToggleButton*) editElmnt_radio_1)) {
			return 1;
		} else {
			return 2;
		}
	}

	//! Metodo que retorna o valor numérico contido na caixa de posição X na rotação relativa à um ponto.
	/*!
		/return a coordenada X desse ponto.
	*/
	double getRotRelativoAX() {
		try {
			return getFator(editElmnt_rot_X);
		} catch (int erro) {
			if (erro == -1) {
				//gtk_entry_set_text(editElmnt_rot_X, "1");
				throw -1;
			} else if (erro == -2) {
				return 0;
			}
		}
	}

	//! Metodo que retorna o valor numérico contido na caixa de posição Y na rotação relativa à um ponto.
	/*!
		/return a coordenada Y desse ponto.
	*/
	double getRotRelativoAY() {
		try {
			return getFator(editElmnt_rot_Y);
		} catch (int erro) {
			if (erro == -1) {
				//gtk_entry_set_text(editElmnt_rot_Y, "1");
				throw -1;
			} else if (erro == -2) {
				return 0;
			}
		}
	}

};

#endif