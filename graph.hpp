//HUMBERT Thomas
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iterator>
#include <cstddef> 
#include <vector>
#include <queue>
template<class Vertex> class Graph{
	std::unordered_map<Vertex,std::unordered_set<Vertex>> graph;
public:
	Graph<Vertex>(){}

	std::unordered_map<Vertex,std::unordered_set<Vertex>>getGraph(){return graph;}
//Exercice 1
	void addVertex(Vertex v){
		std::unordered_set<Vertex> buff; //un set vide est lié à chaque Vertex.
		graph.insert({v,buff});
	};
	
	void addEdge(Vertex v1, Vertex v2){
		if(!containsEdge(v1,v2)){
			graph[v1].insert(v2); //ajout dans le set des 2 Vertex passés en paramètre.
			graph[v2].insert(v1); //la même chose de l'autre côté vu que les liaisons sont parallèles.
		}
	}

	bool containsEdge(Vertex v1, Vertex v2){
		if (graph[v1].find(v2) == graph[v1].end())
	        return false;
	 
	    return true;
	}

	size_t countVertices(){
		return graph.size();
	}

	void removeVertex(Vertex v){
		for(auto &buf : graph[v]){
			graph[buf].erase(v);
		}
		graph.erase(v);
	}

	void afficher(){
		for(auto &[k,val] : graph){
			std::cout << k << " :";
			for(auto &buf : val)
				std::cout << " " << buf;
			std::cout << std::endl;
		}
	}

	std::unordered_set<Vertex> neighbors(Vertex v){
		return graph[v];
	}

	Vertex minDegVertex(){
		size_t min = 100000000;
		Vertex bufV;
		for(auto &[k,val] : graph){ //Parcour de la map.
			if(min > val.size()){
				min = val.size(); //On conserve la valeur si elle est plus grande que l'ancienne.
				bufV = k;
			}
		}
		return bufV;
	}
	
	std::vector<Vertex> glouton(){
		std::vector<Vertex> res;
		while(this->countVertices()){
		    Vertex lessNeigh = this->minDegVertex();
		    for(auto n: this->neighbors(lessNeigh)){
		        this->removeVertex(n);
		    }
		    this->removeVertex(lessNeigh);
		    res.push_back(lessNeigh);
		}
		return res;
	}
	
	static Graph<Vertex> readFile(std::string link){
		std::ifstream myfile(link);
		Graph<Vertex> newGraph;
		if(myfile.is_open()){
			Vertex p1, p2;
			while(myfile){
			    myfile >> p1;
			    myfile >> p2;
			    if(p1 != p2)
			    	newGraph.addEdge(p1, p2);
			}
		}
		return newGraph;
	}

	Vertex randomVertex(){
		int random = rand() % this->countVertices() + 1;
		Vertex res;
		int i = 0;
		for(auto& v : this->vertices()){
			if(i == random){
				res = v;
				break;
			}
			i++;
		}
		return res;
	}

	Graph<Vertex> createSub(std::vector<Vertex> subg){
		Graph<Vertex> buf = *(this);
		for(auto& v : this->vertices()){
			typename std::vector<Vertex>::iterator position = std::find(subg.begin(), subg.end(),v);
			if(position == subg.end())
				buf.removeVertex(v);
		}
		return buf;
	}

	std::vector<Vertex> verifyExterior(std::vector<Vertex> &resBfs, std::vector<Vertex> resInit){
		for(auto n: resBfs){
			typename std::vector<Vertex>::iterator position = find(resInit.begin(), resInit.end(),n);
			if(position != resInit.end())
				resInit.erase(position);
		}
		for(auto s: resInit){
			for(auto g : this->neighbors(s)){
				typename std::vector<Vertex>::iterator position = find(resBfs.begin(), resBfs.end(),g);
				if(position != resBfs.end())
					resBfs.erase(position);
			}
		}
		return resInit;
	} 
//Mr Dias Da Fonseca	
	std::vector<Vertex> bfs(Vertex v, size_t maxv = 0) {
		std::set<Vertex> visited;
		std::vector<Vertex> ret;
		std::queue<Vertex> fifo;

		if(maxv == 0)
			maxv = graph.size();
		fifo.push(v);
		while(!fifo.empty() && ret.size() < (size_t) maxv) {
			Vertex u = fifo.front();
			fifo.pop();
			if(visited.count(u) != 0)
				continue;
			ret.push_back(u);
			visited.insert(u);
			for(auto n: this->neighbors(u)){
				fifo.push(n);
			}
		}
		return ret;
	}

	std::set<Vertex> vertices() const{
	    std::set<Vertex> ret;
	    for(const auto &[v, neigh] : graph) {
	      ret.insert(v);
	    }
	    return ret;
	  }
	  
	  std::set<std::pair<Vertex,Vertex>> edges() const{
	    std::set<std::pair<Vertex,Vertex>> ret;
	    
	    for(const auto &[v, neigh] : graph) {
	      for(Vertex u : neigh) {
		if(u < v)
		  ret.insert(std::make_pair(u,v));
	      }
	    }
	    return ret;    
	  }


};


