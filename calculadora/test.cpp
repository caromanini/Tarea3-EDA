#include <iostream>
#include <stack>
#include <string>

using namespace std;


int operadores(char operador) { // QUE OPERADOR VA PRIMERO
    if (operador == '^') {
    return 3;
    } else if (operador == '*' || operador == '/') {
    return 2;
    } else if (operador == '+' || operador == '-'){
    return 1;
    }
    return 0;
}


string postfija(const string& expresion_infija){
    string expresion_postfija = "";
    stack<char> pila;//CAMBIAR STACK A STRING

    for(int i=0; i<expresion_infija.length(); i++){//escanear expresion
        if(expresion_infija[i] == ' '){ //ignorar espacios en blanco
            continue;
        }

        if(expresion_infija[i] >= '0' && expresion_infija[i] <= '9'){
            expresion_postfija += expresion_infija[i];
        } 
        else if(expresion_infija[i] == '('){
            pila.push(expresion_infija[i]);//ACA HAY QUE CONVERTIR expresion_infija[i] A UN STRING
        }
        else if(expresion_infija[i] == ')'){
            while(!pila.empty() && pila.top() != '('){
                expresion_postfija += pila.top();
                pila.pop();
            }
            pila.pop();
        }
        else{
            while(!pila.empty() && operadores(expresion_infija[i]) < operadores(pila.top())){
                expresion_postfija += pila.top();
                pila.pop();
            }
            if(!pila.empty() && operadores(expresion_infija[i]) == operadores(pila.top()) && expresion_infija[i] != '^'){
                expresion_postfija += pila.top();
                pila.pop();
            }
            pila.push(expresion_infija[i]);
        }
       
    }
    while(!pila.empty()){
        expresion_postfija += pila.top();
        pila.pop();
    }
    return expresion_postfija;
}

int main() {


    
    string infija;
    cout << "infija: ";
    getline(cin,infija);

    string expresion_postfija = postfija(infija);

    cout << "postfija: " << expresion_postfija << endl;
 

   /*
   string operacion;
   cout << "Ingresar operacion: ";
   getline(cin, operacion);

   cout << "Esto es operacion[0]:" << operacion[0] << endl;
    */


   

    return 0;
}




