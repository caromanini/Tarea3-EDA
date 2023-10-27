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

string postfija(const string& expresion_infija){
    string expresion_postfija = "";
    stack<string> pila; //se cambio stack para que sea de tipo string, no char

    for(int i=0; i<expresion_infija.length(); i++){//escanear expresion
        string expresion = "";//Este string va a ser el que se agregue a "expresion_postfija"

        if(expresion_infija[i] == ' '){//ignorar espacios en blanco
            continue;
        }

        expresion += expresion_infija[i];//se asigna el caracter i de expresion_infija a "expresion"
        int count = 0;

        if(isInt(expresion)){//si expresion es un numero, entra a este if
        
        /*
            CAMBIO EN EL CODIGO: Separar las cosas que se insertan a "expresion_postfija" por comas, para luego poder identificar los numeros con 
            más de un digito. 

            Ej: infija= 10+(1+2)*2
            Antes = 1012+2*+
            Ahora = 10,1,2,+,2,*,+

            Si expresion (expresion guarda expresion_infija[i]) es un numero, se chequea si este esta compuesto por mas de un digito.
            Esto se hace con el for de la linea 65. Al principio expresion toma el caracter actual. Si este es un numero, y los caracteres siguientes tambien son numeros,
            estos se agregan al string "expresion". Si se llega a un caracter distinto a un numero, se termina el ciclo.

            Se hace para que en el caso de ser un numero con más de un digito, se guarde completo y no todos sus digitos separados por comas.
 
        */

            for(int j=i+1; j<expresion_infija.length(); j++){
                if(expresion_infija[j] >= '0' && expresion_infija[j] <= '9'){
                    expresion += expresion_infija[j];
                    count += 1;
                }
                else
                    break;
            }
            expresion_postfija += expresion;
            expresion_postfija += ",";
            i += count;
        }
        else if(expresion == "("){
            pila.push(expresion);
        }
        else if(expresion == ")"){
            while(!pila.empty() && pila.top() != "("){
                expresion_postfija += pila.top();
                expresion_postfija += ",";
                pila.pop();
            }
            pila.pop();
        }
        else{
            while(!pila.empty() && operadores(expresion) < operadores(pila.top())){
                expresion_postfija += pila.top();
                expresion_postfija += ",";
                pila.pop();
            }
            if(!pila.empty() && operadores(expresion) == operadores(pila.top()) && expresion != "^"){
                expresion_postfija += pila.top();
                expresion_postfija += ",";
                pila.pop();
            }
            pila.push(expresion);
        }
    }
    while(!pila.empty()){
        expresion_postfija += pila.top();
        expresion_postfija += ",";
        pila.pop();
    }

    if(!expresion_postfija.empty() && expresion_postfija.back() == ','){//si hay una coma extra al final, este if la borra
        expresion_postfija.pop_back();
    }

    return expresion_postfija;
}


int calcular_resultado(string operacion_original){//función para calcular resultado de operacion usando la expresion postfija

    /*
    PASOS:
        1. Leer expresion postfija de izquierda a derecha
        2. Si el elemento es un numero, meter este al stack (stack.push(numero))
        3. Esto se hace hasta que se llega a un operador
        4. Si es un operador, se hace pop de los dos ultimos elementos del stack y se realiza la operación con estos dos
        5. Se hace push del resultado de esta operación al stack
        6. Esto se repite hasta que la expresión se termine
        7. El resultado final va a quedar en el stack, por eso al final se retorna stack.top()


    Ej: 10 + (1+2) * 2 -> expresion postfija = 10,1,2,+,2,*,+
    1. stack.push(10)
    2. stack.push(1)
    3. stack.push(2)
    4. Se llega a un operador (+):
        - operador1 = stack.top() = 2
        - stack.pop()
        - operador2 = stack.top() = 1
        - stack.pop()
    5. Realizar la operación con esos dos operadores y hacer push del resultado -> stack.push(1+2) = stack.push(3)
    6. stack.push(2)
    7. Se llega a un operador (*):
        - operador1 = stack.top() = 2
        - stack.pop()
        - operador2 = stack.top() = 3
    8. Realizar la operación y hacer push del resultado -> stack.push(3*2) = stack.push(6)
    9. Se llega a un operador (+):
        - operador1 = stack.top() = 6
        - stack.pop()
        - operador2 = stack.top() = 10    
        - stack.pop()
    10. Se realiza la operación y se hace push del resultado -> stack.push(10+6) = stack.push(16)
    11. Se llegó al final de la operación. Se retorna stack.top() = 16
    
    */

    stack<int> stack;

    std::vector<std::string> operacion_postfija;

    string expresion_postfija = postfija(operacion_original);
    std::istringstream iss(expresion_postfija);
    std::string token;

    while(std::getline(iss, token, ',')){
        operacion_postfija.push_back(token);
    }

    //VECTOR operacion_postfija TIENE LA OPERACION QUE SE TIENE QUE RESOLVER
    for(int i=0; i<operacion_postfija.size(); i++){//RECORRE VECTOR CON OPERACION
        //Si valor i de vector es un numero, se hace push de ese numero al stack
        if(isInt(operacion_postfija[i])){
            stack.push(stoi(operacion_postfija[i]));
        }

        //si el valor i del vector es un operador, se entra a este bloque else
        else{
            //pop de los dos numeros que estan arriba en el stack
            //A estos se le va a aplicar la operacion
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

int main(){
    std::map<std::string, int> variables;//diccionario que guarda las variables con sus valores
    variables["ans"] = 0;//se agrega la variable ans al diccionario, con valor 0 
    //ans va a guardar el resultado de cada operación. Se pueden realizar 80 operaciones y va a ir guardando el resultado de cada una

    while(true){ 
        /*
            El codigo va a estar en un loop infinito hasta que escribas end. 

            SE ASUME QUE LAS OPERACIONES SIEMPRE ESTARAN SEPARADAS POR ESPACIOS.
            EJ. x = 6
        */

        std::vector<std::string> valores_operacion;

        string operacion;
        cout << "Ingresar operacion: ";
        getline(cin, operacion);

        if(operacion == "end"){
            cout << "Chao!"<<endl;
            break;
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
            std::string operacion_limpia = "";
            /*
                variable "operacion_limpia" guarda la operación con los valores de las variables
                Ej:
                    x = 6
                    x * 2
                    operacion_limpia = 6 * 2
            */

            for(int i=0; i<valores_operacion.size();i++){//SE ASUME QUE EN LAS OPERACIONES SE UTILIZAN VARIABLES QUE YA EXISTEN, YA DECLARADAS

                auto it = variables.find(valores_operacion[i]);
                std::string aux = valores_operacion[i];

                if(it != variables.end()){//Buscar si la variable esta en el diccionario. Si es que si, valores_operacion[i] ahora es igual al valor de la variable
                    std::string aux = std::to_string(it->second);
                    valores_operacion[i] = aux;    
                }

                operacion_limpia += valores_operacion[i];
            }

            int resultado  = calcular_resultado(operacion_limpia);
            cout << "RESULTADO: "<< resultado<<endl;

            variables["ans"] = resultado;//actualizar valor de variable ans a resultado 
        }

    }

    return 0;
}


