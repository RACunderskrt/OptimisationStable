//HUMBERT Thomas
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <stdlib.h>
#include <ilcplex/ilocplex.h>
#include <chrono>
#include "graph.hpp"
using namespace std;


void saveIND(vector<string> res, string fileName){
	ofstream newFile("./output/"+fileName+".ind");
	for(string r : res)
		newFile << r << endl;
	newFile.close();
}

vector<string> exact(const Graph<string> &g) {  
  IloEnv env;

  map<string,IloNumVar> variables;
  for(string v : g.vertices()) {
    variables[v] = IloNumVar(env, 0, 1, ILOINT);
  }

  IloModel model(env);

	for(std::pair<string,string> e : g.edges()) {
		IloExpr expr(env);
		expr += variables[e.first];
		expr += variables[e.second];
		model.add(expr <= 1);
	  }

 
  // Maximize the number of stringices
  IloExpr expr(env);
  for(string v : g.vertices())
    expr += variables[v];
  model.add(IloMaximize(env,expr));

 
  IloCplex cplex(model);
  cplex.setOut(env.getNullStream()); // Disable console output
  cplex.setParam(IloCplex::Param::TimeLimit, 3*60); // 3 minutes time limit
  bool solved = cplex.solve();

  vector<string> dom;
  if(solved) {
  // Pick stringices of value 1 and add to dom
    for(string v : g.vertices()) {
      if(cplex.getValue(variables[v]) > .5)
        dom.push_back(v);
    }  
  }
  env.end(); // Free memory, as the destructor does not free it!

  return dom;  
}


int main(int argc, char* argv[]){
  //démarrage du chronometre
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    string fileName = argv[1];
    cout << fileName + " is running" << endl;
  //création du graph  
    Graph<string> g = Graph<string>::readFile("./input/"+fileName+".edges");
  //copie de ce graph
    Graph<string> g2 = g;
  //initialisation du premier stable  
    vector<string> resInit = g.glouton();
    srand(time(0));
  //tant que le chronometre est en dessous de 2min
    while(1){
    //retourne un vertex aléatoire du graph  
      string randomV = g2.randomVertex();
    //retourne une liste de points autour du vertex  
      vector<string> sousGraph = g2.bfs(randomV, 1000);
    //vérifie si des points du nouveau stable sont à côté d'un point du stable 
      vector<string> newStable = g2.verifyExterior(sousGraph, resInit);
    //créer un Graph avec sousGraph
      Graph<string> subg = g2.createSub(sousGraph);
    //trouve un nouveau stable  
      vector<string> resBuf = exact(subg);
    //ajoute ce nouveau stable dans le stable precedent
      newStable.insert(newStable.end(),resBuf.begin(), resBuf.end());

      if(newStable.size() > resInit.size()){
        resInit = newStable;
      }

      chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();

      if(t2 - t1 > chrono::seconds(120))
        break;
    } 
  //sauvegarde du stable
    saveIND(resInit, fileName);
  	cout << fileName + " is ended" << endl;
    return 0;
}
