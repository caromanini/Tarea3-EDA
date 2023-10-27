#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <vector>
#include <sstream>

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
            mas de un digito. 

            Ej: infija= 10+(1+2)*2
            Antes = 1012+2*+
            Ahora = 10,1,2,+,2,*,+

            Si expresion (expresion guarda expresion_infija[i]) es un numero, se chequea si este esta compuesto por mas de un digito.
            Esto se hace con el for de la linea 51. Al principio expresion toma el caracter actual. Si este es un numero, y los caracteres siguientes tambien son numeros,
            estos se agregan al string "expresion". Si se llega a un caracter distinto a un numero, se termina el ciclo.

            Se hace para que en el caso de ser un numero con mas de un digito, se guarde completo y no todos sus digitos separados por comas.
 
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



int main(){
    std::map<std::string, int> variables;
    std::vector<std::string> valores_operacion;

    while(true){ 
        /*
            SE ASUME QUE LAS OPERACIONES SIEMPRE ESTARAN SEPARADAS POR ESPACIOS.
            EJ. x = 6
                10 * x
        */
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
            //stoi(valores_operacion[2])
            variables[valores_operacion[0]] = stoi(valores_operacion[2]);
        }

        std::map<std::string, int>::iterator it = variables.begin();

        while(it != variables.end()){
            std::cout << "key: "<< it->first << ", Value: "<< it->second << std::endl;
            ++it;
        }
        

        cout << "valores_operacion[1]: "<<valores_operacion[1] << endl;




/*      PARA IMPIRIR VALORES DE VECTOR "valores_operacion"
    
        for(const std::string& value : valores_operacion){
            std::cout << value << std::endl;
        }
*/






        string expresion_postfija = postfija(operacion);

        cout << "postfija: "<< expresion_postfija << endl;










    }



    







    return 0;
}


