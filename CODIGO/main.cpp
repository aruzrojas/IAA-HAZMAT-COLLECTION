#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

void tokenize(std::string const &str, const char delim,   //SEPARA LINEA DE TEXTO POR ESPACIOS
            std::vector<std::string> &out)
{
    std::stringstream ss(str);

    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

struct Nodo {
    string tipo_material;
    int posicion;   //posicion dentro de la matriz (en que fila se encuentra)
    int carga;

};

struct Camion{
    int capacidad;
    int nodo_actual;
    string mat_dom;
    string ruta;
    vector<string> cargados;
};

int letra_to_pos(string letra, vector<string> materiales){
        unsigned co;
        for (co = 0; co < materiales.size(); co++){
            if (materiales[co] == letra){
                return co;
            }
        }
        return -1;
}

char compatible(string& m, string& r, vector<string> materiales){   //STRING M ES EL QUE TIENE, STRING R ES EL QUE QUIERE LLEVAR
    vector<vector<char>> COM{ {'A', '-', 'C', 'D', '-' }, 
                        {'-', 'B', 'C', 'D', 'E' }, 
                        {'C', 'C', 'C', '-', 'E' },
                        {'D', 'D', '-', 'D', 'E' },
                        {'-', 'E', 'E', 'E', 'E' }};     

    int posi = letra_to_pos(m, materiales);
    int posj = letra_to_pos(r, materiales);
    return COM[posi][posj];
}

int compatible_todos(vector<string> cargados, string & r, vector<string> materiales){
    int pos = letra_to_pos(r, materiales); //POSICION DEL MATERIAL r
    vector<vector<char>> COM{ {'A', '-', 'C', 'D', '-' }, 
                        {'-', 'B', 'C', 'D', 'E' }, 
                        {'C', 'C', 'C', '-', 'E' },
                        {'D', 'D', '-', 'D', 'E' },
                        {'-', 'E', 'E', 'E', 'E' }};     
    unsigned i;
    int pos_c;
    char guion = '-';

    for (i = 0; i < cargados.size(); i++){
        pos_c = letra_to_pos(cargados[i],materiales);
        if (COM[pos_c][pos] == guion){
            return 0;
        }
    }
    return 1;
}


int main (){

    vector<float> alfas {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
    int i_alfa;
    float alpha;
    unsigned i_arc;
    fstream file;  
    vector<string> archivos {"peligro-mezcla4-min-riesgo-zona1-2k-AE.2.hazmat",
                            "peligro-mezcla4-min-riesgo-zona2-2k-AE.2.hazmat",
                            "peligro-mezcla4-min-riesgo-zona3-2k-AE.2.hazmat",
                            "peligro-mezcla4-min-riesgo-zona4-2k-AE.2.hazmat",
                            "peligro-mezcla4-min-riesgo-zona5-2k-AE.2.hazmat",
                            "peligro-mezcla4-min-riesgo-zona6-2k-AE.2.hazmat",
                            "peligro-mezcla4-min-riesgo-zona7-2k-AE.2.hazmat"}; 
    file.open("output.txt",ios::out);
    for (i_arc = 0; i_arc < archivos.size(); i_arc++){ 
        file << "Archivo: " << archivos[i_arc] << endl;
        file << endl;
        for (i_alfa = 0; i_alfa < 11; i_alfa++){ 
            alpha = alfas[i_alfa];
            vector<string> q[100];
            string line;
            ifstream myfile;
            myfile.open(archivos[i_arc]);
            vector<float> c_vacio;
            vector<float> capacidades;
            vector<string> materiales;
            vector<Nodo> Nodos;
            vector<Camion> Camiones;
            int n_camiones;
            int n_nodos;
            int cont = -1;
            int i, j;
            int n_materiales = 0;
            if(!myfile.is_open()) {
              perror("Error open");
              exit(EXIT_FAILURE);
            }
            while(getline(myfile, line)) {
                cont = cont + 1;
                if (cont == 0){ 
                    n_camiones = std::stoi(line);     //cantidad de camiones

                }
                else if (cont == 1){
                    i = 0;
                    std::vector<std::string> out;
                    tokenize(line, ' ', out);
                    for (auto &line: out) {
                       capacidades.push_back(std::stoi(line));
                    }
                    i++;

                }

                else if (cont == 2){
                    n_nodos = std::stoi(line);   //cantidad de nodos
                    
                
                }
                else if (cont == 3){ 
                    i = 0;
                    std::vector<std::string> out;
                    tokenize(line, ' ', out);
                    //cout << line <<endl;
                    for (auto &line: out) {
                       q[i].push_back(line);
                    }
                    i++;
                    cont = cont + 1;
                    while (getline(myfile,line) && cont != n_nodos + 3){ 

                        cont = cont + 1;
                        
                        j = 0;

                        std::vector<std::string> out;
                        tokenize(line, ' ', out);
                        for (auto &line: out) {
                            q[i].push_back(line);

                            j = j + 1;
                            if (j == 3){
                                   
                                if (std::find(materiales.begin(), materiales.end(), line) != materiales.end() ){
                                    continue;
                                }
                                else{
                                    materiales.push_back(line);
                                    n_materiales = n_materiales + 1;
                                }
                            }
                        }
                        i++;
                        
                    }
                }
                
                if(cont == n_nodos + 3){
                    i = 0;
                    std::vector<std::string> out;
                    tokenize(line, ' ', out);
                    //cout << line <<endl;
                    for (auto &line: out) {
                       c_vacio.push_back(std::stof(line));
                    }
                    i++;
                    /*for (auto it = c_vacio[0].begin(); 
                        it != c_vacio[0].end(); it++) { 
                            cout << *it << ' '; 
                    } 
                    cout << endl;*/ //PARA LEER EL VECTOR DE DISTANCIA DESDE EL DEPOSITO
                    break;
                }
                //cout << cont << endl;
                //cout <<  line << endl;
            }
            float matrices[n_materiales][n_nodos][n_nodos];
            float distancias[n_materiales][n_nodos][n_nodos];
            float riesgos[n_materiales][n_nodos][n_nodos];
            vector<int> visitados;
            for (j = 0; j < n_nodos; j++){
                visitados.push_back(0);  
            }

            int fila = 0;
            int columna = 0;
            int n_matriz = 0;
            while (getline(myfile, line) ){ 

                if(fila < n_nodos){          //VA DE 0 A 11
                    columna = 0;
                    std::vector<std::string> out;
                    tokenize(line, ' ', out);
                    for (auto &line: out) {
                        matrices[n_matriz][fila][columna] = (1-alpha)*std::stoi(line); //distancia
                        distancias[n_matriz][fila][columna] = std::stoi(line);
                        //cout << "Elemento" << n_matriz << " " << fila << " "<< columna << ": " << matrices[n_matriz][fila][columna] << endl;
                        columna = columna + 1;     //actualizar columna
                    }            

                }
                fila = fila + 1;
                if (n_matriz == n_materiales - 1 && fila == n_nodos && columna == n_nodos ){
                    break;
                }
                if(fila == n_nodos){
                    fila = 0;
                    columna = 0;
                    n_matriz = n_matriz + 1;
                } 

            }

            fila = 0;
            columna = 0;
            n_matriz = 0;
            while (getline(myfile, line) ){ 

                if(fila < n_nodos){
                    columna = 0;
                    std::vector<std::string> out;
                    tokenize(line, ' ', out);
                    for (auto &line: out) {
                        matrices[n_matriz][fila][columna] = matrices[n_matriz][fila][columna] + alpha*std::stoi(line); //riesgo
                        riesgos[n_matriz][fila][columna] = std::stoi(line);
                        //cout << "Elemento" << n_matriz << " " << fila << " "<< columna << ": " << matrices[n_matriz][fila][columna] << endl;
                        columna = columna + 1;     //actualizar columna
                    }            

                }
                fila = fila + 1;
                if (n_matriz == n_materiales - 1 && fila == n_nodos && columna == n_nodos ){
                    break;
                }
                if(fila == n_nodos){
                    fila = 0;
                    columna = 0;
                    n_matriz = n_matriz + 1;
                }


            }

            int smallest = 99999;
            int second = 99999;
            int pos = 0;

            i = 0;
            sort(materiales.begin(), materiales.end()); //ORDENAR TIPO MATERIALES, COMO EN LA LA VARIABLE MATRIZ

            /*for (auto i = materiales.begin(); i != materiales.end(); ++i)
                std::cout << *i << ' ';   //LEER TIPO DE MATERIALES (ESTAN EN ORDEN)*/

            char guion = '-';
            for (auto it = c_vacio.begin(); 
                it != c_vacio.end(); it++) { 
                if (*it <= smallest){
                    second = smallest;
                    smallest = *it;
                }
                else if (*it < second){
                    pos = i;
                    second = *it;
                }
                i++;
            }                  //SE OBTIENE EL NODO MAS CERCANO PARA EL PRIMER CAMION (pos)


            vector<float> rutas[n_camiones];

            for (j = 0; j < n_nodos; j++){
                Nodos.push_back(Nodo());
                Nodos[j].tipo_material = q[j][2];
                Nodos[j].posicion = j;
                Nodos[j].carga = std::stoi(q[j][1]);
            }

            for (j = 0; j < n_camiones; j++){
                Camiones.push_back(Camion());
                Camiones[j].capacidad = (capacidades[j]);
                Camiones[j].nodo_actual = 0;
                Camiones[j].ruta = "Deposito";
            }
            int cantidad = 0;

            for (i = 0; i < n_nodos; i++){
                cantidad = cantidad + Nodos[i].carga;
            };
            int pos_letra_solucion;
            int flag;
            float minimo = 99999;
            float Z = 0;
            float Z_distancias = 0;
            float Z_riesgos = 0;
            int iteraciones = 0;
            j = 0;
            while (cantidad != 0 && j < n_camiones){
                for (j = 0; j < n_camiones; j++){
                    iteraciones = iteraciones + 1;
                  
                    minimo = 99999999;
                    if (Camiones[j].nodo_actual == 0){  //SI ESTA EN EL NODO DEPOSITO
                        for (i = 0; i < n_nodos; i++){
                            if (Nodos[i].carga != 0 && c_vacio[i] != 0 && c_vacio[i] < minimo){
                                pos = i;
                                minimo = c_vacio[i];              //SE GUARDA LA POS DEL VECTOR DEPOSITO -> CLIENTES Y EL VALOR MINIMO (LA DIST)
                            }
                        }
                    }
                    if (Nodos[pos].carga != 0 && Camiones[j].nodo_actual == 0){                //SI EL NODO NO HA SIDO DESCARGADO Y CAMION ESTA EN DEPOSITOl;
                            Camiones[j].capacidad = Camiones[j].capacidad - Nodos[pos].carga;
                            Camiones[j].cargados.push_back(Nodos[pos].tipo_material);
                            pos_letra_solucion = letra_to_pos(Nodos[pos].tipo_material, materiales);
                            Camiones[j].nodo_actual = pos;
                            Camiones[j].ruta = Camiones[j].ruta + " - " + std::to_string(pos);
                            cantidad = cantidad - Nodos[pos].carga;
                            Nodos[pos].carga = 0;
                            Z = Z + (1 - alpha ) * minimo;                           //SE LLEVA EL CAMION AL NODO NO DESCARGADO Y SE SUMA SOLO DISTANCIA, NO HAY RIESGO
                            Z_distancias = Z_distancias + minimo;
                            Camiones[j].mat_dom = Nodos[pos].tipo_material;
                         
                    }

                    if (/*Nodos[pos].carga != 0 &&*/ Camiones[j].nodo_actual != 0){
                        pos_letra_solucion = letra_to_pos(Camiones[j].mat_dom, materiales);
                        flag = 1;
                        while(flag){ 
                            
                            //pos = -1;
                            minimo = 9999999;
                            for (i = 0; i < n_nodos; i++){
                                
                                if (Nodos[i].carga != 0 && compatible(Camiones[j].mat_dom, Nodos[i].tipo_material, materiales) != guion && 
                                    matrices[pos_letra_solucion][Camiones[j].nodo_actual][i] != 0 && 
                                    matrices[pos_letra_solucion][Camiones[j].nodo_actual][i] < minimo &&
                                    compatible_todos(Camiones[j].cargados, Nodos[i].tipo_material,materiales)                                
                                    ){
                                   
                                    pos = i;
                                    minimo = matrices[pos_letra_solucion][Camiones[j].nodo_actual][i];
                                    
                                }

                            }
                            if (Nodos[pos].carga != 0 && Camiones[j].nodo_actual != 0 && pos != -1 &&
                                compatible(Camiones[j].mat_dom, Nodos[pos].tipo_material, materiales) && minimo != 9999999 
                                && compatible_todos(Camiones[j].cargados, Nodos[pos].tipo_material,materiales) 
                                ){
                                Camiones[j].capacidad = Camiones[j].capacidad - Nodos[pos].carga;
                                Camiones[j].cargados.push_back(Nodos[pos].tipo_material);
                                pos_letra_solucion = letra_to_pos(Camiones[j].mat_dom, materiales);

                                Z = Z + matrices[pos_letra_solucion][Camiones[j].nodo_actual][pos];  
                                Z_distancias = Z_distancias + distancias[pos_letra_solucion][Camiones[j].nodo_actual][pos];
                                Z_riesgos = Z_riesgos + riesgos[pos_letra_solucion][Camiones[j].nodo_actual][pos]; 
                                Camiones[j].nodo_actual = pos;
                                Camiones[j].ruta = Camiones[j].ruta + " - " + std::to_string(pos);
                                Camiones[j].mat_dom =  compatible(Camiones[j].mat_dom, Nodos[pos].tipo_material, materiales);
                                cantidad = cantidad - Nodos[pos].carga;
                                Nodos[pos].carga = 0;               
                 
                            }
                            else{
                                Camiones[j].nodo_actual = 0;
                                flag = 0;

                            }
                        }
                    }
                    if (cantidad == 0){ //CAMIONES AL DEPOSITO
                        j = n_camiones + 1;
                        for(i = 0; i < n_camiones; i++){
                            minimo = 999999;
                            Camiones[i].ruta = Camiones[i].ruta + " - Deposito";
                            pos_letra_solucion = letra_to_pos(Camiones[i].mat_dom, materiales);
                            for (j = 0; j < n_nodos; j++){
                                if (matrices[pos_letra_solucion][Camiones[i].nodo_actual][j] != 0 &&
                                    Camiones[i].cargados.size() != 0){ 
                                    Camiones[i].nodo_actual = 0;
                                    Z = Z + matrices[pos_letra_solucion][Camiones[i].nodo_actual][j];
                                    Z_distancias = Z_distancias + distancias[pos_letra_solucion][Camiones[i].nodo_actual][j];
                                    Z_riesgos = Z_riesgos + riesgos[pos_letra_solucion][Camiones[i].nodo_actual][j]; 
                                    j = n_nodos + 1;     
                                }
                            }
                        }
                    }            
                }
            }
            if (cantidad == 0){
                
                file << "Valor de alfa = "<< alpha << "|" << " Riesgos: " << Z_riesgos<< "|" <<" Costos: " << Z_distancias << "|" << " Total: " << Z<<endl;
            }
            else{
                file << "No se logro solucion con alfa = " << alpha << endl;
            }
        }
        file << "\n" << endl;
    }
    return 0;
}   