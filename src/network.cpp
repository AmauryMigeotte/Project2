#include "network.h"
#include "random.h"
#include <algorithm>
#include <cmath>

Network::Network() {}


void Network::resize(const size_t& n){
	values.resize(n);
	RNG.normal(values);
}

bool Network::add_link(const size_t& a, const size_t& b){
	if ((a == b) or (a >= values.size()) or (b >= values.size())) return false;
	vector<size_t> neighborsA(neighbors(a));
	vector<size_t> neighborsB(neighbors(b));
	for(auto i : neighborsA){
		if (i == b) return false; 
	}
	for(auto i : neighborsB){
		if (i == a) return false;
	}
	
	links.insert(std::pair<size_t, size_t > (a,b)); 
	links.insert(std::pair<size_t, size_t > (b,a)); 
	
	return true;
}

size_t Network::random_connect(const double& n){
	RandomNumbers randomGenerator;
	vector<int> numberOfConnections(values.size());
	randomGenerator.poisson(numberOfConnections, n);
	links.clear();
	size_t result(0);
	
	
	for(size_t i(0); i < values.size(); i++){
		size_t nOC = numberOfConnections[i];
		vector<int> randomConnection(values.size()); // va tester seulemtent values.size() valeurs, si pas trouvé de node libre parmis ces valeurs tant pis
		for(size_t j(0); j<randomConnection.size() and degree(i) < nOC; j++){
			randomGenerator.uniform_int(randomConnection, 0, values.size()-1);
			if (degree(randomConnection[j]) < size_t (numberOfConnections[randomConnection[j]])){
				if (add_link(i, randomConnection[j])) result++;
			}
		}
	}
	return result*2; //liens bidirectionnels dans le test
}

size_t Network::set_values(const std::vector<double>& newValues){
	size_t result(0);
	for( size_t i(0); i < newValues.size() and i < values.size(); i++){
		values[i] = newValues[i];		
		result++;
	}	
	return result;
}

size_t Network::size() const{
	return values.size();
}

size_t Network::degree(const size_t &_n) const{
	if (_n >= values.size()) throw out_of_range("In function degree: There's not " + to_string(_n) + " nodes");
	return links.count(_n);
}

double Network::value(const size_t &_n) const{
	if (_n >= values.size()) throw out_of_range("In function value: There's not " + to_string(_n) + " nodes");
	return values[_n];
}

vector<double> Network::sorted_values() const{
	vector<double> result(values);
	sort(result.begin(), result.end());
	reverse(result.begin(), result.end());
	return result;
}



std::vector<size_t> Network::neighbors(const size_t& n) { // j'ai du enlever le const, marchait pas avec le equal_range
	if (n >= values.size()) throw out_of_range("In function neighboors: There's not " + to_string(n) + " nodes");
	vector<size_t> result;
	std::pair<std::multimap<size_t, size_t>::iterator, std::multimap<size_t, size_t>::iterator> voisins;
	voisins = links.equal_range(n);
	for(std::multimap<size_t, size_t>::iterator i(voisins.first); i != voisins.second; i++ ){
		result.push_back(i->second);
	}
	return result;
}
















