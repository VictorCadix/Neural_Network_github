#include "stdafx.h"

#include "Red.h"

using namespace std;

Red::Red()
{
	this->nLayers = 0;
	this->nNeuronas = 0;
	this->structure = NULL;
	this->genes = NULL;
	this->avgError = 9999;
	this->result = NULL;
}

Red::Red(int *structure_in) {
	this->structure = structure_in;
	this->nLayers = structure[0];
	this->avgError = 9999;
	this->result = new double[structure[nLayers]];

#ifdef PRINTDEBUG 
	cout << "Net with " << this->nLayers << " layers" << endl;
#endif

	// Creation du reseau

	for (int c = 0; c < this->nLayers; c++) {
		this->layers.push_back(Layer());
		for (int n = 0; n < this->structure[c + 1]; n++) {
			if (c != 0)
				this->layers.back().push_back(Neurona(c, n, structure[c]));
			else //input neuron
				this->layers.back().push_back(Neurona(c, n, 0));
		}

		if (c < this->nLayers - 1) //All layers but the last one
			this->layers.back().push_back(Neurona(c, this->structure[c + 1], -1));
	}

	//Weights initialization
	
	this->genes = new double **[nLayers-1];
	for (int i = 0; i < this->nLayers-1; i++) {
		this->genes[i] = new double*[structure[i+1]+1]; //[]+1 for bias
		for (int j = 0; j < this->structure[i+1]+1; j++) { //same
			this->genes[i][j] = new double[structure[i+2]];
		}
	}


	for (int i = 0; i < this->nLayers-1; i++) {
		for (int j = 0; j < this->structure[i+1]+1; j++) { //[]+1 for bias
			for (int k = 0; k < this->structure[i+2]; k++) {
				this->genes[i][j][k] = i + j + k; // changer i+j par rand ou similaire
			}
		}
	}
}

void Red::inputs(double *in){
	#ifdef PRINTDEBUG 
		cout<<endl<<"- Inputs -"<<endl;
	#endif
	
	for( int e=0; e<this->structure[1]; e++){
		this->layers[0][e].setValor(in[e]);
		#ifdef PRINTDEBUG 
			cout<<"Input "<<e<<" = "<<this->layers[0][e].getValor()<<endl;
		#endif
	}
	#ifdef PRINTDEBUG 
		cout<<endl;
	#endif
}

void Red::print(int mode) {
	if (mode == 0 || mode == 1) {
		for (int c = 0; c < this->nLayers; c++) {
			for (int n = 0; n < this->structure[c + 1] + 1; n++) {
				if (c == nLayers - 1 && n == structure[c + 1])
					break;
				this->layers[c][n].print();
				//cout<<"Neurona en : capa "<<Capas[c][n].posCapa<<", pos "<<Capas[c][n].posNeurona<<endl;
			}
		}
	}
	//Weights print
	if (mode == 0 || mode == 2) {
		cout << endl << " Weights tensor" << endl;
		for (int i = 0; i < this->nLayers - 1; i++) {
			cout << endl << "Weight matrix " << i + 1 << endl;
			for (int j = 0; j < this->structure[i + 1] + 1; j++) { //[]+1 for bias
				for (int k = 0; k < this->structure[i + 2]; k++) {
					cout << this->genes[i][j][k] << '\t';
				}
				cout << endl;
			}
		}
	}

	//Result print
	cout << "Result = " << getResult() << endl;
}

void Red::geneForwardProp() {
	for (int n = 0; n < nLayers - 1; n++) {
		for (int i = 0; i < structure[n + 2]; i++) {
			double result = 0;
			for (int j = 0; j < structure[n + 1]+1; j++) { //[]+1 for bias
				result += layers[n][j].getValor()*genes[n][j][i];
			}
			layers[n + 1][i].setValor(result);
			layers[n + 1][i].activate_sigmoid();
		}
	}
#ifdef PRINTDEBUG 
	cout << endl << "Result = " << getResult() << endl;
#endif
}

double Red::getResult() {
	for (int i = 0; i < structure[nLayers]; i++) {
		result[i] = layers[nLayers - 1][i].getValor();
	}
	return (layers[nLayers-1][0].getValor()); // Only returns the first output
}

double *** Red::getGenes() {
	return genes;
}

void Red::setGenes(double***genes) {
	this->genes = genes;
}

void Red::setRandomGenes() {
	for (int i = 0; i < this->nLayers - 1; i++) {
		for (int j = 0; j < this->structure[i + 1]+1; j++) { //[]+1 for bias
			for (int k = 0; k < this->structure[i + 2]; k++) {
				this->genes[i][j][k] = (double)(rand() % 200) / 100 - 1;
			}
		}
	}
}
  
void Red::forwardProp()
{
	//We skip the input layer

	for (int i = 1; i < this->nLayers; i++)
	{
#ifdef PRINTDEBUG 
		cout << endl << "== NEW LAYER ==" << endl;
#endif
		//In a layer
		Layer currentLayer = this->layers[i];

		//Previous layer
		Layer previousLayer = this->layers[i - 1];

		for (int j = 0; j < currentLayer.size(); j++)
		{
			//In a neuron
#ifdef PRINTDEBUG 
			cout << "NEURON" << endl;
#endif

			Neurona currentNeuron = currentLayer[j];
			double value = 0;
			if (currentNeuron.neuronasCapaAnterior == 0) //Is a bias?
			{
#ifdef PRINTDEBUG 
				cout << "Is a bias" << endl;
				cout << "Value = " << currentNeuron.getValor() << endl;
#endif
				break;
			}

			//For that neuron, we calculate it's value with the previous layer's neurons
			for (int k = 0; k < currentNeuron.neuronasCapaAnterior + 1; k++)
			{
				//In a previous neuron
				double previousNeuronValue = previousLayer[k].getValor();
				value = value + (currentNeuron.pesos[k] * previousNeuronValue);
#ifdef PRINTDEBUG 
				cout << "Previous neuron value : " << previousNeuronValue << endl;
#endif
			}

			//We save the value in the current neuron
			currentNeuron.setValor(value);
			layers[i][j].setValor(value);
			layers[i][j].activate_sigmoid();
			double test = layers[i][j].getValor();
#ifdef PRINTDEBUG 
			cout << "Neuron value : " << test << endl;
			cout << endl;
#endif
		}
	}
}

void Red::genes2weights() {
	for (int i = 0; i < this->nLayers - 1; i++) {
		for (int j = 0; j < this->structure[i + 1]+1; j++) {//[]+1 for bias 
			for (int k = 0; k < this->structure[i + 2]; k++) {
				layers[i + 1][k].pesos[j] = genes[i][j][k];
			}
		}
	}
}

void Red::calculate_error(double* exp_result) {

	//calculates the total error from outputs
	getResult(); //It updates *result. 

	double err = 0;
	for (int out = 0; out < structure[nLayers]; out++) {
		err += abs(result[out] - exp_result[out]);
	}
	error.push_back(err);
}

double Red::getAverage_error() {

	double tot = 0;
	for (int i = 0; i < error.size(); i++) {
		tot += error[i];
	}
	tot = tot / error.size();
	error.clear();
	this->avgError = tot;
	return (tot);
}