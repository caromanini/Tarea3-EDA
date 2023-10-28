#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;

bool isInt(const std::string& s){//funcion para chequear si string es un numero
    try{
        std::stoi(s);
        return true;
    } catch (...){
        return false;
    }
}

int operadores(string operador) {
    if(operador == "^"){
        return 3;
    }
    else if(operador == "*" || operador == "/"){
        return 2;
    }
    else if(operador == "+" || operador == "-"){
        return 1;
    }
    return 0;
}

string postfija(const string& expresion_infija, map<string,int> variables){

    vector<string> expresion_infija_limpia;

    std::istringstream iss(expresion_infija);
    std::string token;

    while(iss >> token){
        expresion_infija_limpia.push_back(token);
    }

    string expresion_postfija = "";
    stack<string> stack;

    for(int i=0; i<expresion_infija_limpia.size(); i++){
        /*
            Ahora la expresion infija se encuentra en un vector. Cada elemento de un vector es un elemento de la expresion.
            Ej: 
                Expresion infija = 10 + ( 1 + 2 ) * 2
                    - expresion_infija_limpia[0] = 10
                    - expresion_infija_limpia[1] = + 
                    - expresion_infija_limpia[2] = (
        */

        auto it = variables.find(expresion_infija_limpia[i]);//Para buscar si el elemento del vector se encuentra en el diccionario de variables
        
        if(isInt(expresion_infija_limpia[i])){
            expresion_postfija += expresion_infija_limpia[i];
            expresion_postfija += ",";
        }
        else if(it != variables.end()){//chequear si valor es una variable. Se busca en el diccionario de variables
            expresion_postfija += expresion_infija_limpia[i];
            expresion_postfija += ",";
        }
        else if(expresion_infija_limpia[i] == "("){
            stack.push(expresion_infija_limpia[i]);
        }
        else if(expresion_infija_limpia[i] == ")"){
            while(!stack.empty() && stack.top() != "("){
                expresion_postfija += stack.top();
                expresion_postfija += ",";
                stack.pop();
            }
            stack.pop();
        }else{
            while(!stack.empty() && operadores(expresion_infija_limpia[i]) < operadores(stack.top())){
                expresion_postfija += stack.top();
                expresion_postfija += ",";
                stack.pop();
            }
            if(!stack.empty() && operadores(expresion_infija_limpia[i]) == operadores(stack.top()) && expresion_infija_limpia[i] != "^"){
                expresion_postfija += stack.top();
                expresion_postfija += ",";
                stack.pop();
            }

            stack.push(expresion_infija_limpia[i]);
        }
        
    }

    while(!stack.empty()){
        expresion_postfija += stack.top();
        expresion_postfija += ",";
        stack.pop();
    }

    if(!expresion_postfija.empty() && expresion_postfija.back() == ','){
        expresion_postfija.pop_back();
    }

    return expresion_postfija;
}

int calcular_resultado(string operacion_original, map<string, int> variables){
    stack<int> stack;

    std::vector<std::string> operacion_postfija;

    string expresion_postfija = postfija(operacion_original, variables);
    std::istringstream iss(expresion_postfija);
    std::string token;

    while(std::getline(iss, token, ',')){
        operacion_postfija.push_back(token);
    }

    for(int i=0; i<operacion_postfija.size(); i++){

        auto it = variables.find(operacion_postfija[i]);
    
        if(isInt(operacion_postfija[i])){
            stack.push(stoi(operacion_postfija[i]));
        }
        else if(it != variables.end()){//SI VALOR ES UNA VARIABLE, SE HACE PUSH DE SU VALOR AL STACK
            int aux = it->second;
            stack.push(aux);            
        }
        //si el valor i del vector es un operador, se entra a este bloque else
        else if(operacion_postfija[i] == "+" || operacion_postfija[i] == "-" || operacion_postfija[i] == "*" || operacion_postfija[i] == "/"){
            int operador1 = stack.top();
            stack.pop();
            int operador2= stack.top();
            stack.pop();

            if(operacion_postfija[i] == "+"){
                stack.push(operador2 + operador1);
                continue;
            }
            else if(operacion_postfija[i] == "-"){
                stack.push(operador2 - operador1);
                continue;
            }
            else if(operacion_postfija[i] == "*"){
                stack.push(operador2 * operador1);
                continue;
            }
            else if(operacion_postfija[i] == "/"){
                stack.push(operador2 / operador1);
                continue;
            }
            else if(operacion_postfija[i] == "^"){
                stack.push(pow(operador2,operador1));
                continue;
            }

        }
    }
    return stack.top();
}

struct TreeNode {//Struct del arbol para poder armar e imprimir tree
/*
    En el informe se puede poner que es una simplificación del código de arboles que esta
    en el repositorio del profe
*/
    string data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const string& val) : data(val), left(nullptr), right(nullptr) {}
};

bool isOperator(const string& token){
    return token == "+" || token == "-" || token == "*" || token == "/";
}

TreeNode* expresion_postfija_tree(string expresion_infija, map<string,int> variables){
/*
    Funcion para generar el arbol. Retorna el nodo root de este
*/
    string expresion_postfija = postfija(expresion_infija, variables);

    vector<string> operacion_postfija;
    vector<string> tree; 

    istringstream iss(expresion_postfija);
    string token;

    while(getline(iss, token, ',')){
        operacion_postfija.push_back(token);
    }

    stack<TreeNode*> stack;

    for(const string& token : operacion_postfija){//recorrer vector operacion_postfija
        TreeNode* node = new TreeNode(token);
//Por cada elemento en el vector, se crea un nodo en el arbol con data = token (que seria el valor de la posicion de operacion_postfija) 

        if(isOperator(token)){//chequear si el valor es un operador
            node -> right = stack.top();
            stack.pop();
            node -> left = stack.top();
            stack.pop();

    /*
        Si el valor es un operador, se hace pop de los dos ultimos elementos del stack.
        El operador va a ser el nodo padre, mientras que los elementos a los que se le hizo 
        pop van a ser los nodos hijos del operador (nodo hijo izquierdo y derecho).
    */
        }

        stack.push(node);//Se hace push del elemento actual al stack
    }

    return stack.top();//El ultimo elemento en la operacion_postfija sera el nodo root del arbol. Se retorna este valor
}

void printTree(TreeNode* root, string espacio = ""){
/*
    Funcion encargada de imprimir arbol
    Toma como parametros un puntero a TreeNode, que seria el nodo root del arbol

    El parametro "espacio" es para ordenar el output (que se vea bonito jiji) 
*/
    if(root == nullptr){//si el nodo actual raiz es nullptr, termina la funcion
        return;
    }

    cout << espacio;

    cout << "----";
    espacio += "   ";

    cout << root-> data << endl;//imprimir data del nodo actual

    printTree(root->left, espacio);//llamar denuevo a printTree, con root ahora igual al nodo hijo de la izquierda del nodo actual
    printTree(root->right, espacio);//llamar denuevo a printTree, ahora con root igual al nodo hijo de la derecha del nodo actual
}

int main(){
    cout<<"------CALCULADORA INTERACTIVA------"<<endl;
    std::map<std::string, int> variables;
    stack<string> operaciones_pasadas;//un vector que guarda las operaciones pasadas para poder llamarlas despues en printTree

    while(true){ 
        /*
            El codigo va a estar en un loop infinito hasta que escribas FIN. 
        */

        std::vector<std::string> valores_operacion;

        string operacion;
        std::cout << "Ingresar operacion: ";
        getline(cin, operacion);

        if(operacion == "end"){
            std::cout << "Chao!"<<endl;
            break;
        }
        if(operacion == "tree"){
            /*
                Si input es tree, se llama a la funcion expresion_postfija_tree para generar el 
                arbol a partir de una expresion postfija.                 
            */
            
            string aux = operaciones_pasadas.top();
            TreeNode* root = expresion_postfija_tree(aux, variables);//expresion_postfija_tree retorna el nodo root del arbol

            printTree(root);
            
            delete root;
            continue;
        }

        std::istringstream iss(operacion);
        std::string token;

        while(iss >> token){
            valores_operacion.push_back(token); 
        }

        if(valores_operacion[1] == "="){
            variables[valores_operacion[0]] = stoi(valores_operacion[2]);
        }
        else{
            operaciones_pasadas.push(operacion);
            /*
                Se hace push al vector operaciones_pasadas para guardar la operacion actual, en caso de que
                se quiera imprimir el arbol de esta
            */
            int resultado  = calcular_resultado(operacion, variables);
            std::cout << "RESULTADO: "<< resultado<<endl;

            variables["ans"] = resultado;
        }
    }
    return 0;
}
